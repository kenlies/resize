cmake_minimum_required(VERSION 3.28)
project(Resize LANGUAGES CXX) # project name

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(resize src/main.cpp src/helper.cpp) # exexutable name
#add_definitions(-DGL_SILENCE_DEPRECATION) # silence deprecations for mac (seems like this is not needed)
target_link_libraries(resize PRIVATE sfml-graphics sfml-audio) # link smfl
target_compile_features(resize PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET resize
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:resize>
        VERBATIM)
endif()
