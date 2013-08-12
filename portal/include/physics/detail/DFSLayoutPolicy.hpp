/*
 * DFSLayoutPolicy.hpp
 *
 *  Created on: 13/08/2013
 *      Author: svp
 */

#ifndef DFSLAYOUTPOLICY_HPP_
#define DFSLAYOUTPOLICY_HPP_

#include "physics/Octree.hpp"

namespace physics {
namespace detail {

class ConstructionTree;

struct DFSLayoutPolicy {
	void layout(const ConstructionTree& cTree, Octree::Data& data);
};

} /* namespace detail */
} /* namespace physics */

#endif /* DFSLAYOUTPOLICY_HPP_ */
