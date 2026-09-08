cmake_minimum_required(VERSION 3.22)
project(SongCreatorVST3 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  vst3sdk
  GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
  GIT_TAG v3.7.13_build_20
)
FetchContent_MakeAvailable(vst3sdk)
smtg_enable_vst3_sdk()

set(target SongCreator)
smtg_add_vst3plugin(${target}
    src/factory.cpp
    src/plug.h
    src/plug.cpp
    src/editor.h
    src/editor.cpp
)

target_include_directories(${target} PRIVATE ${vst3sdk_SOURCE_DIR})
target_link_libraries(${target} PRIVATE Shlwapi Ole32 OleAut32)

if(SMTG_WIN)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/../Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/Resources/index.html" "$<TARGET_FILE_DIR:${target}>/../Resources/index.html"
  )
endif()
