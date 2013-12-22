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
#include <cry_terra/file_system/file_extension.hpp>
#include <cry_terra/file_system/path.hpp>
#include <cry_terra/utilities/hash.hpp>

namespace cry_terra
{

namespace file_system
{

//class file_system_data_source: public data_source
//{
//    virtual ~package_data_source();
//    virtual std::ifstream open_file(const hash_type) override;
//
//    std::vector<packaged_file_descriptor> file_descriptors_;
//};





class file_manager
{
public:
    file_manager();
    file_manager(const file_manager&) = default;
    file_manager(file_manager&&) = default;

    file_manager& operator=(const file_manager&) = default;
    file_manager& operator=(file_manager&&) = default;

    virtual ~file_manager() {};

//    file_descriptor_handle retrieve_file(const hash_type path_hash);

private:
    std::vector<std::ifstream> data_file_streams_;
    std::vector<std::ifstream> descriptor_file_streams_;

    std::vector<path> open_packages_;
    std::vector<packaged_file_descriptor> file_descriptors_;
    std::map<hash_type, unsigned int> file_index_map_;

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
