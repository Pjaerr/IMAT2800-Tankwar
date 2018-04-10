#pragma once
#include "Cell.h"
#include <vector>
#include <iostream>

class AStar
{
public:
	bool m_bHasFoundPath = false; //!< True if the algorithm has calculated and stored a path.
	bool m_bChooseNewEndCell = false; //!< True if the algorithm has found the end cell and needs another goal.
	bool m_bHasFoundEndCell = false; //!< True if the tank has gotten to the end cell.
	bool m_bGenerateNewPath = true; //!< True if the algorithm should store its most recently found path.

	Cell * m_currentCell = nullptr; //!< The Cell being evaluated.
	Cell * m_endCell = nullptr; //!< The Cell the algorithm will try and find a path to

	std::vector<Cell> m_pathTaken; //!< Copies of the Cells that make up the path found by the algorithm.

	AStar(); //!< The default constructor.

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

private:
	std::vector<Cell*> m_openSet; //!< Collection of Cells that are to be evaluated.
	std::vector<Cell*> m_closedSet; //!< Collection of Cells that are no longer needed.

	/*! \brief Clears all of the data structures that are used by this object.*/
	void cleanup();

	/*! \brief Returns the Cell in m_openSet that has the lowest final score.*/
	Cell* m_findLowestFinalScore();

	/*! \brief Removes a given Cell from m_openSet.
	*	\param cellToRemove The Cell that will be removed.
	*/
	void m_removeFromOpenSet(Cell* cellToRemove);

	/*! \brief Returns true if a given Cell is inside of m_closedSet.
	*	\param cellToCheck The Cell that will be checked.
	*/
	bool m_bIsInClosedSet(Cell* cellToCheck);

	/*! \brief Returns true if a given Cell is inside of m_openSet.
	*	\param cellToCheck The Cell that will be checked.
	*/
	bool m_bIsInOpenSet(Cell* cellToCheck);
};