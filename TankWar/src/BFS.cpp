#include "BFS.h"

BFS::BFS()
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
			m_cellsToCheck.pop();
		}
	}

	m_visitedCells.clear();
}

/*! Finds a path to m_endCell by evaluating a starting Cell and then evaluating all of its neighbours,
and if it hasn't found the end cell, it will evaluate all of the intial Cell's first neighbour's
neighbours and so on until it find the end cell.*/
void BFS::Run(Cell * currentTankPos)
{
	//If we haven't found the end cell yet.
	if (!m_bHasFoundPath) //*THIS USED TO BE HADFOUNDENDCELL, BUT THAT SHOULD BE USED FOR THE TANK NOT THIS.
	{
		m_cellsToCheck.push(m_currentCell); //Add current cell to the queue.

		m_currentCell->setColour(sf::Color::Yellow);

		for (int i = 0; i < m_visitedCells.size(); i++)
		{
			//If current cell has already been visited.
			if (m_currentCell == m_visitedCells.at(i)) 
			{
				m_cellsToCheck.front()->setColour(sf::Color::Green);

				//Remove the item that is in the front of the queue.
				m_cellsToCheck.pop();

				//Set current cell to next Cell in queue.
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
			//Ensure we get the opponent tank's position in the grid from Game.cpp.
			m_bChooseNewEndCell = true;


			//** STORE THE PATH TAKEN HERE LIKE IN ASTAR. REMEMEBER TO SET PREVIOUSES SOMEWHERE.


			m_bHasFoundPath = true;

			m_previousEndCell = m_endCell; //** REMOVE THIS IN FAVOUR OF PATH.

			m_currentCell->setColour(sf::Color::Magenta);

			cleanup();
		}
		else //If we haven't found the end cell.
		{
			m_cellsToCheck.front()->setColour(sf::Color::White);

			m_cellsToCheck.pop(); //Pop the front of the queue.

			m_visitedCells.push_back(m_currentCell); //Add the current cell to visitedCells.

			m_currentCell->setColour(sf::Color::Green);

			m_currentCell = m_cellsToCheck.front(); //Set the current cell to the next item in the queue.
		}
	}
}