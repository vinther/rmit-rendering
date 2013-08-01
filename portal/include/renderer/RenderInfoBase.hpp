/*
 * RenderInfoBase.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef RENDERINFOBASE_HPP_
#define RENDERINFOBASE_HPP_


struct RenderInfoBase
{
    RenderInfoBase()
        : state(State::PRISTINE)
    {};

    enum State
    {
        PRISTINE    = 0x01,
        DIRTY       = 0x02,
        READY       = 0x04,
    };

    Uint16 state;
};

#endif /* RENDERINFOBASE_HPP_ */
