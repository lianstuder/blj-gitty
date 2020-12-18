
#pragma once

#include "cppgit2/repository.hpp"

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/menu.hpp"
#include "ftxui/component/checkbox.hpp"
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

    /* class StagedFiles : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Vertical();
        std::vector<ftxui::Element> elements;
        std::vector<gitty::File> stagedFiles;

    public:
        ~StagedFiles() override {}
        StagedFiles();
        ftxui::Element Render() override;
    }; */

    class FileTracker : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Vertical();

    public:
        ~FileTracker() override {}
        FileTracker();
        ftxui::Element Render() override;
    };

    class GitCommandLine : public ftxui::Component
    {
    private:
        ftxui::Container cli = ftxui::Container::Horizontal();
        ftxui::Input commandinput;
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
        std::vector<ftxui::Component> _components;
        std::vector<gitty::File> trackedFiles;
        std::vector<gitty::File> ignoredFiles;

    public:
        ~Gitty() override {}
        Gitty(std::vector<ftxui::Component> &components);
        ftxui::Element Render() override;
        std::vector<gitty::File> update(cppgit2::repository &repo);
    };
} // namespace gitty
