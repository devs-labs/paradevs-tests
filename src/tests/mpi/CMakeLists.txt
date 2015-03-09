INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${Boost_LIBRARY_DIRS}
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

#ADD_EXECUTABLE(pdevs-mpi-tests graph_manager.hpp tests.cpp)

#TARGET_LINK_LIBRARIES(pdevs-mpi-tests
#  ${Boost_MPI_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_TIMER_LIBRARY}
#  ${MPI_CXX_LIBRARIES})

ADD_EXECUTABLE(pdevs-mpi-main graph_manager.hpp main.cpp
  ../boost_graph/partitioning/utils.hpp ../boost_graph/partitioning/gggp.hpp
  ../boost_graph/partitioning/graph_build.hpp)

TARGET_LINK_LIBRARIES(pdevs-mpi-main
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_MPI_LIBRARY}
  ${MPI_CXX_LIBRARIES})