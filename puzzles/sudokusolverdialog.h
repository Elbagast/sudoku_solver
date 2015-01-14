#ifndef DIALOG_H
#define DIALOG_H
/*
class SudokuSolverDialog
====================================================================================================
A simple dialog to provide a means of solving Sudoku puzzles of potentially any size. Since it's
simple I decided to have it entirely in code rather than use a qt form for the buttons etc.
*/
#include <QDialog>
#include <memory>

// Forward Declarations
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QPushButton;
class QSpacerItem;

namespace puzzles
{
// Forward Declarations
class SudokuBoardWidgetBase;

class SudokuSolverDialog :
        public QDialog
{
    Q_OBJECT

public:
    // Special 6
    //============================================================
    explicit SudokuSolverDialog(QWidget* parent = nullptr);
    ~SudokuSolverDialog() override;

    // No copying
    SudokuSolverDialog(SudokuSolverDialog const& other) = delete;
    SudokuSolverDialog& operator=(SudokuSolverDialog const& other) = delete;

    // Implict default move semantics

    // Slots
    //============================================================
private slots:
    // Change the size of the board
    void slot_setBoardSize(int comboBoxIndex);

private:
    // Helpers
    //============================================================
    // Initialise m_board
    void initialiseBoard(int comboBoxIndex);
    // Make a pointer to a board object of the right size
    std::unique_ptr<SudokuBoardWidgetBase> makeBoard(int comboBoxIndex);

    // Data Members
    //============================================================

    // Loose pointers look weird...but letting Qt do its job of deleting them is less pain.
    QHBoxLayout* m_mainLayout;
    QVBoxLayout* m_interfaceLayout;
    QComboBox* m_comboBox;
    QPushButton* m_clearButton;
    QPushButton* m_resetButton;
    QPushButton* m_solveButton;
    QSpacerItem* m_interfaceEndSpacer;

    std::unique_ptr<SudokuBoardWidgetBase> m_board;
};

} // namespace puzzles

#endif // DIALOG_H
