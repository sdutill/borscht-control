cmake_minimum_required(VERSION 3.10)
project(SQLiteServer C)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/exe)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/exe)

if(MSVC)
    add_compile_options(/MD)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SQLite source path updated to match new location
set(SQLITE_SOURCE
    src/db/sqlite-amalgamation-3470200/sqlite3.c
)

# Common include directories
set(COMMON_INCLUDES
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/db/sqlite-amalgamation-3470200  # Updated SQLite header path
)

# Single executable with all components
add_executable(server_app
    src/main.c
    src/server/server.c
    src/db/sql.c
    ${SQLITE_SOURCE}
)

target_include_directories(server_app PRIVATE ${COMMON_INCLUDES})

target_compile_definitions(server_app PRIVATE 
    SQLITE_ENABLE_RTREE 
    SQLITE_ENABLE_FTS4
    SQLITE_ENABLE_FTS5
    SQLITE_ENABLE_JSON1
    SQLITE_ENABLE_COLUMN_METADATA
)