#include "Cell.h"

Cell::Cell() {};


/*! Cell Constructor.
Sets up the position of this Cell in both Grid and World space and also
initialises any member variables.

\param xPos The x position of this Cell in grid space.
\param yPos The y position of this Cell in grid space.
\param wScaled The scaled width of the grid. (grid width / grid cols)
\param hScaled The scaled height of the gird. (grid height / grid rows)
*/
Cell::Cell(int xPos, int yPos, int wScaled, int hScaled)
{
	m_pos = sf::Vector2i(xPos, yPos);

	m_widthScaled = wScaled;
	m_heightScaled = hScaled;

	/*The position (m_pos) of this Cell is from within the grid. So if the grid was 10x10 and this Cell
	was initialised at grid[1][4], its position would be (1, 4). m_realWorldPos however is its grid pos
	scaled up to the size of the playable area.*/
	m_realWorldPos = sf::Vector2f((xPos * wScaled) + wScaled / 2, (yPos * hScaled) + hScaled / 2);

	m_rect = sf::RectangleShape(sf::Vector2f(m_widthScaled - 1, m_heightScaled - 1));

	m_rect.setPosition(sf::Vector2f(m_pos.x * m_widthScaled, m_pos.y * m_heightScaled));

	m_rect.setFillColor(sf::Color::Green);
	m_rect.setOutlineThickness(0.5f);
	m_rect.setOutlineColor(sf::Color::Black);

	m_neighbours = std::shared_ptr<std::vector<Cell*>>(new std::vector<Cell*>);

	calculateGeoScore();
}


/*! Overloaded sf::Drawable::draw function, draws m_rect.*/
void Cell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_rect);
}


/*! Overloaded == operator. Checks to see if this Cell and another are equal by
comparing their (x, y) grid positions.*/
bool Cell::operator==(const Cell& cell)
{
	/*If this cell and the other cell's x and y position are the same, return true.*/
	return (this->m_pos.x == cell.m_pos.x) && (this->m_pos.y == cell.m_pos.y);
}


/*! Calcuates this Cell's neighbours.
Using this Cell's (x, y) position in the grid, calculate each neighbour this Cell has
Top left, Top, Top Right, Right, Bottom Right, Bottom, Bottom Left, Left.

\param grid A reference to the Grid this Cell is apart of. Will work if not the same grid, but won't give intended results.
*/
void Cell::calculateNeighbours(std::vector< std::vector<Cell> > &grid)
{
	//THERE'S PROBABLY A BETTER WAY OF DOING THIS.

	/*Add the cell to the right and to the left of this cell as long as this cell isn't
	at the very edge of the grid.*/

	if (m_pos.x < grid.size() - 1)
	{
		m_neighbours->push_back(&grid[m_pos.x + 1][m_pos.y]);
	}

	if (m_pos.x > 0)
	{
		m_neighbours->push_back(&grid[m_pos.x - 1][m_pos.y]);
	}


	/*Add the cell above and below this cell as long as this cell isn't
	at the very top or bottom of the grid.*/

	if (m_pos.y < grid[0].size() - 1)
	{
		m_neighbours->push_back(&grid[m_pos.x][m_pos.y + 1]);
	}

	if (m_pos.y > 0)
	{
		m_neighbours->push_back(&grid[m_pos.x][m_pos.y - 1]);
	}

	//Add the neighbour to the top left
	if (m_pos.x > 0 && m_pos.y > 0)
	{
		m_neighbours->push_back(&grid[m_pos.x - 1][m_pos.y - 1]);
	}

	//Add the neighbour to the top right.
	if (m_pos.x < grid.size() - 1 && m_pos.y > 0)
	{
		m_neighbours->push_back(&grid[m_pos.x + 1][m_pos.y - 1]);
	}

	//Add the neighbour to the bottom left
	if (m_pos.x > 0 && m_pos.y < grid[0].size() - 1)
	{
		m_neighbours->push_back(&grid[m_pos.x - 1][m_pos.y + 1]);
	}

	//Add the neighbour to the bottom right.
	if (m_pos.x < grid.size() - 1 && m_pos.y < grid[0].size() - 1)
	{
		m_neighbours->push_back(&grid[m_pos.x + 1][m_pos.y + 1]);
	}
}


/*! Sets the outline colour of this Cell.

\param color The sf::Color to set the Cell to.
*/
void Cell::setColour(sf::Color color)
{
	m_rect.setFillColor(color);
}


/*! Returns this Cell's grid position.*/
sf::Vector2i Cell::getPos()
{
	return m_pos;
}

void Cell::calculateGeoScore() 
{
	m_iGeographicalScore = m_pos.x + m_pos.y;
}

void Cell::calculateHeuScore(Cell * otherCell)
{
	/*RAW EUCLIDEAN DISTANCE BETWEEN THIS CELL AND THE OTHER CELL.*/
	float x = std::pow((m_pos.x - otherCell->m_pos.x), 2);
	float y = std::pow((m_pos.y - otherCell->m_pos.y), 2);

	m_fHeuristicScore = std::sqrt(x + y);

	//m_fHeuristicScore = std::abs(m_pos.x - otherCell->m_pos.x) + abs(m_pos.y - otherCell->m_pos.y);
}