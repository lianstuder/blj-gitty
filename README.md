# Gitty

<!-- Badges -->

![GitHub](https://img.shields.io/badge/OS-Linux%20%2F%20WSL-yellow)

All features and more information on Gitty are available in the [the docs](https://github.com/lianstuder/gitty/blob/master/doc/README.md).

## Dependencies

Gitty uses a libgit2 wrapper called [cppgit2](https://github.com/p-ranav/cppgit2) and the [ftxui](https://github.com/ArthurSonzogni/FTXUI) TUI library.
Both libaries are automatically downloaded and linked by CMake. 

## Build from source

To build gitty from source, just run the `build.sh` script in the projects root directory or the following commands:

- `mkdir build`
- `cd build`
- `cmake ..`
- `make`

## License

This project is licensed under the [GNU LGPLv3](https://github.com/lianstuder/blj-gitty/blob/master/LICENSE)
