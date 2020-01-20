#include <GameBackbone/Core/ActivationProvider.h>
#include <GameBackbone/Core/BasicGameRegion.h>

#include <TGUI/TGUI.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <exception>
#include <utility>

using namespace GB;

/// <summary>
/// Initializes a new instance of the <see cref="BasicGameRegion"/> class.
/// </summary>
BasicGameRegion::BasicGameRegion() : m_regionGUI(), m_nextRegion(*this) {}

/// <summary>
/// Initializes a new instance of the <see cref="BasicGameRegion"/> class.
/// The regions GUI is bound to the passed window.
/// </summary>
/// <param name="window">The window.</param>
BasicGameRegion::BasicGameRegion(sf::RenderWindow& window) : m_regionGUI(window), m_nextRegion(*this) {}

/// <summary>
/// Gets the GUI for this region.
/// </summary>
/// <returns>This regions GUI.</returns>
tgui::Gui& BasicGameRegion::getGUI() {
	return m_regionGUI;
}

/// <summary>
/// The game region that will become active after the next update.
/// </summary>
/// <returns>The region that will become active.</returns>
BasicGameRegion& BasicGameRegion::getNextRegion()
{
	return m_nextRegion;
}

/// <summary>
/// Sets the BasicGameRegion that will become active after the next update.
/// </summary>
/// <param name="nextRegion">The region that will become active.</param>
void BasicGameRegion::setNextRegion(BasicGameRegion& nextRegion)
{
	m_nextRegion = nextRegion;
}
