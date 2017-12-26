cmake_minimum_required(VERSION 3.2)

set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})

project(allocator VERSION 0.0.${PROJECT_VERSION_PATCH})

configure_file(version.h.in version.h @ONLY)

set(SRC
    main.cpp
    small_obj_allocator.cpp
    chunk.cpp
    fixed_allocator.cpp
    global_pool.cpp
    small_obj_alloc_thread_safe.cpp
    )

set(HDR
    my_allocator.h
    my_list.h
    small_obj_allocator.h
    chunk.h
    fixed_allocator.h
    global_pool.h
    small_obj_alloc_thread_safe.h
    )

#Program allocator
add_executable(allocator ${SRC} ${HDR})

set_target_properties(allocator PROPERTIES
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}
)

#Дополнительный свойста компилятора для всех проектов
set_target_properties( allocator PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    #COMPILE_OPTIONS -Wall
)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT aquila-2004@yandex.ru)

include(CPack)
