/*
 * CommandSet.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef COMMANDSET_HPP_
#define COMMANDSET_HPP_

class CommandSet
{
public:
    CommandSet(const std::shared_ptr<Client>& client)
        : client(client)
    {}

    CommandSet()
    {}

    virtual ~CommandSet()
    {}

    virtual void keydown(SDLKey key, SDLMod mod) = 0;
    virtual void keyup(SDLKey key, SDLMod mod) = 0;

    void setClient(const std::shared_ptr<Client>& client)
    {
        this->client = client;
    }

protected:
    std::shared_ptr<Client> client;
};

#endif /* COMMANDSET_HPP_ */
