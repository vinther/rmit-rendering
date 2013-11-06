/*
 * file_manager.hpp
 *
 *  Created on: 26/10/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_MANAGER_HPP_
#define CRY_TERRA_FILE_MANAGER_HPP_

#include <fstream>
#include <string>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

#include <cry_terra/file_system/file_descriptor.hpp>

namespace cry_terra
{

namespace file_system
{

typedef boost::filesystem::path file_path;

class file_manager
{
public:
    file_manager();
    file_manager(const file_manager&) = default;
    file_manager(file_manager&&) = default;

    file_manager& operator=(const file_manager&) = default;
    file_manager& operator=(file_manager&&) = default;

    virtual ~file_manager() {};

    void open_package(const file_path& path);

    file_descriptor_handle retrieve_file(const boost::filesystem::path& rel_path);

private:
    std::vector<std::ifstream> data_file_streams_;
    std::vector<std::ifstream> descriptor_file_streams_;

    std::vector<file_path> open_packages_;
    std::vector<file_descriptor> file_descriptors_;
    std::map<file_path, unsigned int> file_index_map_;

    void parse_file_descriptors(const boost::property_tree::ptree& descriptors);

public:
    const decltype(open_packages_)& open_packages() const {
        return open_packages_;
    }

    const decltype(file_descriptors_)& file_descriptors() const
    {
        return file_descriptors_;
    }
};

}

}

#endif /* CRY_TERRA_FILE_MANAGER_HPP_ */
