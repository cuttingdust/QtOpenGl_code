cmake_minimum_required(VERSION 3.20)
project(SimpleLighting)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SOURCE "*.cpp" "*.c"  "*.cc")
file(GLOB HEAD "*.h" "*.hpp")
file(GLOB QTFILE "*.qrc" "*.ui")

# # 导入QT的库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

# 编译执行程序
message("QT_VERSION_MAJOR= ${QT_VERSION_MAJOR}")
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        ${SOURCE}
        ${HEAD}
        ${QTFILE})
else()
    add_executable(${PROJECT_NAME}
        ${SOURCE}
        ${HEAD}
        ${QTFILE})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt6::Widgets
  Qt6::Core
  Qt6::Gui
  Qt6::OpenGL
  Qt6::OpenGLWidgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

#设置Debug和Release输出路径一致（windows）
set(RUMTIME ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} 
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${RUMTIME}
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${RUMTIME}
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${RUMTIME}
)
