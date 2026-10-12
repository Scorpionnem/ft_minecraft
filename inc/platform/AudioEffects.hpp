//
// Created by bvasseur on 10/12/26.
//

#ifndef MINECRAFT_AUDIOEFFECTS_HPP
# define MINECRAFT_AUDIOEFFECTS_HPP

# include <atomic>
# include <algorithm>
# include <vector>
# include <SDL2/SDL_mixer.h>

class AudioEffects {
public:
	AudioEffects() = delete;
	~AudioEffects() = delete;


	static void init();
	static void cleanup();

// * roomSize 0..1 (tail length), wet 0..1 (reverb level)
	static void setReverbParams(const float roomSize, const float wet) {
		_reverbStruct.roomSize = roomSize;
		_reverbStruct.wetTarget = wet;
	}
// * delayTime in seconds (0.01..2.0), feedback 0..0.95 (number of repeats), wet 0..1 (echo level)
	static void setDelayParams(const float delayTime, const float feedback, const float wet) {
		_delayStruct.delayTime = std::clamp(delayTime, 0.01f, 2.0f);
		_delayStruct.feedback  = std::clamp(feedback, 0.0f, 0.95f);   // >= 1 would never decay
		_delayStruct.wetTarget = std::clamp(wet, 0.0f, 1.0f);
	}
	static void setDelay(const bool on)					{ _delayStruct.enabled = on; }
	static void setReverb(const bool on)				{ _reverbStruct.enabled = on; }
	static void setUnderwater(const bool isUnderwater)	{ _muffleStruct.enabled = isUnderwater; }


private:
	static void _initDelay();
	static void _initReverb();
	static void _initUnderwaterFilter();

/*
 * The filter. z += a * (x - z) is a one-pole low-pass filter. A smaller a means a lower cutoff and a duller sound. The coefficient comes from 1 - exp(-2π·fc/fs), so tune with cutoff.
 * Around 400 to 800 Hz sounds like being underwater, while 1500 to 2500 Hz sounds more like sound through a wall.
 * No click when entering the water. The coefficient slides from 1.0 (no filtering) to the muffled value over 0.3 s, so the transition is smooth instead of abrupt. Change the 0.3f to adjust it.
 * Format check. The code assumes 16-bit samples, which is SDL_mixer’s default format. That’s why initUnderwaterFilter bails out if Mix_QuerySpec reports something else.
 */
	static void _delayEffect(int, void* stream, int len, void*);
	static void _reverbEffect(int, void* stream, int len, void*);
	static void _muffleEffect(int, void* stream, int len, void*);
	struct Muffle {
		std::atomic<bool> enabled{false};  // written by game thread, read by audio thread
		int   channels   = 2;
		float aMuffled   = 0.1f;   // filter coefficient when underwater
		float step       = 0.0f;   // coefficient change per sample (fade speed)
		float a          = 1.0f;   // current coefficient, 1.0 = pass-through
		float z[8]       = {};     // filter memory, one per audio channel
	};

/*
 * roomSize: 0.3 sounds like a small room, 0.6 to 0.8 like a cave or hall, and above 0.9 like a cathedral.
 * wet: 0.2 to 0.4 is subtle and natural. Higher values make everything sound far away.
 * damp: raise it (0.4 to 0.6) for a darker, softer tail, like a stone cave. Lower it for a bright, metallic one.
 * Fade: the 0.5 s fadeSamples only fades the reverb’s level in and out. The existing tail rings out naturally after you disable it, so leaving a cave doesn’t cut off abruptly.
 * Clipping: the output is clamped, so very loud sounds with high wet can distort. If that happens, lower wet or scale the dry signal by 1.0f - 0.3f * r.wet.
 * Cost: about 12 small filters per sample is negligible for the CPU. The buffers total roughly 40 KB.
 */
	struct Comb    { std::vector<float> buf; size_t idx = 0; float store = 0.0f; };
	struct Allpass { std::vector<float> buf; size_t idx = 0; };
	struct Reverb {
		std::atomic<bool>  enabled{false};
		std::atomic<float> roomSize{0.9f};
		std::atomic<float> wetTarget{.6f};
		float damp        = 0.2f;            // high-frequency damping in the tail (0..1)
		float wet         = 0.0f;            // current wet level, slides toward the target
		float fadeSamples = 1.0f;
		int   channels    = 2;
		Comb    combs[2][4];
		Allpass allpasses[2][2];
	};

/*
 * delayTime: 0.05 to 0.12 s sounds like a slapback or a small hard room, 0.25 to 0.5 s is a classic echo, and over 1 s gives distinct canyon-style repeats.
 * feedback: this sets how many repeats you hear. About 0.3 gives 2 to 3 repeats, about 0.6 gives a long trail, and 0.9 or more lasts a very long time. Clamped to 0.95 to prevent infinite trail.
 * Changing delayTime while sound plays can cause a brief pitch bend in the echoes during the slide, like a tape echo. Avoid animating delayTime continuously.
 * Loud input: the output is clamped, but the internal buffer is float and unclamped. A very loud input with high feedback and wet can still distort at the clamp, lower wet if you hear crackling.
 * Cost and memory: negligible. The buffers hold 2 s of float samples per channel (about 700 KB at 44.1 kHz stereo).
 */
	struct Delay {
		std::atomic<bool>  enabled{false};
		std::atomic<float> delayTime{0.28f};   // seconds
		std::atomic<float> feedback{0.2f};
		std::atomic<float> wetTarget{1.0f};
		float wet         = 0.0f;              // current wet level, slides toward the target
		float fadeSamples = 1.0f;
		float curDelay    = 1.0f;              // current delay in samples, slides toward the target
		float sampleRate  = 44100.0f;
		int   channels    = 2;
		size_t idx        = 0;                 // write position, shared by all channels
		std::vector<float> buf[2];
	};

	static Delay	_delayStruct;
	static Reverb	_reverbStruct;
	static Muffle	_muffleStruct;

};

#endif //MINECRAFT_AUDIOEFFECTS_HPP