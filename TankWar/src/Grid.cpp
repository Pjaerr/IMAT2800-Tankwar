#include "Grid.h"

/*! Default Constructor.
	No values supplied, assume the space the grid occupies is 800x600
*/
Grid::Grid() 
{
	m_windowWidth = 800;
	m_windowHeight = 600;
}

/*! Constructor
	Sets the width and the height of the space the Grid will occupy. Usually the window dimensions
	but isn't restricted to that.

	\param windowWidth The width the grid occupies.
	\param windowHeight The height the grid occupies.
*/
Grid::Grid(float windowWidth, float windowHeight)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
}

/*!
	Creates a multidimensional std::vector where each element is a Cell object with its given
	position as a width/height scaled x/y within that multidimensional std::vector.

	\param cols The number of columns in the grid.
	\param rows The number of rows in the grid.
*/
std::vector< std::vector<Cell> > Grid::m_createGrid(int cols, int rows)
{
	/*Take the width and height of the window and divide it by the given columns and rows.
	This results in a grid that always takes up the full width/height of the screen. An
	800x600 resolution with a given 8 cols and 6 rows will be a 1:1 ratio for example.*/
	float widthScaled = m_windowWidth / cols;
	float heightScaled = m_windowHeight / rows;

	std::vector< std::vector<Cell> > grid; //The vector to be returned at the end.

	grid.resize(cols); //Initialise the top level vector with spaces matching that of the given columns.

	//Create the grid with default Cell objects.
	for (int i = 0; i < cols; i++) 
	{
		grid[i].resize(rows); //For every top level vector, create the same number of bottom level vectors as the given rows.

		for (int j = 0; j < rows; j++)
		{
			/*For every vector in the multidimensional vector, create a new Cell object with an (x, y) of its
			position in the multidimensional vector.*/
			grid[i][j] = Cell(i, j, widthScaled, heightScaled);
		}
	}

	//Calculate each Cells neighbours within the grid.
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			grid[i][j].calculateNeighbours(grid);
		}
	}

	return grid;
}