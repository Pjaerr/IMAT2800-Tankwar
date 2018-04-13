#include "Astar.h"

/*! The default constructor.*/
AStar::AStar()
{
}

void AStar::m_setCurrentCell(Cell * newCurrentCell)
{
	m_currentCell = newCurrentCell;

	m_openSet.push_back(m_currentCell);
}

void AStar::m_setEndCell(Cell * newEndCell)
{
	m_endCell = newEndCell;

	m_currentCell->calculateHeuScore(m_endCell);
	m_currentCell->m_fFinalScore = m_currentCell->m_fHeuristicScore;
}

void AStar::m_setTopOfGrid(Cell * topOfGrid)
{
}

/*! Clears all of the data structures that were used when finding the most
*	recent path and sets their pointers to null.
*/
void AStar::cleanup()
{
	for (int i = m_closedSet.size() - 1; i >= 0; i--)
	{
		m_closedSet.at(i)->m_bIsAnObstacle = false;
		m_closedSet.at(i)->setColour(sf::Color::Green);
		m_closedSet.at(i)->m_previousCell = nullptr;
		m_closedSet.at(i) = nullptr;
	}

	m_closedSet.clear();

	for (int i = m_openSet.size() - 1; i >= 0; i--)
	{
		m_openSet.at(i)->m_bIsAnObstacle = false;
		m_openSet.at(i)->setColour(sf::Color::Green);
		m_openSet.at(i)->m_previousCell = nullptr;
		m_openSet.at(i) = nullptr;
	}

	m_openSet.clear();
}

/*! Finds and returns the Cell inside of m_openSet that has the lowest final score.*/
Cell* AStar::m_findLowestFinalScore()
{
	Cell *cellWithLowestFinalScore = m_openSet.at(0); //Default lowest final score.

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

/*! Removes a given Cell from m_openSet.*/
void AStar::m_removeFromOpenSet(Cell * cellToRemove)
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

/*! Returns true if a given Cell is inside of m_closedSet and false if not.*/
bool AStar::m_bIsInClosedSet(Cell * cellToCheck)
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

/*! Returns true if a given Cell is inside of m_openSet and false if not.*/
bool AStar::m_bIsInOpenSet(Cell * cellToCheck)
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

/*! The function responsible for finding the best path to a given Cell in the grid (m_endCell).
*	It does this by evaluating the geographical, heuristic and ultimately the final score of a Cell's
*	neighbouring Cell's until it eventually finds the given end cell. This functionality is seperate
*	from the physical tank itself and solely works on the grid, storing the most recently found path.
*/
void AStar::Run(Cell * currentTankPos)
{
	//While there are Cell's still left to evaluate.
	if (!m_openSet.empty())
	{
		//If the end cell hasn't been found yet.
		if (m_currentCell != m_endCell)
		{
			//Set the current cell to be the cell in the open set with the lowest final score.
			m_currentCell = m_findLowestFinalScore();
		}

		//If we have found the end cell.
		if (m_currentCell == m_endCell) 
		{
			//Store the path taken.
			if (m_bGenerateNewPath)
			{
				Cell *temp = nullptr;
				temp = m_currentCell->m_previousCell;

				//While there is still a previous step in the path.
				while (temp != nullptr)
				{
					m_pathTaken.push_back(*temp); //Add the Cell to the path taken.

					temp->setColour(sf::Color::Yellow);

					temp = temp->m_previousCell; //Get the previous Cell in the path.
				}

				m_bHasFoundPath = true; //We can start moving the tank.
			}

			m_bChooseNewEndCell = true; //We should ask Game.cpp for the position of the opponent tank.

			//If it has already found the end cell, the tank must re-move to the new end cell.
			if (m_bHasFoundEndCell)
			{
				m_bHasFoundEndCell = false;
			}

			//Start the next search from the current position of the tank.
			m_currentCell = currentTankPos;

			cleanup();
		}

		/*Remove current cell from the open set and add it to the closed set.*/
		m_removeFromOpenSet(m_currentCell);
		m_closedSet.push_back(m_currentCell);

		m_currentCell->setColour(sf::Color::Red);

		//For every neighbour of the current cell.
		for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			Cell *currentNeighbour = m_currentCell->m_neighbours->at(i);

			if (!m_bIsInClosedSet(currentNeighbour) && !currentNeighbour->m_bIsAnObstacle) //If the Current neighbour is not in the closed set.
			{
				/*Work out the temporary Geographical score.
				(current cell g score + distance between current neighbour and current cell.)*/
				float fTentativeGeoScore = m_currentCell->m_iGeographicalScore + 1;

				if (!m_bIsInOpenSet(currentNeighbour)) //If the Current neighbour is not in the open set.
				{
					m_openSet.push_back(currentNeighbour); //Add it to the open set.

					currentNeighbour->setColour(sf::Color::Blue);

					//Set its g score to our temp g score.
					currentNeighbour->m_iGeographicalScore = fTentativeGeoScore; 
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

