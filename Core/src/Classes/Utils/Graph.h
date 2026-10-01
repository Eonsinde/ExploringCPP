// Contains helper functions used to explore a GRID(2D matrix)
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <set>
#include <utility>


namespace Core {
    // Represents position/node on a grid
    struct Cell {
        int row, column;

        Cell() : row{}, column{} {};

        Cell(const std::pair<int, int>& values)
            : row{ values.first }, column{ values.second } {
        }

        Cell(int rRow, int cCol)
            : row{ rRow }, column{ cCol } {
        }

        // ------------------------ Operators ------------------------

        // Define a strict weak ordering (lexicographical) so containers like
        // std::set/std::map that rely on ordering behave correctly.
        friend bool operator<(const Cell& lhs, const Cell& rhs) {
            if (lhs.row < rhs.row) return true;
            if (lhs.row > rhs.row) return false;
            return lhs.column < rhs.column;
        }

        // Define operator> in terms of operator< to keep the ordering logic
        // consistent and simple.
        friend bool operator>(const Cell& lhs, const Cell& rhs) {
            return rhs < lhs;
        }

        friend bool operator==(const Cell& lhs, const Cell& rhs) {
            if (&lhs == &rhs) return true;

            if ((lhs.row == rhs.row) && (lhs.column == rhs.column)) {
                return true;
            }

            return false;
        }

        Cell operator-() {
            return { -row, -column };
        }

        friend Cell operator+(const Cell& lhs, const Cell& rhs) {
            return { lhs.row + rhs.row, lhs.column + rhs.column };
        }

        friend Cell operator-(const Cell& lhs, const Cell& rhs) {
            return { lhs.row - rhs.row, lhs.column - rhs.column };
        }

        friend std::ostream& operator<<(std::ostream& os, const Cell& someCell) {
            os << "[" << someCell.row << ", " << someCell.column << "]";
            return os;
        }

        // ------------------------ Nlohmann ------------------------

        // Serialize: from Cell to JSON
        friend void to_json(nlohmann::json& j, const Cell& c) {
            j = nlohmann::json::array({ c.row, c.column });
        }

        // De-serialize: from JSON to Cell
        friend void from_json(const nlohmann::json& j, Cell& c) {
            if (j.is_array()) {
                c.row = j.at(0);
                c.column = j.at(1);
            }
            else {
                j.at("row").get_to(c.row);
                j.at("column").get_to(c.column);
            }
        }
    };

    /// @brief Contains what is needed to create a grid and traverse it
    /// @tparam T represents the value each cell will hold 
    template<typename T>
    class Grid {
    public:
        using Grid2D = std::vector<std::vector<T>>;
        using Grid2DIter = std::vector<std::vector<T>>::iterator;
        using SomeCell = std::pair<int, int>;

        // ------------------------ Grid exploration functions ------------------------

        /// @brief Display a grid in the consoles
        /// @param grid represents the 2D matrix
        /// @param currentPos represents the current cell the entity is
        /// @param startPos represents the start cell for the entity
        /// @param targetPos represents the target cell to be reached
        static void DisplayGrid(
            const Grid2D& grid, const Cell& currentPos={ -1, -1 },
            const Cell& startPos = { -1, -1 }, const Cell& targetPos = { -1, -1 }
        ) {
            for (size_t i{}; i < grid.size(); i++) {
                // Process current row el(the columns)
                for (size_t j{}; j < grid[i].size(); j++) {
                    if (Cell(i, j) == currentPos) {
                        std::cout << activeCellPlaceholder;
                    }
                    else if (Cell(i, j) == startPos) {
                        std::cout << "S ";
                    }
                    else if (Cell(i, j) == targetPos) {
                        std::cout << "G ";
                    }
                    else if (grid[i][j] == 0) {
                        std::cout << "● ";
                    }
                    else {
                        std::cout << "  ";
                    }
                }

                // Move to next line for next row
                std::cout << '\n';
            }
        }

        static std::shared_ptr<std::vector<Cell>> GetAdjacentCells(const Grid2D& someGrid, const Cell& someCell) {
            if (someGrid.empty())
                throw std::exception("Grid<T>: can't perform get adjacent cells operation on empty grid");

            // Store the grid limit for the grid(30, 20): (0, 0) to (29, 19)
            Cell gridLimit{ static_cast<int>(someGrid.size()) - 1, static_cast<int>(someGrid.front().size()) - 1 };

            std::shared_ptr <std::vector<Cell>> adjCellsPtr = std::make_shared<std::vector<Cell>>();
            // TOP, RIGHT, BOTTOM, LEFT
            std::vector<Cell> translations{ Cell(-1, 0), Cell(0, 1), Cell(1, 0), Cell(0, -1) };

            for (std::vector<Cell>::iterator start = translations.begin(); start != translations.end(); ++start) {
                Cell adjCell = someCell + *start;

                // TODO(Graph): Use the grid's type(T) to check if the cell is traversable
                if (
                    (adjCell.row >= 0 && adjCell.row <= gridLimit.row) && (adjCell.column >= 0 && adjCell.column <= gridLimit.column)
                    && someGrid[adjCell.row][adjCell.column] == 0 // Traversable cell
                ) {
                    adjCellsPtr->emplace_back(adjCell.row, adjCell.column);
                }
            }

            return adjCellsPtr;
        }

        static void DepthFirstSearch(const Grid2D& someGrid, const Cell& startCell, const Cell& targetCell) {
            std::vector<Cell> stack{ startCell };
            std::set<Cell> visited{ startCell };
            
            while (!stack.empty()) {
                // Extract cell to be processed
                const Cell currentCell = stack.back();
                stack.pop_back();

                // Process current cell
                DisplayGrid(someGrid, currentCell, startCell, targetCell);
                std::cout << "\n";

                if (currentCell == targetCell)
                    return;

                auto adjCellsPtr = GetAdjacentCells(someGrid, currentCell);

                for (const Cell& c : *adjCellsPtr) {
                    if (!visited.contains(c)) {
                        // Using frontier approach
                        visited.insert(c);
                        // Push cell unto stack for processing
                        stack.emplace_back(c.row, c.column);
                    }
                }
            }
        }

        static void BreadthFirstSearch(const Grid2D& someGrid, const Cell& startCell, const Cell& targetCell) {
            std::queue<Cell> queue;
            queue.emplace(startCell.row, startCell.column);
            
            std::set<Cell> visited{ startCell };

            size_t count{ 1 };

            while (!queue.empty()) {
                // Extract cell to be processed
                const Cell currentCell = queue.front();
                queue.pop();

                // Process current cell
                std::cout << "Step " << count << ":\n";
                DisplayGrid(someGrid, currentCell, startCell, targetCell);
                std::cout << "\n";
                
                if (currentCell == targetCell)
                    return;

                auto adjCellsPtr = GetAdjacentCells(someGrid, currentCell);

                for (const Cell& c : *adjCellsPtr) {
                    if (!visited.contains(c)) {
                        visited.insert(c);
                        queue.emplace(c.row, c.column);
                    }
                }

                ++count;
            }
        }

        // ------------------------ Getters & Setters ------------------------
        
        static const std::string& GetActiveCellPlaceholder() {
            return activeCellPlaceholder;
        }

        static void SetActiveCellPlaceholder(const std::string& value) {
            activeCellPlaceholder = value;
        }
    
    private:
        static inline std::string activeCellPlaceholder = "X ";
    };
}