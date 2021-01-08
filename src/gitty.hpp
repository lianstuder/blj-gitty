
#pragma once

#include "cppgit2/repository.hpp"

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/checkbox.hpp"
#include "ftxui/component/button.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/container.hpp"

#include <iostream>
#include <string>

namespace gitty
{
    class File
    {

    public:
        std::string path;
        std::string status;
    };

    class FileTracker : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Vertical();
        cppgit2::repository _repo;
        cppgit2::index index; 
        std::vector<ftxui::CheckBox> unstagedBoxes;
        std::vector<gitty::File> unstaged;
        std::vector<gitty::File> staged;

        ftxui::Button stageBtn;
        ftxui::Button commitBtn;

    public:
        ~FileTracker() override {}
        FileTracker(cppgit2::repository *repo);
        ftxui::Element Render() override;
    };

    class GitCommandLine : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Horizontal();
        ftxui::Input inputbox;
        std::string result;

    public:
        ~GitCommandLine() override {}
        GitCommandLine();
        ftxui::Element Render() override;
    };

    class Gitty : public ftxui::Component
    {
    private:
        ftxui::Container main_container = ftxui::Container::Vertical();
        
    public:
        ~Gitty() override {}
        Gitty(ftxui::Component &filetracker, ftxui::Component &cli);
        ftxui::Element Render() override;
    };
} // namespace gitty

std::vector<gitty::File> updateFilelist(cppgit2::repository &repo);
int indexOf(std::vector<gitty::File>, gitty::File);
std::string exec(std::string command);