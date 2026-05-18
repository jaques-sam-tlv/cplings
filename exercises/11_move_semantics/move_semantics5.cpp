#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <utility>

// move_semantics5.cpp
// Make me compile and pass the tests!

// The Rule of Five: if you define any one of {destructor, copy constructor,
// copy assignment, move constructor, move assignment}, you should define all five.
// Defining a destructor suppresses the implicit move operations in C++11 and later.
//
// The StringBuffer class below manages a heap-allocated C-string.
// It has a constructor and destructor but is MISSING all four copy/move operations.
//
// Step 1: Add a copy constructor that allocates a new buffer and copies the content.
// Step 2: Add a copy assignment operator (handle self-assignment, free old memory).
// Step 3: Add a move constructor that transfers buf_ and len_, and nulls out the source.
// Step 4: Add a move assignment operator (free current, transfer, null source).

class StringBuffer {
    char* buf_;
    std::size_t len_;
public:
    explicit StringBuffer(const std::string& s)
        : buf_(new char[s.size() + 1]), len_(s.size()) {
        std::copy(s.begin(), s.end(), buf_);
        buf_[len_] = '\0';
    }

    ~StringBuffer() { delete[] buf_; }

    StringBuffer(const StringBuffer& other)
        : buf_(new char[other.len_ + 1]), len_(other.len_) {
        std::copy(other.buf_, other.buf_ + len_ + 1, buf_);
    }

    StringBuffer& operator=(const StringBuffer& other) {
        if (this != &other) {
            delete[] buf_;
            len_ = other.len_;
            buf_ = new char[len_ + 1];
            std::copy(other.buf_, other.buf_ + len_ + 1, buf_);
        }
        return *this;
    }

    StringBuffer(StringBuffer&& other) noexcept
        : buf_(other.buf_), len_(other.len_) {
        other.buf_ = nullptr;
        other.len_ = 0;
    }

    StringBuffer& operator=(StringBuffer&& other) noexcept {
        if (this != &other) {
            delete[] buf_;
            buf_ = other.buf_;
            len_ = other.len_;
            other.buf_ = nullptr;
            other.len_ = 0;
        }
        return *this;
    }

    std::size_t size() const { return len_; }
    const char* c_str() const { return buf_; }
    char* data() const { return buf_; }
};


#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_move_semantics5_copy_constructor") {
    StringBuffer original("hello");
    StringBuffer copy(original);

    REQUIRE(copy.size() == 5);
    REQUIRE(std::string(copy.c_str()) == "hello");
    REQUIRE(copy.data() != original.data()); // deep copy — different pointer
}

TEST_CASE("test_move_semantics5_copy_assignment") {
    StringBuffer a("foo");
    StringBuffer b("longer string");
    b = a;
    REQUIRE(std::string(b.c_str()) == "foo");
    REQUIRE(b.data() != a.data());
}

TEST_CASE("test_move_semantics5_move_constructor") {
    StringBuffer original("world");
    char* ptr = original.data();

    StringBuffer moved(std::move(original));
    REQUIRE(moved.size() == 5);
    REQUIRE(std::string(moved.c_str()) == "world");
    REQUIRE(moved.data() == ptr);     // same pointer — no allocation
    REQUIRE(original.data() == nullptr);
    REQUIRE(original.size() == 0);
}

TEST_CASE("test_move_semantics5_move_assignment") {
    StringBuffer a("bar");
    StringBuffer b("baz");
    char* a_ptr = a.data();

    b = std::move(a);
    REQUIRE(std::string(b.c_str()) == "bar");
    REQUIRE(b.data() == a_ptr);
    REQUIRE(a.data() == nullptr);
}
