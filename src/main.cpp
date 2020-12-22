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
#include "./gitty.hpp"

using namespace cppgit2;
using namespace ftxui;
using namespace std;
using namespace gitty;

/* TODO:
    - Basic refactoring
    - Split code across multiple files
    - Implement method to reload file-lists after rendering tui
    - Extensibility
 */

vector<File> updateFilelist(repository &repo);

int main()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string cwd(buff);
    cout << "Current Working Directory " << cwd << endl;

    auto repo = repository::open(cwd);

    auto screen = ScreenInteractive::Fullscreen();

    FileTracker filetracker(&repo);
    GitCommandLine commandline;

    Gitty gt(filetracker, commandline);

    screen.Loop(&gt);

    return EXIT_SUCCESS;
}

GitCommandLine::GitCommandLine(/* repository repo */) 
{ 
    Add(&container);
}

Element GitCommandLine::Render()
{
    inputbox.placeholder = L"add .";
    inputbox.on_enter = [this] {
        // Execute commands
        inputbox.content = L"";
    };
    container.Add(&inputbox);
    return hbox({
        text(L"git >>> "),
        hbox(container.Render())
    });
}

FileTracker::FileTracker(repository *repo) 
{
    _repo = *repo;
    Add(&container);
}

Element FileTracker::Render()
{
    trackerFilelist = updateFilelist(_repo);
    for (File &file : trackerFilelist)
    {
        files.push_back(text(to_wstring(file.path)));
        //cout << file.path << endl;
    }
    return vbox({window(
        text(L" Untracked Files "),
        vbox(move(files))
        ),
    });
}

Gitty::Gitty(Component &filetracker, Component &cli)
{
    Add(&main_container);
    main_container.Add(&filetracker);
    main_container.Add(&cli);
}

Element Gitty::Render()
{
    return vbox({
        text(L"Gitty Git TUI") | bold | hcenter,
        main_container.Render(),
    });
}

vector<File> updateFilelist(repository &repo)
{
    vector<File> _trackedFiles;
    vector<File> _ignoredFiles;
    repo.for_each_status([&](const string &path, status::status_type status_flags) {
        File repoFile;
        repoFile.path = path;
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::ignored) !=
                status::status_type::ignored)
            {
                repoFile.status = "modified";
                _trackedFiles.push_back(repoFile);
            }
            else
            {
                repoFile.status = "ignored";
                _ignoredFiles.push_back(repoFile);
            }
        }
    });

    auto index = repo.index();
    auto diff = repo.create_diff_index_to_workdir(index);
    diff.for_each(
        [&](const diff::delta &delta, float progress) {
            auto status = delta.status();
            for (File &file : _trackedFiles)
            {
                if (file.path == delta.new_file().path() || file.path == delta.old_file().path())
                {
                    switch (status) {
                        case diff::delta::type::added:
                            file.status = "added";
                            break;
                        case diff::delta::type::deleted:
                            file.status = "deleted";
                            break;
                        case diff::delta::type::renamed:
                            file.status = "renamed";
                            break;
                        case diff::delta::type::untracked:
                            file.status = "untracked";
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    );

    return _trackedFiles;/* , _ignoredFiles; */
}
 