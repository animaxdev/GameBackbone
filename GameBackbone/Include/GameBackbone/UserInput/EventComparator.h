#pragma once

#include <SFML/Window/Event.hpp>

namespace GB
{
	template <typename T>
	using is_event_comparator = std::conjunction<
		std::is_invocable<T, const sf::Event&, const sf::Event&>,
		std::is_same<typename std::invoke_result<T, const sf::Event&, const sf::Event&>::type, bool>
	>;

	template <typename T>
	static inline constexpr bool is_event_comparator_v = is_event_comparator<T>::value;

	// Comparator used to compare two key events
	class KeyEventComparator
	{
	public:
		bool operator()(const sf::Event& userEvent, const sf::Event& gestureEvent) const
		{
			// Short circuit if the type doesn't match or if the types are not handlable.
			if (userEvent.type != gestureEvent.type || (userEvent.type != sf::Event::KeyPressed && userEvent.type != sf::Event::KeyReleased))
			{
				return false;
			}

			// Return true if the buttons match
			return (userEvent.key.code == gestureEvent.key.code && userEvent.key.code != sf::Keyboard::Unknown);
		}
	};

	// Comparator used to compare two joystick button events
	class JoystickButtonEventComparator
	{
	public:
		bool operator()(const sf::Event& userEvent, const sf::Event& gestureEvent) const
		{
			// Short circuit if the type doesn't match or if the types are not handlable.
			if (userEvent.type != gestureEvent.type || (userEvent.type != sf::Event::JoystickButtonPressed && userEvent.type != sf::Event::JoystickButtonReleased))
			{
				return false;
			}

			// Return true if the buttons match
			return userEvent.joystickButton.button == gestureEvent.joystickButton.button;
		}
	};

	// Comparator used to compare two mouse button events
	class MouseButtonEventComparator
	{
	public:
		bool operator()(const sf::Event& userEvent, const sf::Event& gestureEvent) const
		{
			// Short circuit if the type doesn't match or if the types are not handlable.
			if (userEvent.type != gestureEvent.type || (userEvent.type != sf::Event::MouseButtonPressed && userEvent.type != sf::Event::MouseButtonReleased))
			{
				return false;
			}

			// Return true if the buttons match
			return userEvent.mouseButton.button == gestureEvent.mouseButton.button;
		}
	};

	// Comparator used to compare two button down or button up events
	class ButtonEventComparator
	{
	public:
		bool operator()(const sf::Event& userEvent, const sf::Event& gestureEvent) const
		{
			// Short circuit if the type doesn't match or if the types are not handlable.
			if (userEvent.type != gestureEvent.type)
			{
				return false;
			}
			
			// Forward gestures to each comparator.
			return KeyEventComparator{}(userEvent, gestureEvent)
				|| JoystickButtonEventComparator{}(userEvent, gestureEvent)
				|| MouseButtonEventComparator{}(userEvent, gestureEvent);
		}
	};
}