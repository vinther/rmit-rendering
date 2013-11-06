/*
 * file_manager_test.cpp
 *
 *  Created on: 26/10/2013
 *      Author: svp
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE file_manager test

#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include <cry_terra/utilities/ignore.hpp>
#include <cry_terra/file_system/file_manager.hpp>

using namespace cry_terra::file_system;

const file_path TEST_ROOT_DIR = "test";
const file_path TEST_PACKAGE_PATH = TEST_ROOT_DIR / "file_system/test_package.json";

BOOST_AUTO_TEST_CASE( open_non_existing_package )
{
    file_manager f;

    f.open_package("non-existent-package");
    BOOST_CHECK_EQUAL(f.open_packages().size(), 0);
}

BOOST_AUTO_TEST_CASE( open_existing_package )
{
    file_manager f;

    BOOST_CHECK_EQUAL(f.file_descriptors().size(), 1);

    const auto h1 = f.retrieve_file("capsule.obj");
    BOOST_CHECK_EQUAL(h1.index, 0);

    f.open_package(TEST_PACKAGE_PATH);
    BOOST_CHECK_GT(f.open_packages().size(), 0);
    BOOST_CHECK_EQUAL(f.open_packages()[0], TEST_PACKAGE_PATH);

    file_descriptor_handle h = f.retrieve_file("capsule.obj");
    BOOST_CHECK_GE(h.index, 0);

    cry_terra::utilities::ignore(h);
}

