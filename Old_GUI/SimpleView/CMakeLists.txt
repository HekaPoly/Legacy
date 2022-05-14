cmake_minimum_required(VERSION 3.3...3.12 FATAL_ERROR)
project(SimpleView)

find_package(VTK
  COMPONENTS
    vtkCommonCore
    vtkGUISupportQt
    vtkInfovisCore
    vtkRenderingFreeType
    vtkViewsQt
	vtkChartsCore
	vtkCommonDataModel
	vtkInteractionStyle
	vtkRenderingContext2D
	vtkRenderingContextOpenGL2
	vtkRenderingCore
	vtkRenderingOpenGL2
	vtkViewsContext2D QUIET)
if (NOT VTK_FOUND)
  message("Skipping example: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Use VTK
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets Gui)
if (NOT TARGET Qt5::Widgets OR NOT TARGET Qt5::Gui)
  message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
  return ()
endif ()

# Set your files and resources here
set(Srcs
  cnpy.cxx
  Chart.cxx
  SimpleView.cxx
  main.cxx)

set(Hdrs
  cnpy.h
  SimpleView.h
  Chart.h)

set(UIs
  SimpleView.ui)

set(QRCs
  Icons/icons.qrc)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

source_group("Resources" FILES
  ${UIs}
  ${QRCs})

add_executable(SimpleView MACOSX_BUNDLE
  ${Srcs} ${Hdrs} ${UIs} ${QRCs})
target_link_libraries(SimpleView
  PRIVATE
    ${VTK_LIBRARIES}
    Qt5::Gui
    Qt5::Widgets)
