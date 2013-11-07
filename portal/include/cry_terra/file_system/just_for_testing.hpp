/*
 * just_for_testing.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_JUST_FOR_TESTING_HPP_
#define CRY_TERRA_JUST_FOR_TESTING_HPP_

#define CT_FILE_PATH(s, y) y

#define TEST

namespace cry_terra
{

void do_something()
{
    int s = CT_FILE_PATH("Hello/world.jpg", 0);
}


}

#endif /* CRY_TERRA_JUST_FOR_TESTING_HPP_ */
