cmake_minimum_required(VERSION 3.14)

project(NeuroBase LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Widgets)
find_package(OpenCV REQUIRED)

add_executable(NeuroBase
  src/main.cpp
  src/databasemanager.h 
  src/databasemanager.cpp
  src/neuralnetwork.h 
  src/neuralnetwork.cpp
  src/imageprocessor.h 
  src/imageprocessor.cpp
  src/camerarecognition.h 
  src/camerarecognition.cpp
  src/dnnmodel.h 
  src/dnnmodel.cpp
)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

include(GNUInstallDirs)
install(TARGETS NeuroBase
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
