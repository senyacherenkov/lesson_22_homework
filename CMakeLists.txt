cmake_minimum_required(VERSION 3.5)

project(bulk_async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_BUILD_TYPE DEBUG)

add_library(async SHARED async.h async.cpp observer.h observer.cpp subject.h subject.cpp utility.h utility.cpp manager.h manager.cpp)
add_executable(bulkmt_async main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set_target_properties(bulkmt_async async PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_link_libraries(bulkmt_async Threads::Threads async)

install(TARGETS bulkmt_async RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT senyacherenkov@yandex.ru)

include (CPack)
