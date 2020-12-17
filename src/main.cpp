#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif __linux__
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// cppgit2
#include "cppgit2/repository.hpp"

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
#include <iostream>
#include <string>
#include <stdexcept>

// Custom
#include "./gitty.h"

using namespace cppgit2;
using namespace ftxui;
using namespace std;
using namespace gitty;

string cwd;
vector<File> unstagedFiles;
vector<File> ignoredFiles;
// vector<File> stagedFiles;

/* TODO:
    - Basic refactoring
    - Split code across multiple files
    - Implement method to reload file-lists after rendering tui
 */

int main()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string cwd(buff);
    // cout << "Current Working Directory " << cwd << endl;

    repository repo = repository::init(cwd, false);

    auto screen = ScreenInteractive::Fullscreen();

    // Components
    vector<Component> components;
    FileTracker filetracker;
    components.push_back(filetracker);

    GitCommandLine cli;
    components.push_back(cli);

    Gitty gt(components);
    unstagedFiles, ignoredFiles = gt.update(repo);

    screen.Loop(&gt);

    return EXIT_SUCCESS;
}

GitCommandLine::GitCommandLine()
{
    Add(&cli);
}

Element GitCommandLine::Render()
{
    commandinput.placeholder = L"add .";
    commandinput.on_enter = [this] {
        commandinput.content = L"";
    };
    cli.Add(&commandinput);
    return hbox({text(L"git >>> "),
                 hbox(cli.Render()),
                 text(to_wstring(result))});
}

// StagedFiles Component
/* StagedFiles::StagedFiles()
{
    for (File &file : stagedFiles)
    {

        elements.push_back(text(to_wstring(file.path)) | style);
    }
}

Element StagedFiles::Render()
{
    return vbox({window(text(L" Staged Files "),
                        vbox(move(elements)))});
} */

FileTracker::FileTracker()
{
    Add(&container);
}

Element FileTracker::Render()
{
    for (File &file : unstagedFiles)
    {
        CheckBox cb;
        cb.label = to_wstring(file.path);
        cb.state = false;
        container.Add(&cb);
    }
    return vbox({window(
        text(L" Untracked Files "),
        vbox(container.Render()))});
}

Gitty::Gitty(vector<Component> components)
{
    Add(&main_container);
    for (Component &cp : components)
    {
        main_container.Add(&cp);
    }
}

Element Gitty::Render()
{
    return vbox({
        text(L"Gitty Git TUI") | bold | hcenter,
        main_container.Render(),
    });
}

// Gitty Wrapper
vector<File> Gitty::update(repository repo)
{
    repo.for_each_status([&](const string &path, status::status_type status_flags) {
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::ignored) !=
                status::status_type::ignored)
            {
                File mod;
                mod.path = path;
                // mod.path = to_wstring(path);
                mod.status = status::status_type::index_modified;
                unstagedFiles.push_back(mod);
            }
            else
            {
                File ign;
                ign.path = path;
                // ign.path = to_wstring(path);
                ign.status = status::status_type::ignored;
                ignoredFiles.push_back(ign);
            }
        }
    });

    return unstagedFiles, ignoredFiles;
}