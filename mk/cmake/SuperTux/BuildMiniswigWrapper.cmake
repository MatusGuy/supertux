option(GENERATE_WRAPPER "Build miniswig and generate the wrapper" OFF)
if(GENERATE_WRAPPER)
  add_subdirectory(tools/miniswig)
  add_custom_command(
    OUTPUT ${PROJECT_SOURCE_DIR}/src/scripting/wrapper.cpp ${PROJECT_SOURCE_DIR}/src/scripting/wrapper.hpp
    COMMAND cd ${PROJECT_SOURCE_DIR} && ${CMAKE_CXX_COMPILER}
    ARGS -x "c++" -E -CC
         -DSCRIPTING_API src/scripting/wrapper.interface.hpp
         -o ${CMAKE_CURRENT_BINARY_DIR}/miniswig.tmp
         -I${PROJECT_SOURCE_DIR}/src
    COMMAND tools/miniswig/miniswig
    ARGS --input miniswig.tmp
         --input-hpp scripting/wrapper.interface.hpp
         --output-cpp ${PROJECT_SOURCE_DIR}/src/scripting/wrapper.cpp
         --output-hpp ${PROJECT_SOURCE_DIR}/src/scripting/wrapper.hpp
         --output-hpp-include scripting/wrapper.hpp
         --module supertux
         --select-namespace scripting
    DEPENDS tools/miniswig/miniswig
    IMPLICIT_DEPENDS CXX ${PROJECT_SOURCE_DIR}/src/scripting/wrapper.interface.hpp
  )
  if(USE_CLANG_TIDY)
    set_target_properties(miniswig PROPERTIES CXX_CLANG_TIDY "")
  endif()
endif()

# EOF #
