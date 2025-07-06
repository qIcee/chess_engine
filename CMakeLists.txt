cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Header-Ordner einbinden
include_directories(engine/include)

# Automatisch alle .cpp-Dateien im src/ rekursiv finden
file(GLOB_RECURSE SOURCES "engine/src/*.cpp")

# Hauptziel: alle cpp-Dateien rein, egal wie viele
add_executable(chess-engine ${SOURCES})
