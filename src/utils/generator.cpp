#include "utils/generator.hpp"
#include <random>
#include <array>

namespace warden {
namespace utils {

std::string generatePassword(size_t length) {
    static const std::string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()_+-=[]{}|;:,.<>?";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string password;
    password.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        password += chars[dis(gen)];
    }

    return password;
}

} // namespace utils
} // namespace warden
