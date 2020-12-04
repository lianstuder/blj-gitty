
add_executable(gitty main.cpp)


target_include_directories(${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include)
 

target_link_libraries(gitty
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

set_target_properties(gitty PROPERTIES CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")