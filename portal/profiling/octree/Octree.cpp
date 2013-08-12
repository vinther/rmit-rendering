/*
 * Octree.cpp
 *
 *  Created on: 10/08/2013
 *      Author: svp
 */

#include <math.h>
#include <iostream>
#include <memory>
#include <chrono>

#include <boost/program_options.hpp>

#include "assets/Model.hpp"

#include "scene/Scene.hpp"

#include "Utilities.hpp"

namespace profiling
{
namespace octree
{

}
}

void handleOptions(int& argc, char** argv, std::string& sceneAssetPath)
{

    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("scene", po::value<std::string>(), "input scene")
    ;

    po::positional_options_description p;
    p.add("scene", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        (std::cout << desc) << std::endl;
        return;
    }

    if (vm.count("scene")) {
        sceneAssetPath = vm["scene"].as<std::string>();
    } else {
        std::cerr << "Scene asset was not specified." << std::endl;
    }
}

int main(int argc, char** argv)
{
    using namespace std;

    string sceneAssetPath;

    handleOptions(argc, argv, sceneAssetPath);

    if (sceneAssetPath.empty())
    {
        throw runtime_error("Can't continue without a valid scene");
    }


    auto scene = std::make_unique<scene::Scene>();
    auto model = std::make_unique<assets::Model>(sceneAssetPath);

    if (!model->loadFromDisk())
        throw runtime_error("Error loading model from disk!");

	const auto rootMin = scene->intersectionTree->aabb.min;
	const auto rootMax = scene->intersectionTree->aabb.max;

	const unsigned int sampleSize = 1 << 4;

	std::random_device rd;
	std::mt19937_64 gen(rd());

	std::uniform_real_distribution<> X1(rootMin.x, rootMax.x);
	std::uniform_real_distribution<> X2(rootMin.y, rootMax.y);
	std::uniform_real_distribution<> X3(rootMin.z, rootMax.z);

	std::uniform_real_distribution<> Y1(0.0f, 2.0f * M_PI);
	std::uniform_real_distribution<> Y2(0.0f, M_PI);

	std::vector<physics::Ray> rays;

	rays.resize(sampleSize);

	for (unsigned long long i = 0; i < sampleSize; ++i)
	{
		const float theta = Y1(gen);
		const float phi = Y2(gen);

		const float u = std::cos(phi);
		const float sq = std::sqrt(1 - u * u);

		rays[i].origin = glm::vec3(X1(gen), X2(gen), X3(gen));
		rays[i].direction = glm::normalize(glm::vec3(sq * std::cos(theta), sq * std::sin(theta), u));
	}

    scene->root->model = std::move(model);
    scene->initialize();

    for (auto bs: {16, 32, 64, 128, 256, 512, 1024, 2048})
    {
		scene->intersectionTree->createFromScene(*(scene->root), bs);

		auto& tree = *(scene->intersectionTree);

		unsigned int hits = 0;

		const auto start = std::chrono::high_resolution_clock::now();

		physics::IntersectionPoint result;
		for (unsigned long long i = 0; i < sampleSize; ++i)
		{
			hits += tree.trace(rays[i], result) ? 1 : 0;
		}

		const auto end = std::chrono::high_resolution_clock::now();

		std::cout <<
				sampleSize << " traces completed in: " <<
				std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() <<
				" miliseconds. Hits: " << hits << ". Bucket size: " << bs << std::endl;

    }
    std::cout << "Success" << std::endl;

    return 0;
}

