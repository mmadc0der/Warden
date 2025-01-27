#pragma once
#include <memory>
#include <string>
#include <vector>
#include "core/manager.hpp"

namespace warden {
namespace cli {

class CLI {
public:
    explicit CLI(std::shared_ptr<core::PasswordManager> manager);
    void run();

private:
    void processCommand(const std::string& line);
    std::vector<std::string> splitCommand(const std::string& line);
    
    // Command handlers
    void handleAdd(const std::vector<std::string>& args);
    void handleGet(const std::vector<std::string>& args);
    void handleList(const std::vector<std::string>& args);
    void handleDelete(const std::vector<std::string>& args);
    void handleGenerate(const std::vector<std::string>& args);
    void printHelp();

    std::shared_ptr<core::PasswordManager> manager_;
    bool running_;
};

} // namespace cli
} // namespace warden
