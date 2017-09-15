find_package(Magnum REQUIRED
        Shaders
        Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

corrade_add_resource(Jorg_RESOURCES resources.conf)

add_executable(jorg JorgApplication.cpp
    TexturedTriangleShader.cpp
    TexturedTriangleShader.h
    ${Jorg_RESOURCES})

target_include_directories(jorg PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(jorg
        Magnum::Application
        Magnum::Magnum
        Magnum::Shaders)
