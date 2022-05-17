##### Library ######
add_library(
  orbitSim STATIC
  src/atFunctions.cxx
  src/functUtil.cxx
  src/GLAST_slew_estimate.cxx
  src/OrbSim.cxx
  src/read_ephem.cxx
)

target_link_libraries(orbitSim PUBLIC st_app st_facilities st_stream tip)
target_include_directories(
  orbitSim
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:>
)

###### Executables ######
add_executable(gtorbsim src/orbSim/main.cxx)
target_link_libraries(gtorbsim PRIVATE orbitSim)

###############################################################
# Installation
###############################################################
install(DIRECTORY orbitSim DESTINATION ${FERMI_INSTALL_INCLUDEDIR})
install(DIRECTORY data/ DESTINATION ${FERMI_INSTALL_REFDATADIR}/orbitSim)
install(DIRECTORY pfiles/ DESTINATION ${FERMI_INSTALL_PFILESDIR})

install(
  TARGETS orbitSim gtorbsim
  EXPORT fermiTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
