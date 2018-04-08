#pragma once

#include "Cell.h"

class Grid
{
private:
	float m_windowWidth; //!< The width the grid occupies.
	float m_windowHeight; //!< The height the grid occupies.

public:
	/*! Default Constructor.
	No values supplied, assume the space the grid occupies is 800x600
	*/
	Grid();

	/*! Constructor
	Sets the width and the height of the space the Grid will occupy. Usually the window dimensions
	but isn't restricted to that.

	\param windowWidth The width the grid occupies.
	\param windowHeight The height the grid occupies.
	*/
	Grid(float windowWidth, float windowHeight);

	float m_widthScaled;
	float m_heightScaled;

	/*!
	Creates a multidimensional std::vector where each element is a Cell object with its given
	position as a width/height scaled x/y within that multidimensional std::vector.

	\param cols The number of columns in the grid.
	\param rows The number of rows in the grid.
	*/
	std::vector< std::vector<Cell> > m_createGrid(int cols, int rows);
};