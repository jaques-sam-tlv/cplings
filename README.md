![C++ Logo](https://upload.wikimedia.org/wikipedia/commons/1/18/ISO_C%2B%2B_Logo.svg?utm_source=commons.wikimedia.org&utm_campaign=index&utm_content=original)

# cpplings C++❤️

Greetings and welcome to `cpplings`. This project contains small exercises to get you used to reading and writing C++ code. This includes reading and responding to compiler messages!

Alternatively, for a first-time C++ learner, there are several other resources:

- [learncpp.com](https://www.learncpp.com/) - The most comprehensive resource for learning C++, but a bit theoretical sometimes. You will be using this along with cpplings!
- [A Tour of C++, from Bjarne Stroustrup](https://isocpp.org/tour) - Learn C++ by solving little exercises! It's almost like `cpplings`, but online
- [The C++ FAQ](https://isocpp.org/wiki/faq)
- [C++ Core guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

*cpplings got its inspiration from the magnificent [rustlings repository](https://github.com/rust-lang/rustlings), which aims at familiarizing newcomers to the Rust language. This repository is a sort of a fork of it but for the C++ language*


## Getting Started

### Windows

Recommend using WSL and follow the Linux steps.

### Linux

For Ubuntu:

```sh
sudo apt install g++ cmake make gdb git libasan8
```

For Fedora

```sh
sudo dnf install gcc-c++ cmake make gdb git libasan
```

```sh
git clone https://github.com/jaques-sam-tlv/cpplings
cd cpplings
cmake . -B build
cmake --build build [--parallel 8]
```
You should expect the build to fail: your task is to fix each exercise for the build to succeed.


### Doing the exercises

The exercises are sorted by topic and can be found in the subdirectory `cpplings/exercises/<topic>`. For every topic there is an additional README file with some resources to get you started on the topic. We really recommend that you have a look at them before you start.

The task is simple. Most exercises contain an error that keeps them from compiling, and it's up to you to fix it! Some exercises are also run as tests, but cpplings handles them all the same. To run the exercises in the recommended order, execute:


### In Linux
To run all exercises in predetermined order:

(In the `build` directory)
```sh
make
```

This will try to verify the completion of every exercise in a predetermined order (what we think is best for newcomers).

OR, to run a specific exercise, tell make with exercise to run. For example:

```sh
make variable2
```

### In Windows
Visual Studio will let you choose the specific exercise you would like to solve.


## Hints
You will find some help for specific exercises in the hints directory. Please note that this is work in progress.


## Continuing On

Once you've completed cpplings, put your new knowledge to good use! Continue practicing your C++ skills by building your own projects, contributing to cpplings, or finding other open-source projects to contribute to.


## Uninstalling cpplings

If you'd like to uninstall cpplings, you can do so by simply deleting the cpplings directory.

Now you should be done!


## Completion

cpplings isn't done and could still be improved.
- More hints
- Structs and Classes
- Better ownership stuff
- Better safer programming, security stuff
- Threads
- Metaprogramming
- ??? probably more

If you are interested in improving or adding new ones, please feel free to contribute!


## Contributing

Contributions of any kind welcome!


## Some technical notes

The cpplings build system based on CMake and [CPM.make](https://github.com/cpm-cmake/CPM.cmake) for package management

### How to use cmake
https://cliutils.gitlab.io/modern-cmake/

