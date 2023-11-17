# Project Name

## Description

This is a Zig language project that grew out of wondering if I could do anagram searching faster or at least differently than a tree based solution I made in C. In this apporach, I convert each word to a 26 bit integer where each bit represents the presence of a letter in the word. For example, the word "cat" would be converted to the integer 0b00000100000000000000000101. This integer is then used to check if a word is present in a list of words. This is used to prune the list of words to check further. The pruned list is then sorted and checked using a naive approach of checking the distribution of letters in the word compared to the needle.

## Performance

The following was measured on an Apple M2 on the dictionary of 348,244 words in this repository. Both my old C code (in the `c` directory) and this Zig code were compiled with ReleaseFast. The query used was "ENCYCLOPEDIA":

| Method              | Time (ms) |
| ------------------- | --------- |
| C Tree              | 174.23    |
| Zig binary encoding | 29.79     |

## Installation

To install this project, you need to have Zig installed on your machine. Once you have Zig installed, you can clone this repository and build the project using the `zig build`

## Usage

`zig build run -- -d <dictionary> -q <query>`

## Contributing

If you want to contribute or mock this project, please create a pull request with your changes.

## License

This project is licensed under the [MIT License](LICENSE).
