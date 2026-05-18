#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <unordered_map>

// security4.cpp

// Security classification: CWE-119 (buffer overflow) / CWE-129 (Improper
// Validation of Array Index), driven by CWE-194 (sign extension) and CWE-192
// (integer coercion).
//
// The danger to RECOGNISE here is in the index types. set_song takes an
// `unsigned short` index and get_song_name takes an `unsigned char` index,
// while the tests call them with values like 256 and -1. Those values are
// silently *narrowed* to fit the small types:
//   * get_song_name(256): 256 wraps to 0 in an unsigned char, so it returns the
//     WRONG song (index 0) instead of the song at index 256.
//   * set_song(name, -1): -1 becomes 65535 in an unsigned short, writing far
//     from where the caller intended.
// The index is never validated against the real array size, so a request that
// looks in-range at the call site can hit the wrong element -- or, on a smaller
// array, land out of bounds (an out-of-bounds read/write, i.e. buffer
// overflow). Build and run the exercise and watch the printed songs/addresses
// to see the wrap-around happen: this is how an integer-conversion bug becomes
// a memory-safety vulnerability.
//
// Fix: the exercise does not build yet under the strict warning flags.
//      operator new[] catches std::bad_alloc *by value*; catching exceptions by
//      value can slice them and forces a copy at the worst possible moment (you
//      are already out of memory). Once it compiles, run it and make sure you
//      can explain every line of the output.


struct Song;

struct Song {
    std::string name_;
    Song() : name_("") {
        std::cout << "Creating empty song\n";
    };
    Song(const std::string & name): name_(name) {};
    Song(const std::string && name) : name_(name) {
        std::cout << "Move song name "<< name_ <<"\n";
    }
    Song(const Song & song) : name_(song.name_) {
        std::cout << "Copy song "<< name_ <<"\n";
    }
    Song& operator=(const Song&) = delete;
    Song& operator=(const Song&& song) {
        name_ = std::move(song.name_);
        std::cout << "Assign-Move song "<< name_ <<"\n";
        return *this;
    }
    Song(const Song && song) : name_(std::move(song.name_)) {
        std::cout << "Move song "<< name_ <<"\n";
    }
    static std::unordered_map<size_t, ptrdiff_t> alloc_tracker;

    static void * operator new[](const size_t num_songs) noexcept {
        Song * s = nullptr;
        try {
            s = ::new Song[num_songs];
            size_t song_address_code = reinterpret_cast<size_t>(s);
            alloc_tracker[song_address_code] = num_songs*sizeof(Song);
            std::cout << "Allocated "<< std::dec << alloc_tracker[song_address_code] <<"B at " << std::hex << song_address_code<<"\n";
        }
        catch(const std::bad_alloc&) {
            std::cout<<"Program is probably running out of memory ..."<<"\n";
        }
        return s;
    }
    static void operator delete[](void * song_ptr) noexcept {
        Song * s = static_cast<Song *>(song_ptr);
        size_t song_address_code = reinterpret_cast<size_t>(s);
        std::cout << "De-allocated "<< std::dec << alloc_tracker[song_address_code] <<"B at " << std::hex << song_address_code<<"\n";
        alloc_tracker.erase(song_address_code);
        ::delete[](s);
    }
};

std::unordered_map<size_t, ptrdiff_t> Song::alloc_tracker;

class MediaPlayer {
    static constexpr size_t default_max_num_songs = 4;
    Song* song_ar;
    MediaPlayer() = delete;
public:
    MediaPlayer(const size_t max_num_songs) {
        song_ar = static_cast<Song*>(Song::operator new[](max_num_songs));
        std::cout<<"Created MediaPlayer song array at " << std::hex << song_ar <<"\n";
    }
    ~MediaPlayer() {
        std::cout<<"Deleting MediaPlayer song array at " << std::hex << song_ar <<"\n";
        Song::operator delete[](song_ar);
    }
    void set_song(std::string name, const unsigned short  index) {
        song_ar[index] = std::move(Song(std::move(name)));
    }
    std::string get_song_name(const unsigned char index) const{
        return song_ar[index].name_;
    }
};


// The tests below intentionally pass out-of-range values (256, -1) to trigger
// the narrowing/wrap-around described at the top. -Woverflow is silenced so the
// demonstration compiles -- you are not meant to edit the tests.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("test_security4_0") {
    MediaPlayer p(300);
    p.set_song("Old MacDonald had a farm", 0);
    p.set_song("When the circus comes to town", 1);
    p.set_song("We are in an orchestra", 256);
    std::cout << "TESTING : " << p.get_song_name(0) << "\n";
    std::cout << "TESTING : " << p.get_song_name(256) << "\n";
    p.set_song("The pirates song", 4);
    REQUIRE(p.get_song_name(0) == "Old MacDonald had a farm");
}

TEST_CASE("test_security4_1") {
    MediaPlayer p(80'000);
    p.set_song("Old MacDonald had a farm", 0);
    p.set_song("When the circus comes to town", 1);
    p.set_song("We are in an orchestra", -1);
    REQUIRE(p.get_song_name(1) == "When the circus comes to town");
}
#pragma GCC diagnostic pop

