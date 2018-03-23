#pragma once

#include "aitank.h"
#include "Cell.h"
#include <queue>
#include <stack>

/*This is the class we will work in. Every algorithm should be implemented using the grid in the Game class via
this class.*/
class AI : public AITank
{
private:
	Cell * m_startCell = nullptr; //!< The Cell this tank should start at. Only used once.
	Cell * m_endCell = nullptr; //!< The Cell this tank is aiming to get to.
	Cell * m_currentCell = nullptr; //!< The Cell this tank is, or is aiming to, evaluate.

	Cell * m_topOfGrid = nullptr;

	bool forwards;

	bool hasLocatedCell = false; //!< Is the tank facing the current Cell.
	bool hasFoundEndCell = false;

	std::queue<Cell*> cellsToCheck;
	std::vector<Cell*> visitedCells;

	Cell * m_previousEndCell;


	std::stack<Cell*> cellsToEvaluate;
	int iNeighbourToCheck = 0;
	bool bHasBeenVisited = false;

	int count = 0;
	
	void BreadthFirstSearch();

	void DepthFirstSearch();

	void cleanup();

public:
	AI();

	/*! The AI Constructor
		Sets up the start and current cell (they are the same at the start) as well
		as the position of the tank itself.

		\param startCell A pointer to the Cell this tank should start at.
	*/
	AI(Cell * startCell, Cell * topOfGrid);

	/*! Sets the start cell to the cell given to it.

		\param newStartCell A pointer to the new Cell this tank should start at.
	*/
	void m_setStartCell(Cell * newStartCell);

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

	/*! Checks to see if this tank is currently inside of the given Cell.
	
		\param cell The Cell to check for.
	*/
	bool isInCell(Cell * cell);


	/*Overriden Base Functionality. (See: aitank.h)*/
	void move();
	void reset();
	void collided();
	void markTarget(Position p);
	void markEnemy(Position p);
	void markBase(Position p);
	void markShell(Position p);
	bool isFiring();
	void score(int thisScore, int enemyScore);

	bool chooseNewEndCell = false;
	int x;
	int y;
};