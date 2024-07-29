#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(BRAGGPROFILER)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
set(MACRO_DIR ${CMAKE_SOURCE_DIR}/scripts)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB MACRO_FILES "${MACRO_DIR}/*")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(braggProfiler braggProfiler.cc ${sources} ${headers})
target_link_libraries(braggProfiler ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS)

foreach(MACRO_FILE ${MACRO_FILES})
  get_filename_component(MACRO_FILE_NAME ${MACRO_FILE} NAME)
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${MACRO_FILE_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy ${MACRO_FILE} ${CMAKE_BINARY_DIR}/${MACRO_FILE_NAME}
    DEPENDS ${MACRO_FILE}
  )
  list(APPEND COPY_MACRO_FILES ${CMAKE_BINARY_DIR}/${MACRO_FILE_NAME})
endforeach()


foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(BRAGGPROFILER DEPENDS braggProfiler)
add_custom_target(copy_macros ALL DEPENDS ${COPY_MACRO_FILES})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS braggProfiler DESTINATION bin)