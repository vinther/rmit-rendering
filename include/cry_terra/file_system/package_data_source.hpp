/*
 * package_data_source.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_SYSTEM_PACKAGE_DATA_SOURCE_HPP_
#define CRY_TERRA_FILE_SYSTEM_PACKAGE_DATA_SOURCE_HPP_

#include <map>
#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>

#include <cry_terra/file_system/path.hpp>
#include <cry_terra/data_structures/indata_free_list.hpp>
#include <cry_terra/file_system/file_extension.hpp>
#include <cry_terra/file_system/file_stream.hpp>
#include <cry_terra/utilities/hash.hpp>

namespace cry_terra
{

namespace file_system
{

enum class compression_type : std::uint8_t
{
    no_compression = 0,
    python_lzox = 1,
    python_lzoy = 2,

    custom,
};

struct packaged_file_descriptor
{
    std::int64_t path_hash;
    std::uint32_t adler32;
    std::size_t size_before, size_after;
    std::size_t begin;
    compression_type compression;

    file_extension extension;
};

namespace detail
{

struct mmap_binding
{
    mmap_binding();
    ~mmap_binding();

    size_t size;
    int fd;
    void* data;
};

}

class package_data_source
{
public:
    package_data_source() = default;
    package_data_source(const package_data_source&) = default;
    package_data_source(const path& package_path);

    unsigned int num_files() const;

    virtual ~package_data_source();

    std::unique_ptr<package_file_stream> open_file(const hash_type&);
    const packaged_file_descriptor& get_descriptor(const hash_type&);
private:
    indata_free_list<packaged_file_descriptor> file_descriptors_;
    std::map<hash_type, std::size_t> file_index_map_;

    void open_package_(const path& package_path);

    boost::iostreams::mapped_file_source source_;
};

}

}

#endif /* CRY_TERRA_FILE_SYSTEM_PACKAGE_DATA_SOURCE_HPP_ */
