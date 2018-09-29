cmake_minimum_required(VERSION 3.11)
include(GNUInstallDirs)
include(FetchContent)

project(OrbitSim
  VERSION 0.0.0
  LANGUAGES CXX
)

###############################################################################
#
# External dependencies managed through fetchcontent
#
###############################################################################

# Hoops
fetchcontent_declare(hoops
  GIT_REPOSITORY https://github.com/fermi-lat/hoops.git
  GIT_TAG standalone)
fetchcontent_getproperties(hoops)
if(NOT hoops_POPULATED)
  fetchcontent_populate(hoops)
  add_subdirectory(${hoops_SOURCE_DIR} ${hoops_BINARY_DIR})
endif()

# ST App
fetchcontent_declare(st_app
  GIT_REPOSITORY https://github.com/fermi-lat/st_app.git
  GIT_TAG standalone)
fetchcontent_getproperties(st_app)
if(NOT st_app_POPULATED)
  fetchcontent_populate(st_app)
  add_subdirectory(${st_app_SOURCE_DIR} ${st_app_BINARY_DIR})
endif()

# ST graph
fetchcontent_declare(st_graph
  GIT_REPOSITORY https://github.com/fermi-lat/st_graph.git
  GIT_TAG standalone)
fetchcontent_getproperties(st_graph)
if(NOT st_graph_POPULATED)
  fetchcontent_populate(st_graph)
  add_subdirectory(${st_graph_SOURCE_DIR} ${st_graph_BINARY_DIR})
endif()

# ST stream
fetchcontent_declare(st_stream
  GIT_REPOSITORY https://github.com/fermi-lat/st_stream.git
  GIT_TAG standalone)
fetchcontent_getproperties(st_stream)
if(NOT st_stream_POPULATED)
  fetchcontent_populate(st_stream)
  add_subdirectory(${st_stream_SOURCE_DIR} ${st_stream_BINARY_DIR})
endif()

# facilities
fetchcontent_declare(facilities
  GIT_REPOSITORY https://github.com/fermi-lat/facilities.git
  GIT_TAG standalone)
fetchcontent_getproperties(facilities)
if(NOT facilities_POPULATED)
  fetchcontent_populate(facilities)
  add_subdirectory(${facilities_SOURCE_DIR} ${facilities_BINARY_DIR})
endif()

# tip
fetchcontent_declare(tip
  GIT_REPOSITORY https://github.com/fermi-lat/tip.git
  GIT_TAG standalone)
fetchcontent_getproperties(tip)
if(NOT tip_POPULATED)
  fetchcontent_populate(tip)
  add_subdirectory(${tip_SOURCE_DIR} ${tip_BINARY_DIR})
endif()


###############################################################################
# orbitSim targets and executables
###############################################################################

add_library(orbitSim STATIC
  src/GLAST_slew_estimate.cxx
  src/OrbSim.cxx
  src/atFunctions.cxx
  src/functUtil.cxx
  src/read_ephem.cxx
  src/timeline.cxx
)

add_library(ST::orbitSim ALIAS orbitSim)

target_include_directories(orbitSim
  PUBLIC
  $<INSTALL_INTERFACE:include/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(orbitSim
  PRIVATE
    ST::facilities
    ST::stream
    ST::app
    ST::tip
)

add_executable(gtorbsim src/orbSim/main.cxx)
target_include_directories(gtorbsim
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(gtorbsim
  PRIVATE
    ST::app
    ST::facilities
    ST::orbitSim
    ST::stream
    ST::tip
)

###############################################################################
# orbitSim installation and exports
###############################################################################

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/orbitSim)

install(DIRECTORY orbitSim DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS orbitSim
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(TARGETS orbitSim
  EXPORT orbitSimTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

install(EXPORT orbitSimTargets
  NAMESPACE    ST::
  DESTINATION  ${INSTALL_CONFIGDIR}
  )

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/orbitSimConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/cmake/orbitSimConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/orbitSimConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/orbitSimConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/orbitSimConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT orbitSimTargets
  FILE ${CMAKE_CURRENT_BINARY_DIR}/orbitSimTargets.cmake
  NAMESPACE ST::
)

#Register package in user's package registry
export(PACKAGE orbitSim)

