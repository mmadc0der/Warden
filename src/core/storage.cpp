#include "core/storage.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace warden {
namespace core {

Storage::Storage(const std::filesystem::path& data_dir)
    : data_dir_(data_dir), index_path_(data_dir / "index.json") {
    std::filesystem::create_directories(data_dir_);
    loadIndex();
}

Storage::~Storage() {
    saveIndex();
}

void Storage::loadIndex() {
    if (!std::filesystem::exists(index_path_)) {
        index_.clear();
        return;
    }

    try {
        std::ifstream file(index_path_);
        nlohmann::json j;
        file >> j;
        index_ = j.get<std::unordered_map<std::string, std::string>>();
    } catch (const std::exception&) {
        index_.clear();
    }
}

void Storage::saveIndex() {
    nlohmann::json j(index_);
    std::ofstream file(index_path_);
    file << j.dump(2);
}

std::filesystem::path Storage::getBlockPath(const std::string& block_id) {
    return data_dir_ / ("block_" + block_id + ".dat");
}

bool Storage::write(const std::string& key, const std::vector<uint8_t>& data) {
    if (index_.find(key) != index_.end()) {
        return false;
    }

    // Создаем ID блока из текущего количества блоков
    std::string block_id = std::to_string(index_.size());
    auto block_path = getBlockPath(block_id);

    // Подготавливаем данные с паддингом до размера блока
    std::vector<uint8_t> padded_data(BLOCK_SIZE, 0);
    std::copy(data.begin(), 
             data.begin() + std::min(data.size(), BLOCK_SIZE),
             padded_data.begin());

    // Записываем блок
    std::ofstream block_file(block_path, std::ios::binary);
    if (!block_file) {
        return false;
    }

    block_file.write(reinterpret_cast<const char*>(padded_data.data()), BLOCK_SIZE);
    if (!block_file) {
        return false;
    }

    // Обновляем индекс
    index_[key] = block_id;
    saveIndex();
    return true;
}

std::optional<std::vector<uint8_t>> Storage::read(const std::string& key) {
    auto it = index_.find(key);
    if (it == index_.end()) {
        return std::nullopt;
    }

    auto block_path = getBlockPath(it->second);
    std::ifstream block_file(block_path, std::ios::binary);
    if (!block_file) {
        return std::nullopt;
    }

    std::vector<uint8_t> data(BLOCK_SIZE);
    block_file.read(reinterpret_cast<char*>(data.data()), BLOCK_SIZE);

    // Удаляем паддинг (нули в конце)
    auto it_end = std::find(data.begin(), data.end(), 0);
    data.erase(it_end, data.end());

    return data;
}

bool Storage::update(const std::string& key, const std::vector<uint8_t>& data) {
    auto it = index_.find(key);
    if (it == index_.end()) {
        return false;
    }

    auto block_path = getBlockPath(it->second);

    // Подготавливаем данные с паддингом
    std::vector<uint8_t> padded_data(BLOCK_SIZE, 0);
    std::copy(data.begin(), 
             data.begin() + std::min(data.size(), BLOCK_SIZE),
             padded_data.begin());

    // Записываем блок
    std::ofstream block_file(block_path, std::ios::binary);
    if (!block_file) {
        return false;
    }

    block_file.write(reinterpret_cast<const char*>(padded_data.data()), BLOCK_SIZE);
    return static_cast<bool>(block_file);
}

bool Storage::remove(const std::string& key) {
    auto it = index_.find(key);
    if (it == index_.end()) {
        return false;
    }

    auto block_path = getBlockPath(it->second);
    
    try {
        std::filesystem::remove(block_path);
        index_.erase(it);
        saveIndex();
        return true;
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}

std::vector<std::string> Storage::listKeys() {
    std::vector<std::string> keys;
    keys.reserve(index_.size());
    for (const auto& [key, _] : index_) {
        keys.push_back(key);
    }
    return keys;
}

} // namespace core
} // namespace warden
