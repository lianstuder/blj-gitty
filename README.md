# Gitty

<!-- Badges -->

![GitHub](https://img.shields.io/badge/OS-Linux%20%2F%20WSL-yellow)

All features and more information on Gitty are available in the [the docs](https://github.com/lianstuder/blob/master/docs.md).

## Dependencies

Gitty requires one dependency to be added manually if you want to build this project from source.
It requires the [cppgit2](https://github.com/p-ranav/cppgit2) library to be built from source and manually added like this:

In the projects root dir create a folder called `build` and create the directories `include`, `lib`, `samples` and `test` inside the build folder.

Build the cppgit2 library and paste the following contents from the build directory that got created after building the library into the appropriate directory inside the build folder in your project.

```
include/
├── cppgit2/
├── git2/
└── git2.h
lib/
├── libcppgit2.so -> libcppgit2.so.1
├── libcppgit2.so.0.1.0
├── libcppgit2.so.1 -> libcppgit2.so.0.1.0
├── libcppgit2.static.a
├── libgit2_clar
├── libgit2.pc
├── libgit2.so -> libgit2.so.0
├── libgit2.so.1.0.0
├── libgit2.so.0 -> libgit2.so.1.0.0
└── ...
samples/
test/
```

## License

This project is licensed under the [MIT License](https://github.com/lianstuder/blj-gitty/blob/master/LICENSE)
