#include "DFS.h"

/*! The default constructor*/
DFS::DFS()
{
}

void DFS::m_setCurrentCell(Cell * newCurrentCell)
{
	//m_currentCell = newCurrentCell;
}

void DFS::m_setEndCell(Cell * newEndCell)
{
	m_endCell = newEndCell;
}

void DFS::m_setTopOfGrid(Cell * topOfGrid)
{
	m_topOfGrid = topOfGrid;
}

/*! Clears all of the data structures that were used when finding the most
*	recent path.
*/
void DFS::cleanup()
{
	if (m_cellsToCheck.size() > 0)
	{
		for (int i = m_cellsToCheck.size(); i == 0; i--)
		{
			m_cellsToCheck.pop();
		}
	}

	m_visitedCells.clear();
}


/*! Finds a path to the end cell by travering the top left corner of the grid from
*	left to right until it finds the end cell.
*/
void DFS::Run(Cell * currentTankPos)
{
	//If no cells have been checked yet.
	if (m_visitedCells.empty())
	{
		//Evaluate the top left corner of the grid first.
		m_currentCell = m_topOfGrid;
		m_cellsToCheck.push(m_currentCell);
	}

	//Whilst we still haven't found the end cell.
	if (!m_cellsToCheck.empty())
	{
		m_currentCell->setColour(sf::Color::Red);

		for (int i = 0; i < m_visitedCells.size(); i++)
		{
			//If the current cell's neighbour that is being evaluated has already been visited.
			if (m_currentCell->m_neighbours->at(m_iNeighbourToCheck) == m_visitedCells.at(i))
			{
				m_bHasBeenVisited = true;
			}
		}

		//If the cell being evaluated hasn't been visited already.
		if (!m_bHasBeenVisited)
		{
			if (m_iNeighbourToCheck < m_currentCell->m_neighbours->size())
			{
				//Add current cell's first neighbour to the stack.
				m_cellsToCheck.push(m_currentCell->m_neighbours->at(m_iNeighbourToCheck)); 

				//Current cell becomes the top item in the stack.
				m_currentCell = m_cellsToCheck.top(); 


				if (m_currentCell == m_endCell) //Check if we've found our goal.
				{
					m_currentCell->setColour(sf::Color::Yellow);


					if (m_bGenerateNewPath)
					{
						for (int i = 0; i < m_visitedCells.size(); i++)
						{
							m_pathTaken.push_back(*m_visitedCells.at(i));
						}

						m_visitedCells.clear();

						m_bHasFoundPath = true; //We can start moving the tank.
					}

					m_bChooseNewEndCell = true;

					m_currentCell = m_topOfGrid;

					cleanup();
				}
				else
				{

					m_currentCell->setColour(sf::Color::Blue);

					m_visitedCells.push_back(m_currentCell); //Add the current cell to visited cells.

					//Add the first neighbour of the current cell to the cells to check.
					m_cellsToCheck.push(m_currentCell->m_neighbours->at(m_iNeighbourToCheck)); 
				}

				m_iNeighbourToCheck = 0; //Reset the neighbours to check index.
			}
		}
		else
		{
			m_iNeighbourToCheck++; //Check the next neighbour.

			//If there are no more neighbours on the current cell.
			if (m_iNeighbourToCheck >= m_currentCell->m_neighbours->size())
			{
				m_cellsToCheck.pop(); //Remove the item from the top of the stack.

				m_iNeighbourToCheck = 0; //Reset the neighbours to check index.
			}

			m_bHasBeenVisited = false;
		}
	}
}