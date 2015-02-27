INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

ADD_EXECUTABLE(pdevs-multithreading-tests graph_manager.hpp tests.cpp)

TARGET_LINK_LIBRARIES(pdevs-multithreading-tests
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})

ADD_EXECUTABLE(pdevs-multithreading-main graph_manager.hpp main.cpp
  ../boost_graph/graph_builder.hpp ../boost_graph/graph_generator.hpp
  ../boost_graph/graph_defs.hpp ../boost_graph/graph_manager.hpp
  ../boost_graph/graph_partitioning.hpp ../boost_graph/models.hpp)

TARGET_LINK_LIBRARIES(pdevs-multithreading-main partitioning
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})