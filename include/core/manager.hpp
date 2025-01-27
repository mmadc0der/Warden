#pragma once
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include "core/storage.hpp"

namespace warden {
namespace core {

struct PasswordEntry {
    std::string service;
    std::string username;
    std::string password;
};

class PasswordManager {
public:
    explicit PasswordManager(std::shared_ptr<Storage> storage);

    bool createEntry(const std::string& service, 
                    const std::string& username, 
                    const std::string& password);

    std::optional<PasswordEntry> getEntry(const std::string& service);
    
    bool updateEntry(const std::string& service, 
                    const std::string& username, 
                    const std::string& password);

    bool deleteEntry(const std::string& service);
    
    std::vector<std::string> listEntries();

private:
    std::shared_ptr<Storage> storage_;
};

} // namespace core
} // namespace warden
