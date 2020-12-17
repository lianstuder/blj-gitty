CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  # Specify a GIT TAG here.
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


FetchContent_Declare(cppgit2
  GIT_REPOSITORY https://github.com/p-ranav/cppgit2
  # Specify a GIT TAG here.
)
 
FetchContent_GetProperties(cppgit2)
if(NOT cppgit2_POPULATED)
  FetchContent_Populate(cppgit2)
  add_subdirectory(${cppgit2_SOURCE_DIR} ${cppgit2_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

PROJECT(
    gitty
    LANGUAGES CXX
)

INCLUDE_DIRECTORIES("build/include")

ADD_SUBDIRECTORY("src")