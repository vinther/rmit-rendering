#ifndef SCENE_SCENE_GRAPH_HPP_
#define SCENE_SCENE_GRAPH_HPP_

#include <memory>

#include "physics/Octree.hpp"

#include "scene/camera.hpp"
#include "scene/scene_node.hpp"

namespace scene
{

class scene_graph
{
public:
    scene_graph();
    virtual ~scene_graph();

    void initialize();

    std::unique_ptr<camera> main_camera;
    std::unique_ptr<scene_node> root;
};

}

#endif /* SCENE_SCENE_GRAPH_HPP_ */
