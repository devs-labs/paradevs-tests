INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src
  ${PARADEVS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GLIBMM_INCLUDE_DIRS}
  ${LIBXML_INCLUDE_DIRS})

LINK_DIRECTORIES(
  ${GLIBMM_LIBRARY_DIRS}
  ${LIBXML_LIBRARY_DIR})

ADD_EXECUTABLE(pdevs-tests graph_manager.hpp models.hpp tests.cpp)

TARGET_LINK_LIBRARIES(pdevs-tests
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})

ADD_EXECUTABLE(pdevs-main graph_manager.hpp models.hpp main.cpp)

TARGET_LINK_LIBRARIES(pdevs-main
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY})