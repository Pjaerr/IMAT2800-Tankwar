#pragma once
#include "Cell.h"
#include <queue>


class BFS
{
public:
	bool m_bChooseNewEndCell = false; //!< True if the algorithm needs a new end cell from Game.cpp.
	bool m_bHasFoundPath = false; //!< True if the algorithm has found the end cell.
	bool m_bHasFoundEndCell = false; //!< True if the tank is within the end cell.

	Cell * m_currentCell = nullptr; //!< The Cell currently being evaluated.
	Cell * m_endCell = nullptr; //!< The Cell the algorithm is trying to find a path to.

	Cell * m_previousEndCell = nullptr; //* SHOULD BE REPLACED IN THE IMPLEMENTATION FOR CONSTRUCTING A PATH LIKE ASTAR.

	std::vector<Cell> m_pathTaken; //!< The path to the end cell that the algorithm has most recently found.

	BFS(); //!< The default constructor.

	/*! \brief The function that should be called to allow the algorithm to find a path.
	*	\param currentTankPos The Cell the tank is inside of.
	*/
	void Run(Cell * currentTankPos);

private:
	std::queue<Cell*> m_cellsToCheck; //!< The Cells that need to be evaluated.
	std::vector<Cell*> m_visitedCells; //!< The Cells that are no longer needed.

	/*! \brief Clears all of the data structures that are used by this object.*/
	void cleanup();

};