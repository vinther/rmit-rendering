/*
 * Interface.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <string>
#include <memory>

#include <SDL2/SDL.h>

class Client;

class Interface
{
public:
    Interface(std::shared_ptr<Client> client);

	struct Data
	{
	    Uint16 fps;
	} data;

    void display();


private:
    std::shared_ptr<Client> client;
    void writeText(const std::string& str);
};

#endif /* INTERFACE_HPP_ */
