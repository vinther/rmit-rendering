#include <cry_terra/file_system/file_manager.hpp>

#include <fstream>
#include <lzo/lzo1.h>
#include <boost/property_tree/json_parser.hpp>

#include <cry_terra/utilities/ignore.hpp>
#include <cry_terra/log/log.hpp>

cry_terra::file_system::file_manager::file_manager()
    : file_descriptors_()
{
    file_descriptors_.emplace_back();
}

//cry_terra::file_system::file_descriptor_handle cry_terra::file_system::file_manager::retrieve_file(
//        const boost::filesystem::path& rel_path)
//{
//    const auto& it = file_index_map_.find(rel_path);
//
//    if (file_index_map_.end() == it)
//        return {0, 0};
//    else
//        return {it->second, 0};
//}

