set(LIBDNF_TEST_SOURCES
    ${LIBDNF_TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ModuleProfileTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModulePackageTest.cpp
    PARENT_SCOPE
)

set(LIBDNF_TEST_HEADERS
    ${LIBDNF_TEST_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/ModuleProfileTest.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModulePackageTest.hpp
    PARENT_SCOPE
)
