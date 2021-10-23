cmake_minimum_required(VERSION 3.10)

project(cg-vault)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(VAULT_DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)
message(STATUS "VAULT_DEPS_DIR: ${VAULT_DEPS_DIR}")
set(VAULT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

set(BUILD_VAULT_LIB ON)
if (BUILD_VAULT_LIB)
    add_subdirectory(${VAULT_SRC_DIR}/vault-lib)
endif()
