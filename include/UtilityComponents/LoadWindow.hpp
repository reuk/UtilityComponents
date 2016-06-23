#pragma once

#include "FileDropComponent.hpp"

class LoadWindow : public DocumentWindow {
public:
    LoadWindow(String name,
               DocumentWindow::TitleBarButtons buttons,
               const std::string& file_formats,
               ApplicationCommandManager& command_manager);

    void closeButtonPressed() override;

    const FileDropComponent& get_content() const;
    FileDropComponent& get_content();

    void addListener(FileDropComponent::Listener* l);
    void removeListener(FileDropComponent::Listener* l);

private:
    FileDropComponent content_component{"drop a file here, or",
                                        "click to load"};
};