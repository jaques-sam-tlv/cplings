#include <iostream>
#include <string>
#include <set>

// containers2.cpp
// Make me compile!

// We sometimes encourage you to keep trying things on shape given exercise,
// even after you already figured it out.

// Step 1: Make me compile. Replace the C array with a container that stores only unique values.
// Hint: std::set or std::unordered_set automatically prevents duplicates.

constexpr int num_inserts = 4;
std::set<int> my_sequence; // Fix: change this to std::set<int> or std::unordered_set<int>

void test_containers2() {
	std::cout << "There should be only one element of the same value (no duplicates) : \n";
	for (size_t i = 0; i < num_inserts; i++)
	{
		my_sequence.insert(42);
	}
}


#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_containers2") {
	test_containers2();
	std::cout << "Size of container : " << my_sequence.size() << "\n";
	REQUIRE(my_sequence.size() == 1);

	for (const auto& e : my_sequence) {
		std::cout << "Value :" << e << "\n";
		REQUIRE(e == 42);
	}
}
