#include "AI.h"
#include <random>

template <class T>
AI<T>::AI()
{

}

/*!Sets up the start and current cell (they are the same at the start) as well
*  as the position of the tank itself.
*/
template <class T>
AI<T>::AI(Cell *startCell, Cell *topOfGrid)
{
	clearMovement();

	m_setCurrentCell(startCell);

	//Store DFS top of grid here.
	algorithm.m_setTopOfGrid(topOfGrid);

	resetTank(startCell->m_realWorldPos.x, startCell->m_realWorldPos.y, 0, 0); //Set the tanks position to the start cell's postion.
}

/*! Calls the m_setCurrentCell function of the algorithm being used.*/
template <class T>
void AI<T>::m_setCurrentCell(Cell *newCurrentCell)
{
	algorithm.m_setCurrentCell(newCurrentCell);
}

/*! Calls the m_setEndCell function of the algorithm being used.*/
template <class T>
void AI<T>::m_setEndCell(Cell *newEndCell)
{
	algorithm.m_setEndCell(newEndCell);
}

/*! Checks to see if this tank is currently inside of the given Cell.

\param cell The Cell to check for.
*/
template <class T>
bool AI<T>::isInCell(Cell *cell)
{
	return ((int)getX() >= cell->getPos().x * cell->m_widthScaled &&
		(int)getX() <= (cell->getPos().x * cell->m_widthScaled) + cell->m_widthScaled - 1 &&
		(int)getY() >= cell->getPos().y * cell->m_heightScaled &&
		(int)getY() <= (cell->getPos().y * cell->m_heightScaled) + cell->m_heightScaled - 1);
}

template <class T>
bool AI<T>::m_shouldChooseNewEndCell() // Grab from the desired algorithm object
{
	bool temp = algorithm.m_bChooseNewEndCell;

	algorithm.m_bChooseNewEndCell = !temp;

	return temp;
}

template <class T>
void AI<T>::m_setOpponentBoundingBox(BoundingBox bb)
{
	opponentBB = bb;
}

template <class T>
void AI<T>::move()
{

	stopTurret();
	m_bShouldFireShell = false;
	algorithm.Run(m_currentTankPos);


	if (algorithm.m_bHasFoundPath)
	{
		if (algorithm.m_pathTaken.size() > 1)
		{
			Cell cellToMoveTo = algorithm.m_pathTaken.at(algorithm.m_pathTaken.size() - 2);

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
				}
				else if(delta < 180)
				{
					goLeft();
				}

				if (*m_currentTankPos == cellToMoveTo)
				{
					algorithm.m_pathTaken.pop_back();
				}

				algorithm.m_bGenerateNewPath = false;

				implementMove();
			}
			else
			{
				stop();

				float totalStepsX = algorithm.m_endCell->getPos().x - m_currentTankPos->getPos().x;
				float totalStepsY = m_currentTankPos->getPos().y - algorithm.m_endCell->getPos().y;

				int stepsX;
				int stepsY;

				totalStepsX == 0 ? stepsX = 0 : stepsX = totalStepsX / abs(totalStepsX);
				totalStepsY == 0 ? stepsY = 0 : stepsY = totalStepsY / abs(totalStepsY);

				int angle = m_currentTankPos->calculateAngleToCell(stepsX, stepsY);

				/*Take delta = target - current (if it is negative, then add 360 until it is in the range 0 to 360)

				Now if it is above 180 it is right, and if it is below 180 then it is left*/

				int delta = turretTh - angle;

				std::cout << "Delta Angle: " << angle << std::endl;

				std::cout << "Turret Th: " << turretTh << std::endl;

				if (delta < 0)
				{
					delta += 360;
				}

				if ((int)turretTh == angle)
				{
					m_bShouldFireShell = true;
				}
				else if ((int)turretTh != angle)
				{
					turretGoRight();
					m_bShouldFireShell = false;
				}

				implementMove();
			}
		}
		else
		{
			algorithm.m_bGenerateNewPath = true;
			algorithm.m_bHasFoundEndCell = true;
			algorithm.m_pathTaken.clear();
		}
	}
}

/*OVERWRITTEN PARENT FUNCTIONALITY. (see: aitank.h for docs)*/
template <class T>
void AI<T>::collided()
{
	forwards = !forwards;
}

template <class T>
void AI<T>::markTarget(Position p)
{
	std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

template <class T>
void AI<T>::markEnemy(Position p)
{
	std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

template <class T>
void AI<T>::markBase(Position p)
{
	std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

template <class T>
void AI<T>::markShell(Position p)
{
	std::cout << "Shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

template <class T>
bool AI<T>::isFiring()
{
	return !forwards; // Fire when going backwards
}

template <class T>
void AI<T>::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}

template <class T>
void AI<T>::reset()
{
	algorithm.m_bGenerateNewPath = true;
	algorithm.m_bHasFoundEndCell = true;
	algorithm.m_pathTaken.clear();
	forwards = true;

	pos.set(pos.getX(), pos.getY(), 0);

}

/*AI<AStar> astar;
AI<BFS> bfs;
AI<DFS> dfs;*/
