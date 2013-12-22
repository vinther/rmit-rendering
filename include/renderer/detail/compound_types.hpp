#ifndef COMPOUND_TYPES_HPP_
#define COMPOUND_TYPES_HPP_

#include "renderer/detail/data_object_types.hpp"

typedef unsigned int GLuint;

namespace renderer
{

namespace detail
{

struct mesh
{
    vertex_buffer_handle vertex_buffer, normal_buffer, tangent_buffer, texcoord_buffer;
    vertex_array_handle vertex_array;
    index_buffer_handle index_buffer;

    unsigned int numVertices;
    unsigned int numFaces;
};

struct material
{
    texture_handle texture_emissive;
    texture_handle texture_ambient;
    texture_handle texture_diffuse;
    texture_handle texture_specular;
    texture_handle texture_bump;

    glm::vec4 emission;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
};

struct shader_program
{
    shader_handle vertex_shader, fragment_shader, geometry_shader;
    GLuint program;
};

}

typedef handle<detail::mesh> mesh_handle;
typedef handle<detail::material> material_handle;
typedef handle<detail::shader_program> shader_program_handle;

}

#endif /* COMPOUND_TYPES_HPP_ */
