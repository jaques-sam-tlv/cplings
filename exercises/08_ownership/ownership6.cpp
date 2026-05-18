#include <iostream>
#include <string>
#include <memory>
#include <vector>

// ownership6.cpp
// Goal: share ownership of a single vector between several owners with
// std::shared_ptr, and watch how the reference count (use_count) changes as
// owners appear and disappear.
//
// fill_vec1 receives a shared_ptr to the vector and then hands it to fill_vec2.
// The test checks the number of owners fill_vec2 sees. To reach the expected
// count you must create one extra, "artificial" owner inside fill_vec1.
//
// Replace the ...? below with the missing line.


// Display value for a vector
template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& collection) {
    out << "{";
    if (collection.size() > 0) {
        const auto it_begin = collection.begin();
        const auto it_before_end = collection.end() - 1;
        auto it = it_begin;
        for (; it != it_before_end; it++) {
            out << *it << ", ";
        }
        out << *it;
    }
    out << "}";
    return out;
}

class MyInteger {
public:
    int number;
    MyInteger(int n) :number(n) { std::cout << "Constructing " << number << "\n"; }
    MyInteger(const MyInteger& i) :number(i.number) { std::cout << "Copying " << number << "\n"; }
    MyInteger(const MyInteger&& i) noexcept :number(i.number) { std::cout << "Moving " << number << "\n"; }
    ~MyInteger() { std::cout << "Destroying " << number << "\n"; }
};

std::ostream& operator<< (std::ostream& out, const MyInteger& integer) {
    return out << integer.number;
}

int fill_vec2(std::shared_ptr<std::vector<MyInteger>> vec_ptr) {
    std::cout << "vec has length " << vec_ptr->size() << ", number of owners:" <<
        vec_ptr.use_count() << " in fill_vec2\n";
    const int total_past_owners = vec_ptr.use_count();
    vec_ptr->emplace_back(33);
    return total_past_owners;
}

int fill_vec1(std::shared_ptr<std::vector<MyInteger>> vec_ptr) {
    // Fix: create an additional ("artificial") owner of the vector here and
    //      name it `vec`. A second std::shared_ptr can share ownership of the
    //      same object; constructing it raises the reference count, and it is
    //      released again when fill_vec1 returns.
    std::shared_ptr<std::vector<MyInteger>> vec(vec_ptr);

    vec->push_back(22);
    std::cout << "vec has length " << vec_ptr->size() << ", number of owners should be 3:" <<
        vec_ptr.use_count() << " in fill_vec1\n";

    std::cout << "fill_vec2 is starting" << std::endl;
    const int total_past_owners = fill_vec2(vec_ptr);
    std::cout << "fill_vec2 has ended" << std::endl;

    std::cout << "vec has length " << vec_ptr->size() << ", number of owner:" <<
        vec_ptr.use_count() << " in fill_vec1\n";
    return total_past_owners;
}

int test_ownership6() {
    const int desired_vector_space = 3;
    auto vec_ptr  = std::make_shared<std::vector<MyInteger>>();
    vec_ptr->reserve(desired_vector_space);

    vec_ptr->emplace_back(11); // emplace_back instead of push back avoids unnecessary copy
    std::cout << "vec has length " << vec_ptr->size() << ", number of owner:" <<
        vec_ptr.use_count() << "\n";

    std::cout << "fill_vec1 is starting" << std::endl;
    const int total_past_owners = fill_vec1(vec_ptr);
    std::cout << "fill_vec1 has ended" << std::endl;

    std::cout << "vec has length " << vec_ptr->size() << ", current number of owners:" <<
        vec_ptr.use_count() << "\n";

    std::cout<< "vec has length "<< vec_ptr->size() << " content "<< *vec_ptr <<"\n";
    return total_past_owners;
}

#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_ownership6") {
    std::cout << "\ntest_ownership6\n";
    const int v = test_ownership6();
    REQUIRE(v == 4);
}
