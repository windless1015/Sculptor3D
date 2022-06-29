CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(Sculptor3D LANGUAGES CXX)

SET(Qt5_DIR D:/Librarys/Qt/Qt5.12.10/5.12.10/msvc2015_64/lib/cmake/Qt5)
SET(OpenMesh_DIR D:/Librarys/OpenMesh8.1/share/OpenMesh/cmake)


SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SEt(CMAKE_AUTOMOC ON)
SEt(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
 src/ui
)
 
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} Qt5_DIR OpenMesh_DIR)


ADD_DEFINITIONS(-D_USE_MATH_DEFINES)


FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)
FIND_PACKAGE(OpenMesh REQUIRED)


SET(SCULPTOR3D_SRCS
	src/main.cpp
	src/view/viewGLwidget.cpp
	src/view/mainWindow.cpp
	src/view/meshViewerWidget.cpp
	src/utility/makeGeometries.cpp
)

SET(SCULPTOR3D_HDRS
	src/view/viewGLwidget.h
	src/view/meshViewerWidget.h
	src/view/mainWindow.h
	src/utility/makeGeometries.h
	src/baseDataTypes.h
)

SET(UI_FILES
	src/ui/mainwindow.ui
)


set(QRC_SOURCE_FILES 
	resources/resources.qrc)


#add the resource files
QT5_ADD_RESOURCES(QRC_FILES ${QRC_SOURCE_FILES})


ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME} WIN32
  ${SCULPTOR3D_SRCS}
  ${SCULPTOR3D_HDRS}
  ${QRC_FILES}
  ${UI_FILES}
)

INCLUDE_DIRECTORIES(
	${OpenMesh_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
	D:/Librarys/OpenMesh8.1/lib
)

target_link_libraries( ${CMAKE_PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets  
  Qt5::Gui
  Qt5::OpenGL
  debug OpenMeshCored OpenMeshToolsd
  optimized OpenMeshCore OpenMeshTools
)