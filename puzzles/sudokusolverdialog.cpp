#include "sudokusolverdialog.h"

#include "sudokuboardwidgetbase.h"
#include "sudokuboardwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

// Special 6
//============================================================
puzzles::SudokuSolverDialog::SudokuSolverDialog(QWidget *parent) :
    QDialog(parent),
    m_mainLayout{new QHBoxLayout},
    m_interfaceLayout{new QVBoxLayout},
    m_comboBox{new QComboBox(this)},
    m_clearButton{new QPushButton("Clear", this)},
    m_resetButton{new QPushButton("Reset", this)},
    m_solveButton{new QPushButton("Solve", this)},
    m_interfaceEndSpacer{new QSpacerItem(1,1,QSizePolicy::Minimum, QSizePolicy::Expanding)},
    m_board(nullptr)
{
    m_mainLayout->addLayout(m_interfaceLayout);
    m_interfaceLayout->addWidget(m_comboBox);
    m_interfaceLayout->addWidget(m_clearButton);
    m_interfaceLayout->addWidget(m_resetButton);
    m_interfaceLayout->addWidget(m_solveButton);
    m_interfaceLayout->addSpacerItem(m_interfaceEndSpacer);
    setLayout(m_mainLayout);

    // make the solve button the default button
    m_solveButton->setDefault(true);

    // disable resizing
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    // Setup the QComboBox
    m_comboBox->addItem("4 x 4");
    m_comboBox->addItem("9 x 9");
    m_comboBox->addItem("16 x 16");
    // Why oh why doesn't this syntax work with int signals....
    //QObject::connect(m_ui->comboBox, &QComboBox::currentIndexChanged,
    //                 this, &SudokuSolverDialog::slot_boardSizeChanged);
    QObject::connect(m_comboBox, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(slot_setBoardSize(int)));
    // Set the index, invoking the above connection to finish initialisation of this
    m_comboBox->setCurrentIndex(1);
}

puzzles::SudokuSolverDialog::~SudokuSolverDialog() = default;


// Slots
//============================================================
// Change the size of the board
void puzzles::SudokuSolverDialog::slot_setBoardSize(int comboBoxIndex)
{
    initialiseBoard(comboBoxIndex);
    // Resize the dialog to fit the new contents
    adjustSize();
}

// Helpers
//============================================================
// Initialise m_board
void puzzles::SudokuSolverDialog::initialiseBoard(int comboBoxIndex)
{
    m_board = makeBoard(comboBoxIndex);

    QObject::connect(m_clearButton, &QPushButton::clicked,
                     m_board.get(), &SudokuBoardWidgetBase::slot_clear);
    QObject::connect(m_resetButton, &QPushButton::clicked,
                     m_board.get(), &SudokuBoardWidgetBase::slot_reset);
    QObject::connect(m_solveButton, &QPushButton::clicked,
                     m_board.get(), &SudokuBoardWidgetBase::slot_solve);
    layout()->addWidget(m_board.get());
}

// Make a pointer to a board object of the right size
std::unique_ptr<puzzles::SudokuBoardWidgetBase> puzzles::SudokuSolverDialog::makeBoard(int comboBoxIndex)
{
    // Since size is templated these have to be hard-coded.
    switch(comboBoxIndex)
    {
    case 0:
        return std::unique_ptr<puzzles::SudokuBoardWidgetBase>(new SudokuBoardWidget<2>(this));
    case 1:
        return std::unique_ptr<puzzles::SudokuBoardWidgetBase>(new SudokuBoardWidget<3>(this));
    case 2:
        return std::unique_ptr<puzzles::SudokuBoardWidgetBase>(new SudokuBoardWidget<4>(this));
    default:
        return std::unique_ptr<puzzles::SudokuBoardWidgetBase>(nullptr);
    }
}
