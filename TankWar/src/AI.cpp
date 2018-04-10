#include "AI.h"
#include <random>


/*COULD PROBABLY MAKE AI A TEMPLATE CLASS WHERE THE TYPE IT TAKES IS ONE OF THE ALGORITHMS
AND THEN RUN THE ALGORITHMS T*/
AI::AI()
{
}

/*!Sets up the start and current cell (they are the same at the start) as well
*  as the position of the tank itself.
*/
AI::AI(Cell *startCell, Cell *topOfGrid)
{
	clearMovement();

	m_setCurrentCell(startCell);

	//Store DFS top of grid here.


	resetTank(startCell->m_realWorldPos.x, startCell->m_realWorldPos.y, 0, 0); //Set the tanks position to the start cell's postion.
}

/*! Calls the m_setCurrentCell function of the algorithm being used.*/
void AI::m_setCurrentCell(Cell *newCurrentCell)
{
	Astar.m_setCurrentCell(newCurrentCell);
}

/*! Calls the m_setEndCell function of the algorithm being used.*/
void AI::m_setEndCell(Cell *newEndCell)
{
	Astar.m_setEndCell(newEndCell);
}

/*! Checks to see if this tank is currently inside of the given Cell.

\param cell The Cell to check for.
*/
bool AI::isInCell(Cell *cell)
{
	return ((int)getX() >= cell->getPos().x * cell->m_widthScaled &&
		(int)getX() <= (cell->getPos().x * cell->m_widthScaled) + cell->m_widthScaled - 1 &&
		(int)getY() >= cell->getPos().y * cell->m_heightScaled &&
		(int)getY() <= (cell->getPos().y * cell->m_heightScaled) + cell->m_heightScaled - 1);
}

bool AI::m_shouldChooseNewEndCell() // Grab from the desired algorithm object
{
	bool temp = Astar.m_bChooseNewEndCell;

	Astar.m_bChooseNewEndCell = !temp;

	return temp;
}

void AI::m_setOpponentBoundingBox(BoundingBox bb)
{
	opponentBB = bb;
}

void AI::move()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		reset();
	}

	stopTurret();
	m_bShouldFireShell = false;
	Astar.Run(m_currentTankPos);


	if (Astar.m_bHasFoundPath)
	{
		if (Astar.m_pathTaken.size() > 1)
		{
			Cell cellToMoveTo = Astar.m_pathTaken.at(Astar.m_pathTaken.size() - 2);

			if (!canSee(opponentBB))
			{
				float totalStepsX = cellToMoveTo.getPos().x - m_currentTankPos->getPos().x;
				float totalStepsY = m_currentTankPos->getPos().y - cellToMoveTo.getPos().y;

				int stepsX;
				int stepsY;

				totalStepsX == 0 ? stepsX = 0 : stepsX = totalStepsX / abs(totalStepsX);
				totalStepsY == 0 ? stepsY = 0 : stepsY = totalStepsY / abs(totalStepsY);

				int angle = m_currentTankPos->calculateAngleToCell(stepsX, stepsY);

				float th = pos.getTh();

				/*Take delta = target - current (if it is negative, then add 360 until it is in the range 0 to 360)

				Now if it is above 180 it is right, and if it is below 180 then it is left*/

				int delta = th - angle;

				if (delta < 0)
				{
					delta += 360;
				}

				if (th == angle)
				{
					goForward();
				}
				else if (delta > 180)
				{
					goRight();
					currentBodyAngle++;

					if (currentBodyAngle > 360)
					{
					currentBodyAngle = 0;
					}
				}
				else if(delta < 180)
				{
					goLeft();
					currentBodyAngle--;

					if (currentBodyAngle < -360)
					{
					currentBodyAngle = 0;
					}
				}

				if (*m_currentTankPos == cellToMoveTo)
				{
					Astar.m_pathTaken.pop_back();
				}

				Astar.m_bGenerateNewPath = false;

				implementMove();
			}
			else
			{
				stop();
				float totalStepsX = Astar.m_endCell->m_realWorldPos.x - m_currentTankPos->m_realWorldPos.x;
				float totalStepsY = m_currentTankPos->m_realWorldPos.y - Astar.m_endCell->m_realWorldPos.y;

				int stepsX;
				int stepsY;

				totalStepsX == 0 ? stepsX = 0 : stepsX = totalStepsX / abs(totalStepsX);
				totalStepsY == 0 ? stepsY = 0 : stepsY = totalStepsY / abs(totalStepsY);

				int angle = m_currentTankPos->calculateAngleToCell(stepsX, stepsY);

				float angleToFace = atan2(Astar.m_endCell->m_realWorldPos.y - pos.getY(), Astar.m_endCell->m_realWorldPos.x - pos.getX());

				angleToFace = RAD2DEG(angleToFace);

				int th = turretTh;

				std::cout << "Our Angle: " << currentTurretAngle << std::endl;
				std::cout << "Our Angle Corrected: " << currentTurretAngle + currentBodyAngle << std::endl;
				std::cout << "Body Angle: " << currentBodyAngle << std::endl;
				std::cout << "Angle: " << (int)angleToFace << std::endl;

				if (currentTurretAngle == (int)angleToFace)
				{
					m_bShouldFireShell = true;
				}
				else if (currentTurretAngle < (int)angleToFace)
				{
					turretGoRight();
					currentTurretAngle++;

					if (currentTurretAngle > 360)
					{
						currentTurretAngle = 0;
					}
				}
				else if (currentTurretAngle > (int)angleToFace)
				{
					turretGoLeft();
					currentTurretAngle--;

					if (currentTurretAngle < -360)
					{
						currentTurretAngle = 0;
					}
				}
				else
				{
					m_bShouldFireShell = false;
				}


				/*Take delta = target - current (if it is negative, then add 360 until it is in the range 0 to 360)

				Now if it is above 180 it is right, and if it is below 180 then it is left*/

				/*int delta = th - angle;

				if (delta < 0)
				{
					delta += 360;
				}


				if (th == angle)
				{
					m_bShouldFireShell = true;
				}
				else if (delta > 180)
				{
					turretGoRight();
					m_bShouldFireShell = false;
				}
				else if (delta < 180)
				{
					turretGoLeft();
					m_bShouldFireShell = false;
				}
				else
				{
					m_bShouldFireShell = false;
				}*/

				implementMove();

			}
		}
		else
		{
			Astar.m_bGenerateNewPath = true;
			Astar.m_bHasFoundEndCell = true;
			Astar.m_pathTaken.clear();
		}
	}
}


/*OVERWRITTEN PARENT FUNCTIONALITY. (see: aitank.h for docs)*/
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

void AI::reset()
{
	Astar.m_bGenerateNewPath = true;
	Astar.m_bHasFoundEndCell = true;
	Astar.m_pathTaken.clear();
	forwards = true;
}