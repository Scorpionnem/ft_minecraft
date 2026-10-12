//
// Created by bvasseur on 10/12/26.
//

#include "platform/AudioEffects.hpp"


AudioEffects::Muffle	AudioEffects::_muffleStruct;
AudioEffects::Reverb	AudioEffects::_reverbStruct;
AudioEffects::Delay		AudioEffects::_delayStruct;


/* ==================== METHODS ==================== */


void AudioEffects::init() {
	_initDelay();
	_initReverb();
	_initUnderwaterFilter();
}

void AudioEffects::cleanup() {
	Mix_UnregisterEffect(MIX_CHANNEL_POST, _delayEffect);   // waits for the audio thread, so freeing is safe
	Mix_UnregisterEffect(MIX_CHANNEL_POST, _reverbEffect); // waits for the audio thread, so freeing is safe
	Mix_UnregisterEffect(MIX_CHANNEL_POST, _muffleEffect); // waits for the audio thread, so freeing is safe
	for (auto& b : _delayStruct.buf) b.clear();
	for (auto& side : _reverbStruct.combs)    for (auto& c : side) c.buf.clear();
	for (auto& side : _reverbStruct.allpasses) for (auto& a : side) a.buf.clear();
}


/* ==================== PRIVATE METHODS ==================== */


void AudioEffects::_initDelay() {
	int freq, ch; Uint16 fmt;
	if (!Mix_QuerySpec(&freq, &fmt, &ch) || fmt != AUDIO_S16SYS || ch > 2)
		return;                                    // unexpected format: skip the effect

	Delay& d = _delayStruct;
	d.channels    = ch;
	d.sampleRate  = static_cast<float>(freq);
	d.fadeSamples = 0.3f * freq;                   // 0.3 s fade of the wet level
	d.curDelay    = std::max(1.0f, d.delayTime * freq);
	d.idx         = 0;
	for (int c = 0; c < ch; ++c)
		d.buf[c].assign(static_cast<size_t>(2.0f * freq) + 2, 0.0f);   // room for the 2 s maximum

	Mix_RegisterEffect(MIX_CHANNEL_POST, _delayEffect, nullptr, nullptr);
}

void AudioEffects::_initReverb() {
	int freq, ch; Uint16 fmt;
	if (!Mix_QuerySpec(&freq, &fmt, &ch) || fmt != AUDIO_S16SYS || ch > 2)
		return;                                    // unexpected format: skip the effect

	// Freeverb delay lengths (tuned for 44.1 kHz), scaled to the actual sample rate
	static const int combLen[4]    = {1116, 1188, 1277, 1356};
	static const int allpassLen[2] = {556, 441};
	const int   spread = 23;                       // right channel is slightly longer: stereo width
	const float scale  = freq / 44100.0f;

	Reverb& r = _reverbStruct;
	r.channels    = ch;
	r.fadeSamples = 0.5f * freq;                   // 0.5 s fade of the wet level
	for (int c = 0; c < ch; ++c) {
		for (int i = 0; i < 4; ++i) {
			r.combs[c][i].buf.assign(std::max(1, int((combLen[i] + c * spread) * scale)), 0.0f);
			r.combs[c][i].idx = 0;
			r.combs[c][i].store = 0.0f;
		}
		for (int i = 0; i < 2; ++i) {
			r.allpasses[c][i].buf.assign(std::max(1, int((allpassLen[i] + c * spread) * scale)), 0.0f);
			r.allpasses[c][i].idx = 0;
		}
	}
	Mix_RegisterEffect(MIX_CHANNEL_POST, _reverbEffect, nullptr, nullptr);
}

void AudioEffects::_initUnderwaterFilter() {
	int freq, ch; Uint16 fmt;
	if (!Mix_QuerySpec(&freq, &fmt, &ch) || fmt != AUDIO_S16SYS || ch > 8)
		return;                                    // unexpected format: skip the effect

	const float cutoff = 600.0f;                   // Hz, lower = more muffled
	_muffleStruct.channels = ch;
	_muffleStruct.aMuffled = 1.0f - std::exp(-2.0f * M_PI * cutoff / freq);
	_muffleStruct.step     = (1.0f - _muffleStruct.aMuffled) / (0.3f * freq);   // 0.3 s fade

	Mix_RegisterEffect(MIX_CHANNEL_POST, _muffleEffect, nullptr, nullptr);
}

void AudioEffects::_delayEffect(int, void* stream, int len, void*) {
	Delay& d = _delayStruct;
	Sint16* s = static_cast<Sint16*>(stream);
	const int ch     = d.channels;
	const int frames = len / (int)(sizeof(Sint16) * ch);
	const size_t size = d.buf[0].size();

	const float wetMax    = d.wetTarget;
	const float wetGoal   = d.enabled ? wetMax : 0.0f;
	const float wetStep   = std::max(wetMax, 0.01f) / d.fadeSamples;
	const float fb        = d.feedback;
	const float delayGoal = std::clamp(d.delayTime * d.sampleRate, 1.0f, static_cast<float>(size - 2));

	for (int f = 0; f < frames; ++f) {
		// slide the wet level toward the target so there is no click when toggling
		if (d.wet < wetGoal)      d.wet = std::min(d.wet + wetStep, wetGoal);
		else if (d.wet > wetGoal) d.wet = std::max(d.wet - wetStep, wetGoal);

		// slide the delay length toward the target (changing it abruptly would click)
		d.curDelay += (delayGoal - d.curDelay) * 0.0005f;

		// read position with linear interpolation, since curDelay is fractional
		float rp = static_cast<float>(d.idx) - d.curDelay;
		if (rp < 0.0f) rp += static_cast<float>(size);
		const size_t i0 = static_cast<size_t>(rp);
		const size_t i1 = (i0 + 1 == size) ? 0 : i0 + 1;
		const float frac = rp - static_cast<float>(i0);

		for (int c = 0; c < ch; ++c) {
			const float delayed = d.buf[c][i0] * (1.0f - frac) + d.buf[c][i1] * frac;
			const float in = s[f * ch + c];
			d.buf[c][d.idx] = in + delayed * fb; // feedback: echoes of echoes
			if (c == 1)
				d.buf[1 - c][d.idx] = in + delayed * fb;
			const float mixed = in + delayed * d.wet;
			s[f * ch + c] = static_cast<Sint16>(std::clamp(mixed, -32768.0f, 32767.0f));
		}
		if (++d.idx == size) d.idx = 0;
	}
}

void AudioEffects::_reverbEffect(int, void* stream, int len, void*) {
	Reverb& r = _reverbStruct;
	Sint16* s = static_cast<Sint16*>(stream);
	const int ch     = r.channels;
	const int frames = len / (int)(sizeof(Sint16) * ch);

	const float wetMax   = r.wetTarget;
	const float target   = r.enabled ? wetMax : 0.0f;
	const float step     = std::max(wetMax, 0.01f) / r.fadeSamples;
	const float feedback = 0.7f + 0.28f * r.roomSize;
	const float damp     = r.damp;

	for (int f = 0; f < frames; ++f) {
		// slide the wet level toward the target; the tail keeps decaying by itself when disabled
		if (r.wet < target)      r.wet = std::min(r.wet + step, target);
		else if (r.wet > target) r.wet = std::max(r.wet - step, target);

		float in = 0.0f;                           // mono input, scaled for 4 combs
		for (int c = 0; c < ch; ++c) in += s[f * ch + c];
		in *= 0.09f / ch;

		for (int c = 0; c < ch; ++c) {
			float out = 0.0f;
			for (Comb& cb : r.combs[c]) {          // parallel damped feedback combs
				float y = cb.buf[cb.idx];
				cb.store = y * (1.0f - damp) + cb.store * damp;
				cb.buf[cb.idx] = in + cb.store * feedback;
				if (++cb.idx == cb.buf.size()) cb.idx = 0;
				out += y;
			}
			for (Allpass& ap : r.allpasses[c]) {   // series allpass: smooths the echoes
				float b = ap.buf[ap.idx];
				ap.buf[ap.idx] = out + b * 0.5f;
				if (++ap.idx == ap.buf.size()) ap.idx = 0;
				out = b - out;
			}
			float mixed = s[f * ch + c] + out * r.wet;
			s[f * ch + c] = static_cast<Sint16>(std::clamp(mixed, -32768.0f, 32767.0f));
		}
	}
}

void AudioEffects::_muffleEffect(int, void* stream, int len, void*) {
	Sint16* s = static_cast<Sint16*>(stream);
	const int ch     = _muffleStruct.channels;
	const int frames = len / (int)(sizeof(Sint16) * ch);
	const float target = _muffleStruct.enabled ? _muffleStruct.aMuffled : 1.0f;

	for (int f = 0; f < frames; ++f) {
		// slide the coefficient toward the target so there is no click
		if (_muffleStruct.a < target)      _muffleStruct.a = std::min(_muffleStruct.a + _muffleStruct.step, target);
		else if (_muffleStruct.a > target) _muffleStruct.a = std::max(_muffleStruct.a - _muffleStruct.step, target);

		for (int c = 0; c < ch; ++c) {
			float x = s[f * ch + c];
			_muffleStruct.z[c] += _muffleStruct.a * (x - _muffleStruct.z[c]);   // one-pole low-pass
			s[f * ch + c] = static_cast<Sint16>(_muffleStruct.z[c]);
		}
	}
}
