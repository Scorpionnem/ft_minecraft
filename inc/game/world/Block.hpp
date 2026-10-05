#pragma once

#include "mbl.hpp"
#include "game/world/BlockModel.hpp"

struct	BlockProperty
{
	public:
		enum	Id
		{
			FACING = 0,
			LIT = 1,
			WATERLOGGED = 2,
			AGE = 3,
			AXIS = 4,
		};
	public:
		constexpr BlockProperty(BlockProperty::Id id, u8 minVal, u8 maxVal)
		: id(static_cast<int>(id))
		{
			this->minVal = minVal;
			this->maxVal = maxVal;
			bitCount = (int)(log(maxVal) / log(2) + 1);
		}

		u8		id;

		u8		minVal = 0;
		u8		maxVal = 0;

		u8		bitCount = 0;
};

namespace BlockProperties
{
	const BlockProperty	FACING(BlockProperty::FACING, 0, 5); // up down north south east west
	const BlockProperty	LIT(BlockProperty::LIT, false, true);
	const BlockProperty	WATERLOGGED(BlockProperty::WATERLOGGED, false, true);
	const BlockProperty	AGE(BlockProperty::AGE, 0, 32);
	const BlockProperty	AXIS(BlockProperty::AXIS, 0, 2); // north south east west
	const BlockProperty	SLAB_POS(BlockProperty::AXIS, 0, 1); // top bottom
};

class	Block;

using blockStateHash = u32;
using blockStateId = u32;
class	BlockState
{
	public:
		BlockState(Block& b, blockStateHash h, const std::string &model_path, mbl::render::TextureAtlas* atlas) : _block(b), _hash(h)
		{
			if (!model_path.empty())
				_model.load(model_path, atlas);
		};

		Block&	getBlock() {return (_block);}
		u8	getPropertyValue(BlockProperty::Id prop);

		blockStateId	id() {return (_id);}
		void	setId(blockStateId id) {_id = id;}
		blockStateHash	hash() {return (_hash);}
		BlockModel&	model() {return (_model);}
		void	computeTextures(mbl::render::TextureAtlas* atlas)
		{
			_model.computeTextures(atlas);
		}
	private:
		static constexpr u8	_bitMask(u8 n) {return ((1u << n) - 1u);}
		Block&			_block;
		blockStateHash	_hash;
		blockStateId	_id;

		BlockModel	_model;
};

class	Block
{
	public:
		static blockStateId	AIR;
		static blockStateId	STONE;
		static blockStateId	GRASS_BLOCK;
		static blockStateId	GRASS;
		static blockStateId	BLUE_ORCHID;
		static blockStateId	STONE_SLAB;
		static blockStateId	SNOW;
		static blockStateId	SAND;
		static blockStateId	MOSS;
		static blockStateId	COBBLESTONE;
	public:
		Block(const std::string &name, bool solid, const std::vector<BlockProperty>& properties, const std::string &model_path, mbl::render::TextureAtlas* atlas)
		{
			_name = name;
			_solid = solid;

			_processLayout(properties);
			_generateStates(properties, model_path, atlas);
		}
		void	computeBlock(mbl::render::TextureAtlas* atlas)
		{
			for (auto& s : _states)
				s.computeTextures(atlas);
		}
		~Block() {}

		u8	offsetOf(BlockProperty::Id id) {return (_offsets[id]);}
		u8	sizeOf(BlockProperty::Id id) {return (_sizes[id]);}

		std::string	name() {return (_name);}

		void	setId(u16 id) {_id = id;}
		std::vector<BlockState>&	getStates() {return (_states);}
		BlockState&	getDefaultState() {return (_states[0]);}
		BlockState&	getState(const std::vector<std::pair<BlockProperty, u8>>& properties_values)
		{
			blockStateHash	h = 0;
			for (auto& [bp, val] : properties_values)
			{
				h |= (static_cast<blockStateHash>(val) << _offsets[bp.id]);
			}
			for (auto& s : _states)
			{
				if (s.hash() == h)
					return (s);
			}
			throw std::runtime_error("Block doesnt exist with these properties");
		}

		bool	solid() {return (_solid);}
	private:
		void	_processLayout(const std::vector<BlockProperty>& properties);
		void	_generateStates(const std::vector<BlockProperty>& properties, const std::string &model_path, mbl::render::TextureAtlas* atlas);
		void	_generateStatesRec(const std::vector<BlockProperty>& properties, size_t idx, blockStateHash hash, const std::string &model_path, mbl::render::TextureAtlas* atlas);
	private:
		std::string	_name;

		bool	_solid = false;

		std::vector<BlockState>	_states;

		u8		_offsets[256] = {};
		u8		_sizes[256] = {};

		u16	_id;
};

using blockPtr = std::shared_ptr<Block>;
using blockId = u32;
class	BlockRegistry
{
	public:
		void	setAtlas(mbl::render::TextureAtlas* atlas)
		{
			_atlas = atlas;
		}
		blockPtr	registerBlock(const std::string& name, bool solid = true, const std::vector<BlockProperty>& properties = {}, const std::string& model_path = "")
		{
			blockPtr	b = std::make_shared<Block>(name, solid, properties, model_path, _atlas);
			_blocks_map.insert({name, b});
			_blocks.push_back(b);
			_blocks.back()->setId(static_cast<blockId>(_blocks.size() - 1));
			for (auto& s : b->getStates())
			{
				_blockStates.push_back(&s);
				_blockStates.back()->setId(static_cast<blockStateId>(_blockStates.size() - 1));
			}
			return (b);
		}
		void	computeBlocks()
		{
			for (blockPtr b : _blocks)
				b->computeBlock(_atlas);
		}

		blockPtr	get(blockId id) {return (_blocks[id]);}
		BlockState&	getState(blockStateId id) {return (*_blockStates[id]);}
		blockPtr	get(const std::string& name) {return (_blocks_map[name]);}
	private:
		std::map<std::string, blockPtr>	_blocks_map;
		std::vector<blockPtr>				_blocks;

		std::vector<BlockState*>				_blockStates;

		mbl::render::TextureAtlas*	_atlas = nullptr;
};
