#include "stdafx.h"

#include <AnimationSet.h>

#include <vector>

typedef std::vector<unsigned int> Animation;
typedef std::vector<Animation> AnimationVector;

/// <summary>
/// Struct to store generic animations that can be used for
/// multiple test cases
/// </summary>
struct ReusableGenericAnimations {
	Animation anim1 = { 0, 1, 2, 3 };
	Animation anim2 = { 3, 2, 1, 0 };
	AnimationVector numericAnimations = { anim1, anim2 };
};

//contains all tests for AnimationSet
BOOST_AUTO_TEST_SUITE(AnimationSet_Tests)

//all the ctrs for AnimationSet
BOOST_AUTO_TEST_SUITE(AnimationSet_ctrs)

//Test the row and col ctr for AnimationSet
BOOST_AUTO_TEST_CASE(AnimationSet_Default_ctr) {
	AnimationSet* animSet = new AnimationSet(2,2);

	//ensure that animations initializes to empty
	BOOST_CHECK(animSet->getAnimations()->empty());

	delete animSet;
}

//Test the ctr for AnimationSet that includes the dimensions of the animation texture
//and the frames for each animation
BOOST_FIXTURE_TEST_CASE(AnimationSet_Texture_Size_ctr, ReusableGenericAnimations) {
	AnimationSet* animSet = new AnimationSet(numericAnimations, 100, 100, 2, 2);

	//ensure that animations have been produced
	BOOST_CHECK(!animSet->getAnimations()->empty());

	//check that produced animations are logical
	std::vector<std::vector<sf::IntRect>>* rectAnims = animSet->getAnimations();

	sf::IntRect rect0(0, 0, 50, 50);
	sf::IntRect rect1(50, 0, 50, 50);
	sf::IntRect rect2(0, 50, 50, 50);
	sf::IntRect rect3(50, 50, 50, 50);

		//check the first animation
	BOOST_CHECK((*rectAnims)[0][0] == rect0);
	BOOST_CHECK((*rectAnims)[0][1] == rect1);
	BOOST_CHECK((*rectAnims)[0][2] == rect2);
	BOOST_CHECK((*rectAnims)[0][3] == rect3);

		//check the second animation
	BOOST_CHECK((*rectAnims)[1][0] == rect3);
	BOOST_CHECK((*rectAnims)[1][1] == rect2);
	BOOST_CHECK((*rectAnims)[1][2] == rect1);
	BOOST_CHECK((*rectAnims)[1][3] == rect0);

	delete animSet;
}

BOOST_AUTO_TEST_SUITE_END() // end AnimationSet_ctrs


BOOST_AUTO_TEST_SUITE_END()// end AnimationSet_Tests