#pragma once

#include <Backbone\GameRegion.h>
#include <Navigation\PathFinder.h>
#include <Util\Array2D.h>
#include <Util\Point.h>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics.hpp>

namespace GB {

	enum SELECTED_NAVIGATOR_BUTTON_TYPE
	{
		NAVIGATOR_1,
		NAVIGATOR_2,
		ALL_NAVIGATORS
	};

	/// <summary>
	/// GameRegion with logic for demonstrating basic path-finding demonstrations.
	/// </summary>
	/// <seealso cref="GameRegion" />
	class libGameBackbone NavigationDemoRegion : public GameRegion {
	public:

		// ctr / dtr
		NavigationDemoRegion();
		NavigationDemoRegion(const NavigationDemoRegion& other) = delete;
		NavigationDemoRegion(NavigationDemoRegion&& other) = delete;
		NavigationDemoRegion& operator= (const NavigationDemoRegion& other) = delete;
		NavigationDemoRegion& operator= (NavigationDemoRegion&& other) = delete;
		NavigationDemoRegion(sf::RenderWindow & window);
		virtual ~NavigationDemoRegion();

		//behavior

		virtual void behave(sf::Time currentTime) override;

		virtual void handleMouseClick(sf::Vector2f newPosition, sf::Mouse::Button button);



	protected:

		//ctr
		void init();

		//helper functions
		void initGUI();
		void initMaze(std::vector<Point2D<int>> nonBlockablePositions);
		sf::Vector2f gridCoordToWorldCoord(const Point2D<int>& gridCoordinate);
		Point2D<int> worldCoordToGridCoord(const sf::Vector2f& worldCoordinate);

		//movement functions
		void moveSpriteTowardsPoint(sf::Sprite* sprite, sf::Vector2f destination, float distance);
		void moveSpriteAlongPath(sf::Sprite* sprite, std::list<Point2D<int>>* path, sf::Int64 msPassed, float speed);

		//update logic storage
		sf::Time lastUpdateTime;

		// sprite textures
		sf::Texture* navigatorTexture;
		sf::Texture* gridTexture;

		//	store visual representation of maze and maze solvers
		Array2D<sf::Sprite*>* visualNavigationGrid;
		std::vector<sf::Sprite*> navigators;

		//path-finding
		Pathfinder regionPathfinder;
		NavigationGrid* navGrid;
		const unsigned int NAV_GRID_DIM = 20;
		const float VISUAL_GRID_SCALE = 1.0f;
		std::vector<std::list<Point2D<int>>> pathsReturn;

		//GUI handle functions
		SELECTED_NAVIGATOR_BUTTON_TYPE selectedNavigatorOption;
		void Navigator1CB();
		void Navigator2CB();
		void AllNavigatorsCB();

	};

}