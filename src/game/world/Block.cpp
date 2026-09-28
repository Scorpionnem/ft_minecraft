#include "game/world/Block.hpp"

blockStateId	Block::AIR = 0;
blockStateId	Block::STONE = 0;
blockStateId	Block::GRASS_BLOCK = 0;
blockStateId	Block::GRASS = 0;
blockStateId	Block::BLUE_ORCHID = 0;
blockStateId	Block::STONE_SLAB = 0;

u8	BlockState::getPropertyValue(BlockProperty::Id prop)
{
	return ((_hash >> _block.offsetOf(prop)) & _bitMask(_block.sizeOf(prop)));
}

void	Block::_processLayout(const std::vector<BlockProperty>& properties)
{
	u8	offset = 0;

	for (const BlockProperty& prop : properties)
	{
		_offsets[prop.id] = offset;
		_sizes[prop.id] = prop.bitCount;
		offset += prop.bitCount;
	}
}

void	Block::_generateStates(const std::vector<BlockProperty>& properties, const std::string &model_path, mbl::render::TextureAtlas* atlas)
{
	_generateStatesRec(properties, 0, 0, model_path, atlas);
}

void	Block::_generateStatesRec(const std::vector<BlockProperty>& properties, size_t idx, blockStateHash hash, const std::string &model_path, mbl::render::TextureAtlas* atlas)
{
	if (idx == properties.size())
	{
		_states.emplace_back(*this, hash, model_path, atlas);
		return ;
	}

	const BlockProperty& prop = properties[idx];
	for (u8 val = prop.minVal; val <= prop.maxVal; val++)
	{
		blockStateHash next = hash | (static_cast<blockStateHash>(val) << _offsets[prop.id]);
		_generateStatesRec(properties, idx + 1, next, model_path, atlas);
	}
}
