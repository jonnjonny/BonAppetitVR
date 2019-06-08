//
// Created by Manqing (Stefanie) Zhang on 2019-05-02.
//

#ifndef LEVELS_OF_IMMERSION_ENUMS_H
#define LEVELS_OF_IMMERSION_ENUMS_H


enum class propsID {
	CHOPPING_BOARD,
	KNIFE,
	STAND_MIXER,
	BARREL,
	SUGAR_CUBES,
	EGG_CRATE,
	
};

enum class ingredientsID {
	SINGLE_EGG,
	CRACKED_EGG,
	SUGAR_CUBE,
	FLOUR,
	WATER,
	CHOCOLATE,
	STRAWBERRY
};

enum class SizeOption {
  ORIGINAL,
  SHRINK,
  EXPAND,
  RESET,
};

enum class RenderingOption_X {
  ENTIRE_STEREO,
  SKYBOX_STEREO,
  SKYBOX_MONO,
  DESERT_MONO,
  CLASSROOM_STEREO,
  CLASSROOM_MONO,
};

enum class RenderingOption_A {
  STEREO,
  MONO,
  LEFT_EYE_ONLY,
  RIGHT_EYE_ONLY,
  INVERSED,
};

enum class RenderingOption_B {
  REGULAR_TRACKING,
  ORIENTATION_ONLY_TRACKING,
  POSITION_ONLY_TRACKING,
  NO_TRACKING,
};

enum class WallCollections {
	LEFT,
  RIGHT,
  BOTTOM,

};
#endif //LEVELS_OF_IMMERSION_ENUMS_H
