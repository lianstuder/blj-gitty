#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#elif __linux__
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
    unstagedFiles, ignoredFiles, stagedFiles = gt.update(repo);

    screen.Loop(&gt);

    return EXIT_SUCCESS;
}

// CLI Component
GitCommandLine::GitCommandLine()
{
    Add(&cli);
    commandinput.placeholder = L"add .";
    commandinput.on_enter = [this] {
        commandinput.content = L"";
    };
    cli.Add(&commandinput);
}

Element GitCommandLine::Render()
{
    return hbox({
        text(L"git >>> "),
        hbox(cli.Render()),
    });
}

// StagedFiles Component
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
    return vbox({window(text(L" Project "),
                        vbox(move(elements)))});
}

// FileTracker Component
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
    return vbox({window(
        text(L"Untracked Files"),
        vbox(container.Render()))});
}

// Gitty Wrapper
vector<File> Gitty::update(repository repo)
{
    /* auto index = repo.index();
    auto diff = repo.create_diff_index_to_workdir(index);

    diff.for_each(
        // File Callback
        [&](const diff::delta &delta, float progress) {
            File diff;
            auto status = delta.status();
            switch (status)
            {
            case diff::delta::type::added:
                diff.path = delta.new_file().path();
                diff.status = diff::delta::type::added;
                stagedFiles.push_back(diff);
                break;
            case diff::delta::type::deleted:
                std::cout << "Deleted " << delta.new_file().path() << std::endl;
                break;
            case diff::delta::type::modified:
                std::cout << "Modified " << delta.new_file().path() << std::endl;
                break;
            case diff::delta::type::renamed:
                std::cout << "Renamed " << delta.old_file().path() << " -> "
                          << delta.new_file().path() << std::endl;
                break;
            case diff::delta::type::copied:
                std::cout << "Copied " << delta.new_file().path() << std::endl;
                break;
            case diff::delta::type::untracked:
                std::cout << "Untracked " << delta.new_file().path() << std::endl;
                break;
            default:
                break;
            }
        }); */
    repo.for_each_status([&](const string &path, status::status_type status_flags) {
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::index_modified) == status::status_type::index_modified)
            {
                File mod;
                // mod.path = path;
                mod.path = to_wstring(path);
                mod.status = status::status_type::index_modified;
                unstagedFiles.push_back(mod);
            }
            else if ((status_flags & status::status_type::ignored) == status::status_type::ignored)
            {
                File ign;
                // ign.path = path;
                ign.path = to_wstring(path);
                ign.status = status::status_type::index_modified;
                ignoredFiles.push_back(ign);
            }
            else
            {
                File stg;
                // stg.path = path;
                stg.path = to_wstring(path);
                stg.status = status::status_type::current;
                stagedFiles.push_back(stg);
            }
        }
    });
    return unstagedFiles, ignoredFiles, stagedFiles;
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
    return vbox({
        text(L"Gitty Git TUI") | bold | hcenter,
        main_container.Render(),
    });
}