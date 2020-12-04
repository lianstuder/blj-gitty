#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main(void)
{
    using namespace ftxui;

    // Define the document
    Element document =
        hbox({
            text(L"left") | border,
            text(L"middle") | border | flex,
            text(L"right") | border,
        });

    auto screen = Screen::Create(
        Dimension::Full(),       // Width
        Dimension::Fit(document) // Height
    );
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;

    return EXIT_SUCCESS;
}