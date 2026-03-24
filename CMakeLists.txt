cmake_minimum_required(VERSION 3.15)
project(Meteid VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pastikan path ini benar sesuai tempat kamu simpan Raylib MSVC
set(RAYLIB_DIR "C:/raylib_msvc")

include_directories(
    "${RAYLIB_DIR}/include"
    "src/includes"
    "src/includes/utils"
    "src/includes/utils/gui"
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
# Tambahkan ini setelah baris add_executable

if(MSVC)
    # Flag ini bakal matiin CMD/Console hanya kalau kamu pilih mode "Release"
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif()

# Kita tidak pakai target_compile_definitions yang aneh-aneh lagi
# Kita akan benerin raygui langsung di file .cpp nya saja

target_link_libraries(${PROJECT_NAME} 
    "${RAYLIB_DIR}/lib/raylib.lib"
    winmm
    gdi32
    opengl32
    shell32
)

# Tentukan nama folder folder rilis
set(RELEASE_DIR "${CMAKE_SOURCE_DIR}/Meteid_Release")

# Perintah untuk copy file .exe dan folder assets secara otomatis setelah build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "${RELEASE_DIR}/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/src/assets" "${RELEASE_DIR}/src/assets"
    COMMENT "Menyiapkan folder rilis di: ${RELEASE_DIR}"
)

# Tambahkan ini di paling bawah CMakeLists.txt
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/src/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/src/assets"
    COMMENT "Copying assets to build directory so the game can find them!"
)