project(geocoder)

include_directories(${OMIM_ROOT}/3party/jansson/src)

set(
  SRC
  geocoder.cpp
  geocoder.hpp
  hierarchy.cpp
  hierarchy.hpp
  hierarchy_reader.cpp
  hierarchy_reader.hpp
  index.cpp
  index.hpp
  name_dictionary.cpp
  name_dictionary.hpp
  result.cpp
  result.hpp
  types.cpp
  types.hpp
)

omim_add_library(${PROJECT_NAME} ${SRC})

add_subdirectory(geocoder_cli)
omim_add_test_subdirectory(geocoder_tests)
