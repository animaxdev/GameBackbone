#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <type_traits>

namespace GB {

	/// <summary>
	/// Used to compare which of two sf::Vector2 is less
	/// Designed to facilitate map and set use of sf::Vector2
	/// </summary>
	template <typename T>
	class IsVector2Less {
	public:
		constexpr bool operator()(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) const {
			return lhs.x < rhs.x || (!(rhs.x < lhs.x) && lhs.y < rhs.y);
		}
	};

	/// <summary>
	/// Used to create new Drawable* vectors using Drawable child pointer vectors
	/// Designed to improve compatibility between vectors of Drawable and Drawable children
	/// </summary>
	template <
		class T,
		std::enable_if_t <
			std::is_pointer_v<T> &&
			!std::is_const_v<std::remove_pointer_t<T>> &&
			std::is_base_of_v<sf::Drawable, std::remove_pointer_t<T>>,
			bool
		> = true
	>
	std::vector<sf::Drawable*> toDrawableVector(const std::vector<T>& DrawableChild)
	{
		// Create vector of drawable
		std::vector<sf::Drawable*> drawableVector(DrawableChild.begin(), DrawableChild.end());
		return drawableVector;
	}

	/// <summary>
	/// Used to create new Drawable* vectors using Drawable child vectors
	/// Designed to improve compatibility between vectors of Drawable and Drawable children
	/// </summary>
	template <
		class T,
		std::enable_if_t<
			!std::is_pointer_v<T> &&
			!std::is_reference_v<T> &&
			!std::is_const_v<T> &&
			std::is_base_of_v<sf::Drawable, T>,
			bool
		> = true
	>
	std::vector<sf::Drawable*> toDrawableVector(const std::vector<T>& drawableChildren)
	{
		// Create vector of drawable
		std::vector<sf::Drawable*> drawableVector(drawableChildren.size());
		auto getPointer = [](const T& drawableChild)
		{ 
			// Strip const (its only there because of the vector API) and
			// cast to base class pointer
			return static_cast<sf::Drawable*>(&const_cast<std::remove_const_t<T&>>(drawableChild));  
		};
		std::transform(drawableChildren.begin(), drawableChildren.end(), drawableVector.begin(), getPointer);
		return drawableVector;
	}

	template <class T>
	class myit
	{
	private:
		T wrapped_;
		//class charholder
		//{
		//	const char value_;
		//public:
		//	charholder(const char value) : value_(value) {}
		//	char operator*() const { return value_; }
		//};
	public:
		// Previously provided by std::iterator
		typedef T::value_type::element_type*     value_type;
		typedef T::difference_type				 difference_type;
		typedef T::value_type::element_type**    pointer;
		typedef T::value_type::element_type*&    reference;
		typedef T::iterator_category			 iterator_category;

		explicit myit(T wrapped) : wrapped_(wrapped) {}
		value_type operator*() const
		{
			return wrapped_->get();
		}
		bool operator==(const myit& other) const { return wrapped_ == other.wrapped_; }
		bool operator!=(const myit& other) const { return !(*this == other); }
		myit& operator++()
		{
			++wrapped_;
			return *this;
		}

		// value_type operator*() const { return std::toupper(*wrapped_); }
		/*charholder operator++(int)
		{
			charholder ret(std::toupper(*wrapped_));
			++wrapped_;
			return ret;
		}*/

	};


	// Unique Pointer
	template <class Container>
	auto uniqueToDumb(Container::iterator iteratorToConvert) -> wrappedIterator {

	}
}
