cmake_minimum_required(VERSION 3.17.2)

# название TARGET
project(BRILLIANT_0_04)

# ХРЕН ЗНАЕТ ДЛЯ ЧЕГО
# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)
# Instruct CMake to run uic automatically when needed.
#set(CMAKE_AUTOUIC ON)


# ИСПОЛЬЗОВАНИЕ СТАНДАРТА 17
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ИЛИ ТАК :-)))
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# УВЕЛИЧЕНИЕ СКОРОСТИ ПРИ КОМПИЛЯЦИИ
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

# СТАНДАРТНЫЕ ФЛАГИ КОМПИЛЯТОРА
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# ПРЕДУПРЕЖДЕНИЯ == ОШИБКИ
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# ДЛЯ ПОДКЛЮЧЕНИЯ ФАЙЛОВОЙ СИСТЕМЫ (#include <filesystem>) 
#          ПОКА НЕ РАЗОБРАЛСЯ
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")

# ОПРЕДЕЛЕНИЕ SOURCE and HEADERS
set(SOURCES main.cpp brilliant.cpp)
set(HEADERS brilliant.h)

# СОЗДАНИЕ TARGET
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# ЕСЛИ НУЖЕН QT
# НАЙТИ и подключить QT к TARGET, что РАВНОЗНАЧНО QT += widgets
#find_package(Qt5Widgets REQUIRED)
#target_link_libraries(${PROJECT_NAME}  Qt5::Widgets)

# ЕСЛИ НУЖЕН OPENGL
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${GLUT_INCLUDE_DIR} )
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_LIBRARY})
