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
    - Commit files
    - Reset tracker after staging files
    - Git functionallity (not only tui functions)
 */


int main()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string cwd(buff);
    cout << "Current Working Directory " << cwd << endl;

    repository repo = repository::open(cwd);

    //cout << typeid(repo.index()).name() << endl;

    auto screen = ScreenInteractive::Fullscreen();

    FileTracker filetracker(&repo);
    GitCommandLine commandline;

    Gitty gt(filetracker, commandline);

    screen.Loop(&gt);

    return EXIT_SUCCESS;
}

GitCommandLine::GitCommandLine() 
{ 
    Add(&container);
}

Element GitCommandLine::Render()
{
    inputbox.placeholder = L"add .";
    inputbox.on_enter = [this] {
        string result = exec(to_string(inputbox.content));
        inputbox.content = to_wstring(result);
    };
    container.Add(&inputbox);
    return hbox({
        text(L"gitty $ "),
        hbox(container.Render())
    });
}

FileTracker::FileTracker(repository *repo) 
{
    _repo = *repo;
    Add(&container);
    unstaged = updateFilelist(*repo);

    unstagedBoxes.resize(unstaged.size());
    for (int i=0; i < unstaged.size(); ++i)
    {
        unstagedBoxes.at(i).label = to_wstring(unstaged.at(i).path);
        container.Add(&unstagedBoxes.at(i));
    }

    container.Add(&stageBtn);
    stageBtn.label = L"Stage";
    stageBtn.on_click = [&] {
        auto index = _repo.index();
        for (int i = 0; i < unstaged.size(); i++)
        {
            bool added = false;
            for (auto &stagedItem : staged)
            {
                if (unstaged.at(i).path == stagedItem.path)
                    added = true;
            }

            if (unstagedBoxes.at(i).state && !added)
            {
                vector<File>::iterator it = unstaged.begin() + i;
                staged.push_back(unstaged.at(i));
                unstaged.erase(it);
                index.add_entry_by_path(staged.at(i).path);
                index.write();
            }
        }
    };

    container.Add(&commitBtn);
    commitBtn.label = L"Commit";
    commitBtn.on_click = [&] {
        try
        {
            auto index = _repo.index();
            auto author = signature("lianstuder", "lianstuder12@gmail.com");
            auto tree_oid = index.write_tree();
            _repo.create_commit(
                "HEAD", 
                author,
                author,
                "utf-8",
                "test commit",
                _repo.lookup_tree(tree_oid), {}
            );
        }
        catch(const cppgit2::git_exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    };
}

Element FileTracker::Render()
{
    Elements tracker;
    for (auto &it : unstagedBoxes)
        tracker.push_back(it.Render());
    tracker.push_back(stageBtn.Render());

    Elements commiter;
    for (File &file : staged)
        commiter.push_back(text(to_wstring(file.path)));
    commiter.push_back(commitBtn.Render());

    return hbox({
        window(
            text(L" Unstaged Files "),
            vbox(move(tracker))
        ),
        window(
            text(L" Staged Files "),
            vbox(move(commiter))
        )
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
        main_container.Render() | flex_grow,
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

    return _trackedFiles;
}
 
string exec(string command) {
   char buffer[128];
   string result = "";

   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}