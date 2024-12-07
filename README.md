# Advent of Code 2023 - C++ Solutions

This repository contains my solutions for [Advent of Code 2024](https://adventofcode.com/2024) implemented in C++. This project serves as my learning journey into C++ programming.

## Lessons Learned

### Day 1

* `g++ file_path.cpp -o file_path` to compile to binaries.
* `./file_path` to run binaries.
* In C++ `unordered_map` is a hash map, not `map`, which is instead a red black tree.
* Basic file I/O using std::ifstream and string manipulation.

### Day 2

* Within the context of streams `<<` is insertion and `>>` is extraction.
* In C++ most things are not hashable by default, this means that if you want to memoise function arguments
  you should reconstruct the arguments into some user defined type which implements a hashing function.
* Bit manipulation can be used to create efficient composite keys.
* std::tuple provides a convenient way to group related values.

### Day 3

* I learnt a useful parsing pattern where you alternate between a "enabled" and "disabled" mode within a while loop.
* Using const references (`const std::string&`) improves performance by avoiding unnecessary copying.
* String parsing requires careful boundary checking and state management.

### Day 4

* If you know the size of your array use `std::array`, otherwise use `std::vector`.
* You can use uniform initialisation {...} for a clear initialisation that is consistent across different types.

## Day 5

* You can use a combination of `stringstream` and `getline(stream, tmp, <delimiter>)` to iterate through some delimited data. When no delimiter is passed the default behaviour is deliminate by lines.
* You can use the pattern `for (item : iterator)` to iterate over the items in an iterator.
* The interface for getting elements from an unordered map changes depending if the unordered map is constant or not. If constant you cannot use `[key]` since this by default will populate the map with a default value if teh `key` is not present.
* Some useful `map` methods like `contains()` are not available below `c++20`, make sure to add `-std=c++23` to your compile command.


### Day 6
* A trick to make int tuple hashes is to xor the tuple elements. Although you should bit shift each index, else hash(x, y) == hash(y, x), i.e. the hash will be permutation invariant.
