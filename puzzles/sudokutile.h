#ifndef SUDOKUTILE_H
#define SUDOKUTILE_H
/*
class SudokuTile<N>
====================================================================================================
Where N is the maximum number that can appear in the puzzle. N must be a square number. This class
stores data as an array of N bools, where:

array[index] = state
value = index + 1

if (array[index] == true) then the tile can be index + 1
*/
#include <array>
#include <algorithm>
#include <vector>

namespace puzzles
{

template <std::size_t N>
class SudokuTile
{
    static_assert(N >= 4, "SudokuTile cannot be instantiated with a template value less than 4.");

public:
    using canbe_type = std::array<bool, N>;

    SudokuTile() :
        m_canbe()
    {
        clear();
    }

    // Is this value a valid tile value?
    //constexpr
    static bool isValidValue(std::size_t value)
    {
        return value <= N;
    }

    //constexpr
    static std::size_t indexToValue(std::size_t index)
    {
        return index + 1;
    }

    //constexpr
    static std::size_t valueToIndex(std::size_t value)
    {
        return value - 1;
    }

    // Is this tile solved?
    bool isSolved() const
    {
        // if only one canbe is true, then this tile is solved
        return std::count(m_canbe.cbegin(), m_canbe.cend(), true) == 1;
    }

    // Get the tile solution, returning 0 if it isn't solved
    std::size_t solution() const
    {
        if (isSolved())
            return indexToValue(std::distance(m_canbe.cbegin(), std::find(m_canbe.cbegin(), m_canbe.cend(), true)));
        else
            return 0;
    }

    // This tile is this value.
    void setSolution(std::size_t value)
    {
        // if the value is 0, then this tile is cleared
        if (value == 0)
        {
            m_canbe.fill(true);
        }
        // if the value is within the range, set only that value to true
        else if (value <= N)
        {
            m_canbe.fill(false);
            m_canbe[valueToIndex(value)] = true;
        }
    } // canbe is a one value vector

    // This tile cannot be this value.
    bool cannotBe(std::size_t value)
    {
        // if the tile is not solved, value is valid and the tile can be value,
        if (!isSolved() && value != 0 && value <= N && m_canbe[valueToIndex(value)])
        {
            // set can be value to false
            m_canbe[valueToIndex(value)] = false;
            // we have changed something
            return true;
        }
        else
            // we haven't changed anything
            return false;
    }

    // Clear the tile data
    void clear()
    {
        m_canbe.fill(true);
    }

    // Get the number of values this tile could be.
    std::size_t possibleNumbersCount() const
    {
        return std::count(m_canbe.cbegin(), m_canbe.cend(), true);
    }

    // Get the numbers this tile could be
    std::vector<std::size_t> possibleNumbers()
    {
        // initialise the vector with the count of possilbe numbers
        std::vector<std::size_t> result{};
        result.reserve(possibleNumbersCount());

        // For each possible index value of m_canbe
        for (std::size_t index = 0; index != N; ++index)
        {
            // if the index value is true,
            if (m_canbe[index])
                // convert the index to the number and add it to result
                result.push_back(indexToValue(index));
        }
        return result;
    }

    std::ostream& print(std::ostream& os) const
    {
        // if the tile is solved
        if (isSolved())
        {
            if (N > 10 && solution() < 10)
                os << ' ';
            // output the solution
            os << solution();
        }
        // else not solved
        else
        {
            if (N > 10 )
                os << '-';
            os << '-';
        }
        return os;
    }

    std::ostream& print_canbe(std::ostream& os) const
    {
        os << '(';
        // For each possible index value of m_canbe
        for (std::size_t index = 0; index != N; ++index)
        {
            // if the index value is true, output it
            if (m_canbe[index])
                os << indexToValue(index) << ' ';
        }
        os << ')';
        return os;
    }

private:
    // Data Members
    //============================================================
    std::array<bool, N> m_canbe;
};


} // namespace puzzles

#endif // SUDOKUTILE_H
