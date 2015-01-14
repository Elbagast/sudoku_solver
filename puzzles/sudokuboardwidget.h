#ifndef SUDOKUBOARDWIDGET_H
#define SUDOKUBOARDWIDGET_H
/*
class SudokuBoardWidget<N>
====================================================================================================
Consists of a SudokuBoard<N> and N^4 SudokuTileWidget to enter and display the values.
*/
#include "sudokuboardwidgetbase.h"
#include "sudokuboard.h"
#include "sudokutilewidget.h"

#include <QSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

namespace puzzles
{

template <std::size_t N>
class SudokuBoardWidget final:
        public SudokuBoardWidgetBase
{
    static_assert(N >= 2, "SudokuBoardWidget cannot be instantiated with a template value less than 2.");
public:
    // Typedefs
    //============================================================
    using SudokuTileWidgetArray = std::array<std::unique_ptr<SudokuTileWidget>, N*N*N*N>;
    using QFrameArray = std::array<std::unique_ptr<QFrame>, N*N*(N-1)*2>;

    using HLineArray = std::array<std::unique_ptr<QFrame>, N+1>;
    using VLineArray = std::array<std::unique_ptr<QFrame>, N+1>;

    // Special 6
    //============================================================
    explicit SudokuBoardWidget(QWidget *parent = nullptr):
        SudokuBoardWidgetBase(parent),
        m_board(),
        m_tileWidgetArray(),
        m_hlineFrameArray(),
        m_vlineFrameArray(),
        m_gridLayout(new QGridLayout)
    {

        // Initialise the tile widgets and add them to the layout
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            m_tileWidgetArray[index].reset(new SudokuTileWidget);
            m_tileWidgetArray[index]->setValue(0);
            m_tileWidgetArray[index]->setRange(0, N*N);

            int xPos = static_cast<int>(index / (N*N));
            int yPos = static_cast<int>(index % (N*N));

            int xPosModified = xPos + xPos / N +1; // if xp is 5 and N is 3, xpm = 5 + 1
            int yPosModified = yPos + yPos / N +1;
            m_gridLayout->addWidget(m_tileWidgetArray[index].get(), xPosModified, yPosModified);
        }


        // Initialise the hline frame array and add them to the layout
        for(std::size_t index = 0, end = m_hlineFrameArray.size(); index != end; ++index)
        {
            m_hlineFrameArray[index].reset(new QFrame);
            m_hlineFrameArray[index]->setFrameShape(QFrame::HLine);
            m_hlineFrameArray[index]->setFrameShadow(QFrame::Sunken);

            int xPos = static_cast<int>((index * N) + ((index * N + N) -1) / N);
            int yPos = 0;

            m_gridLayout->addWidget(m_hlineFrameArray[index].get(), xPos, yPos, 1, -1); // widget, x, y, xwidth, ywidth
        }


        // Initialise the vline frame array and add them to the layout
        for(std::size_t index = 0, end = m_vlineFrameArray.size(); index != end; ++index)
        {
            m_vlineFrameArray[index].reset(new QFrame);
            m_vlineFrameArray[index]->setFrameShape(QFrame::VLine);
            m_vlineFrameArray[index]->setFrameShadow(QFrame::Sunken);

            int xPos = 0;
            int yPos = static_cast<int>((index * N) + ((index * N + N) -1) / N);

            m_gridLayout->addWidget(m_vlineFrameArray[index].get(), xPos, yPos, -1, 1); // widget, x, y, xwidth, ywidth
        }

        // Set the layout
        setLayout(m_gridLayout.get());
    }
    ~SudokuBoardWidget() override = default;

    // No copying
    SudokuBoardWidget(SudokuBoardWidget const& other) = delete;
    SudokuBoardWidget& operator=(SudokuBoardWidget const& other) = delete;

    // Implict default move semantics

protected:
    // Virtual Functions
    //============================================================
    void clear() override final
    {
        m_board.clearAll();
        clearTileWidgetValues();
    }
    // Zero all tiles not marked as start tiles
    void reset() override final
    {
        m_board.clearAll();
        resetTileWidgetValues();
    }
    void solve() override final
    {
        updateBoardValues();
        colourStartTiles();
        m_board.solveAll();
        updateTileWidgetValues();
        colourUnsolvedTiles();
    }

private:
    // Private Interface
    //============================================================
    void setTileSolution(SudokuTilePosition position, std::size_t value)
    {
        m_board.setTileSolution(position, value);
    }

    // Reset all the tiles
    void clearTileWidgetValues()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            m_tileWidgetArray[index]->setValue(0);
            m_tileWidgetArray[index]->setState(SudokuTileState::Empty);
        }
    }

    // Reset only tiles not marked as start tiles
    void resetTileWidgetValues()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            if (m_tileWidgetArray[index]->getState() == SudokuTileState::Start)
            {
                m_tileWidgetArray[index]->setState(SudokuTileState::Solved);
            }
            else
            {
                m_tileWidgetArray[index]->setValue(0);
                m_tileWidgetArray[index]->setState(SudokuTileState::Empty);
            }

        }
    }

    // Set the tile widgets to use the data stored the board
    void updateTileWidgetValues()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            if (m_tileWidgetArray[index]->getState() != SudokuTileState::Start)
                m_tileWidgetArray[index]->setValue(static_cast<int>(m_board.getTileSolution(index / (N*N), index % (N*N))));
        }
    }

    // Set the board to use the data in the QSpinBoxes
    void updateBoardValues()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            m_board.setTileSolution(index / (N*N), index % (N*N), static_cast<std::size_t>(m_tileWidgetArray[index]->value()));
        }
    }

    void colourStartTiles()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            if (m_board.getTileSolution(index / (N*N), index % (N*N)) != 0)
                m_tileWidgetArray[index]->setState(SudokuTileState::Start);
        }
    }

    // Colour the board
    void colourUnsolvedTiles()
    {
        for (std::size_t index = 0, end = m_tileWidgetArray.size(); index != end; ++index)
        {
            if (m_board.getTileSolution(index / (N*N), index % (N*N)) == 0)
                m_tileWidgetArray[index]->setState(SudokuTileState::Unsolved);
        }
    }

    // Data Members
    //============================================================

    SudokuBoard<N> m_board;
    SudokuTileWidgetArray m_tileWidgetArray;
    HLineArray m_hlineFrameArray;
    VLineArray m_vlineFrameArray;
    std::unique_ptr<QGridLayout> m_gridLayout;
};


} // namespace puzzles

#endif // SUDOKUBOARDWIDGET_H
