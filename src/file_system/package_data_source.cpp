/*
 * package_data_source.cpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#include <cry_terra/file_system/package_data_source.hpp>

#include <sys/mman.h>

#include <chrono>

#include <lzo/lzo1x.h>
#include <lzo/lzo1y.h>

#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cry_terra/log/log.hpp>
#include <cry_terra/utilities/memory.hpp>
#include <cry_terra/utilities/timer.hpp>

using namespace cry_terra::file_system;

package_data_source::package_data_source(const path& package_path)
{
    if (lzo_init() != LZO_E_OK)
        throw std::runtime_error("LZO failed to initialize");

    open_package_(package_path);
}

package_data_source::~package_data_source()
{
}

unsigned int package_data_source::num_files() const
{
    return file_descriptors_.size();
}

std::unique_ptr<package_file_stream> package_data_source::open_file(const hash_type& path_hash)
{
    const auto& descriptor = get_descriptor(path_hash);



    switch (descriptor.compression)
    {
    case compression_type::no_compression:
    {
        return make_unique<package_file_stream>(
                reinterpret_cast<char const *>(source_.data() + descriptor.begin),
                reinterpret_cast<char const *>(source_.data() + descriptor.begin + descriptor.size_after)
        );

        break;
    }
    case compression_type::python_lzox:
    {
        typedef unsigned char uchar;

        const size_t python_magic_offset = 5;

        const uchar* src = reinterpret_cast<const uchar*>(source_.data()) + descriptor.begin + python_magic_offset;
        const size_t src_size = descriptor.size_after - python_magic_offset;

        std::vector<package_file_stream::char_type> data(descriptor.size_before);
        uchar* dst = reinterpret_cast<uchar*>(data.data());
        size_t dst_size = data.size();

        int r = lzo1x_decompress(src, src_size, dst, &dst_size, nullptr);

        if (LZO_E_OK != r || dst_size != descriptor.size_before)
        {
            log::log_error(log::category::file_system, "LZO decompression failed with code: %d", r);

            break;
        }

        auto stream = make_unique<package_file_stream>(nullptr, nullptr);

        (*stream)->container(std::move(data));

        return std::move(stream);

        break;
    }
    default:
    {
        log::log_error(log::category::file_system, "Compression format not supported: %d", descriptor.compression);

        break;
    }
    }

    return make_unique<package_file_stream>();
}

const packaged_file_descriptor& cry_terra::file_system::package_data_source::get_descriptor(const hash_type& hash)
{
    return file_descriptors_[file_index_map_[hash]];
}

void package_data_source::open_package_(const path& package_path)
{
    using namespace boost::property_tree;

    if (!boost::filesystem::exists(package_path))
    {
        log::log_error(log::category::file_system, "File does not exist: '%s'", package_path.c_str());

        return;
    }

    std::ifstream file_descriptors_stream(package_path.string());

    try
    {
        ptree pt;
        read_json(file_descriptors_stream, pt);

        for (const std::pair<std::string, ptree>& child : pt.get_child("file_descriptors"))
        {
            const auto& data = child.second;

            packaged_file_descriptor desc;

            desc.path_hash = data.get<decltype(desc.path_hash)>("path_mmh3");
            desc.size_after = data.get<decltype(desc.size_after)>("size_after");
            desc.size_before = data.get<decltype(desc.size_before)>("size_before");
            desc.begin = data.get<decltype(desc.begin)>("begin");
            desc.adler32 = data.get<decltype(desc.adler32)>("adler32");
            desc.compression = static_cast<compression_type>(data.get<std::uint8_t>("compression"));

            file_index_map_[desc.path_hash] = file_descriptors_.insert(std::move(desc));
        }

        if (!file_descriptors_.size() == pt.get<unsigned int>("num_files"))
        {
            log::log_error(log::category::file_system, "Unmatched number of files: %d vs. %d", file_descriptors_.size(), pt.get<unsigned int>("num_files"));

            throw std::runtime_error("");
        }

        const path data_path = package_path.parent_path() / pt.get<path>("data_file_path");
        const size_t data_size = pt.get<size_t>("size");

        source_.open(data_path, data_size, 0);

        if (!source_.is_open())
        {
            log::log_error(log::category::file_system, "Could not open package data file '%s'.", data_path.c_str());

            throw std::runtime_error("");
        }

//        detail::mmap_binding temp_mmap_binding;
//
//        temp_mmap_binding.fd = open(data_path.c_str(), O_RDONLY);
//
//        if (-1 == temp_mmap_binding.fd)
//        {
//            log::log_error(log::category::file_system, "Could not open package data file '%s'.", data_path.c_str());
//
//            throw std::runtime_error("");
//        }
//
//        temp_mmap_binding.data = mmap(0, data_size, PROT_READ, MAP_SHARED, temp_mmap_binding.fd, 0);
//        temp_mmap_binding.size = data_size;
//
//        if (nullptr == temp_mmap_binding.data || MAP_FAILED == temp_mmap_binding.data)
//        {
//            log::log_error(log::category::file_system, "Could not MMAP package data file '%s'.", data_path.c_str());
//
//            throw std::runtime_error("");
//        }
//
//        mmap_binding_ = std::move(temp_mmap_binding);

        std::unique_ptr<package_file_stream> f;

        {
            utilities::timer a;
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
            f = open_file(hash("models/crytek-sponza/sponza.obj", 0x4a81a11665fbc02c));
            f->rdbuf();
        }

        std::ofstream a("out.pdf");

        a << f->rdbuf();

    } catch (const std::exception& e)
    {
        log::log_error(log::category::file_system, "Error while opening package '%s': %s", package_path.c_str(), e.what());
    }
}

detail::mmap_binding::mmap_binding()
    : size(0), fd(-1), data(nullptr)
{ }

detail::mmap_binding::~mmap_binding()
{
    if (data && size)
        munmap(data, size);

    if (fd != -1)
        close(fd);
}
