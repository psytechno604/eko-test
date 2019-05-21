// eko-delivery.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>

#include "Delivery.h"



bool isTest(int argc, char* argv[]) {
	for (int i = 0; i < argc; i++) {
		if (!std::string("--test").compare(argv[i])) {
			return true;
		}
	}
	return false;
}

int main(int argc, char* argv[])
{
	if (isTest(argc, argv)) {
		// --test		
		return Catch::Session().run(1, argv);
	}

	try {
		if (argc == 3) {
			auto d = new Delivery(std::string(argv[1]));
			int result = d->deliveryCost(std::string(argv[2]));
			if (result == -1) std::cout << "No Such Route" << std::endl;
			else std::cout << result << std::endl;
			return 0;
		}
		if (argc == 7) {
			auto d = new Delivery(std::string(argv[1]));
			bool useSameRouteTwice = std::string(argv[4]) == "true";
			int maxSteps, maxCost;
			if (std::string(argv[5]) == "INT_MAX")
				maxSteps = INT_MAX;
			else
				maxSteps = std::stoi(std::string(argv[5]));
			if (std::string(argv[6]) == "INT_MAX")
				maxCost = INT_MAX;
			else
				maxCost = std::stoi(std::string(argv[6]));

			std::cout << d->numRoutes(std::string(argv[2]), std::string(argv[3]), useSameRouteTwice, maxSteps, maxCost) << std::endl;
			return 0;
		}
		if (argc == 4) {
			auto d = new Delivery(std::string(argv[1]));
			std::cout << d->cheapestRoute(std::string(argv[2]), std::string(argv[3])) << std::endl;
			return 0;
		}

		std::cout << "Usage:" << std::endl << std::endl;
		std::cout << "Delivery cost for route:" << std::endl << std::endl;
		std::cout << "<executable> graph route" << std::endl << std::endl;
		std::cout << "e.g. <executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 A-B-E" << std::endl << std::endl << std::endl;
		std::cout << "The number of routes:" << std::endl << std::endl;
		std::cout << "<executable> graph from to useSameRouteTwice maxSteps maxCost" << std::endl << std::endl;
		std::cout << "e.g. <executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 E D false 4 INT_MAX" << std::endl << std::endl << std::endl;
		std::cout << "The cost of cheapest delivery route:" << std::endl << std::endl;
		std::cout << "<executable> graph from to" << std::endl << std::endl;
		std::cout << "e.g. <executable> AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1 E D" << std::endl << std::endl << std::endl;
		std::cout << "<executable>  --test to run pre-defined tests" << std::endl << std::endl << std::endl;
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl << "Please check input data";
	}

}

std::string testData = "AB1,AC4,AD10,BE3,CD4,CF2,DE1,EB3,EA2,FD1";

TEST_CASE("read correct data", "[input]") {
	REQUIRE((new Delivery(testData))->size() == 6);
}

TEST_CASE("crash on wrong data", "[input]") {
	REQUIRE_THROWS_AS(new Delivery("AB1, BC2"), std::exception);
	REQUIRE_THROWS_AS(new Delivery("AB1,SBC2"), std::exception);
	auto d = new Delivery(testData);
	REQUIRE_THROWS_AS(d->deliveryCost("A-B--E"), std::exception);
	REQUIRE_THROWS_AS(d->deliveryCost("A-B-M"), std::exception);
}

TEST_CASE("Case1", "[deliveryCost]") {
	auto d = new Delivery(testData);
	REQUIRE(d->deliveryCost("A-B-E") == 4);
	REQUIRE(d->deliveryCost("A-D") == 10);
	REQUIRE(d->deliveryCost("E-A-C-F") == 8);
	REQUIRE(d->deliveryCost("A-D-F") == -1);
}

TEST_CASE("simplest1", "[numRoutes]") {
	auto d = new Delivery("AB1,BA1");
	REQUIRE(d->numRoutes("A", "B", false, INT_MAX, INT_MAX) == 1);
	REQUIRE(d->numRoutes("A", "A", false, INT_MAX, INT_MAX) == 1);
}

TEST_CASE("simplest2", "[numRoutes]") {
	auto d = new Delivery("AB1,BC1,CD1,DE1,EF1");
	REQUIRE(d->numRoutes("A", "F", false, INT_MAX, INT_MAX) == 1);
	REQUIRE(d->numRoutes("F", "A", false, INT_MAX, INT_MAX) == 0);
}
TEST_CASE("Case2", "[numRoutes]") {
	auto d = new Delivery(testData);
	REQUIRE(d->numRoutes("E", "D", false, 4, INT_MAX) == 4);
	REQUIRE(d->numRoutes("E", "E", false, INT_MAX, INT_MAX) == 5);
	REQUIRE(d->numRoutes("E", "E", true, INT_MAX, 20) == 29);
}


TEST_CASE("Case3", "[cheapestRoute]") {
	auto d = new Delivery(testData);
	REQUIRE(d->cheapestRoute("E", "D") == 9);
	REQUIRE(d->cheapestRoute("E", "E") == 6);
}
