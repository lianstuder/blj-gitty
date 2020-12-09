
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  libgit2
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)