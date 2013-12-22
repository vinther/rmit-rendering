#ifndef RENDERER_DETAIL_STORAGE_BACKEND_HPP_
#define RENDERER_DETAIL_STORAGE_BACKEND_HPP_

#include <memory>
#include <unordered_map>

#include "shared/utilities.hpp"
#include "shared/handle.hpp"

#include "renderer/detail/compound_types.hpp"

namespace renderer
{

class storage_backend
{
public:
    storage_backend();
    virtual ~storage_backend();

    template <class T>
    const std::shared_ptr<T> getByAsset(std::shared_ptr<const typename T::asset_type> asset);

    template <class T>
    const std::shared_ptr<T> getByHash(size_t hash);

    void updateResources();

    vertex_buffer_handle create_vertex_buffer();
    void delete_vertex_buffer(const vertex_buffer_handle& handle);
    detail::vertex_buffer_object& access_vertex_buffer(const vertex_buffer_handle& handle);

    index_buffer_handle create_index_buffer();
    void delete_index_buffer(const index_buffer_handle& handle);
    detail::index_buffer_object& access_index_buffer(const index_buffer_handle& handle);

    frame_buffer_handle create_frame_buffer();
    void delete_frame_buffer(const frame_buffer_handle& handle);
    detail::frame_buffer_object& access_frame_buffer(const frame_buffer_handle& handle);

    uniform_buffer_handle create_uniform_buffer();
    void delete_uniform_buffer(const uniform_buffer_handle& handle);
    detail::uniform_buffer_object& access_uniform_buffer(const uniform_buffer_handle& handle);

    vertex_array_handle create_vertex_array();
    void delete_vertex_array(const vertex_array_handle& handle);
    detail::vertex_array_object& access_vertex_array(const vertex_array_handle& handle);

    texture_handle create_texture_buffer();
    void delete_texture_buffer(const texture_handle& handle);
    detail::texture_buffer& access_texture_buffer(const texture_handle& handle);

    shader_handle create_shader();
    void delete_shader(const shader_handle& handle);
    detail::shader& access_shader(const shader_handle& handle);

    mesh_handle create_mesh();
    void delete_mesh(const mesh_handle& handle);
    detail::mesh& access_mesh(const mesh_handle& handle);

    material_handle create_material();
    void delete_material(const material_handle& handle);
    detail::material& access_material(const material_handle& handle);

    shader_program_handle create_shader_program();
    void delete_shader_program(const shader_program_handle& handle);
    detail::shader_program& access_shader_program(const shader_program_handle& handle);

private:
    struct data_t
    {
        /* Pure types */
        std::vector<std::pair<std::uint32_t, detail::vertex_buffer_object>> vertex_buffer_object;
        std::vector<std::pair<std::uint32_t, detail::index_buffer_object>> index_buffer_objects;
        std::vector<std::pair<std::uint32_t, detail::frame_buffer_object>> frame_buffer_objects;
        std::vector<std::pair<std::uint32_t, detail::uniform_buffer_object>> uniform_buffer_objects;
        std::vector<std::pair<std::uint32_t, detail::vertex_array_object>> vertex_array_objects;
        std::vector<std::pair<std::uint32_t, detail::texture_buffer>> texture_buffers;
        std::vector<std::pair<std::uint32_t, detail::shader>> shaders;

        /* Compound types */
        std::vector<std::pair<std::uint32_t, detail::mesh>> meshes;
        std::vector<std::pair<std::uint32_t, detail::material>> materials;
        std::vector<std::pair<std::uint32_t, detail::shader_program>> shader_programs;
    } data;
};

}

#endif /* RENDERER_DETAIL_STORAGE_BACKEND_HPP_ */
