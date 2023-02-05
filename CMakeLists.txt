cmake_minimum_required(VERSION 3.20)

project(jsonParser)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_executable(${PROJECT_NAME} main.cpp  "modules/leptjson/leptjson.ixx" "modules/leptjson/leptjson-exceptions.ixx" "modules/leptjson/leptjson-parser.ixx"  "modules/leptjson/leptjson-parser.cpp" "modules/test/leptjson-parser_test.ixx" "modules/test/leptjson-parser_test.cpp" "modules/test/test_base.ixx" "modules/test/test_base.cpp")


if(0 AND CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    option(ASAN_ENABLED "Build this target with AddressSanitizer" ON)
    if(ASAN_ENABLED)
      if(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address)
      else()
        target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address <additional-options>)
        target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
      endif()

      file(GLOB libs ${CMAKE_SOURCE_DIR}/lib/*.dll)
      foreach(lib ${libs})
          get_filename_component(fname ${lib} NAME)
          file(COPY ${lib} DESTINATION ${CMAKE_BINARY_DIR}/Debug)
      endforeach()

    endif()
endif()




