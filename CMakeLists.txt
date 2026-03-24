cmake_minimum_required(VERSION 3.15)
project(Meteid VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. OTOMATISASI RAYLIB (FETCHCONTENT) ---
include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG         master 
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES     OFF CACHE BOOL "" FORCE)
set(WITH_PIC       ON  CACHE BOOL "" FORCE) 

FetchContent_MakeAvailable(raylib)

# --- 2. SETTING INCLUDE & SOURCES ---
include_directories(
    "src/includes"
    "src/includes/utils"
    "src/includes/utils/gui"
)

file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")

# --- 3. DEFINISI TARGET ---

if(PLATFORM STREQUAL "Android")
    set(NATIVE_APP_GLUE_DIR "${CMAKE_ANDROID_NDK}/sources/android/native_app_glue")
    include_directories(${NATIVE_APP_GLUE_DIR})

    add_library(${PROJECT_NAME} SHARED ${SOURCES} "${NATIVE_APP_GLUE_DIR}/android_native_app_glue.c")
    add_definitions(-DPLATFORM_ANDROID)
    
    target_link_libraries(${PROJECT_NAME} raylib android log GLESv2 EGL OpenSLES m atomic c)

    # --- Konfigurasi Path Tools ---
    set(SDK_PATH "C:/Android") 
    set(BUILD_TOOLS_VER "34.0.0")
    set(AAPT2 "${SDK_PATH}/build-tools/${BUILD_TOOLS_VER}/aapt2.exe")
    set(ZIPALIGN "${SDK_PATH}/build-tools/${BUILD_TOOLS_VER}/zipalign.exe")
    set(APKSIGNER "${SDK_PATH}/build-tools/${BUILD_TOOLS_VER}/apksigner.bat")
    set(ANDROID_JAR "${SDK_PATH}/platforms/android-34/android.jar")
    set(DEBUG_KS "${CMAKE_CURRENT_SOURCE_DIR}/debug.keystore")
    
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=default)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # 1. Hapus sisa build lama agar bersih
        COMMAND ${CMAKE_COMMAND} -E remove -f Meteid_Unsigned.apk
        COMMAND ${CMAKE_COMMAND} -E remove -f Meteid_Aligned.apk
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets"

        # 2. Bikin APK Kosong (Tanpa flag -A agar AAPT2 tidak merusak path)
        COMMAND ${AAPT2} link -o Meteid_Unsigned.apk 
                -I ${ANDROID_JAR} 
                --manifest "${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml"
                --auto-add-overlay

        # 3. SUNTIK ASSETS (Pake 'jar' biar path-nya fix pakai /)
        # Kita jalankan 'jar' di folder build_android agar struktur 'assets/...' terjaga
        COMMAND jar -uf Meteid_Unsigned.apk assets

        # 4. SUNTIK LIBRARY (.so)
        COMMAND ${CMAKE_COMMAND} -E make_directory "lib/arm64-v8a"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "lib/arm64-v8a/"
        COMMAND jar -uf Meteid_Unsigned.apk lib/arm64-v8a/libMeteid.so

        # 5. ZIPALIGN (Wajib buat Android 13+)
        COMMAND ${ZIPALIGN} -v 4 Meteid_Unsigned.apk Meteid_Aligned.apk

        # 6. SIGNING
        COMMAND ${APKSIGNER} sign --ks "${DEBUG_KS}" 
                --ks-pass pass:android 
                --ks-key-alias androiddebugkey 
                --out Meteid_Debug.apk Meteid_Aligned.apk

        COMMENT "Automated Build: Injecting assets with JAR to fix path separators..."
    )
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
    add_definitions(-DPLATFORM_DESKTOP)
    target_link_libraries(${PROJECT_NAME} raylib winmm gdi32 opengl32 shell32)
endif()