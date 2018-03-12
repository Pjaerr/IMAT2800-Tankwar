#include "AI.h"

AI::AI() 
{

}

/*! The AI Constructor
Sets up the start and current cell (they are the same at the start) as well
as the position of the tank itself.

\param startCell A pointer to the Cell this tank should start at.
*/
AI::AI(Cell * startCell)
{
	clearMovement();
	m_setStartCell(startCell);
	m_setCurrentCell(startCell);

	resetTank(startCell->m_realWorldPos.x, startCell->m_realWorldPos.y, 0, 0); //Set the tanks position to the start cell's postion.
}

/*! Sets the start cell to the cell given to it.

\param newStartCell A pointer to the new Cell this tank should start at.
*/
void AI::m_setStartCell(Cell * newStartCell)
{
	m_startCell = newStartCell;
}

/*! Sets the current cell this tank is, or will be evaluting and/or moving towards if not already there.

	Changes the previous current cell and all of its neighbours, if they exist, to white and then
	sets the current cell to the given cell pointer and then changes it, and all of its neighbours, to red.

	\param newCurrentCell A pointer to the new current Cell the tank will be evaluating next.
*/
void AI::m_setCurrentCell(Cell * newCurrentCell)
{
	if (m_currentCell != nullptr)
	{
		m_currentCell->setColour(sf::Color::White);

		/*for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			m_currentCell->m_neighbours->at(i)->setColour(sf::Color::White);
		}*/
	}

	m_currentCell = newCurrentCell;

	m_currentCell->setColour(sf::Color::Red);
	/*for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
	{
		m_currentCell->m_neighbours->at(i)->setColour(sf::Color::Red);
	}*/
}

/*! Sets the end Cell to the given Cell pointer.

\param newEndCell A pointer to the new cell this tank will try to reach.
*/
void AI::m_setEndCell(Cell * newEndCell)
{
	m_endCell = newEndCell;
}

/*! Checks to see if this tank is currently inside of the given Cell.

\param cell The Cell to check for.
*/
bool AI::isInCell(Cell * cell)
{
	return ((int)getX() >= cell->getPos().x * cell->m_widthScaled &&
		(int)getX() <= (cell->getPos().x * cell->m_widthScaled) + cell->m_widthScaled - 1 &&
		(int)getY() >= cell->getPos().y * cell->m_heightScaled &&
		(int)getY() <= (cell->getPos().y * cell->m_heightScaled) + cell->m_heightScaled - 1);
}

void AI::reset()
{
	forwards = true;
}

void AI::move()
{
	/*PUT CODE IN HERE TO TURN TO AND THEN MOVE TOWARDS THE CURRENTCELL.
	
	The code sets up the movement variables, and then calling implementMove() will
	then start the movement itself.
	*/

	/*
		* Turn until rotated at the angle the desired Cell is at.
		* Go forward.
		* Check to see if at the desired Cell, if not, Go forward again.
		* Stop if reached desired Cell.
	
	*/
	if (!hasFoundEndCell)
	{
		if (!hasLocatedCell)
		{


			/*NEED TO WORK OUT THE ANGLE (not amount rotated [th]) THAT THE TANK IS FACING
			AND THEN THE ANGLE FROM THE TANK THAT THE CURRENT CELL IS AND THEN ROTATE UNTIL
			THOSE ANGLES ARE THE SAME, THEN MOVE FORWARD.*/


			int tankX = getX();
			int tankY = getY();
			int cellX = m_currentCell->m_realWorldPos.x;
			int cellY = m_currentCell->m_realWorldPos.y;


			float angle = atan2(cellY - tankY, cellX - tankX);
			
			angle = RAD2DEG(angle);

			float currentRotation = RAD2DEG(atan2(tankY - firingPosition().getY(), tankX - firingPosition().getX()));

			std::cout << "Current Rotation: " << abs(currentRotation) << std::endl;
			std::cout << "Angle to face: " << abs(angle) << std::endl;

			if (abs(currentRotation) != abs(angle))
			{
				goRight();
			}
			
			else
			{
				hasLocatedCell = true;
			}
		}
		else
		{
			goForward();

			if (isInCell(m_currentCell))
			{
				stop();
				hasFoundEndCell = true;
			}
		}
	}
	
	

	implementMove();
}

void AI::collided()
{
	forwards = !forwards;
}

void AI::markTarget(Position p)
{
	std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void AI::markEnemy(Position p)
{
	std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void AI::markBase(Position p)
{
	std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void AI::markShell(Position p)
{
	std::cout << "Shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

bool AI::isFiring()
{
	return !forwards; // Fire when going backwards
}

void AI::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}