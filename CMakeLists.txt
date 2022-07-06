CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(Sculptor3D LANGUAGES CXX)

#set qt5 and openmesh libraries directory
SET(Qt5_DIR D:/Librarys/Qt/Qt5.12.10/5.12.10/msvc2015_64/lib/cmake/Qt5)
SET(OpenMesh_DIR D:/Librarys/OpenMesh8.1/share/OpenMesh/cmake)

SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} Qt5_DIR OpenMesh_DIR)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOUIC_SEARCH_PATHS src/ui)

#add USE_MATH_DEFINES macro definition
ADD_DEFINITIONS(-D_USE_MATH_DEFINES)

FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)
FIND_PACKAGE(OpenMesh REQUIRED)

#group source files, header files, resource files and ui files
SET(SCULPTOR3D_SRCS
	src/main.cpp
	src/view/viewGLwidget.cpp
	src/view/mainWindow.cpp
	src/view/infoWidget.cpp
	src/view/toolWidget.cpp
	src/model/meshDataModel.cpp
	src/utility/makeGeometries.cpp
)

SET(SCULPTOR3D_HDRS
	src/view/viewGLwidget.h
	src/view/mainWindow.h
	src/view/infoWidget.h
	src/view/toolWidget.h
	src/model/meshDataModel.h
	src/utility/makeGeometries.h
	src/baseDataTypes.h
)

SET(UI_FILES
	src/ui/mainwindow.ui
	src/ui/infoWidget.ui
)

SET(QRC_SOURCE_FILES 
	resources/resources.qrc)

#add the resource files into qt project
QT5_ADD_RESOURCES(QRC_FILES ${QRC_SOURCE_FILES})

#integrate all the files to generate the executable program
ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME}
  ${SCULPTOR3D_SRCS}
  ${SCULPTOR3D_HDRS}
  ${QRC_FILES}
  ${UI_FILES}
)

INCLUDE_DIRECTORIES(
	${OpenMesh_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
)

TARGET_LINK_DIRECTORIES(${CMAKE_PROJECT_NAME} PUBLIC
	D:/Librarys/OpenMesh8.1/lib
)

#libraries needed for linkage when compile
target_link_libraries( ${CMAKE_PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets  
  Qt5::Gui
  Qt5::OpenGL
  debug OpenMeshCored OpenMeshToolsd
  optimized OpenMeshCore OpenMeshTools
)