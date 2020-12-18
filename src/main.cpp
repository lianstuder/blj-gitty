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

string cwd;
vector<File> trackedFiles;
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
    cout << "Current Working Directory " << cwd << endl;

    repository repo = repository::init(cwd, false);

    auto screen = ScreenInteractive::Fullscreen();

    // Components
    vector<Component> components;
    FileTracker filetracker;
    components.push_back(filetracker);

    GitCommandLine commandline;
    components.push_back(commandline);

    Gitty gt(components);
    trackedFiles, ignoredFiles = gt.update(repo);

    for (File &file : trackedFiles)
        cout << "Unstaged: " << file.path << " " << file.status << endl;

    for (File &file : ignoredFiles)
        cout << "Ignored: " << file.path << endl;

    //screen.Loop(&gt);

    return EXIT_SUCCESS;
}

GitCommandLine::GitCommandLine() { Add(&cli); }

Element GitCommandLine::Render()
{
    commandinput.placeholder = L"add .";
    commandinput.on_enter = [this] {
        commandinput.content = L"";
    };
    cli.Add(&commandinput);
    return hbox({
        text(L"git >>> "),
        hbox(cli.Render()),
        text(to_wstring(result)),
    });
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

FileTracker::FileTracker() { Add(&container); }

Element FileTracker::Render()
{
    for (File &file : trackedFiles)
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

Gitty::Gitty(vector<Component> &components)
{
    _components = components;
    Add(&main_container);
}

Element Gitty::Render()
{
    for (Component &cp : _components)
        main_container.Add(&cp);

    return vbox({
        text(L"Gitty Git TUI") | bold | hcenter,
        main_container.Render(),
    });
}

vector<File> Gitty::update(repository &repo)
{
    repo.for_each_status([&](const string &path, status::status_type status_flags) {
        File repoFile;
        repoFile.path = path;
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::ignored) !=
                status::status_type::ignored)
            {
                repoFile.status = "modified";
                trackedFiles.push_back(repoFile);
            }
            else
            {
                repoFile.status = "ignored";
                ignoredFiles.push_back(repoFile);
            }
        }
    });

    for (File &file : trackedFiles)
    {
        cout << file.path << endl;
    }

    auto index = repo.index();
    auto diff = repo.create_diff_index_to_workdir(index);
    diff.for_each(
        [&](const diff::delta &delta, float progress) {
            auto status = delta.status();
            for (File &file : trackedFiles)
            {
                /* cout << file.path << endl;
                cout << delta.new_file().path() << endl; */
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

    return trackedFiles, ignoredFiles;
}
