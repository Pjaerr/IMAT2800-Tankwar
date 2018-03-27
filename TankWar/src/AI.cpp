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

	/*OUTSIDE LOOP ASTAR STUFF PLS TO CLEAN*/
	m_openSet.push_back(m_currentCell);

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

	m_currentCell->calculateHeuScore(m_endCell);
	m_currentCell->m_fFinalScore = m_currentCell->m_fHeuristicScore;
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
	if (m_cellsToCheckQueue.size() > 0)
	{
		for (int i = m_cellsToCheckQueue.size(); i == 0; i--)
		{
			m_cellsToCheckQueue.pop();
		}
	}
	
	if (m_cellsToCheckStack.size() > 0)
	{
		for (int i = m_cellsToCheckStack.size(); i == 0; i--)
		{
			m_cellsToCheckStack.pop();
		}
	}
	

	m_visitedCells.clear();


	for (int i = m_closedSet.size() - 1; i >= 0; i--)
	{
		m_closedSet.at(i)->setColour(sf::Color::Green);
	}

	m_closedSet.clear();

	for (int i = m_openSet.size() - 1; i >= 0; i--)
	{
		m_openSet.at(i)->setColour(sf::Color::Green);
	}

	m_openSet.clear();
}

void AI::BreadthFirstSearch()
{
	if (!m_bHasFoundEndCell)
	{
		m_cellsToCheckQueue.push(m_currentCell); //Add current cell to queue.

		m_currentCell->setColour(sf::Color::Yellow);

		for (int i = 0; i < m_visitedCells.size(); i++)
		{
			if (m_currentCell == m_visitedCells.at(i)) //If current cell is in visitedCells
			{
				m_cellsToCheckQueue.front()->setColour(sf::Color::Green);
				m_cellsToCheckQueue.pop(); //Pop front of queue.
				m_currentCell = m_cellsToCheckQueue.front(); //Set current cell to next Cell in queue.
				i = 0; //Re-check if new current cell is in visitedCells.
			}
		}

		//If current cell hasn't been visited already.

		for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			/*Add all of the current cell's neighbours to the queue.*/
			m_cellsToCheckQueue.push(m_currentCell->m_neighbours->at(i));

			m_currentCell->m_neighbours->at(i)->setColour(sf::Color::Yellow);
		}

		if (m_currentCell == m_endCell) //If current cell is our goal.
		{
			m_bChooseNewEndCell = true;

			x = rand() % 10;
			y = rand() % 10;

			//We win.
			m_bHasFoundEndCell = true;
			m_previousEndCell = m_endCell;

			m_currentCell->setColour(sf::Color::Magenta);

			cleanup();
			
		}
		else //If not
		{
			m_cellsToCheckQueue.front()->setColour(sf::Color::White);

			m_cellsToCheckQueue.pop(); //Pop the front of the queue.
			m_visitedCells.push_back(m_currentCell); //Add the current cell to visitedCells.

			m_currentCell->setColour(sf::Color::Green);

			m_currentCell = m_cellsToCheckQueue.front(); //Set the current cell to the next item in the queue.
		}
	}
}

void AI::DepthFirstSearch()
{
	if (m_visitedCells.empty())
	{
		m_currentCell = m_topOfGrid;
	}

	if (!m_bHasFoundEndCell)
	{
		m_currentCell->setColour(sf::Color::Red);

		for (int i = 0; i < m_visitedCells.size(); i++)
		{
			if (m_currentCell->m_neighbours->at(m_iNeighbourToCheck) == m_visitedCells.at(i))
			{
				m_bHasBeenVisited = true;
			}
		}

		if (!m_bHasBeenVisited)
		{
			if (m_iNeighbourToCheck < m_currentCell->m_neighbours->size())
			{
				m_cellsToCheckStack.push(m_currentCell->m_neighbours->at(m_iNeighbourToCheck)); //Add current cells first neighbour to the stack.

				m_currentCell = m_cellsToCheckStack.top(); //Current cell becomes the top item in the stack.


				if (m_currentCell == m_endCell) //Check if we've found our goal.
				{
					m_currentCell->setColour(sf::Color::Yellow);
					m_bHasFoundEndCell = true;

					m_bChooseNewEndCell = true;

					x = rand() % 10;
					y = rand() % 10;
	
					m_previousEndCell = m_endCell;

					m_currentCell = m_topOfGrid;

					cleanup();
				}
				else
				{
					
					m_currentCell->setColour(sf::Color::Blue);
					m_visitedCells.push_back(m_currentCell); //Add the current cell to visited cells.

					m_cellsToCheckStack.push(m_currentCell->m_neighbours->at(m_iNeighbourToCheck)); //Add the first neighbour of the current cell.
				}

				m_iNeighbourToCheck = 0;
			}
		}
		else
		{
			m_iNeighbourToCheck++;

			if (m_iNeighbourToCheck >= m_currentCell->m_neighbours->size())
			{
				m_cellsToCheckStack.pop();
				m_iNeighbourToCheck = 0;
			}

			m_bHasBeenVisited = false;
		}
	}
}

Cell* AI::m_findLowestFinalScore()
{
	Cell * cellWithLowestFinalScore = m_openSet.at(0); //Default lowest final score.

	for (int i = 0; i < m_openSet.size(); i++)
	{
		//If the final score of i in the open set is lower than the current lowest final score.
		if (m_openSet.at(i)->m_fFinalScore < cellWithLowestFinalScore->m_fFinalScore)
		{
			cellWithLowestFinalScore = m_openSet.at(i); //Set that Cell as the cell with the lowest final score.
		}
	}

	return cellWithLowestFinalScore;
}

void AI::m_removeFromOpenSet(Cell * cellToRemove)
{
	for (int i = m_openSet.size() - 1; i >= 0; i--)
	{
		if (m_openSet.at(i) == cellToRemove)
		{
			m_openSet.erase(m_openSet.begin() + i);
			break;
		}
	}
}

bool AI::m_bIsInOpenSet(Cell * cellToCheck)
{
	for (int i = m_openSet.size() - 1; i >= 0; i--)
	{
		if (m_openSet.at(i) == cellToCheck)
		{
			return true;
		}
	}

	return false;
}

bool AI::m_bIsInClosedSet(Cell * cellToCheck)
{
	for (int i = m_closedSet.size() - 1; i >= 0; i--)
	{
		if (m_closedSet.at(i) == cellToCheck)
		{
			return true;
		}
	}

	return false;
}

void AI::AStar()
{
	//While the open set is not empty.
	if (!m_openSet.empty())
	{

		//If the end cell hasn't been found
		if (m_currentCell != m_endCell)
		{
			//Set the current cell to be the cell in the open set with the lowest final score.
			m_currentCell = m_findLowestFinalScore(); 
		}
		

		if (m_currentCell == m_endCell) //We have found the end cell.
		{
			m_bChooseNewEndCell = true;

			//Choose a new random position in the grid.
			x = rand() % 10;
			y = rand() % 10;

			m_bHasFoundEndCell = true;
			m_previousEndCell = m_endCell;

			cleanup();
		}

		/*Remove current cell from the open set and add it to the closed set.*/
		m_removeFromOpenSet(m_currentCell);
		m_closedSet.push_back(m_currentCell);

		m_currentCell->setColour(sf::Color::Red);

		//For every neighbour of the current cell.
		for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			Cell * currentNeighbour = m_currentCell->m_neighbours->at(i);

			if (!m_bIsInClosedSet(currentNeighbour)) //Current neighbour is not in the closed set.
			{
				/*Work out the temporary Geographical score. 
				(current cell g score + distance between current neighbour and current cell.)*/
				float fTentativeGeoScore = m_currentCell->m_iGeographicalScore + 1;

				if (!m_bIsInOpenSet(currentNeighbour)) //Current neighbour is not in the open set.
				{
					m_openSet.push_back(currentNeighbour); //Add it to the open set.
					currentNeighbour->setColour(sf::Color::Blue);
					currentNeighbour->m_iGeographicalScore = fTentativeGeoScore; //Set its g score to our temp g score.
				}
				else //If it is in the open set.
				{
					//If our temporary g score is better than the neighbours current g score.
					if (fTentativeGeoScore < currentNeighbour->m_iGeographicalScore)
					{
						//Set the current neighbours g score to our temp g score.
						currentNeighbour->m_iGeographicalScore = fTentativeGeoScore;
					}
				}

				//Calculate the heuristic cost of our current neighbour to the end cell.
				currentNeighbour->calculateHeuScore(m_endCell); 

				//Calculate our current neighbour's final score.
				currentNeighbour->m_fFinalScore = currentNeighbour->m_iGeographicalScore + currentNeighbour->m_fHeuristicScore;

				//Set the previous cell of our current neighbour to be our current cell.
				currentNeighbour->m_previousCell = m_currentCell;
			}
		}

	}
}

void AI::move()
{
	AStar();
	//BreadthFirstSearch();

	//DepthFirstSearch();

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
	/*if (m_bHasFoundEndCell)
	{
		if (!m_bHasLocatedCell)
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
				m_bHasLocatedCell = true;
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
					m_bHasFoundEndCell = true;
				}
				else
				{
					m_bHasLocatedCell = false;
					m_bHasFoundEndCell = false; //Temporarily set to true to avoid continued movement without re-evaluation.
				}

				
			}
		}
	}
	
	

	implementMove();*/
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