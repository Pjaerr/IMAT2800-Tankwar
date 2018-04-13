#pragma once

#include "Cell.h"
#include <stack>
#include <iostream> 

class DFS
{
public:
	bool m_bHasFoundEndCell = false; //!< True if the tank is within the end cell.
	bool m_bHasFoundPath = false; //!< True if the algorithm has found the end cell.
	bool m_bChooseNewEndCell = false; //!< True if the algorithm needs a new end cell from Game.cpp.
	bool m_bGenerateNewPath = true;

	int m_iNeighbourToCheck = 0; //!< The index of the neighbour that needs to be checked next.

	Cell * m_currentCell = nullptr; //!< The Cell that is currently being evaluated.
	Cell * m_endCell = nullptr; //!< The Cell that the algorithm is trying to find a path to.
	Cell * m_topOfGrid = nullptr; //!< A reference to the top of the grid.
	Cell * m_previousEndCell = nullptr; //** SHOULD BE REPLACED WITH A PATH.

	std::vector<Cell> m_pathTaken; //!< The most recently found path to the end cell.

	DFS(); //!< The default constructor.

	/*! \brief The function that should be called to allow the algorithm to find a path.
	*	\param currentTankPos The Cell the tank is inside of.
	*/
	void Run(Cell * currentTankPos);

	/*! Sets the current cell this tank is, or will be evaluting and/or moving towards if not already there.

	Changes the previous current cell and all of its neighbours, if they exist, to white and then
	sets the current cell to the given cell pointer and then changes it, and all of its neighbours, to red.

	\param newCurrentCell A pointer to the new current Cell the tank will be evaluating next.
	*/
	void m_setCurrentCell(Cell * newCurrentCell);

	/*! Sets the end Cell to the given Cell pointer.

	\param newEndCell A pointer to the new cell this tank will try to reach.
	*/
	void m_setEndCell(Cell * newEndCell);

	void m_setTopOfGrid(Cell * topOfGrid);

private:
	std::vector<Cell*> m_visitedCells; //!< Cells that are no longer needed.
	std::stack<Cell*> m_cellsToCheck; //!< Cells that need to be evaluated.

	bool m_bHasBeenVisited = false; //!< True if the Cell being evaluated has already been visited.

	/*! \brief Clears all of the data structures that are used by this object.*/
	void cleanup();
};