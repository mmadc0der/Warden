#include "cli/cli.hpp"
#include "core/manager.hpp"
#include "core/storage.hpp"
#include <filesystem>
#include <iostream>
#include <memory>

int main() {
    try {
        auto data_dir = std::filesystem::current_path() / "data";
        auto storage = std::make_shared<warden::core::Storage>(data_dir);
        auto manager = std::make_shared<warden::core::PasswordManager>(storage);
        
        warden::cli::CLI cli(manager);
        cli.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
