cmake_minimum_required (VERSION 3.11)

project(
  gitty
  LANGUAGES CXX  
)

add_subdirectory(src)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME ftxui
  GITHUB_REPOSITORY ArthurSonzogni/ftxui
  GIT_TAG master
)

CPMAddPackage(
  NAME git2
  GITHUB_REPOSITORY libgit2/libgit2
  VERSION 1.1.0
)
