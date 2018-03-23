#include "AI.h"
#include <random>



AI::AI()
{

}

/*! The AI Constructor
Sets up the start and current cell (they are the same at the start) as well
as the position of the tank itself.

\param startCell A pointer to the Cell this tank should start at.
*/
AI::AI(Cell * startCell, Cell * topOfGrid)
{
	clearMovement();
	m_setStartCell(startCell);
	m_setCurrentCell(startCell);

	m_topOfGrid = topOfGrid;

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
	m_endCell->setColour(sf::Color::Blue);
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

void AI::cleanup()
{
	if (cellsToCheck.size() > 0)
	{
		for (int i = cellsToCheck.size(); i == 0; i--)
		{
			cellsToCheck.pop();
		}
	}
	
	if (cellsToEvaluate.size() > 0)
	{
		for (int i = cellsToEvaluate.size(); i == 0; i--)
		{
			cellsToEvaluate.pop();
		}
	}
	

	visitedCells.clear();
}

void AI::BreadthFirstSearch()
{
	if (!hasFoundEndCell)
	{
		cellsToCheck.push(m_currentCell); //Add current cell to queue.

		m_currentCell->setColour(sf::Color::Yellow);

		for (int i = 0; i < visitedCells.size(); i++)
		{
			if (m_currentCell == visitedCells.at(i)) //If current cell is in visitedCells
			{
				cellsToCheck.front()->setColour(sf::Color::Green);
				cellsToCheck.pop(); //Pop front of queue.
				m_currentCell = cellsToCheck.front(); //Set current cell to next Cell in queue.
				i = 0; //Re-check if new current cell is in visitedCells.
			}
		}

		//If current cell hasn't been visited already.

		for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			/*Add all of the current cell's neighbours to the queue.*/
			cellsToCheck.push(m_currentCell->m_neighbours->at(i));

			m_currentCell->m_neighbours->at(i)->setColour(sf::Color::Yellow);
		}

		if (m_currentCell == m_endCell) //If current cell is our goal.
		{
			chooseNewEndCell = true;

			x = rand() % 10;
			y = rand() % 10;

			//We win.
			hasFoundEndCell = true;
			m_previousEndCell = m_endCell;

			m_currentCell->setColour(sf::Color::Magenta);

			cleanup();
			
		}
		else //If not
		{
			cellsToCheck.front()->setColour(sf::Color::White);

			cellsToCheck.pop(); //Pop the front of the queue.
			visitedCells.push_back(m_currentCell); //Add the current cell to visitedCells.

			m_currentCell->setColour(sf::Color::Green);

			m_currentCell = cellsToCheck.front(); //Set the current cell to the next item in the queue.
		}
	}
}

void AI::DepthFirstSearch()
{
	if (visitedCells.empty())
	{
		m_currentCell = m_topOfGrid;
	}

	if (!hasFoundEndCell)
	{
		m_currentCell->setColour(sf::Color::Red);

		for (int i = 0; i < visitedCells.size(); i++)
		{
			if (m_currentCell->m_neighbours->at(iNeighbourToCheck) == visitedCells.at(i))
			{
				bHasBeenVisited = true;
			}
		}

		if (!bHasBeenVisited)
		{
			if (iNeighbourToCheck < m_currentCell->m_neighbours->size())
			{
				cellsToEvaluate.push(m_currentCell->m_neighbours->at(iNeighbourToCheck)); //Add current cells first neighbour to the stack.

				m_currentCell = cellsToEvaluate.top(); //Current cell becomes the top item in the stack.


				if (m_currentCell == m_endCell) //Check if we've found our goal.
				{
					m_currentCell->setColour(sf::Color::Yellow);
					hasFoundEndCell = true;

					chooseNewEndCell = true;

					x = rand() % 10;
					y = rand() % 10;
	
					m_previousEndCell = m_endCell;

					m_currentCell = m_topOfGrid;

					cleanup();
				}
				else
				{
					
					m_currentCell->setColour(sf::Color::Blue);
					visitedCells.push_back(m_currentCell); //Add the current cell to visited cells.

					cellsToEvaluate.push(m_currentCell->m_neighbours->at(iNeighbourToCheck)); //Add the first neighbour of the current cell.
				}

				iNeighbourToCheck = 0;
			}
		}
		else
		{
			iNeighbourToCheck++;

			if (iNeighbourToCheck >= m_currentCell->m_neighbours->size())
			{
				cellsToEvaluate.pop();
				iNeighbourToCheck = 0;
			}

			bHasBeenVisited = false;
		}
	}
}

void AI::move()
{
	//BreadthFirstSearch();

	DepthFirstSearch();

	/*
		The code sets up the movement variables, and then calling implementMove() will
		then start the movement itself.
	*/

	/*
		* Turn until rotated at the angle the desired Cell is at.
		* Go forward.
		* Check to see if at the desired Cell, if not, Go forward again.
		* Stop if reached desired Cell.
	
	*/
	if (hasFoundEndCell)
	{
		if (!hasLocatedCell)
		{
			//The x and y position of the tank in the game world.
			int tankX = getX(); 
			int tankY = getY();

			//The x and y position of the current cell in the game world.
			int cellX = m_previousEndCell->m_realWorldPos.x;
			int cellY = m_previousEndCell->m_realWorldPos.y;

			//The angle the current cell is from the tank in degrees.
			float cellAngleFromTank = RAD2DEG(atan2(cellY - tankY, cellX - tankX));

			//The x and y direction the tank's turret is facing.
			//Could have issue of tank trying to go forward to its turret but turrent is not the current forward dir.
			//To solve, maybe put a point at the current forward dir that rotates /w the tank body.
			int tankLookX = firingPosition().getX();
			int tankLookY = firingPosition().getY();

			float angleTankIsFacing = RAD2DEG(atan2(tankLookY - tankY, tankLookX - tankX));

			std::cout << "Angle Tank Is Facing: " << (int)angleTankIsFacing << std::endl;
			std::cout << "Cell's Angle From Tank: " << (int)cellAngleFromTank << std::endl;

			if ((int)angleTankIsFacing < (int)cellAngleFromTank)
			{
				goRight();
			}
			else if ((int)angleTankIsFacing > (int)cellAngleFromTank)
			{
				goLeft();
			}
			else
			{
				hasLocatedCell = true;
			}
		}
		else
		{
			goForward();

			if (isInCell(m_previousEndCell))
			{
				stop();

				if (m_previousEndCell == m_endCell)
				{
					hasFoundEndCell = true;
				}
				else
				{
					hasLocatedCell = false;
					hasFoundEndCell = false; //Temporarily set to true to avoid continued movement without re-evaluation.
				}

				
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