#pragma once

#include <GameBackbone/Core/AnimatedSprite.h>
#include <GameBackbone/Core/Updatable.h>
#include <GameBackbone/Util/DllUtil.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <cstddef>
#include <set>
#include <vector>

namespace GB {

	/// <summary> Controls several sprites and animated sprites as one logical unit. </summary>
	class libGameBackbone CompoundSprite : public Updatable, public sf::Drawable, public sf::Transformable {
	public:
		//ctr / dtr
		CompoundSprite();
		explicit CompoundSprite(std::vector<sf::Sprite> components);
		CompoundSprite(std::vector<sf::Sprite> components, sf::Vector2f position);
		CompoundSprite(std::vector<sf::Sprite> components, std::vector<AnimatedSprite> animatedComponents);
		CompoundSprite(std::vector<sf::Sprite> components, std::vector<AnimatedSprite> animatedComponents, sf::Vector2f position);
		explicit CompoundSprite(sf::Vector2f position);
		CompoundSprite(const CompoundSprite&) = default;
		CompoundSprite& operator=(const CompoundSprite&) = default;
		CompoundSprite(CompoundSprite&&) noexcept = default;
		CompoundSprite& operator=(CompoundSprite&&) noexcept = default;
		virtual ~CompoundSprite() = default;

		// Component Getters
		virtual sf::Sprite& getSpriteComponent(std::size_t componentIndex);
		virtual AnimatedSprite& getAnimatedComponent(std::size_t componentIndex);
		virtual std::size_t getSpriteComponentCount() const;
		virtual std::size_t getAnimatedComponentCount() const;
		virtual bool isEmpty() const;

		// Add/Remove Components
		virtual std::size_t addComponent(sf::Sprite component);
		virtual std::size_t addComponent(AnimatedSprite component);
		virtual void removeSpriteComponent(std::size_t componentIndex);
		virtual void removeAnimatedComponent(std::size_t componentIndex);
		virtual void clearComponents();

		// Transformable API
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		void setRotation(float angle);
		void setScale(float factorX, float factorY);
		void setScale(const sf::Vector2f& factors);
		void setOrigin(float x, float y);
		void setOrigin(const sf::Vector2f& origin);
		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);
		void rotate(float angle);
		void scale(float factorX, float factorY);
		void scale(const sf::Vector2f& factor);

		// Updatable
		virtual void update(sf::Int64 elapsedTime) override;

	protected:
		
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		std::vector<sf::Sprite> m_components;
		std::vector<AnimatedSprite> m_animatedComponents;

	};
}
