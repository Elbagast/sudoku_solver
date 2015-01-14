#ifndef SUDOKUTILEPOSITION_H
#define SUDOKUTILEPOSITION_H
/*
struct SudokuTilePosition
====================================================================================================
Simple struct to store a 2D coordinate.
*/
namespace puzzles
{

struct SudokuTilePosition
{
    SudokuTilePosition(std::size_t xPosition, std::size_t yPosition) :
        x{ xPosition },
        y{ yPosition }
    {}

    std::size_t x, y;
};

inline bool operator==(SudokuTilePosition lhs, SudokuTilePosition rhs) { return (lhs.x == rhs.x) && (lhs.x == rhs.y); }
inline bool operator!=(SudokuTilePosition lhs, SudokuTilePosition rhs) { return !operator==(lhs, rhs); }

} // namespace puzzles

#endif // SUDOKUTILEPOSITION_H
