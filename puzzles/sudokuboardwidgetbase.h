#ifndef SUDOKUBOARDWIDGETBASE_H
#define SUDOKUBOARDWIDGETBASE_H
/*
abstract class SudokuBoardWidgetBase
====================================================================================================
Abstract base class for SudokuBoardWidget<N> so it can be manipulated without knowing the true type.
Why? SudokuSolverDialog doesn't need to know the details of data input of layout, but does need
access to generic actions that apply regardless of the size of the board.
*/
#include <QWidget>
#include "sudokutileposition.h"

namespace puzzles
{

class SudokuBoardWidgetBase :
        public QWidget
{
    Q_OBJECT
public:
    // Special 6
    //============================================================
    explicit SudokuBoardWidgetBase(QWidget *parent = nullptr):
        QWidget(parent)
    {}
    ~SudokuBoardWidgetBase() override = default;

    // No copying
    SudokuBoardWidgetBase(SudokuBoardWidgetBase const& other) = delete;
    SudokuBoardWidgetBase& operator=(SudokuBoardWidgetBase const& other) = delete;

    // Implict default move semantics

    // Slots
    //============================================================
public slots:
    void slot_clear()   { this->clear(); }
    void slot_reset()   { this->reset(); }
    void slot_solve()   { this->solve(); }

protected:
    // Virtual Functions
    //============================================================
    virtual void clear() = 0;
    virtual void reset() = 0;
    virtual void solve()  = 0;
};

} // namespace puzzles

#endif // SUDOKUBOARDWIDGETBASE_H
