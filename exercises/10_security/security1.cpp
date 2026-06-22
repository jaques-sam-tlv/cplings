#include <iostream>
#include <string>
#include <array>

// security1.cpp

// Security classification: CWE-416 (Use After Free).
//
// convert_to_C_style_string() is meant to hand back a C-style string
// (const char*) for a song name, and the test checks that the returned pointer
// is still usable. The problem: get_song_name() returns a std::string *by
// value*, which produces a temporary string. Calling .c_str() on that
// temporary yields a pointer into memory that is freed the instant the
// temporary is destroyed (at the end of the full expression). So
// convert_to_C_style_string returns a dangling pointer into already-freed
// memory -- a use-after-free. Reading through such a pointer is undefined
// behaviour and a classic, exploitable memory-safety bug (the address
// sanitizer this project builds with can flag it).
//
// Fix: make the characters the returned pointer refers to outlive the call, so
//      the pointer is still valid when the test inspects it. Think about
//      *where* the song text lives and how long it lasts.
//
// Note: the test compares two const char* with ==, i.e. it compares pointers
//       (addresses), not contents (no strcmp). Your fix must therefore return a
//       pointer to the very same characters the song was created from.


struct Song {
    std::string name_;
    Song() : name_("") {};
    Song(const std::string & name) : name_(name) {};
};
class MediaPlayer {
    static constexpr size_t max_num_songs = 4;
    std::array<Song, max_num_songs> song_ar;
public:
    MediaPlayer() {}
    void set_song(std::string name, int index) {
        song_ar[index] = Song(name);
    }
    std::string get_song_name(int index) const {
        return song_ar[index].name_;
    }
};

// Fix unsafe function
const char * convert_to_C_style_string(MediaPlayer & p, int index) {
    return p.get_song_name(index).c_str();
}

// The test below compares const char* with ==, which compares pointers
// (addresses), not contents. -Waddress would reject comparing against a string
// literal, so it is silenced here -- you are not meant to edit the test.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_security1_0") {
    MediaPlayer p;
    p.set_song("Old MacDonald had a farm", 0);
    p.set_song("When the circus comes to town", 1);
    REQUIRE(p.get_song_name(0) == "Old MacDonald had a farm");
    const char * second_song = convert_to_C_style_string(p, 0);
    REQUIRE(second_song == "Old MacDonald had a farm");
}
#pragma GCC diagnostic pop
