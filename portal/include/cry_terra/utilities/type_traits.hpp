/*
 * type_traits.hpp
 *
 *  Created on: 06/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_TYPE_TRAITS_HPP_
#define CRY_TERRA_TYPE_TRAITS_HPP_

#include <iostream>
#include <type_traits>

namespace cry_terra
{



template<typename T>
union hack {
  hack () { } ~hack () { }
  T dummy;
};

template<typename T>
union simplified_node {
  simplified_node () { }
  simplified_node (T const& value) : item (value) { }
  simplified_node (T&&      value) : item (std::move (value)) { }

  simplified_node&
  operator= (simplified_node const& rhs) {
    (void) decltype (
      std::declval<hack<T>> () = std::declval<hack<T>> (),
      int { }
    ) {};

    return *this;
  }

  T item;
};

int
main (int argc, char *argv[])
{
  using TestType = int;

  simplified_node<TestType> u1, u2;
  u1 = u2;
}



}

#endif /* CRY_TERRA_TYPE_TRAITS_HPP_ */
