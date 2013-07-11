# Make sure it finds the headers
#include_directories (../include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include ${GNULIBS_INCLUDE_DIR})
LINK_LIBRARIES(${PROJECT_NAME}  ${REQUIRED_LIBRARIES} )

ADD_EXECUTABLE(cluster cluster.cpp)
INSTALL(TARGETS cluster  RUNTIME DESTINATION bin)

ADD_EXECUTABLE(clustercv clustercv.cpp)
INSTALL(TARGETS clustercv  RUNTIME DESTINATION bin)

