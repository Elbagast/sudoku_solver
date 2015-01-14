#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T21:51:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku_solver
TEMPLATE = app


SOURCES += main.cpp \
    puzzles/sudokusolverdialog.cpp \
    puzzles/sudokutilewidget.cpp

HEADERS  += \
    puzzles/sudokusolverdialog.h \
    puzzles/sudokutile.h \
    puzzles/sudokuboard.h \
    puzzles/sudokuboardwidgetbase.h \
    puzzles/sudokuboardwidget.h \
    puzzles/sudokutileposition.h \
    puzzles/sudokutilewidget.h

FORMS    +=
