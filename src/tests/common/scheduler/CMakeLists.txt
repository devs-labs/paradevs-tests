INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

ADD_EXECUTABLE(common-scheduler-tests ${COMMON_HPP} tests.cpp)

TARGET_LINK_LIBRARIES(common-scheduler-tests
  ${GLIBMM_LIBRARIES}
  ${LIBXML_LIBRARIES}
  ${GTHREAD_LIBRARIES})