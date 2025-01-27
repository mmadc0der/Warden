#include "core/manager.hpp"
#include <nlohmann/json.hpp>

namespace warden {
namespace core {

PasswordManager::PasswordManager(std::shared_ptr<Storage> storage)
    : storage_(std::move(storage)) {}

bool PasswordManager::createEntry(const std::string& service,
                                const std::string& username,
                                const std::string& password) {
    nlohmann::json entry = {
        {"service", service},
        {"username", username},
        {"password", password}
    };

    std::string json_str = entry.dump();
    std::vector<uint8_t> data(json_str.begin(), json_str.end());
    
    return storage_->write(service, data);
}

std::optional<PasswordEntry> PasswordManager::getEntry(const std::string& service) {
    auto data = storage_->read(service);
    if (!data) {
        return std::nullopt;
    }

    try {
        std::string json_str(data->begin(), data->end());
        auto entry = nlohmann::json::parse(json_str);

        return PasswordEntry{
            entry["service"].get<std::string>(),
            entry["username"].get<std::string>(),
            entry["password"].get<std::string>()
        };
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool PasswordManager::updateEntry(const std::string& service,
                                const std::string& username,
                                const std::string& password) {
    nlohmann::json entry = {
        {"service", service},
        {"username", username},
        {"password", password}
    };

    std::string json_str = entry.dump();
    std::vector<uint8_t> data(json_str.begin(), json_str.end());
    
    return storage_->update(service, data);
}

bool PasswordManager::deleteEntry(const std::string& service) {
    return storage_->remove(service);
}

std::vector<std::string> PasswordManager::listEntries() {
    return storage_->listKeys();
}

} // namespace core
} // namespace warden
