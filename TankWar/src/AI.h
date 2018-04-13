#pragma once

#include "aitank.h"
#include "Cell.h"
#include "Astar.h"
#include "BFS.h"
#include "DFS.h"

/* \class This is the class we will work in. Every algorithm should be implemented using the grid in the Game class via
this class.*/

template <class T>
class AI : public AITank
{
private:
	AStar Astar; //!< Reference to the AStar object.
	BFS BreadthFirstSearch; //!< Reference to the BFS object.
	DFS DepthFirstSearch; //!< Reference to the DFS object.

	T algorithm;

	bool forwards; //!< Is the tank moving forwards?

	/*POTENTIALLY DON'T NEED. FIGURE OUT AFTER GETTING WHOLE THING WORKING.*/
	int m_iTankNormalisedX = 0;
	int m_iTankNormalisedY = 0;

	int m_iTargetNormalisedX = 0;
	int m_iTargetNormalisedY = 0;

	bool m_bReachedCell = true;
	float dotProd;

	BoundingBox opponentBB;

	

public:
	AI(); //!< The default constructor.

	bool m_shouldChooseNewEndCell(); //!< Called by Game.cpp and returns true if a new end cell is needed.

	bool m_canSeeEnemy = false; //** TEMPORARY ENEMY DETECTION (use existing code.)

	bool m_bShouldFireShell = false;

	float currentTurretAngle = 0;
	float currentBodyAngle = 0;

	bool m_bcanSeeBase = false;

	Cell * m_currentTankPos = nullptr; //!< The position of this tank within the grid.


	/*! The AI Constructor
		Sets up the algortithm being used as well as the tank's position.

		\param startCell A pointer to the Cell this tank should start at.
	*/
	AI(Cell * startCell, Cell * topOfGrid);


	/*! \brief Calls the m_setCurrentCell function of the algorithm being used.

	*	\param newCurrentCell A pointer to the new current Cell the algorithm will be evaluating next.
	*/
	void m_setCurrentCell(Cell * newCurrentCell);

	/*! \brief Calls the m_setEndCell function of the algorithm being used.
	
		\param newEndCell A pointer to the new cell this tank will try to reach.
	*/
	void m_setEndCell(Cell * newEndCell);

	/*! Checks to see if this tank is currently inside of the given Cell.
	
		\param cell The Cell to check for.
	*/
	bool isInCell(Cell * cell);

	void m_setOpponentBoundingBox(BoundingBox bb);


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

	bool m_bChooseNewEndCell = false;
	
	int x;
	int y;

	bool m_bCanShootEnemy = false;
};