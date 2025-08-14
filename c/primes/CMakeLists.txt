cmake_minimum_required(VERSION 3.21)

project(primes VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(primes)

file(GLOB primes_files CONFIGURE_DEPENDS "src/**/*.c" "src/*.c")
target_sources(primes PRIVATE ${primes_files})

# Uncomment to add link library
# target_link_libraries(primes PRIVATE #[[ link library here (-lm) ]])

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -fsanitize=address")
    target_link_libraries(primes PRIVATE -fsanitize=address)
endif()
