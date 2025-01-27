#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <filesystem>

namespace warden {
namespace core {

class Storage {
public:
    static constexpr std::size_t BLOCK_SIZE = 4096;  // Fixed block size in bytes

    explicit Storage(const std::filesystem::path& data_dir);
    ~Storage();

    // Запрещаем копирование и перемещение
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;

    bool write(const std::string& key, const std::vector<uint8_t>& data);
    std::optional<std::vector<uint8_t>> read(const std::string& key);
    bool update(const std::string& key, const std::vector<uint8_t>& data);
    bool remove(const std::string& key);
    std::vector<std::string> listKeys();

private:
    void loadIndex();
    void saveIndex();
    std::filesystem::path getBlockPath(const std::string& block_id);

    std::filesystem::path data_dir_;
    std::filesystem::path index_path_;
    std::unordered_map<std::string, std::string> index_;  // key -> block_id
};

} // namespace core
} // namespace warden
