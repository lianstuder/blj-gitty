#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// cppgit2
#include <cppgit2/repository.hpp>

// FTXUI
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/menu.hpp"
#include "ftxui/component/checkbox.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/input.hpp"

// C++ Standard
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

// Custom
#include <gitty/gitty.h>

using namespace cppgit2;
using namespace ftxui;
using namespace std;
using namespace gitty;

string cwd;
vector<File> unstagedFiles;
vector<File> ignoredFiles;
vector<File> stagedFiles;

/* TODO:
    - Override FileTracker Render method
    - Display staged files container as separate Component (like FileTracker)
    - Switch to InteractiveScreen (debug)
    - Basic refactoring
    - Split code across multiple files
 */

int main()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string cwd(buff);

    repository repo = repository::init(cwd, false);

    cout << "Current Working Directory " << cwd << endl;

    auto screen = ScreenInteractive::Fullscreen();

    Gitty gt;
    unstagedFiles, ignoredFiles = gt.update(repo);
    Element gitty = gt.Render();

    /* thread update([&screen] {
        for (;;)
        {
            using namespace chrono_literals;
            this_thread::sleep_for(0.05s);
            screen.PostEvent(Event::Custom);
        }
    }); */

    screen.Loop(&gt);

    return EXIT_SUCCESS;
}

GitCommandLine::GitCommandLine()
{
    commandinput.placeholder = L"add .";
    commandinput.on_enter = [this] {
        command = commandinput.content;
    };
}

Element GitCommandLine::Render()
{
    return commandinput.Render();
}

StagedFiles::StagedFiles()
{
    for (File &file : stagedFiles)
    {
        auto style = (file.status == status::status_type::index_modified) ? color(Color::Red) : color(Color::GrayDark);
        elements.push_back(text(file.path) | style);
    }
}

Element StagedFiles::Render()
{
    return vbox({window(text(L" Project "), vbox(move(elements)))});
}

FileTracker::FileTracker()
{
    Add(&container);
    for (File &file : unstagedFiles)
    {
        CheckBox cb;
        cb.label = file.path;
        cb.state = false;
        container.Add(&cb);
    }
}

Element FileTracker::Render()
{
    return container.Render();
}

vector<File> Gitty::update(repository repo)
{
    repo.for_each_status([&](const string &path, status::status_type status_flags) {
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::ignored) != status::status_type::ignored)
            {
                File mod;
                mod.path = to_wstring(path);
                mod.status = status::status_type::index_modified;
                unstagedFiles.push_back(mod);
            }
            else
            {
                File ign;
                ign.path = to_wstring(path);
                ign.status = status::status_type::index_modified;
                ignoredFiles.push_back(ign);
            }
        }
    });

    return unstagedFiles, ignoredFiles;
}

Gitty::Gitty()
{
    Add(&main_container);
    main_container.Add(&ft);
    main_container.Add(&sf);
    main_container.Add(&cli);
}

Element Gitty::Render()
{
    return main_container.Render();
}
