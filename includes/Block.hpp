/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Block.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:38:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 20:51:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "libs.hpp"

/*

	How it would work is that the Game class owns a palette of blockstates
	On init, it computes all blocks and stores all the blockstates in a big vector.

	The chunks in the world store index to blocks inside of the palette.

	for example the palette could looke like this:

	{"oak_stairs", facing: "north"}, {"oak_stairs", facing: "east"}, {"oak_stairs", facing: "down"}

	With such a palette, the chunk would store 1 to store a oak_stair thats facing east.

	
	Each blockstate would also have to store its own model, this way when meshing the chunk, we can:
	- access the blockstate from the block's "id"
	- get the model
	- get the face and "doescull" of the model to cull and add to mesh (facing up down east west)

	All of that allows us to store the actual block and model once and reuse it a bunch of times in our chunks by indexing them

	For custom blocks like chests, we can store the block itself in the chunk and the data of it would be in a separate vector,
	so something like

	std::map<position, CustomBlock>	_customBlocks;

	So when we need the data of a custom block, we look it up in a separate vector.
*/

/*

	Example chunk class

	class	Chunk
	{
		public:
			Chunk();
			~Chunk();
		private:
			// std::map<glm::vec3, customBlocks>	_customBlocks;
			std::vector<int>	_blocks; //Stores indexes to the blockstates
	};

*/

class	Block;

class	BlockState
{
	public:
		BlockState() {}
		~BlockState() {}

		//property	getProperty(); //returns the asked property
	private:
		Block	*_parent; //Parent block to get "gameplay properties" such as explosion reistance and all...
		//properties;
};

class	Block
{
	public:
		Block(const std::string &id)
		{
			_id = id;
		}
		~Block() {}

		// BlockState	*getBlockState(properties) returns the blockstate of the block that has the given set of properties

		void	computeStates() {} //Computes all of the block's states
	private:
		std::string				_id;
		std::vector<BlockState>	_states; //All possible states of this block based on its properties
		//possibleProperties (FACING {north south east west up down}, OPENED {true, false}, ...)
};

#endif
