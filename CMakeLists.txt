CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(Sculptor3D LANGUAGES CXX)

SET(Qt5_DIR D:/Librarys/Qt/Qt5.12.10/5.12.10/msvc2015_64/lib/cmake/Qt5)

# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# run moc automatically
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} Qt5_DIR)


FIND_PACKAGE(Qt5 REQUIRED COMPONENTS Core Widgets Gui OpenGL)



SET(SCULPTOR3D_SRCS
	src/main.cpp
	src/view/viewGLwidget.cpp
)

SET(SCULPTOR3D_HDRS
	src/view/viewGLwidget.h
)

ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME} WIN32
  ${SCULPTOR3D_SRCS}
  ${SCULPTOR3D_HDRS}
)

target_link_libraries( ${CMAKE_PROJECT_NAME}
  Qt5::Core
  Qt5::Widgets  
  Qt5::Gui
  Qt5::OpenGL
)