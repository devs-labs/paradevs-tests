INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

ADD_EXECUTABLE(pdevs-plot-main graph_builder.hpp graph_defs.hpp
  graph_manager.hpp main.cpp models.hpp
  ../boost_graph/partitioning/utils.hpp
  ../boost_graph/partitioning/defs.hpp
  ../boost_graph/partitioning/gggp.hpp)

TARGET_LINK_LIBRARIES(pdevs-plot-main shp geos
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})