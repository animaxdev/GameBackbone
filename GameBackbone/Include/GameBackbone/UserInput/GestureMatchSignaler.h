#pragma once

#include <GameBackbone/Util/DllUtil.h>
#include <GameBackbone/UserInput/EventComparator.h>
#include <GameBackbone/UserInput/EventFilter.h>
#include <GameBackbone/UserInput/InputHandler.h>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <cassert>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>


namespace GB
{
	struct GestureMatchSignalerProcessEventResult
	{
		bool actionFired;
		bool readyForInput;
		bool inputConsumed;
	};

	namespace detail 
	{
		template <class GestureMatchSignalerType>
		using GestureMatchSignalerProcessEvent = decltype(
			std::declval<GestureMatchSignalerProcessEventResult>() = std::declval<GestureMatchSignalerType>().processEvent(std::declval<sf::Int64>(), std::declval<const sf::Event&>())
		);

		template <class GestureMatchSignalerType, class = void>
		struct supports_gesture_match_signaler_process_event : std::false_type {};

		template <class GestureMatchSignalerType>
		struct supports_gesture_match_signaler_process_event <GestureMatchSignalerType, std::void_t<GestureMatchSignalerProcessEvent<GestureMatchSignalerType>>> :
			std::true_type {};
	}

	template <class GestureMatchSignalerType>
	using is_gesture_match_signaler =
		std::conjunction<
			std::is_copy_constructible<GestureMatchSignalerType>,
			detail::supports_gesture_match_signaler_process_event<GestureMatchSignalerType>
		>;

	template <class GestureMatchSignalerType>
	inline constexpr bool is_gesture_match_signaler_v = is_gesture_match_signaler<GestureMatchSignalerType>::value;

	template <
		typename EventCompare,
		typename EventFilter,
		std::enable_if_t<is_event_comparator_v<EventCompare>, bool> = true,
		std::enable_if_t<is_event_filter_v<EventFilter>, bool> = true
	>
	class GestureMatchSignaler
	{
	public:

		static constexpr sf::Int64 defaultMaxTimeBetweenInputs = 1000;

		using EventComparatorType = EventCompare;
		using EventFilterType = EventFilter;
		using ProcessEventResult = GestureMatchSignalerProcessEventResult;

		enum class EndType
		{
			Continuous,
			Reset,
			Block
		};

		GestureMatchSignaler(
			std::vector<sf::Event> gesture,
			std::function<void()> action,
			EndType endType,
			sf::Int64 maxTimeBetweenInputs,
			EventCompare eventComparator,
			EventFilter eventFilter
		) :
			m_gesture(std::move(gesture)),
			m_action(std::move(action)),
			m_endType(endType),
			m_maxTimeBetweenInputs(maxTimeBetweenInputs),
			m_position(0),
			m_readyForInput(true),
			m_eventComparator(std::move(eventComparator)),
			m_eventFilter(std::move(eventFilter))
		{
		}

		template <
			typename = std::enable_if_t< std::is_default_constructible_v<EventCompare>>,
			typename = std::enable_if_t< std::is_default_constructible_v<EventFilter>>
		>
		GestureMatchSignaler(
			std::vector<sf::Event> gesture,
			std::function<void()> action,
			EndType endType,
			sf::Int64 maxTimeBetweenInputs
		) :
			GestureMatchSignaler(
				std::move(gesture),
				std::move(action),
				endType,
				maxTimeBetweenInputs,
				EventCompare{},
				EventFilter{})
		{
		}

		template <
			typename = std::enable_if_t< std::is_default_constructible_v<EventCompare>>,
			typename = std::enable_if_t< std::is_default_constructible_v<EventFilter>>
		>
		GestureMatchSignaler(
			std::vector<sf::Event> gesture,
			std::function<void()> action,
			EndType endType) :
			GestureMatchSignaler(
				std::move(gesture),
				std::move(action),
				endType,
				defaultMaxTimeBetweenInputs)
		{
		}

		template <
			typename = std::enable_if_t< std::is_default_constructible_v<EventCompare>>,
			typename = std::enable_if_t< std::is_default_constructible_v<EventFilter>>
		>
		GestureMatchSignaler(
			std::vector<sf::Event> gesture,
			std::function<void()> action) :
			GestureMatchSignaler(
				std::move(gesture),
				std::move(action),
				EndType::Block,
				defaultMaxTimeBetweenInputs)
		{
		}

		ProcessEventResult processEvent(sf::Int64 elapsedTime, const sf::Event& event)
		{
			// Exit early if not ready for input
			if (!readyForInput())
			{
				return { false, false, false };
			}

			// Do not process the input if it does not pass the event filter
			if (!std::invoke(m_eventFilter, event))
			{
				return { false, readyForInput(), false };
			}

			// Process the input
			bool actionFired = false;
			bool inputConsumed = false;
			if (compareEvents(getNextEvent(), event) && elapsedTime < m_maxTimeBetweenInputs)
			{
				++m_position;
				if (m_position == m_gesture.size())
				{
					fireAction();
					actionFired = true;
				}
				inputConsumed = true;
			}
			// The input did not match. Disable this GestureMatchSignaler.
			else
			{
				m_position = 0;
				m_readyForInput = false;
			}

			// Return state to caller
			return { actionFired, readyForInput(), inputConsumed };
		}

		const std::vector<sf::Event>& getGesture() const
		{
			return m_gesture;
		}

		const std::function<void()>& getAction() const
		{
			return m_action;
		}

		std::function<void()>& getAction()
		{
			return m_action;
		}

		sf::Int64 getMaxTimeBetweenInputs() const
		{
			return m_maxTimeBetweenInputs;
		}

		EndType getEndType() const
		{
			return m_endType;
		}

		void setGesture(std::vector<sf::Event> gesture)
		{
			m_gesture = std::move(gesture);
		}

		void setAction(std::function<void()> action)
		{
			m_action = std::move(action);
		}

		void setMaxTimeBetweenInputs(sf::Int64 maxTimeBetweenInputs)
		{
			m_maxTimeBetweenInputs = maxTimeBetweenInputs;
		}

		void setEndType(EndType endType)
		{
			m_endType = endType;
		}

		void reset()
		{
			m_position = 0;
			m_readyForInput = true;
		}

		bool readyForInput() const
		{
			return m_readyForInput && !m_gesture.empty();
		}

	private:

		bool compareEvents(const sf::Event& lhs, const sf::Event& rhs)
		{
			return std::invoke(m_eventComparator, lhs, rhs);
		}

		void fireAction()
		{
			switch (getEndType())
			{
			case EndType::Continuous:
			{
				--m_position;
				break;
			}
			case EndType::Reset:
			{
				reset();
				break;
			}
			case EndType::Block:
			{
				m_position = 0;
				m_readyForInput = false;
				break;
			}
			}
			std::invoke(m_action);
		}

		const sf::Event& getNextEvent() const
		{
			assert(m_position < m_gesture.size());
			return m_gesture[m_position];
		}

		std::vector<sf::Event> m_gesture;
		std::function<void()> m_action;
		EndType m_endType;
		sf::Int64 m_maxTimeBetweenInputs;
		std::size_t m_position;
		bool m_readyForInput;
		EventCompare m_eventComparator;
		EventFilter m_eventFilter;
	};

	using KeyDownMatchSignaler = GestureMatchSignaler<KeyEventComparator, KeyDownEventFilter>;

	using JoystickButtonDownMatchSignaler = GestureMatchSignaler<JoystickButtonEventComparator, JoystickButtonDownEventFilter>;

	using MouseButtonDownMatchSignaler = GestureMatchSignaler<MouseButtonEventComparator, MouseButtonDownEventFilter>;

	using ButtonDownMatchSignaler = GestureMatchSignaler<ButtonEventComparator, AnyButtonDownEventFilter>;
}