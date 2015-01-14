#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H
/*
class SudokuBoard<N>
====================================================================================================
Where N*N is the maximum number that can appear in the puzzle. This is mostly so that we don't have
to square root the templated number to check it makes sense.
*/
#include "sudokutile.h"
#include "sudokutileposition.h"
#include <iosfwd>
#include <map>

namespace puzzles
{

template <std::size_t N>
class SudokuBoard
{
    static_assert(N >= 2, "SudokuBoard cannot be instantiated with a template value less than 2.");

public:
    // Special 6
    //============================================================
    // SudokuTile has a default constuctor so the arrays should auto-initialise to empty correctly...
    SudokuBoard() = default;
    SudokuBoard(std::array<std::size_t, N*N*N*N> const& tileValuesArray) :
        m_tileMatrix{}
    {
        // For each value in the array
        for (std::size_t index = 0, end = tileValuesArray.size(); index != end; ++index)
        {
            // Set the solution for the corresponding tile
            m_tileMatrix[index / maxNumber()][index % maxNumber()].setSolution(tileValuesArray[index]);
        }
    }
    SudokuBoard(std::vector<std::size_t> const& tileValuesVector) :
        m_tileMatrix{}
    {
        //

        // For each value in the vector
        // if the vector is as long as or longer than N^4, use N^4, otherwise use its size
        for (std::size_t index = 0,
            end = (N*N*N*N <= tileValuesVector.size() ? N*N*N*N : tileValuesVector.size());
            index != end; ++index)
        {
            // Set the solution for the corresponding tile
            m_tileMatrix[index / maxNumber()][index % maxNumber()].setSolution(tileValuesArray[index]);
        }
    }

    // Others are implicitly default

    // Interface
    //============================================================

    // What is the highest number that can be on the board?
    //constexpr
    static std::size_t maxNumber()
    {
        return N*N;
    }

    // Is this value a valid tile value?
    //constexpr
    static bool isValidValue(std::size_t value)
    {
        return value <= maxNumber();
    }

    // This tile is this value.
    void setTileSolution(std::size_t xPosition, std::size_t yPosition, std::size_t value)
    {
        if (isValidValue(value))
            m_tileMatrix[xPosition][yPosition].setSolution(value);
    }
    // This tile is this value.
    void setTileSolution(SudokuTilePosition postition, std::size_t value)
    {
        setTileSolution(postition.x, postition.y, value);
    }

    // Get the solution for a given tile, return 0 if not solved
    std::size_t getTileSolution(std::size_t xPosition, std::size_t yPosition) const
    {
        return m_tileMatrix[xPosition][yPosition].solution();
    }
    std::size_t getTileSolution(SudokuTilePosition postition) const
    {
        return getTileSolution(postition.x, postition.y);
    }

    // Is the board solved?
    bool isSolved() const
    {
        // for each row on the board (starting at the top)
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
        {
            // for each column in that row (starting on the left)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
            {
                // If a tile that isn't solved is found, then the board isn't solved, return false
                if (!m_tileMatrix[xPostition][yPosition].isSolved())
                    return false;
            }
        }
        // All tiles were checked and solved, so the board is solved, return true
        return true;
    }

    // Solve the puzzle.
    void solveAll()
    {
        // get list of newlySolved
        std::vector<SudokuTilePosition> newlySolved{};
        // for each row on the board (starting at the top)
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
        {
            // for each column in that row (starting on the left)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
            {
                if (m_tileMatrix[xPostition][yPosition].isSolved())
                    newlySolved.push_back(makeTilePosition(xPostition, yPosition));
            }
        }
        // the loop counter is there to stop the loop if it manages to get stuck
        std::size_t loop{ 0 };
        while (!(isSolved() || newlySolved.empty() || loop >100))
        {
            ++loop;
            // Copy the last batch of solved tiles and clear the container to store ones
            // that will be stored this loop.
            std::vector<SudokuTilePosition> toCheck{ newlySolved };
            newlySolved.clear();

            for (auto tile : toCheck)
            {
                std::vector<SudokuTilePosition> solved{};
                // For each solving function, capture the list of tiles solved and add them
                // to newSolved.
                solved = resolve_row(tile);
                newlySolved.insert(newlySolved.end(), solved.begin(), solved.end());
                solved = resolve_column(tile);
                newlySolved.insert(newlySolved.end(), solved.begin(), solved.end());
                solved = resolve_square(tile);
                newlySolved.insert(newlySolved.end(), solved.begin(), solved.end());
                solved = check_singular(tile);
                newlySolved.insert(newlySolved.end(), solved.begin(), solved.end());
            }
        }
    }

    void clearAll()
    {
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
                m_tileMatrix[xPostition][yPosition].clear();
    }


    std::ostream& print(std::ostream& os)
    {
        // for each row on the board (starting at the top)
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
        {
            // for each column in that row (starting on the left)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
            {
                //if (N*N > 10 && m_tileMatrix[xPostition][yPosition].solution() < 10)
                //    os << ' ';
                m_tileMatrix[xPostition][yPosition].print(os);
                os << ' ';
                if ((yPosition + 1) % N == 0 && yPosition + 1 != maxNumber())
                    os << ' ';
            }
            os << std::endl;
            // Spacer rows
            if ((xPostition + 1) % N == 0 && xPostition + 1 != maxNumber())
            {
                // space for > 10 values
                if (maxNumber() > 10)
                    os << std::string(maxNumber(), ' ');
                // the spacers
                os << std::string(N, ' ');
                // the numbers and commas
                os << std::string(maxNumber()*2, ' ') << std::endl;
            }
        }
        return os;
    }

    std::ostream& print_canbe(std::ostream& os)
    {
        // for each row on the board (starting at the top)
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
        {
            // for each column in that row (starting on the left)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
            {
                m_tileMatrix[xPostition][yPosition].print_canbe(os);
                //os << std::string(N*N - m_tileMatrix[xPostition][yPosition].possibleNumbersCount(), '-') << ':';
                if ((yPosition + 1) % N == 0 && yPosition + 1 != maxNumber())
                    os << ' ';
            }
            os << std::endl;
            if ((xPostition + 1) % N == 0 && xPostition + 1 != maxNumber())
                os << std::string(maxNumber(), ' ') << std::endl;
        }
        return os;
    }

    // Read a sequence of N*N*N values from the stream
    std::istream& read_stream(std::istream& is)
    {
        // for each row on the board (starting at the top)
        for (std::size_t xPostition = 0; xPostition != maxNumber(); ++xPostition)
        {
            // for each column in that row (starting on the left)
            for (std::size_t yPosition = 0; yPosition != maxNumber(); ++yPosition)
            {
                std::size_t tileSolution{ 0 };
                is >> tileSolution;
                m_tileMatrix[xPostition][yPosition].setSolution(tileSolution);
            }
        }
        return is;
    }

private:
    using tile_type = SudokuTile<N*N>;
    using tile_matrix = std::array<std::array<tile_type, N*N>, N*N>;

    // Helpers
    //============================================================

    static SudokuTilePosition makeTilePosition(std::size_t xPosition, std::size_t yPosition)
    {
        return SudokuTilePosition{ xPosition, yPosition };
    }

    // Convert a coordinate value to that of the local square it's in.
    static std::size_t squareStartPosition(std::size_t postition)
    {
        return postition / N * N;
        /*
        This looks a bit off, but it's just integer math.

        For a 9x9 board:
        0,1,2, 3,4,5, 6,7,8
        1..
        2..

        3..
        4..
        5..

        6..
        7..
        8..

        So the board is composed of 3 3x3 squares that are:
        0 1 2
        1..
        2..

        So tile of coordinate
        0..2 goes to square coordinate 0,
        3..5 -> 1
        6..8 -> 2

        To convert from full coordinate to square coordinate use integer division:

        0 / 3 = 0
        1 / 3 = 0
        2 / 3 = 0

        3 / 3 = 1
        4 / 3 = 1
        5 / 3 = 1

        6 / 3 = 2
        7 / 3 = 2
        8 / 3 = 2

        Then to convert it back to the position on the board that the top left of the square is, multiply by N


        */

    }
    static std::size_t squareEndPosition(std::size_t postition)
    {
        return squareStartPosition(postition) + N;
    }

    // If the given tile is solved, then no other tiles inside this local square can have this tile's solution.
    std::vector<SudokuTilePosition> resolve_square(std::size_t xPosition, std::size_t yPosition)
    {
        // vector of the tiles that have been solved by this function
        std::vector<SudokuTilePosition> solvedTiles{};
        // if the given tile is solved
        if (m_tileMatrix[xPosition][yPosition].isSolved())
        {
            // for each tile in this row starting from the local square and ending at the end of the local square
            for (std::size_t xLoop = squareStartPosition(xPosition), xEnd = squareEndPosition(xPosition); xLoop != xEnd; ++xLoop)
            {
                // for each tile in this column starting from the local square and ending at the end of the local square
                for (std::size_t yLoop = squareStartPosition(yPosition), yEnd = squareEndPosition(yPosition); yLoop != yEnd; ++yLoop)
                {
                    // if the value was removed from the tile by this action to remove it
                    if (m_tileMatrix[xLoop][yLoop].cannotBe(m_tileMatrix[xPosition][yPosition].solution()))
                        // if this action solved it
                        if (m_tileMatrix[xLoop][yLoop].isSolved())
                            // a tile has been solved
                            solvedTiles.push_back(makeTilePosition(xLoop, yLoop));
                }
            }
        }
        return solvedTiles;
    }

    std::vector<SudokuTilePosition> resolve_square(SudokuTilePosition tilePosition)
    {
        return resolve_square(tilePosition.x, tilePosition.y);
    }

    // If the given tile is solved, then no other tiles inside this row can have this tile's solution.
    std::vector<SudokuTilePosition> resolve_row(std::size_t xPosition, std::size_t yPosition)
    {
        // vector of the tiles that have been solved by this function
        std::vector<SudokuTilePosition> solvedTiles{};
        // if the given tile is solved
        if (m_tileMatrix[xPosition][yPosition].isSolved())
        {
            // for each tile in this row
            for (std::size_t yLoop = 0; yLoop != maxNumber(); ++yLoop)
            {
                // if the tile isn't solved
                if (!m_tileMatrix[xPosition][yLoop].isSolved())
                    // if the value was removed from the tile by this action to remove it
                    if (m_tileMatrix[xPosition][yLoop].cannotBe(m_tileMatrix[xPosition][yPosition].solution()))
                        // if this action solved it
                        if (m_tileMatrix[xPosition][yLoop].isSolved())
                            // a tile has been solved
                            solvedTiles.push_back(makeTilePosition(xPosition, yLoop));
            }
        }
        return solvedTiles;
    }

    std::vector<SudokuTilePosition> resolve_row(SudokuTilePosition tilePosition)
    {
        return resolve_row(tilePosition.x, tilePosition.y);
    }

    // If the given tile is solved, then no other tiles inside this row can have column tile's solution.
    std::vector<SudokuTilePosition> resolve_column(std::size_t xPosition, std::size_t yPosition)
    {
        // vector of the tiles that have been solved by this function
        std::vector<SudokuTilePosition> solvedTiles{};
        // if the given tile is solved
        if (m_tileMatrix[xPosition][yPosition].isSolved())
        {
            // for each tile in this column
            for (std::size_t xLoop = 0; xLoop != maxNumber(); ++xLoop)
            {
                // if the tile isn't solved and the one checked is
                if (!m_tileMatrix[xLoop][yPosition].isSolved())
                    // if the value was removed from the tile by this action to remove it
                    if (m_tileMatrix[xLoop][yPosition].cannotBe(m_tileMatrix[xPosition][yPosition].solution()))
                        // if this action solved it
                        if (m_tileMatrix[xLoop][yPosition].isSolved())
                            // a tile has been solved
                            solvedTiles.push_back(makeTilePosition(xLoop, yPosition));
            }
        }
        return solvedTiles;
    }

    std::vector<SudokuTilePosition> resolve_column(SudokuTilePosition tilePosition)
    {
        return resolve_column(tilePosition.x, tilePosition.y);
    }

    // If the given tile is solved, then check whether the other tiles in the local square can be solved by being the only tile that can be a number (any number)
    std::vector<SudokuTilePosition> check_singular(std::size_t xPosition, std::size_t yPosition)
    {
        // vector of the tiles that have been solved by this function
        std::vector<SudokuTilePosition> solvedTiles{};

        // if the given tile is solved
        if (m_tileMatrix[xPosition][yPosition].isSolved())
        {
            // We will build a map of number : tilePosition for the local square

            using PossibleNumber_TilePosition_multimap = std::multimap<std::size_t, SudokuTilePosition>;
            PossibleNumber_TilePosition_multimap existingPossibleNumbers;

            // for each tile in this row starting from the local square and ending at the end of the local square
            for (std::size_t xLoop = squareStartPosition(xPosition), xEnd = squareEndPosition(xPosition); xLoop != xEnd; ++xLoop)
            {
                // for each tile in this column starting from the local square and ending at the end of the local square
                for (std::size_t yLoop = squareStartPosition(yPosition), yEnd = squareEndPosition(yPosition); yLoop != yEnd; ++yLoop)
                {
                    // for each number in the current tile's possible numbers
                    for (auto number : m_tileMatrix[xLoop][yLoop].possibleNumbers())
                    {
                        existingPossibleNumbers.emplace(number, makeTilePosition(xLoop, yLoop));
                    }
                }
            }
            // For all possible numbers
            for (std::size_t number = 1; number <= maxNumber(); ++number)
            {
                // if there is only one entry for the number
                if (existingPossibleNumbers.count(number) == 1)
                {
                    auto foundIterator = existingPossibleNumbers.find(number);
                    std::size_t xOnlyPossible = foundIterator->second.x;
                    std::size_t yOnlyPossible = foundIterator->second.y;

                    // if the tile isn't solved
                    if (!m_tileMatrix[xOnlyPossible][yOnlyPossible].isSolved())
                    {
                        m_tileMatrix[xOnlyPossible][yOnlyPossible].setSolution(number);
                        // a tile has been solved
                        solvedTiles.push_back(makeTilePosition(xOnlyPossible, yOnlyPossible));
                    }
                }
            }
        }
        return solvedTiles;
    }

    std::vector<SudokuTilePosition> check_singular(SudokuTilePosition tilePosition)
    {
        return check_singular(tilePosition.x, tilePosition.y);
    }

    // Data Members
    //============================================================

    tile_matrix m_tileMatrix;
};

} // namespace puzzles

#endif // SUDOKUBOARD_H
