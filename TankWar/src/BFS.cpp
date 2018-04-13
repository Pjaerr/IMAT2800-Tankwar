#include "BFS.h"

BFS::BFS()
{
}

void BFS::m_setCurrentCell(Cell * newCurrentCell)
{
	m_currentCell = newCurrentCell;

	m_cellsToCheck.push(m_currentCell);
}

void BFS::m_setEndCell(Cell * newEndCell)
{
	m_endCell = newEndCell;
}

void BFS::m_setTopOfGrid(Cell * topOfGrid)
{
}

/*! Clears all of the data structures that were used by this object when searching for
a path to the end cell.*/
void BFS::cleanup()
{
	if (m_cellsToCheck.size() > 0)
	{
		for (int i = m_cellsToCheck.size(); i == 0; i--)
		{
			m_cellsToCheck.front()->setColour(sf::Color::Green);
			m_cellsToCheck.front()->m_previousCell = nullptr;
			m_cellsToCheck.front() = nullptr;
			m_cellsToCheck.pop();
		}
	}

	for (int i = 0; i < m_visitedCells.size(); i++)
	{
		m_visitedCells.at(i)->setColour(sf::Color::Green);
		m_visitedCells.at(i)->m_previousCell = nullptr;
		m_visitedCells.at(i) = nullptr;
	}
	m_visitedCells.clear();

	
	
}

/*! Finds a path to m_endCell by evaluating a starting Cell and then evaluating all of its neighbours,
and if it hasn't found the end cell, it will evaluate all of the intial Cell's first neighbour's
neighbours and so on until it find the end cell.*/
void BFS::Run(Cell * currentTankPos)
{
	//If we haven't found the end cell yet.
	if (!m_cellsToCheck.empty()) //*THIS USED TO BE HADFOUNDENDCELL, BUT THAT SHOULD BE USED FOR THE TANK NOT THIS.
	{
		m_cellsToCheck.push(m_currentCell); //Add current cell to the queue.

		m_currentCell->setColour(sf::Color::Yellow);

		for (int i = 0; i < m_visitedCells.size(); i++)
		{
			//If current cell has already been visited.
			if (m_currentCell == m_visitedCells.at(i)) 
			{
				m_cellsToCheck.front()->setColour(sf::Color::Red);

				//Remove the item that is in the front of the queue.
				m_cellsToCheck.pop();


				
				m_temporaryPath.push_back(m_currentCell);
				m_currentCell = m_cellsToCheck.front();

				i = 0; //Re-check if new current cell is in visitedCells.
			}
		}

		//If current cell hasn't been visited already.
		for (int i = 0; i < m_currentCell->m_neighbours->size(); i++)
		{
			/*Add all of the current cell's neighbours to the queue.*/
			m_cellsToCheck.push(m_currentCell->m_neighbours->at(i));

			m_currentCell->m_neighbours->at(i)->setColour(sf::Color::Yellow);
		}

		//If we have found the end cell.
		if (m_currentCell == m_endCell) 
		{
			//Store the path taken.
			if (m_bGenerateNewPath)
			{
				/*Cell *temp = nullptr;
				temp = m_currentCell->m_previousCell;

					

				//While there is still a previous step in the path.
				while (temp != nullptr)
				{
					std::cout << "loopyloop.f.f" << std::endl;
					m_pathTaken.push_back(*temp); //Add the Cell to the path taken.

					temp->setColour(sf::Color::Yellow);

					temp = temp->m_previousCell; //Get the previous Cell in the path.
				}*/

				for (int i = 0; i < m_temporaryPath.size(); i++)
				{
					m_pathTaken.push_back(*m_temporaryPath.at(i));
					//m_temporaryPath.at(i)->setColour(sf::Color::Red);
				}

				m_temporaryPath.clear();

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
		else //If we haven't found the end cell.
		{
			m_cellsToCheck.front()->setColour(sf::Color::White);

			m_cellsToCheck.pop(); //Pop the front of the queue.

			m_visitedCells.push_back(m_currentCell); //Add the current cell to visitedCells.

			m_currentCell->setColour(sf::Color::Red);

			m_temporaryPath.push_back(m_currentCell);

			m_currentCell = m_cellsToCheck.front(); //Set the current cell to the next item in the queue.
		}
	}
}