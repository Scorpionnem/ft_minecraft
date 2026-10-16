//
// Created by bvasseur on 10/16/26.
//

#ifndef MINECRAFT_ABIOMETYPE_HPP
# define MINECRAFT_ABIOMETYPE_HPP

# include "../../../../../lib/mbl/inc/math/vec.hpp"
# include "math/math.hpp"

class ABiomeType {
public:
	ABiomeType() {};
	ABiomeType(std::string name) : _name(name) {};
	virtual ~ABiomeType() {};

	virtual vec3d	getTermColor() = 0;

	std::string getName()	{ return _name; }

private:
	std::string _name;
};


class Forest final : public ABiomeType {
public:
	Forest() : ABiomeType("Forest") {};
	~Forest() = default;

	vec3d	getTermColor() override { return vec3d{50, 148, 52}; }
};

class Desert final : public ABiomeType {
public:
	Desert() : ABiomeType("Desert") {};
	~Desert() = default;

	vec3d	getTermColor() override { return vec3d{222, 177, 31}; }
};

class Lava final : public ABiomeType {
public:
	Lava() : ABiomeType("Lava")  {};
	~Lava() = default;

	vec3d	getTermColor() override { return vec3d{242, 64, 19}; }
};

class Ocean final : public ABiomeType {
public:
	Ocean() : ABiomeType("Ocean")  {};
	~Ocean() = default;

	vec3d	getTermColor() override { return vec3d{60, 65, 201}; }
};

#endif //MINECRAFT_ABIOMETYPE_HPP