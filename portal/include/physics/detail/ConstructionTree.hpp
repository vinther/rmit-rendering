/*
 * ConstructionTree.hpp
 *
 *  Created on: 12/08/2013
 *      Author: svp
 */

#include <memory>

#include <assimp/scene.h>

#include "physics/Types.hpp"

namespace scene
{
class scene_node;
}

namespace physics
{

namespace detail
{

class ConstructionTree
{
public:
	ConstructionTree(const scene::scene_node& sceneRoot, unsigned int bucketSize);

	struct ConstructionNode
	{
		ConstructionNode()
	        : isLeaf(true)
	    {}

	    std::array<std::unique_ptr<ConstructionNode>, 8> children;
	    std::vector<unsigned int> bucket;

	    AABB aabb;

	    bool isLeaf;
	};

	std::unique_ptr<ConstructionNode> root;
	std::vector<Triangle> objects;

	void countNodes(unsigned int& internalNodes, unsigned int& leaves) const;
private:
	void insertTriangleIndex(unsigned int idx, const Triangle& triangle, ConstructionNode& node);
	void insertFace(const aiFace& face, const aiVector3D* vertices);
	void insertMesh(const aiMesh& mesh);

	unsigned int bucketSize;
};

} // namespace detail

} // namespace physics
