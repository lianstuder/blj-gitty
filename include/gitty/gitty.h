
#pragma once

#include <cppgit2/repository.hpp>

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
        std::wstring path;
        cppgit2::status::status_type status;
    };

    class StagedFiles : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Vertical();
        std::vector<ftxui::Element> elements;
        std::vector<gitty::File> stagedFiles;

    public:
        ~StagedFiles() override {}
        StagedFiles();
        ftxui::Element Render() override;
    };

    class FileTracker : public ftxui::Component
    {
    private:
        ftxui::Container container = ftxui::Container::Vertical();
        std::vector<gitty::File> unstagedFiles;
        std::vector<gitty::File> ignoredFiles;

    public:
        ~FileTracker() override {}
        FileTracker();
        ftxui::Element Render() override;
    };

    class GitCommandLine : public ftxui::Component
    {
    private:
        ftxui::Input commandinput;
        std::wstring command;

    public:
        ~GitCommandLine() override {}
        GitCommandLine();
        ftxui::Element Render() override;
    };

    class Gitty : public ftxui::Component
    {
    private:
        ftxui::Container main_container = ftxui::Container::Vertical();

        gitty::FileTracker ft;
        gitty::StagedFiles sf;
        gitty::GitCommandLine cli;

    public:
        ~Gitty() override {}
        Gitty(repository repo);
        ftxui::Element Render() override;
        std::vector<gitty::File> update(cppgit2::repository);
    };
} // namespace gitty
