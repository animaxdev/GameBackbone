#include "stdafx.h"

#include <Backbone\AnimatedSprite.h>
#include <Backbone\CompoundSprite.h>

#include <SFML\Graphics.hpp>

#include <string>

using namespace GB;


BOOST_AUTO_TEST_SUITE(CompoundSpriteTests)

struct ReusableObjects
{
	ReusableObjects() {
		aSpriteTexture = new sf::Texture();
		std::string testTexturePath = "..\\..\\Textures\\testSprite.png";
		aSpriteTexture->loadFromFile(testTexturePath);

		sprite = new sf::Sprite(*aSpriteTexture);

		sf::Vector2u textureDim = aSpriteTexture->getSize();
		std::vector<std::vector<unsigned int>> aSpriteAnims;
		std::vector<unsigned int> aSpriteAnim1 = { 0, 1, 2, 3 };
		aSpriteAnims.push_back(aSpriteAnim1);
		animSet1 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);
		animSet2 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);

		animSpriteWithAnim1 = new AnimatedSprite(*aSpriteTexture, animSet1);
		animSpriteWithAnim2 = new AnimatedSprite(*aSpriteTexture, animSet2);
	}
	~ReusableObjects() {

		delete aSpriteTexture;
		delete animSet1;
		delete animSet2;
		delete animSpriteWithAnim1;
		delete animSpriteWithAnim2;
		delete sprite;

	}

	AnimatedSprite* animSpriteWithAnim1;
	AnimatedSprite* animSpriteWithAnim2;
	AnimationSet* animSet1;
	AnimationSet* animSet2;
	sf::Texture* aSpriteTexture;
	sf::Sprite* sprite;
};

struct ReusableObjectsForOperations {

	ReusableObjectsForOperations() {

		aSpriteTexture = new sf::Texture();
		std::string testTexturePath = "..\\..\\Textures\\testSprite.png";
		aSpriteTexture->loadFromFile(testTexturePath);

		sprite = new sf::Sprite(*aSpriteTexture);
		sprite->setPosition(10, 0);
		sprite2 = new sf::Sprite(*aSpriteTexture);
		sprite2->setPosition(0, 10);

		sf::Vector2u textureDim = aSpriteTexture->getSize();
		std::vector<std::vector<unsigned int>> aSpriteAnims;
		std::vector<unsigned int> aSpriteAnim1 = { 0, 1, 2, 3 };
		aSpriteAnims.push_back(aSpriteAnim1);
		animSet1 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);
		animSet2 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);

		animSpriteWithAnim1 = new AnimatedSprite(*aSpriteTexture, animSet1);
		animSpriteWithAnim2 = new AnimatedSprite(*aSpriteTexture, animSet2);

		compoundSprite = new CompoundSprite({ sprite, sprite2 }, { animSpriteWithAnim1, animSpriteWithAnim2 });

	}

	~ReusableObjectsForOperations() {
		delete aSpriteTexture;
		delete animSet1;
		delete animSet2;
		delete animSpriteWithAnim1;
		delete animSpriteWithAnim2;
		delete sprite;
		delete sprite2;
		delete compoundSprite;
	}

	AnimatedSprite* animSpriteWithAnim1;
	AnimatedSprite* animSpriteWithAnim2;
	AnimationSet* animSet1;
	AnimationSet* animSet2;
	sf::Texture* aSpriteTexture;
	sf::Sprite* sprite;
	sf::Sprite* sprite2;
	CompoundSprite* compoundSprite;

};

BOOST_AUTO_TEST_SUITE(CompoundSprite_ctr)

// Test that the default constructor initializes values to empty and (0,0)
BOOST_FIXTURE_TEST_CASE(CompoundSprite_default_ctr, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	BOOST_CHECK(compoundSprite->getSfSprites()->empty());
	BOOST_CHECK(compoundSprite->getAnimatedSprites()->empty());
	BOOST_CHECK(compoundSprite->getPosition().x == 0 && compoundSprite->getPosition().y == 0);

	delete compoundSprite;
}

// Test that constructing the CompundSprite with its components correctly sets the components.
BOOST_FIXTURE_TEST_CASE(CompoundSprite_addAnimatedSprite_ctr, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector{sprite};
	std::vector<AnimatedSprite*> animatedSpriteVector{animSpriteWithAnim1, animSpriteWithAnim2};

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	//check that the internal vectors have the correct values
	BOOST_CHECK(!compoundSprite->getSfSprites()->empty());
	for (unsigned int i = 0; i < spriteVector.size(); ++i) {
		BOOST_CHECK(compoundSprite->getSfSprites()->at(i) == spriteVector[i]);
	}
	BOOST_CHECK(!compoundSprite->getAnimatedSprites()->empty());
	for (unsigned int i = 0; i < animatedSpriteVector.size(); ++i) {
		BOOST_CHECK(compoundSprite->getAnimatedSprites()->at(i) == animatedSpriteVector[i]);
	}

	// check that the initial position is set correctly.
	BOOST_CHECK(compoundSprite->getPosition().x == 0 && compoundSprite->getPosition().y == 0);

	delete compoundSprite;
}

// Test that the CompoundSprite is safe to construct with empty component vectors.
BOOST_AUTO_TEST_CASE(CompoundSprite_Empty_Component_Vector_ctr) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;
	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	//check that the component vectors are empty
	BOOST_CHECK(compoundSprite->getAnimatedSprites()->empty());
	BOOST_CHECK(compoundSprite->getSfSprites()->empty());


	delete compoundSprite;
}

// Test that CompoundSprite gets its components and position correctly set.
BOOST_FIXTURE_TEST_CASE(CompoundSprite_addSprites_setPosition_ctr, ReusableObjects) {
	std::vector<sf::Sprite*> sprites{sprite};
	std::vector<AnimatedSprite*> animSprites{animSpriteWithAnim1, animSpriteWithAnim2};
	const Point2D<float> compoundSpritePos{ 3,3 };

	CompoundSprite* compoundSprite = new CompoundSprite(sprites, animSprites, compoundSpritePos);

	// check that the components of the sprite are correctly assigned.
	BOOST_CHECK_EQUAL_COLLECTIONS(compoundSprite->getSfSprites()->begin(), compoundSprite->getSfSprites()->end(), sprites.begin(), sprites.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(compoundSprite->getAnimatedSprites()->begin(), compoundSprite->getAnimatedSprites()->end(), animSprites.begin(), animSprites.end());

	// check that the position is correctly assigned.
	BOOST_CHECK(compoundSprite->getPosition().x == compoundSpritePos.x && compoundSprite->getPosition().y == compoundSpritePos.y);

	delete compoundSprite;
}

// Test that CompoundSprite correctly sets its position.
BOOST_AUTO_TEST_CASE(CompoundSprite_setPosition_ctr) {
	const Point2D<float> compoundSpritePos{ 3,3 };

	CompoundSprite* compoundSprite = new CompoundSprite(compoundSpritePos);

	BOOST_CHECK(compoundSprite->getPosition().x == compoundSpritePos.x && compoundSprite->getPosition().y == compoundSpritePos.y);
	BOOST_CHECK(compoundSprite->getSfSprites()->empty());
	BOOST_CHECK(compoundSprite->getAnimatedSprites()->empty());

	delete compoundSprite;
}


BOOST_AUTO_TEST_SUITE_END() // END CompoundSprite_ctr

BOOST_AUTO_TEST_SUITE(CompoundSprite_getter)

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getSfSprites_Populated_Input_Vectors, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	spriteVector.push_back(sprite);
	animatedSpriteVector.push_back(animSpriteWithAnim1);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);
	
	BOOST_CHECK(compoundSprite->getSfSprites()->size() == 1);
	BOOST_CHECK(compoundSprite->getSfSprites()->at(0) == sprite);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getSfSprites_Populated_Sprite_Vector, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	spriteVector.push_back(sprite);
	animatedSpriteVector.push_back(animSpriteWithAnim1);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	BOOST_CHECK(compoundSprite->getSfSprites()->size() == 1);
	BOOST_CHECK(compoundSprite->getSfSprites()->at(0) == sprite);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getSfSprites_Populated_Animated_Vector, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	animatedSpriteVector.push_back(animSpriteWithAnim1);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	BOOST_CHECK(compoundSprite->getSfSprites()->size() == 0);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getSfSprites_Empty_Input_Vectors, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> emptyAnimatedSpriteVector;

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, emptyAnimatedSpriteVector);

	BOOST_CHECK(compoundSprite->getSfSprites()->empty());
	
	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getSfSprites_default_ctr, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	BOOST_CHECK(compoundSprite->getSfSprites()->empty());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getAnimatedSprites_Populated_Input_Vectors, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	spriteVector.push_back(sprite);
	animatedSpriteVector.push_back(animSpriteWithAnim1);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	BOOST_CHECK(compoundSprite->getAnimatedSprites()->size() == 1);
	BOOST_CHECK(compoundSprite->getAnimatedSprites()->at(0) == animSpriteWithAnim1);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getAnimatedSprites_Populated_Sprite_Vector, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	spriteVector.push_back(sprite);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	BOOST_CHECK(compoundSprite->getAnimatedSprites()->size() == 0);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getAnimatedSprites_Populated_Animated_Vector, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> animatedSpriteVector;

	animatedSpriteVector.push_back(animSpriteWithAnim1);

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, animatedSpriteVector);

	BOOST_CHECK(compoundSprite->getAnimatedSprites()->size() == 1);
	BOOST_CHECK(compoundSprite->getAnimatedSprites()->at(0) == animSpriteWithAnim1);

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getAnimatedSprites_Empty_Input_Vectors, ReusableObjects) {
	std::vector<sf::Sprite*> spriteVector;
	std::vector<AnimatedSprite*> emptyAnimatedSpriteVector;

	CompoundSprite* compoundSprite = new CompoundSprite(spriteVector, emptyAnimatedSpriteVector);

	BOOST_CHECK(compoundSprite->getAnimatedSprites()->empty());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_getAnimatedSprites_default_ctr, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	BOOST_CHECK(compoundSprite->getAnimatedSprites()->empty());

	delete compoundSprite;
}

BOOST_AUTO_TEST_SUITE_END() // END CompoundSprite_getter


BOOST_AUTO_TEST_SUITE(CompoundSprite_add)

BOOST_FIXTURE_TEST_CASE(CompoundSprite_addSprite, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->addSprite(sprite);
	std::vector<sf::Sprite*> *spriteVector = compoundSprite->getSfSprites();

	auto it = std::find(spriteVector->begin(), spriteVector->end(), sprite);
	BOOST_CHECK(it != spriteVector->end());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_addAnimatedSprite, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->addAnimatedSprite(animSpriteWithAnim1);
	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();

	auto it = std::find(animatedSpriteVector->begin(), animatedSpriteVector->end(), animSpriteWithAnim1);
	BOOST_CHECK(it != animatedSpriteVector->end());

	delete compoundSprite;
}

BOOST_AUTO_TEST_SUITE_END() // END CompoundSprite_add


BOOST_AUTO_TEST_SUITE(CompoundSprite_remove)

BOOST_FIXTURE_TEST_CASE(CompoundSprite_removeSprite, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->addSprite(sprite);
	compoundSprite->removeSprite(sprite);
	std::vector<sf::Sprite*> *spriteVector = compoundSprite->getSfSprites();

	BOOST_CHECK(spriteVector->empty());

	auto it = std::find(spriteVector->begin(), spriteVector->end(), sprite);
	BOOST_CHECK(it == spriteVector->end());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_removeSprite_empty, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->removeSprite(sprite);
	std::vector<sf::Sprite*> *spriteVector = compoundSprite->getSfSprites();

	BOOST_CHECK(spriteVector->empty());

	auto it = std::find(spriteVector->begin(), spriteVector->end(), sprite);
	BOOST_CHECK(it == spriteVector->end());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_removeAnimatedSprite, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->addAnimatedSprite(animSpriteWithAnim1);
	compoundSprite->removeAnimatedSprite(animSpriteWithAnim1);
	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();

	BOOST_CHECK(animatedSpriteVector->empty());
	auto it = std::find(animatedSpriteVector->begin(), animatedSpriteVector->end(), animSpriteWithAnim1);
	BOOST_CHECK(it == animatedSpriteVector->end());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_removeAnimatedSprite_empty, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();
	
	compoundSprite->removeSprite(sprite);
	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();

	BOOST_CHECK(animatedSpriteVector->empty());
	auto it = std::find(animatedSpriteVector->begin(), animatedSpriteVector->end(), animSpriteWithAnim1);
	BOOST_CHECK(it == animatedSpriteVector->end());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_clearComponents, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->addSprite(sprite);
	compoundSprite->addAnimatedSprite(animSpriteWithAnim1);
	compoundSprite->addAnimatedSprite(animSpriteWithAnim2);

	compoundSprite->clearComponents();
	std::vector<sf::Sprite*> *spriteVector = compoundSprite->getSfSprites();
	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();

	BOOST_CHECK(spriteVector->empty());
	BOOST_CHECK(animatedSpriteVector->empty());

	delete compoundSprite;
}

BOOST_FIXTURE_TEST_CASE(CompoundSprite_clearComponents_empty, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	compoundSprite->clearComponents();
	std::vector<sf::Sprite*> *spriteVector = compoundSprite->getSfSprites();
	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();

	BOOST_CHECK(spriteVector->empty());
	BOOST_CHECK(animatedSpriteVector->empty());

	delete compoundSprite;
}

BOOST_AUTO_TEST_SUITE_END() // END CompoundSprite_remove


BOOST_AUTO_TEST_SUITE(CompoundSprite_operations)

BOOST_FIXTURE_TEST_CASE(CompoundSprite_update, ReusableObjects) {
	CompoundSprite* compoundSprite = new CompoundSprite();

	std::vector<AnimatedSprite*> *animatedSpriteVector = compoundSprite->getAnimatedSprites();
	for (unsigned int i = 0; i < animatedSpriteVector->size(); ++i) {
		animatedSpriteVector->at(i)->runAnimation(0);
	}
	
	compoundSprite->update(sf::milliseconds(2));

	for (unsigned int i = 0; i < animatedSpriteVector->size(); ++i) {
		BOOST_CHECK(animatedSpriteVector->at(i)->getCurrentFrame() == 1);
	}

	delete compoundSprite;
}

BOOST_AUTO_TEST_CASE(CompoundSprite_scale) {
	//for some reason cant use reusable objects. recreate them here.
	sf::Texture* aSpriteTexture = new sf::Texture();
	std::string testTexturePath = "..\\..\\Textures\\testSprite.png";
	aSpriteTexture->loadFromFile(testTexturePath);
	sf::Vector2u textureDim = aSpriteTexture->getSize();
	std::vector<std::vector<unsigned int>> aSpriteAnims;
	std::vector<unsigned int> aSpriteAnim1 = { 0, 1, 2, 3 };
	aSpriteAnims.push_back(aSpriteAnim1);
	AnimationSet* animSet1 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);
	AnimationSet* animSet2 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);

	AnimatedSprite* animSpriteWithAnim1 = new AnimatedSprite(*aSpriteTexture, animSet1);
	AnimatedSprite* animSpriteWithAnim2 = new AnimatedSprite(*aSpriteTexture, animSet2);

	//create compound sprite
	sf::Sprite* sfSprite = new sf::Sprite(*aSpriteTexture);
	std::vector<sf::Sprite*> spriteVec = { sfSprite };
	std::vector<AnimatedSprite*> animSpriteVec = { animSpriteWithAnim1, animSpriteWithAnim2 };
	CompoundSprite* compoundSprite = new CompoundSprite(spriteVec, animSpriteVec);
	const float X_SCALE_FACTOR = 0.1f;
	const float Y_SCALE_FACTOR = 0.2f;

	compoundSprite->scale(X_SCALE_FACTOR, Y_SCALE_FACTOR);

	//ensure all sprites got the correct scale
	for (sf::Sprite* sprite : *(compoundSprite->getSfSprites()))
	{
		BOOST_CHECK(sprite->getScale().x == X_SCALE_FACTOR);
		BOOST_CHECK(sprite->getScale().y == Y_SCALE_FACTOR);
	}

	//ensure all animated sprites got the correct scale
	for (AnimatedSprite* animSprite : *(compoundSprite->getAnimatedSprites()))
	{
		BOOST_CHECK(animSprite->getScale().x == X_SCALE_FACTOR);
		BOOST_CHECK(animSprite->getScale().y == Y_SCALE_FACTOR);
	}

	delete compoundSprite;
	delete sfSprite;
	delete animSpriteWithAnim1;
	delete animSpriteWithAnim2;
	delete animSet1;
	delete animSet2;
	delete aSpriteTexture;
}

// Test that setting the scale of a compound sprite scales all of its component sprites
BOOST_FIXTURE_TEST_CASE(CompoundSprite_setScale_Two_Inputs, ReusableObjectsForOperations) {
	const float SCALE_FACTOR_X = 0.123f;
	const float SCALE_FACTOR_Y = 0.234f;
	compoundSprite->setScale(SCALE_FACTOR_X, SCALE_FACTOR_Y);

	// ensure that the scale of the sprite components has been set
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getScale().x, SCALE_FACTOR_X);
		BOOST_CHECK_EQUAL(sprite->getScale().y, SCALE_FACTOR_Y);
	}

	// ensure that the scale of the animated sprite components has been set
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getScale().x, SCALE_FACTOR_X);
		BOOST_CHECK_EQUAL(sprite->getScale().y, SCALE_FACTOR_Y);
	}
}

// Test that setting the scale of the compound sprite sets the scale of all of its component sprites
BOOST_FIXTURE_TEST_CASE(CompoundSprite_setScale_One_Input, ReusableObjectsForOperations) {
	const float SCALE_FACTOR_X = 0.123f;
	const float SCALE_FACTOR_Y = 0.234f;
	const Point2D<float> SCALE_FACTOR{ SCALE_FACTOR_X, SCALE_FACTOR_Y };
	compoundSprite->setScale(SCALE_FACTOR);

	// ensure that the scale of the sprite components has been set
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getScale().x, SCALE_FACTOR_X);
		BOOST_CHECK_EQUAL(sprite->getScale().y, SCALE_FACTOR_Y);
	}

	// ensure that the scale of the animated sprite components has been set
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getScale().x, SCALE_FACTOR_X);
		BOOST_CHECK_EQUAL(sprite->getScale().y, SCALE_FACTOR_Y);
	}

}

// Tests that scaling the compound sprite scales all of its components
BOOST_FIXTURE_TEST_CASE(CompoundSprite_Scale_Point2D, ReusableObjects) {
	std::vector<sf::Sprite*> sprites{ sprite };
	std::vector<AnimatedSprite*> animSprites{ animSpriteWithAnim1, animSpriteWithAnim2 };
	const Point2D<float> SCALE{0.5, 0.6};
	CompoundSprite compoundSprite(sprites, animSprites);
	compoundSprite.scale(SCALE);

	//ensure that all components were scaled correctly
	for (auto sprite : *compoundSprite.getSfSprites()) {
		BOOST_CHECK(sprite->getScale().x == SCALE.x && sprite->getScale().y == SCALE.y);
	}
	for (auto sprite : *compoundSprite.getAnimatedSprites()) {
		BOOST_CHECK(sprite->getScale().x == SCALE.x && sprite->getScale().y == SCALE.y);
	}
}

// Tests that rotating a compound sprite rotates all of its components
BOOST_FIXTURE_TEST_CASE(CompoundSprite_rotate_rotate_from_zero, ReusableObjectsForOperations) {
	const float ROTATION = 10;
	compoundSprite->rotate(ROTATION);

	// ensure all sprites were rotated
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
}

// Test that applying a rotation to a compound sprite is cumulative.
BOOST_FIXTURE_TEST_CASE(CompoundSprite_Rotate_Additive_Rotate, ReusableObjectsForOperations) {
	const float ROTATION = 10;
	const unsigned int NUM_ROTATIONS = 2;

	for (unsigned int i = 0; i < NUM_ROTATIONS; i++) {
		compoundSprite->rotate(ROTATION);
	}
	// ensure all sprites were rotated
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), NUM_ROTATIONS * ROTATION);
	}
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), NUM_ROTATIONS * ROTATION);
	}
}


// Test that setting the rotation of a compound sprite sets the rotation for all of its components
BOOST_FIXTURE_TEST_CASE(CompoundSprite_setRotation_rotate_from_zero, ReusableObjectsForOperations) {
	const float ROTATION = 10;
	compoundSprite->setRotation(10);

	// ensure all sprites were rotated
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
}

// Test that applying a rotation to a compound sprite is not cumulative.
BOOST_FIXTURE_TEST_CASE(CompoundSprite_setRotation_NonAdditive_Rotate, ReusableObjectsForOperations) {
	const float ROTATION = 10;
	const unsigned int NUM_ROTATIONS = 2;

	for (unsigned int i = 0; i < NUM_ROTATIONS; i++) {
		compoundSprite->setRotation(ROTATION);
	}
	// ensure all sprites were rotated
	for (auto sprite : *compoundSprite->getAnimatedSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
	for (auto sprite : *compoundSprite->getSfSprites()) {
		BOOST_CHECK_EQUAL(sprite->getRotation(), ROTATION);
	}
}

BOOST_AUTO_TEST_CASE(CompoundSprite_move) {
	//for some reason cant use reusable objects. recreate them here.
	sf::Texture* aSpriteTexture = new sf::Texture();
	std::string testTexturePath = "..\\..\\Textures\\testSprite.png";
	aSpriteTexture->loadFromFile(testTexturePath);
	sf::Vector2u textureDim = aSpriteTexture->getSize();
	std::vector<std::vector<unsigned int>> aSpriteAnims;
	std::vector<unsigned int> aSpriteAnim1 = { 0, 1, 2, 3 };
	aSpriteAnims.push_back(aSpriteAnim1);
	AnimationSet* animSet1 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);
	AnimationSet* animSet2 = new AnimationSet(aSpriteAnims, textureDim.x, textureDim.y, 2, 2);

	AnimatedSprite* animSpriteWithAnim1 = new AnimatedSprite(*aSpriteTexture, animSet1);
	AnimatedSprite* animSpriteWithAnim2 = new AnimatedSprite(*aSpriteTexture, animSet2);

	//create compound sprite
	sf::Sprite* sfSprite = new sf::Sprite(*aSpriteTexture);
	std::vector<sf::Sprite*> spriteVec = { sfSprite };
	std::vector<AnimatedSprite*> animSpriteVec = { animSpriteWithAnim1, animSpriteWithAnim2 };
	CompoundSprite* compoundSprite = new CompoundSprite(spriteVec, animSpriteVec);
	const float X_OFFSET = 500.6f;
	const float Y_OFFSET = 100.2f;

	compoundSprite->move(X_OFFSET, Y_OFFSET);

	//ensure all sprites got the correct position
	for (sf::Sprite* sprite : *(compoundSprite->getSfSprites()))
	{
		BOOST_CHECK_CLOSE_FRACTION(sprite->getPosition().x, X_OFFSET, 1.0f);
		BOOST_CHECK_CLOSE_FRACTION(sprite->getPosition().y, Y_OFFSET, 1.0f);
	}

	//ensure all animated sprites got the correct position
	for (AnimatedSprite* animSprite : *(compoundSprite->getAnimatedSprites()))
	{
		BOOST_CHECK_CLOSE_FRACTION(animSprite->getPosition().x, X_OFFSET, 1.0f);
		BOOST_CHECK_CLOSE_FRACTION(animSprite->getPosition().y, Y_OFFSET, 1.0f);
	}

	delete compoundSprite;
	delete sfSprite;
	delete animSpriteWithAnim1;
	delete animSpriteWithAnim2;
	delete animSet1;
	delete animSet2;
	delete aSpriteTexture;
}

BOOST_AUTO_TEST_SUITE_END() // END CompoundSprite_opertaions

BOOST_AUTO_TEST_SUITE_END() // END CompoundSpriteTests