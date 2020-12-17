ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

find_library(CPPGIT2_LIBRARY cppgit2 HINTS ./build/lib)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    cppgit2
    ftxui::dom
    ftxui::screen
    ftxui::component
)

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)