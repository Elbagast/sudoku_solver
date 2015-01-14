#include "puzzles/sudokusolverdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    puzzles::SudokuSolverDialog window;
    window.show();

    return application.exec();
}
