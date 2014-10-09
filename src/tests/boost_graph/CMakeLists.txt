INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

# graph tests
ADD_EXECUTABLE(graph-tests graph_builder.hpp graph_generator.hpp graph_defs.hpp graph_manager.hpp
  graph_partitioning.hpp tests.cpp models.hpp)

# file generator
ADD_EXECUTABLE(file-generator graph_generator.hpp graph_defs.hpp graph_partitioning.hpp file_generator.cpp)

TARGET_LINK_LIBRARIES(graph-tests partitioning
  ${Boost_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${LIBXML_LIBRARIES}
  ${GTHREAD_LIBRARIES})

TARGET_LINK_LIBRARIES(file-generator partitioning
  ${Boost_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${LIBXML_LIBRARIES}
  ${GTHREAD_LIBRARIES})

ADD_SUBDIRECTORY(partitioning)
