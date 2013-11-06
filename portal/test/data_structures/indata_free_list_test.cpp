/*
 * indata_free_list_test.cpp
 *
 *  Created on: 26/10/2013
 *      Author: svp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE data_structures test

#include <string>
#include <iostream>
#include <array>
#include <boost/test/unit_test.hpp>

#include <cry_terra/data_structures/indata_free_list.hpp>
#include <cry_terra/utilities/ignore.hpp>

BOOST_AUTO_TEST_CASE( trivial_insertion )
{
    cry_terra::indata_free_list<unsigned int> l;

    l.insert(2);
    l.insert(4);
    l.insert(6);
    l.insert(7);
    l.insert(10);

    const size_t idx = l.insert(1337);

    BOOST_CHECK_EQUAL(idx, 5);
    BOOST_CHECK_EQUAL(l[idx], 1337);
    BOOST_CHECK_EQUAL(l.size(), 6);
    BOOST_CHECK_EQUAL(l.head(), std::numeric_limits<size_t>::max());

    BOOST_CHECK_EXCEPTION(l.at(10), std::out_of_range, [](const std::out_of_range&){ return true; });
}


BOOST_AUTO_TEST_CASE( pod_insertion )
{
    unsigned int d = 1;

    struct POD
    {
        POD(unsigned int& d)
        { std::generate(std::begin(x), std::end(x), [&](){ d *= 2; return d; }); };

        std::array<unsigned int, 4> x;
    };

    cry_terra::indata_free_list<POD> l;

    l.insert(POD(d));
    l.insert(POD(d));
    l.insert(POD(d));
    l.insert(POD(d));
    l.insert(POD(d));

    BOOST_CHECK_EQUAL(l.size(), 5);
    BOOST_CHECK_EQUAL(l.head(), std::numeric_limits<size_t>::max());
}

BOOST_AUTO_TEST_CASE( deletion_and_insertion )
{
    cry_terra::indata_free_list<double> l;

    l.insert(2.0);
    l.insert(4.0);
    const size_t idx = l.insert(6.0);
    l.insert(8.0);
    l.insert(10.0);

    l.remove(idx);

    BOOST_CHECK_EQUAL(l.size(), 4);
    BOOST_CHECK_EQUAL(l.head(), idx);

    const size_t idx1 = l.insert(42.0);

    BOOST_CHECK_EQUAL(idx1, idx);
    BOOST_CHECK_EQUAL(l.size(), 5);
    BOOST_CHECK_CLOSE(l[idx1], 42.0, 0.1);
    BOOST_CHECK_EQUAL(l.head(), std::numeric_limits<size_t>::max());

    const size_t idx2 = l.insert(64.0);
    BOOST_CHECK_CLOSE(l[idx2], 64.0, 0.1);

    l.remove(idx2);
    l.remove(idx1);
}

BOOST_AUTO_TEST_CASE( clear_test )
{
    cry_terra::indata_free_list<double> l;

    l.insert(2.0);
    l.insert(4.0);
    l.insert(6.0);
    l.insert(8.0);
    l.insert(10.0);

    l.clear();

    BOOST_CHECK_EQUAL(l.size(), 0);
}

BOOST_AUTO_TEST_CASE( check_constructors )
{
    {
        cry_terra::indata_free_list<double> l1(5);

        BOOST_CHECK_EQUAL(l1.size(), 5);
    }

    {
        const size_t size = 256;

        std::array<double, size> data;
        std::fill(std::begin(data), std::end(data), 42.0);

        cry_terra::indata_free_list<double> l2(std::begin(data), std::end(data));

        BOOST_CHECK_EQUAL(l2.size(), size);
        BOOST_CHECK_CLOSE(l2[size - 1], 42.0, 0.001);
    }
}

BOOST_AUTO_TEST_CASE( check_iterators )
{
    {
        unsigned int d = 0;
        const size_t size = 256;

        std::array<unsigned int, size> data;
        std::generate(std::begin(data), std::end(data), [&]() { return ++d; });

        cry_terra::indata_free_list<unsigned int> l1(std::begin(data), std::end(data));
        BOOST_CHECK_EQUAL(l1.size(), size);

        unsigned int sum1 = 0;

        for (const auto& it : l1)
        {
            sum1 += it;
        }

        BOOST_CHECK_EQUAL(sum1, std::accumulate(std::begin(data), std::end(data), 0));

        if (size > 16)
        {
            l1.remove(15);
            l1.remove(13);
            l1.remove(12);

            l1.insert(4);
            l1.insert(8);
            l1.insert(10);

            data[15] = 4;
            data[13] = 8;
            data[12] = 10;

            BOOST_CHECK_EQUAL(l1.head(), std::numeric_limits<size_t>::max());
        }

        unsigned int sum2 = 0;

        for (auto& it : l1)
        {
            sum2 += it;
        }

        BOOST_CHECK_EQUAL(sum2, std::accumulate(std::begin(data), std::end(data), 0));
    }
}

#include <memory>

BOOST_AUTO_TEST_CASE( nonpod_types )
{
    {
        struct NONPODBase
        {
            NONPODBase()
                : x1(new float(4.0f))
            {
            }

            virtual ~NONPODBase()
            {
                delete x1;

                x1 = nullptr;
            }

            float* x1;
        };

        struct NONPOD: public NONPODBase
        {
            NONPOD()
                : x3(new float(6.0f))
            {
            }

            NONPOD(const NONPOD& other)
                : x3(other.x3)
            {
            }

            virtual ~NONPOD()
            {
            }

            std::shared_ptr<float> x3;
        };

        cry_terra::indata_free_list<NONPOD> l1;

        NONPOD a;

        auto idx1 = l1.insert(a);

        BOOST_CHECK_EQUAL(a.x3.use_count(), 2);

        l1.remove(idx1);

        BOOST_CHECK_EQUAL(a.x3.use_count(), 1);
    }
}

