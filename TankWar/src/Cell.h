#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Cell : public sf::Drawable
{
private:
	sf::RectangleShape m_rect; //!< The rectangle that gets drawn to the screen for this Cell.

	sf::Vector2i m_pos; //!< The x and y position of this Cell in the grid.

public:
	


	Cell();


	/*! Cell Constructor.
		Sets up the position of this Cell in both Grid and World space and also
		initialises any member variables.

		\param xPos The x position of this Cell in grid space.
		\param yPos The y position of this Cell in grid space.
		\param wScaled The scaled width of the grid. (grid width / grid cols)
		\param hScaled The scaled height of the gird. (grid height / grid rows)
	*/
	Cell(int xPos, int yPos, int wScaled, int hScaled);

	std::shared_ptr<std::vector<Cell*>> m_neighbours; //!< Vector of Cell pointers consisting of this Cell's neighbours.


	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Overloaded sf::Drawable::draw function.


	bool operator==(const Cell& cell); //!< Overload the == operator. Allows us to check if a Cell is equal to another.


	/*! Calcuates this Cell's neighbours.
		Using this Cell's (x, y) position in the grid, calculate each neighbour this Cell has
		Top left, Top, Top Right, Right, Bottom Right, Bottom, Bottom Left, Left.

		\param grid A reference to the Grid this Cell is apart of. Will work if not the same grid, but won't give intended results.
	*/
	void calculateNeighbours(std::vector< std::vector<Cell> > &grid);


	/*! Sets the outline colour of this Cell.

	\param color The sf::Color to set the Cell to.
	*/
	void setColour(sf::Color color);


	/*! Returns this Cell's grid position.*/
	sf::Vector2i getPos();

	sf::Vector2f m_realWorldPos; //!< This Cell's position in the real world (not grid space.)	

	int m_widthScaled; //!< The scaled width, used to convert between real world and grid space.
	int m_heightScaled; //!< The scaled height, used to convert between real world and grid space.

	Cell * m_previousCell = nullptr; //!< Used when recalling the final path. A pointer to the Cell that was visited before this one.

	int m_iGeographicalScore = 0;
	float m_fHeuristicScore = 0.0f;
	float m_fFinalScore = 0.0f;

	void calculateGeoScore();
	void calculateHeuScore(Cell * otherCell);
};