INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

SET(PARTITIONING_HPP utils.hpp gggp.hpp graph_build.hpp)

SET(PARTITIONING_CPP )

#ADD_LIBRARY(partitioning SHARED ${PARTITIONING_HPP};${PARTITIONING_CPP})

#TARGET_LINK_LIBRARIES(partitioning
#  ${Boost_LIBRARIES}
#  ${GLIBMM_LIBRARIES}
#  ${LIBXML_LIBRARIES}
#  ${GTHREAD_LIBRARIES})

ADD_EXECUTABLE(partitioning-tests main.cpp
  ${PARTITIONING_HPP};${PARTITIONING_CPP})

TARGET_LINK_LIBRARIES(partitioning-tests #partitioning
  ${Boost_LIBRARIES}
  ${GLIBMM_LIBRARIES}
  ${LIBXML_LIBRARIES}
  ${GTHREAD_LIBRARIES})
