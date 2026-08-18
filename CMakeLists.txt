cmake_minimum_required(VERSION 3.16)
project(GestiuneActivitati)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Aici ii spunem unde este instalat Qt pe calculatorul tau
set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64")

# Acum CMake va sti de unde sa ia librariile
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(GestiuneActivitati
        main.cpp
        Domain.cpp Domain.h
        repo.cpp repo.h FileRepo.h
        service.cpp service.h
        lista_activitati.cpp lista_activitati.h
        MainWindow.cpp MainWindow.h
        ListaWindow.cpp ListaWindow.h
        test.cpp test.h
        Observer.h
        MyTableModel.h
        CosReadOnlyGUI.h
)

target_link_libraries(GestiuneActivitati PRIVATE Qt6::Widgets)