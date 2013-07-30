/*
 * Player.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>

#include <glm/glm.hpp>


class Player
{
public:
	Player();

	const std::string& name() const {
		return name_;
	}

	void name(const std::string& name) {
		name_ = name;
	}

private:
	std::string name_;

	glm::vec3 position;
};

#endif /* PLAYER_HPP_ */
