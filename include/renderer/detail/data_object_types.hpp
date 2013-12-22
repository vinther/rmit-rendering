#ifndef DATA_OBJECT_TYPES_HPP_
#define DATA_OBJECT_TYPES_HPP_

#include "shared/handle.hpp"

typedef unsigned int GLuint;

namespace renderer
{

namespace detail
{

struct vertex_buffer_object
{
    GLuint buffer;
};

struct index_buffer_object
{
    GLuint buffer;
};

struct frame_buffer_object
{
    GLuint buffer;
};

struct uniform_buffer_object
{
    GLuint buffer;
};

struct vertex_array_object
{
    GLuint buffer;
};

struct texture_buffer
{
    GLuint buffer;
};

struct shader
{
    GLuint buffer;
};

}

typedef handle<detail::vertex_buffer_object> vertex_buffer_handle;
typedef handle<detail::index_buffer_object> index_buffer_handle;
typedef handle<detail::frame_buffer_object> frame_buffer_handle;
typedef handle<detail::uniform_buffer_object> uniform_buffer_handle;
typedef handle<detail::vertex_array_object> vertex_array_handle;
typedef handle<detail::texture_buffer> texture_handle;
typedef handle<detail::shader> shader_handle;

}

#endif /* DATA_OBJECT_TYPES_HPP_ */
