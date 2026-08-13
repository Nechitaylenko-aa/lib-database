cmake_minimum_required(VERSION 3.20)

project(database)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CXX_STANDARD 17)


option(USE_FALLBACK_OPENSSL "Use internal mirror if system OpenSSL lacks Config" OFF)

include(FetchContent)

FetchContent_Declare(
            octo-encryption-cpp
            GIT_REPOSITORY https://github.com/ofiriluz/octo-encryption-cpp.git
            GIT_TAG        8c4a99b29672a33aba766c27e1250ad18c0f0d14
    )

FetchContent_MakeAvailable(octo-encryption-cpp)

get_target_property(OCTO_INC octo-encryption-cpp INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Octo includes: ${OCTO_INC}")

find_package(OpenSSL REQUIRED)
find_package (Threads REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_compile_definitions(
            FreeBSD
    )
    include_directories(/usr/local/include)
    set(lib_path /usr/local/lib)
    set(libraries
            ${lib_path}/mysql/libmysqlclient.so
            ${lib_path}/libpq.so
            )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_compile_definitions(
            Linux
    )
    include_directories(/usr/local/include)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(OS "Windows")

    set(lib_path C:/msys64/mingw64/lib)

endif()

file(GLOB_RECURSE SRC
        src/*.cpp
        my-maria/*.cpp
        postgre/*.cpp
        include/*.cpp
        models/*.cpp
        CTestModel.cpp
        CUserModelDB.cpp
        CUser.cpp
        )

file(GLOB_RECURSE include
        src/*.h
        include/*.h
        my-maria/*.h
        postgre/*.h
        include/*.h
        models/*.h
        )



add_library(${PROJECT_NAME} SHARED

        ${SRC}
        ${include}
        )   # compile "shared.cpp" and create a SHARED library called "shared"

add_library(db::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
        octo-encryption-cpp
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${libraries}
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

include (GenerateExportHeader)


GENERATE_EXPORT_HEADER(${PROJECT_NAME}
        BASE_NAME ${PROJECT_NAME}
        EXPORT_MACRO_NAME shared_EXPORTS
        EXPORT_FILE_NAME shared_EXPORTS.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC
        )

add_executable(test-db
        main.cpp
        )
target_link_libraries(test-db
        ${PROJECT_NAME}
        OpenSSL::SSL
        OpenSSL::Crypto
        Threads::Threads
        ${libraries}
        )
