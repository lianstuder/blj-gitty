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

// C++ Standard
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

using namespace cppgit2;
using namespace ftxui;
using namespace std;

class File
{

public:
    wstring path;
    status::status_type status;
};

class FileTracker : public Component
{
private:
    vector<CheckBox> fileCheckboxes;
    Container container_ = Container::Vertical();

public:
    FileTracker(vector<File> files)
    {
        Add(&container_);
        for (CheckBox &cb : fileCheckboxes)
        {
            for (File &file : files)
            {
                container_.Add(&cb);
                cb.label = file.path;
                cb.state = false;
            }
        }
    }

    Element Render() override
    {
        // Override render method
    }
};

/* TODO:
    - Override FileTracker Render method
    - Display staged files container as separate Component (like FileTracker)
    - Switch to InteractiveScreen (debug)
    - Basic refactoring
    - Split code across multiple files
 */

vector<File> stagedFiles;
vector<File> unstagedFiles;

string cwd;

void updateRepoStatus(auto &repo)
{
    repo.for_each_status([](const string &path, status::status_type status_flags) {
        if ((status_flags & status::status_type::index_modified) != status::status_type::index_modified)
        {
            if ((status_flags & status::status_type::ignored) != status::status_type::ignored)
            {
                File mod;
                mod.path = to_wstring(path);
                mod.status = status::status_type::index_modified;
                stagedFiles.push_back(mod);
                /* cout << "Modified: " << to_string(mod.path) << endl; */
            }
            else
            {
                File ign;
                ign.path = to_wstring(path);
                ign.status = status::status_type::ignored;
                stagedFiles.push_back(ign);
                /* cout << "Ignored: " << to_string(ign.path) << endl; */
            }
        }
    });
}

int main()
{
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    string cwd(buff);

    auto repo = repository::init(cwd, false);

    updateRepoStatus(repo);

    cout << "Current Working Directory " << cwd << endl;

    auto renderFile = [&](const File &file) {
        auto style =
            (file.status == status::status_type::index_modified)
                ? color(Color::Red)
                : color(Color::GrayDark);
        return text(file.path) | style;
    };

    auto render = [&]() {
        vector<Element> elements;
        for (File &file : stagedFiles)
            elements.push_back(renderFile(file));
        return vbox({
            window(text(L" Project "), vbox(move(elements))),
        });
    };

    auto document = render();
    document = document | size(WIDTH, LESS_THAN, 80);

    /*     auto screen = ScreenInteractive::Create(Dimension::Full(), Dimension::Fit(document)); */
    auto screen = ScreenInteractive::TerminalOutput();

    FileTracker ft(stagedFiles);

    Render(screen, document);

    screen.Loop(&ft);
    /*    cout << screen.ToString() << endl; */

    return EXIT_SUCCESS;
}
