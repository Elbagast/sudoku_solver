#include "sudokutilewidget.h"

#include <QPalette>


// Special 6
//============================================================
puzzles::SudokuTileWidget::SudokuTileWidget(QWidget *parent) :
    QSpinBox(parent),
    m_state{SudokuTileState::Empty},
    m_defaultPalette{this->palette()},
    m_solvedPalette{this->palette()},
    m_unsolvedPalette{this->palette()},
    m_startPalette{this->palette()}
{
    m_solvedPalette.setColor(QPalette::Base, Qt::yellow);
    m_unsolvedPalette.setColor(QPalette::Base, Qt::red);
    m_startPalette.setColor(QPalette::Base, Qt::green);

    this->setAutoFillBackground(true);

    //QObject::connect(this, &QSpinBox::valueChanged,
    //                 this, &SudokuTileWidget::slot_valueChanged);
    QObject::connect(this, SIGNAL(valueChanged(int)),
                    this, SLOT(slot_valueChanged(int)));
}

puzzles::SudokuTileWidget::~SudokuTileWidget() = default;

// Interface
//============================================================
// Set the QSpinBox value without it emitting any signals
void puzzles::SudokuTileWidget::setValue_noSignals(int value)
{
    this->blockSignals(true);
    this->setValue(static_cast<int>(value));
    this->blockSignals(false);
}
void puzzles::SudokuTileWidget::setValue_noSignals(std::size_t value)
{
    this->setValue_noSignals(static_cast<int>(value));
}

// Tile state access
void puzzles::SudokuTileWidget::setState(SudokuTileState state)
{
    m_state = state;
    switch(state)
    {
    case SudokuTileState::Empty:    this->setPalette(m_defaultPalette); break;
    case SudokuTileState::Start:    this->setPalette(m_startPalette); break;
    case SudokuTileState::Solved:   this->setPalette(m_solvedPalette); break;
    case SudokuTileState::Unsolved: this->setPalette(m_unsolvedPalette); break;

    default:                        this->setPalette(m_defaultPalette); break;
    }
    this->show();
}
puzzles::SudokuTileState puzzles::SudokuTileWidget::getState() const
{
    return m_state;
}

// Slots
//============================================================
// Internal slot to extend behaviour when the user changes the QSpinBox value
void puzzles::SudokuTileWidget::slot_valueChanged(int value)
{
    if (value == 0)
        setState(SudokuTileState::Empty);
    else
        setState(SudokuTileState::Solved);
    this->show();
}
