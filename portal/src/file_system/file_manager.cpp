#include <cry_terra/file_system/file_manager.hpp>

#include <fstream>
#include <lzo/lzo1.h>
#include <boost/property_tree/json_parser.hpp>

#include <cry_terra/utilities/ignore.hpp>
#include <cry_terra/utilities/logging.hpp>

cry_terra::file_system::file_manager::file_manager()
    : file_descriptors_()
{
    file_descriptors_.emplace_back();
}

void cry_terra::file_system::file_manager::open_package(
        const boost::filesystem::path& path)
{
    using namespace boost::property_tree;

    if (!boost::filesystem::exists(path))
    {
        utilities::log_error(utilities::log_category::file_system, "File does not exist: '%s'", path.c_str());

        return;
    }

    std::ifstream file_descriptors_stream(path.string());

    try
    {
        ptree pt;
        read_json(file_descriptors_stream, pt);

        parse_file_descriptors(pt);

        open_packages_.push_back(path);

    } catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

cry_terra::file_system::file_descriptor_handle cry_terra::file_system::file_manager::retrieve_file(
        const boost::filesystem::path& rel_path)
{
    const auto& it = file_index_map_.find(rel_path);

    if (file_index_map_.end() == it)
        return {0, 0};
    else
        return {it->second, 0};
}

void cry_terra::file_system::file_manager::parse_file_descriptors(const boost::property_tree::ptree& descriptors)
{
    using namespace boost::property_tree;

    for (const auto& child : descriptors.get_child("file_descriptors"))
    {
        const auto& data = child.second;

        file_descriptor descriptor;

        descriptor.path = data.get<decltype(descriptor.path)>("path");
        descriptor.begin = data.get<decltype(descriptor.begin)>("begin");
        descriptor.size = data.get<decltype(descriptor.size)>("size");
        descriptor.original_size = data.get<decltype(descriptor.original_size)>("original_size");
        descriptor.adler32 = data.get<decltype(descriptor.adler32)>("adler32");
        descriptor.compression = data.get<decltype(descriptor.compression)>("compression");

        file_descriptors_.push_back(std::move(descriptor));
        file_index_map_[descriptor.path] = file_descriptors_.size() - 1;
    }
}
