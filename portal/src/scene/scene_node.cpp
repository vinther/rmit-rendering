#include "scene/scene_node.hpp"

#include "shared/utilities.hpp"

scene::scene_node::scene_node()
    : models()
    , children()
    , transformation(glm::mat4(1.0f))
{

}

scene::scene_node::~scene_node()
{
}

void scene::scene_node::initialize()
{
    bvh = std::make_unique<physics::octree>();
    bvh->from_node(*this, 32);
}
