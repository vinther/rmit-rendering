/*
 * file_stream.hpp
 *
 *  Created on: 08/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_FILE_SYSTEM_FILE_STREAM_HPP_
#define CRY_TERRA_FILE_SYSTEM_FILE_STREAM_HPP_

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>

#include <cry_terra/utilities/ignore.hpp>

namespace cry_terra
{

namespace file_system
{

namespace detail
{

class ct_file_stream
{
public:
    typedef char char_type;
    typedef std::vector<char_type> container_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::pointer pointer;
    typedef boost::iostreams::source_tag  category;

    ct_file_stream(const char_type * const begin, const char_type * const end)
        : cursor_(begin), begin_(begin), end_(end)
    {
    }

    template <typename Cont>
    void container(Cont&& container)
    {
        container_ = std::move(container);
        begin_ = container_.data();
        end_ = container_.data() + container_.size();
        cursor_ = begin_;
    }

    std::streamsize read(char_type* s, std::streamsize n)
    {
        std::streamsize amt = static_cast<std::streamsize>(end_ - cursor_);
        std::streamsize result = std::min(n, amt);

        if (result != 0)
        {
            std::copy(cursor_, cursor_ + result, s);

            cursor_ += result;

            return result;
        } else
        {
            return -1;
        }
    }

private:
    container_type container_;
    const char_type * cursor_;
    const char_type * begin_, * end_;
};

}

typedef boost::iostreams::stream<detail::ct_file_stream> package_file_stream;

}

}

#endif /* CRY_TERRA_FILE_SYSTEM_FILE_STREAM_HPP_ */
