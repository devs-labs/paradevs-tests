INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

ADD_EXECUTABLE(pdevs-multithreading-lifegame-main graph_manager.hpp main.cpp
  models.hpp
  ../../boost_graph/partitioning/utils.hpp
  ../../boost_graph/partitioning/gggp.hpp
  ../../boost_graph/partitioning/graph_build.hpp)

TARGET_LINK_LIBRARIES(pdevs-multithreading-lifegame-main
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})