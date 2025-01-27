cmake_minimum_required(VERSION 3.15)
project(warden VERSION 0.1.0 LANGUAGES CXX)

message(STATUS "Configuring ${PROJECT_NAME} version ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

# Подключаем FetchContent для загрузки зависимостей
include(FetchContent)
message(STATUS "Downloading dependencies...")

# Загружаем nlohmann/json
message(STATUS "Fetching nlohmann/json...")
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)
message(STATUS "nlohmann/json configured successfully")

# Опции сборки
option(WARDEN_BUILD_TESTS "Build tests" OFF)

message(STATUS "Configuring main executable...")

# Основной исполняемый файл
add_executable(warden
    src/main.cpp
    src/cli/cli.cpp
    src/core/manager.cpp
    src/core/storage.cpp
    src/utils/generator.cpp
)

# Включаем директории с заголовочными файлами
target_include_directories(warden PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Подключаем зависимости
target_link_libraries(warden PRIVATE nlohmann_json::nlohmann_json)

# Настройка компилятора
if(MSVC)
    target_compile_options(warden PRIVATE /W4)
    message(STATUS "Using MSVC compiler with /W4 warnings")
else()
    target_compile_options(warden PRIVATE -Wall -Wextra -Wpedantic)
    message(STATUS "Using GCC/Clang compiler with all warnings")
endif()

# Установка
install(TARGETS warden
    RUNTIME DESTINATION bin
)

message(STATUS "Configuration completed successfully")
