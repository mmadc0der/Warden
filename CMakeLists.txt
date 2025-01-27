cmake_minimum_required(VERSION 3.15)
project(warden VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции сборки
option(WARDEN_BUILD_TESTS "Build tests" OFF)

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

# Настройка компилятора
if(MSVC)
    target_compile_options(warden PRIVATE /W4)
else()
    target_compile_options(warden PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Установка
install(TARGETS warden
    RUNTIME DESTINATION bin
)
