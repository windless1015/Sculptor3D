CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(Sculptor3D LANGUAGES CXX)

SET(Qt5_DIR D:/Librarys/Qt/Qt5.12.10/5.12.10/msvc2015_64/lib/cmake/Qt5)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SEt(CMAKE_AUTOMOC ON)
SEt(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} Qt5_DIR)


FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)



SET(SCULPTOR3D_SRCS
	src/main.cpp
	src/view/viewGLwidget.cpp
	src/utility/makeGeometries.cpp
)

SET(SCULPTOR3D_HDRS
	src/view/viewGLwidget.h
	src/utility/makeGeometries.h
)



set(QRC_SOURCE_FILES 
	resources/resources.qrc)


#add the resource files
QT5_ADD_RESOURCES(QRC_FILES ${QRC_SOURCE_FILES})


ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME} WIN32
  ${SCULPTOR3D_SRCS}
  ${SCULPTOR3D_HDRS}
  ${QRC_FILES}
)

INCLUDE_DIRECTORIES(
	D:/projects/Sculptor3D/Sculptor3D/src/resources
)

target_link_libraries( ${CMAKE_PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets  
  Qt5::Gui
  Qt5::OpenGL
)