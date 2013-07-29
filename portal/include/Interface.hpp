/*
 * Interface.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <string>

#include <SDL/SDL.h>

class Interface
{
public:
	struct Data
	{
	    Uint16 fps;
	};

    void display(const Data& data);

private:
    void writeText(const std::string& str);
};

#endif /* INTERFACE_HPP_ */
