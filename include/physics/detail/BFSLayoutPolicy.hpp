/*
 * BFSLayoutPolicy.hpp
 *
 *  Created on: 12/08/2013
 *      Author: svp
 */

#ifndef BFSLAYOUTPOLICY_HPP_
#define BFSLAYOUTPOLICY_HPP_

#include "physics/Octree.hpp"

namespace physics {
namespace detail {

class ConstructionTree;

struct BFSLayoutPolicy {
	void operator()(const ConstructionTree& cTree, Octree::Data& data);
};

} /* namespace detail */
} /* namespace physics */
#endif /* BFSLAYOUTPOLICY_HPP_ */
