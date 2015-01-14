#ifndef SUDOKUTILEWIDGET_H
#define SUDOKUTILEWIDGET_H
/*
enum class SudokuTileState
====================================================================================================
Enum for SudokuTileWidget to track the logical state of a tile. It's mostly just for colouring the
widgets.

class SudokuTileWidget
====================================================================================================
QSpinBox subclass with a number of colour palettes to signify different states. It provides
functions to set the value without emitting signals.
*/
#include <QSpinBox>

namespace puzzles
{

enum class SudokuTileState
{
    Empty,
    Start,
    Solved,
    Unsolved
};

class SudokuTileWidget :
        public QSpinBox
{
    Q_OBJECT
public:
    // Special 6
    //============================================================
    explicit SudokuTileWidget(QWidget *parent = nullptr);
    ~SudokuTileWidget() override;

    // No copying
    SudokuTileWidget(SudokuTileWidget const& other) = delete;
    SudokuTileWidget& operator=(SudokuTileWidget const& other) = delete;

    // Implict default move semantics

    // Interface
    //============================================================
    // Set the QSpinBox value without it emitting any signals
    void setValue_noSignals(int value);
    void setValue_noSignals(std::size_t value);

    // Tile state access
    void setState(SudokuTileState state);
    SudokuTileState getState() const;

    // Slots
    //============================================================
private slots:
    // Internal slot to extend behaviour when the user changes the QSpinBox value
    void slot_valueChanged(int value);

private:
    // Data Members
    //============================================================
    SudokuTileState m_state;
    QPalette m_defaultPalette;
    QPalette m_solvedPalette;
    QPalette m_unsolvedPalette;
    QPalette m_startPalette;
};

} // namespace puzzles

#endif // SUDOKUTILEWIDGET_H
