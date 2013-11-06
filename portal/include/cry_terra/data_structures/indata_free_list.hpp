/*
 * indata_free_list.hpp
 *
 *  Created on: 05/11/2013
 *      Author: svp
 */

#ifndef CRY_TERRA_INDATA_FREE_LIST_HPP_
#define CRY_TERRA_INDATA_FREE_LIST_HPP_

#include <vector>

#include <cry_terra/utilities/ignore.hpp>

namespace cry_terra
{

template <typename T>
class indata_free_list
{

public:
    typedef size_t size_type;

    class iterator;
    typedef const iterator const_iterator;

private:
    static constexpr size_type size_max() { return std::numeric_limits<size_type>::max(); };

    union item_node_
    {
        explicit item_node_(const T& item)
            : item(item)
        { }

        explicit item_node_(T&& item)
            : item(std::move(item))
        { }

        item_node_(const item_node_& other)
            : item(other.item)
        { }

        item_node_(item_node_&& other)
            : item(other.item)
        { }

        item_node_& operator=(const item_node_& other)
        {
            item = other.item;

            return *this;
        }

        item_node_& operator=(item_node_&& other)
        {
            item = other.item;

            return *this;
        }

        ~item_node_()
        { }

        T item;

        size_type next;
    };

    std::vector<item_node_> nodes_;
    std::vector<bool> nodes_used_;

    size_type head_ = size_max();
    size_type deleted_items_ = 0;

public:
    indata_free_list()
    { }

    indata_free_list(const size_type size)
    {
        for (size_t i = 0; i < size; ++i)
            insert(T());
    }

    template <typename InputIt>
    indata_free_list(InputIt first, InputIt last)
    {
        for (InputIt it = first; it != last; ++it)
            insert(*it);
    }

    indata_free_list(const indata_free_list<T>&) = default;
    indata_free_list(indata_free_list<T>&&) = default;
    virtual ~indata_free_list() { }

    indata_free_list& operator=(const indata_free_list<T>&) = default;
    indata_free_list& operator=(indata_free_list<T>&&) = default;

    size_type insert(const T& item)
    {
        if (size_max() == head_)
        {
            const size_type new_index = nodes_.size();

            nodes_.emplace_back(item);
            nodes_used_.push_back(true);

            return new_index;
        } else
        {
            const size_type new_index = head_;

            head_ = nodes_[head_].next;

            nodes_[new_index] = item_node_(item);
            nodes_used_[new_index] = true;

            --deleted_items_;

            return new_index;
        }
    }

    void remove(const size_type idx)
    {
        if (nodes_.size() <= idx)
            throw new std::out_of_range("Element is out of range: " + std::to_string(idx));

        if (nodes_used_[idx])
            reinterpret_cast<T*>(&(nodes_[idx].item))->~T();

        nodes_[idx].next = head_;
        nodes_used_[idx] = false;

        head_ = idx;

        ++deleted_items_;
    }

    size_type size() const
    {
        return nodes_.size() - deleted_items_;
    }

    size_type head() const
    {
        return head_;
    }

    void clear()
    {
        nodes_.clear();
        nodes_used_.clear();

        deleted_items_ = 0;
        head_ = size_max();
    }

    T& operator[](const size_type idx)
    {
        return nodes_[idx].item;
    }

    const T& operator[](const size_type idx) const
    {
        return nodes_[idx].item;
    }

    T& at(const size_type idx)
    {
        if (!nodes_used_[idx])
            throw new std::out_of_range("Element not used: " + std::to_string(idx));

        return nodes_.at(idx).item;
    }

    const T& at(const size_type idx) const
    {
        if (!nodes_used_[idx])
            throw new std::out_of_range("Element not used: " + std::to_string(idx));

        return nodes_.at(idx).item;
    }

    iterator begin()
    {
        size_type i = 0;

        for (; i < nodes_.size() && !nodes_used_[i]; ++i);

        return (iterator(this, i));
    }

    iterator end()
    {
        size_type i = nodes_.size() - 1;

        for (; i != size_max() && !nodes_used_[i]; --i);

        return (iterator(this, i + 1));
    }

    const_iterator cbegin() const
    {
        size_type i = 0;

        for (; i < nodes_.size() && !nodes_used_[i]; ++i);

        return const_iterator(this, i);
    }

    const_iterator cend() const
    {
        size_type i = nodes_.size() - 1;

        for (; i != size_max() && !nodes_used_[i]; --i);

        return const_iterator(this, i + 1);
    }
};


template <typename T>
class indata_free_list<T>::iterator : std::iterator<std::bidirectional_iterator_tag, T>
{
    friend class indata_free_list<T>;

public:
    iterator()
        : container_(nullptr), pos_(0)
    {
    }

    iterator(const iterator&) = default;
    iterator(iterator&&) = default;
    virtual ~iterator() { }

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&) = default;

    T& operator*()
    {
        return container_->operator [](pos_);
    }

    const T* operator->() const
    {
        return &container_[pos_];
    }

    iterator& operator++()
    {
        ++pos_;

        while(!container_->nodes_used_[pos_] && pos_ < container_->nodes_.size())
            ++pos_;

        return *this;
    }

    iterator& operator--()
    {
        for (--pos_; pos_ != size_max() && !container_->nodes_used_[pos_]; --pos_);

        return *this;
    }

    friend bool operator==(const iterator& lhs, const iterator& rhs)
    {
        return lhs.equal(rhs);
    }

    friend bool operator!=(const iterator& lhs, const iterator& rhs)
    {
        return !(lhs == rhs);
    }

    bool equal(iterator const& rhs) const
    {
        return pos_ == rhs.pos_;
    }

private:
    iterator(indata_free_list<T>* container, size_t pos = 0)
            : container_(container), pos_(pos)
    {
    }

    indata_free_list<T>* container_;
    indata_free_list<T>::size_type pos_;
};


}

#endif /* CRY_TERRA_INDATA_FREE_LIST_HPP_ */
