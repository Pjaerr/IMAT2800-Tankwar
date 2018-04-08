#include "AI.h"
#include <random>

AI::AI()
{
}

//** TEMPORARY ENEMY DETECTION (use existing code.)
void AI::m_enemyWithinRange(float enemyX, float enemyY)
{
	float a = pos.getX() - enemyX;
	float b = pos.getY() - enemyY;

	float dist = sqrt(pow(a, 2) + pow(b, 2));

	if (dist < m_shootRadius.getRadius())
	{
		std::cout << "IN RANGE" << std::endl;
		m_canSeeEnemy = true;
	}
	else
	{
		m_canSeeEnemy = false;
	}
}

/*!Sets up the start and current cell (they are the same at the start) as well
*  as the position of the tank itself.
*/
AI::AI(Cell *startCell, Cell *topOfGrid)
{
	clearMovement();

	m_setCurrentCell(startCell);

	//Store DFS top of grid here.

	//m_shootRadius.setRadius(160.0f);
	m_shootRadius.setOrigin(m_shootRadius.getGlobalBounds().width * 0.5f, m_shootRadius.getGlobalBounds().height * 0.5f);
	m_shootRadius.setFillColor(sf::Color::Transparent);
	m_shootRadius.setOutlineThickness(2.0f);
	m_shootRadius.setOutlineColor(sf::Color::Black);

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
	return Astar.m_bChooseNewEndCell;
}

void AI::move()
{
	Astar.Run(m_currentTankPos);

	m_shootRadius.setPosition(pos.getX(), pos.getY());

	if (Astar.m_bHasFoundPath)
	{
		if (!Astar.m_pathTaken.empty())
		{
			Cell cellToMoveTo = Astar.m_pathTaken.at(Astar.m_pathTaken.size() - 1);

			if (!m_canSeeEnemy)
			{
				cellToMoveTo.setColour(sf::Color::Yellow);
				//std::cout << cellToMoveTo.getPos().x << std::endl;
				//std::cout << cellToMoveTo.getPos().y << std::endl;
				m_totalStepsX = cellToMoveTo.getPos().x - m_currentTankPos->getPos().x;
				m_totalStepsY = m_currentTankPos->getPos().y - cellToMoveTo.getPos().y;

				/*std::cout << "X: " << m_totalStepsX << std::endl;
				std::cout << "Y: " << m_totalStepsY << std::endl;*/

				int stepsX;
				int stepsY;

				m_totalStepsX == 0 ? stepsX = 0 : stepsX = m_totalStepsX / abs(m_totalStepsX);
				m_totalStepsY == 0 ? stepsY = 0 : stepsY = m_totalStepsY / abs(m_totalStepsY);

				int angle = m_currentTankPos->calculateAngleToCell(stepsX, stepsY);

				std::cout << angle << std::endl;

				float th = pos.getTh();

				if (m_bReachedCell)
				{
					m_iRightSteps = 0;
					m_iLeftSteps = 0;

					float temporaryTh = th;

					if (angle != -1)
					{
						while (temporaryTh != angle)
						{

							temporaryTh++;
							if (temporaryTh > 360)
								temporaryTh = 0;
							m_iRightSteps++;
						}

						temporaryTh = th;

						while (temporaryTh != angle)
						{

							temporaryTh--;
							if (temporaryTh < 0)
								temporaryTh = 360;
							m_iLeftSteps++;
						}

						m_bReachedCell = false;
					}
				}

				std::cout << th << std::endl;

				std::cout << "Right: " << m_iRightSteps << std::endl;
				std::cout << "Left: " << m_iLeftSteps << std::endl;

				if (th == angle)
				{
					goForward();
				}
				else if (m_iRightSteps < m_iLeftSteps)
				{
					goRight();
				}
				else
				{
					goLeft();
				}

				if (*m_currentTankPos == cellToMoveTo)
				{
					Astar.m_pathTaken.pop_back();
				}

				Astar.m_bGenerateNewPath = false;

				implementMove();
			}
		}
		else
		{
			Astar.m_bHasFoundPath = false;
			Astar.m_bGenerateNewPath = true;
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
	forwards = true;
}