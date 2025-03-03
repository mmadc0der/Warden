#include "cli/cli.hpp"
#include "utils/generator.hpp"
#include <iostream>
#include <sstream>

namespace warden {
namespace cli {

CLI::CLI(std::shared_ptr<core::PasswordManager> manager)
    : manager_(std::move(manager)), running_(false) {}

void CLI::run() {
    running_ = true;
    std::cout << "Welcome to Warden Password Manager. Type 'help' for commands.\n";

    while (running_) {
        std::cout << "warden> " << std::flush;
        std::string line;
        
        if (std::getline(std::cin, line)) {
            // Удаляем пробелы в начале и конце строки
            line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
            
            if (!line.empty()) {
                processCommand(line);
            }
        } else if (std::cin.eof()) {
            break;
        } else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::vector<std::string> CLI::splitCommand(const std::string& line) {
    std::vector<std::string> args;
    std::string token;
    std::istringstream iss(line);
    
    while (iss >> token) {
        args.push_back(token);
    }
    return args;
}

void CLI::processCommand(const std::string& line) {
    auto args = splitCommand(line);
    if (args.empty()) {
        return;
    }

    std::string command = args[0];
    args.erase(args.begin());

    if (command == "add") {
        handleAdd(args);
    } else if (command == "get") {
        handleGet(args);
    } else if (command == "list") {
        handleList(args);
    } else if (command == "delete") {
        handleDelete(args);
    } else if (command == "generate") {
        handleGenerate(args);
    } else if (command == "help") {
        printHelp();
    } else if (command == "exit" || command == "quit") {
        running_ = false;
        std::cout << "Goodbye!\n";
    } else {
        std::cout << "Unknown command '" << command << "'. Type 'help' for available commands.\n";
    }
}

void CLI::handleAdd(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cout << "Usage: add <service> <username> <password>\n";
        return;
    }

    const auto& service = args[0];
    const auto& username = args[1];
    const auto& password = args[2];

    if (manager_->createEntry(service, username, password)) {
        std::cout << "Password added for service: " << service << "\n";
    } else {
        std::cout << "Service " << service << " already exists\n";
    }
}

void CLI::handleGet(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: get <service>\n";
        return;
    }

    const auto& service = args[0];
    if (auto entry = manager_->getEntry(service)) {
        std::cout << "Service: " << entry->service << "\n"
                 << "Username: " << entry->username << "\n"
                 << "Password: " << entry->password << "\n";
    } else {
        std::cout << "No entry found for service: " << service << "\n";
    }
}

void CLI::handleList(const std::vector<std::string>& args) {
    if (!args.empty()) {
        std::cout << "Usage: list\n";
        return;
    }

    auto services = manager_->listEntries();
    if (services.empty()) {
        std::cout << "No stored services found\n";
        return;
    }

    std::cout << "\nStored services:\n";
    for (const auto& service : services) {
        std::cout << "- " << service << "\n";
    }
}

void CLI::handleDelete(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Usage: delete <service>\n";
        return;
    }

    const auto& service = args[0];
    if (manager_->deleteEntry(service)) {
        std::cout << "Deleted password for service: " << service << "\n";
    } else {
        std::cout << "No entry found for service: " << service << "\n";
    }
}

void CLI::handleGenerate(const std::vector<std::string>& args) {
    size_t length = 16;
    if (!args.empty()) {
        try {
            length = std::stoul(args[0]);
        } catch (const std::exception&) {
            std::cout << "Invalid length specified\n";
            return;
        }
    }

    if (length < 8 || length > 128) {
        std::cout << "Password length must be between 8 and 128 characters\n";
        return;
    }

    std::cout << "Generated password: " << utils::generatePassword(length) << "\n";
}

void CLI::printHelp() {
    std::cout << "\nAvailable commands:\n"
              << "  add <service> <username> <password>  Add a new password entry\n"
              << "  get <service>                        Retrieve a password entry\n"
              << "  list                                 List all stored services\n"
              << "  delete <service>                     Delete a password entry\n"
              << "  generate [length]                    Generate a secure password\n"
              << "  help                                 Show this help message\n"
              << "  exit                                 Exit the program\n\n";
}

} // namespace cli
} // namespace warden
