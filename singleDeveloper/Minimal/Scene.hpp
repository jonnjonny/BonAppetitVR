//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//



#ifndef WHEREISWALDOVR_SCENE_H
#define WHEREISWALDOVR_SCENE_H


#include <random>
#include <stdlib.h>     /* srand, rand */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"

#include <soil.h>

#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>

//#include "text.h"


//////////////////////////////////////////////////////////////////////
//
// The remainder of this code is specific to the scene we want to
// render.  I use glfw to render an array of cubes, but your
// application would perform whatever rendering you want
//

#include <vector>
#include "shader.h"
#include "Skybox.h"

#include "enums.h"

#include "Quad.hpp"
#include "Line.hpp"





// a class for building and rendering cubes
class Scene {

public:
  std::unique_ptr <Skybox> desertbox; //background box


  GLuint shaderID; //shader id for skybox
  GLuint woodShaderID;


  ///controller specific
  Model* sphere;
  glm::vec3 controllerPosition; //controller position passed from exampleApp
  GLuint controllerShaderID;


  ////table specs
  std::vector <glm::mat4> table_positions;
  std::vector <glm::vec3> table_center_positions;
  std::vector <TexturedCube*> tables;


  ///collection of props that constantly updating positions (maybe)
  std::vector<Model*> props;



  GLuint textureShaderID, textureId;
  GLint uniform_texture, attribute_texture;


  Scene() {


    // Shader Program
    shaderID = LoadShaders( "skybox.vert", "skybox.frag" );
	woodShaderID = LoadShaders("woodShader.vert", "woodShader.frag");


    //for desert box
    desertbox = std::make_unique <Skybox>( "desertbox" );
    desertbox->toWorld = glm::scale( glm::mat4( 1.0f ), glm::vec3( 5.0f ) );

    ///controller
    sphere = new Model( "./Models/sphere.obj" );

    // Shader Program
    controllerShaderID = LoadShaders( "shader.vert", "shader.frag" );

    populatingTables();


	//woodenBox = new Model("./Models/cube.obj");



	loadingModels();



	glActiveTexture(GL_TEXTURE0);
	textureShaderID = LoadShaders("textureShader.vert", "textureShader.frag");

	//Setting up textures
	unsigned char* data;
	int width, height, numChannels;

	data = stbi_load("./JPG/wood-texture-pattern.jpg", &width, &height, &numChannels, STBI_rgb_alpha);
	if (!data) {
		throw std::runtime_error("Cannot load file waldo.jpg");
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	uniform_texture = glGetUniformLocation(textureShaderID, "tex");
	glUniform1i(uniform_texture, 0);
	glBindTexture(GL_TEXTURE_2D, textureId);

  }


  void render( const glm::mat4 &projection, const glm::mat4 &view,
               ovrEyeType eyeType, int disabled, glm::vec3 sizeScalingVector,
               glm::vec3 translationVector,
               ovrPosef &eyepos, int freeze ) {


    desertbox->draw(shaderID, projection, view);


    ///controller
    renderController( projection, view, controllerPosition );


	glDisable(GL_CULL_FACE);

	//tables
	render11Tables(projection, view);

	//rendering props, order matters, add after existing lines!!!!!! Make sure matching the enum class propsID
	props.at((int)propsID::CHOPPING_BOARD)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	props.at((int)propsID::CHOPPING_BOARD)->Draw(textureShaderID, projection, view,  2);

	props.at((int)propsID::KNIFE)->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.02, 0)) *glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
	props.at((int)propsID::KNIFE)->Draw(woodShaderID, projection, view, 2);

	props.at((int)propsID::SINGLE_EGG)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.005, 0.005, 0.005))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	props.at((int)propsID::SINGLE_EGG)->Draw(textureShaderID, projection, view, 2);

	props.at((int)propsID::STAND_MIXER)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[2]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0)) ;
	props.at((int)propsID::STAND_MIXER)->Draw(textureShaderID, projection, view, 2);

	props.at((int)propsID::BARREL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[11]) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
	props.at((int)propsID::BARREL)->Draw(textureShaderID, projection, view, 2);

	props.at((int)propsID::SUGAR_BOWL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[4]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0));
	props.at((int)propsID::SUGAR_BOWL)->Draw(textureShaderID, projection, view, 2);

  }


  void loadingModels() {
	  props.push_back(new Model("./Models/ChoppingBoard.obj"));
	  props.push_back(new Model("./Models/Knife.obj"));
	  props.push_back(new Model("./Models/SingleEgg.obj"));
	  props.push_back(new Model("./Models/StandMixer.obj"));
	  props.push_back(new Model("./Models/teapot_s0.obj"));
	  props.push_back(new Model("./Models/SugarBowl.obj"));

  }	


  void render11Tables(const glm::mat4 &projection, const glm::mat4 &view) {
	  for (int i = 0; i < tables.size(); ++i) {
		  tables[i]->toWorld = table_positions[i];
		  tables[i]->draw(shaderID, projection, view);
	  }
  }

  void populatingTables() {
    for( int i = 0; i < 12; ++i ) {
      tables.push_back( new TexturedCube( "Martini" ) );
    }

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.5, 0.25));//glm::vec3(0.25, 0.5, 0.25));
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25, - 1, -0.75)) * scaleMatrix);//0
	//std::cout << glm::to_string(table_positions.at(table_positions.size() - 1)) << std::endl;
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75,  - 1, -0.75)) * scaleMatrix);//1
	//std::cout << glm::to_string(glm::vec4(0.5, 0, 0,1) * table_positions.at(0)   ) << std::endl;

	//std::cout << glm::to_string(table_positions.at(1)/table_positions.at(0)) << std::endl;

	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75, - 1, -0.25)) * scaleMatrix);//2
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75,  - 1, 0.25)) * scaleMatrix);//3
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.75,  - 1, 0.75)) * scaleMatrix);//4
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25, - 1, 0.75)) * scaleMatrix);//5
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25,  - 1, 0.75)) * scaleMatrix);//6
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, - 1, 0.75)) * scaleMatrix);//7
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, - 1, 0.25)) * scaleMatrix);//8
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75,  - 1, -0.25)) * scaleMatrix);//9
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, - 1, -0.75)) * scaleMatrix);//10
	table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25, - 1, -0.75)) * scaleMatrix);//11

	table_center_positions.push_back(glm::vec3(0.25, -0.495, -0.75));//0
	table_center_positions.push_back(glm::vec3(0.75, -0.495, -0.75));//1
	table_center_positions.push_back(glm::vec3(0.75, -0.495, -0.25));//2
	table_center_positions.push_back( glm::vec3(0.75, -0.495, 0.25));//3
	table_center_positions.push_back(glm::vec3(0.75, -0.495, 0.75));//4
	table_center_positions.push_back(glm::vec3(0.25, -0.495, 0.75));//5
	table_center_positions.push_back( glm::vec3(-0.25, -0.495, 0.75));//6
	table_center_positions.push_back(glm::vec3(-0.75, -0.495, 0.75));//7
	table_center_positions.push_back( glm::vec3(-0.75, -0.495, 0.25)); //8
	table_center_positions.push_back(glm::vec3(-0.75, -0.495, -0.25));//9
	table_center_positions.push_back(glm::vec3(-0.75, -0.495, -0.75));//10
	table_center_positions.push_back(glm::vec3(-0.25, -0.495, -0.75));//11


	/*
    glm::mat4 currPos = //glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.25, -0.25 - 1, -0.25 ) ) *
                        glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25, 0.5, 0.25 ) );

    currPos = glm::translate( currPos, glm::vec3( 0.25, -1.25, -0.75 ) );
    table_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -0.25 - 1, -0.25)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.5, 0.25)));//0
    std::cout << glm::to_string(table_positions[0]) << std::endl;

    currPos = glm::translate( currPos, glm::vec3( 10*0.5, 0, 0 ) );
	table_positions.push_back( glm::translate(glm::mat4(1.0f), glm::vec3(0.5, -0.25 - 1, -0.25)) * glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -0.25 - 1, -0.25)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.5, 0.25)));// glm::mat4( currPos ) );//1
	std::cout << glm::to_string(table_positions[1]) << std::endl;
	//std::cout << glm::to_string(currPos) << std::endl;
    //std::cout << std::endl;

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( currPos, glm::vec3( 0, 0, 0.5 ) ); //2,3,4
      table_positions.push_back(  glm::mat4( currPos ) );
    }

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( currPos, glm::vec3( -0.5, 0, 0 ) ); //5,6,7
      table_positions.push_back(  glm::mat4( currPos ) );
    }

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( currPos, glm::vec3( 0, 0, -0.5 ) ); //8,9,10
      table_positions.push_back(  glm::mat4( currPos ) );
    }

    currPos = glm::translate( currPos, glm::vec3( 0.5, 0, 0 ) ); //11
    table_positions.push_back(  glm::mat4( currPos ) );
*/
    ///finished calculating all the positions of tables

  }


  void renderController( const glm::mat4 &projection, const glm::mat4 &view,
                         const glm::vec3 &controllerPosition ) {
    sphere->toWorld =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    sphere->Draw( controllerShaderID, projection, view, 2 );
  }




};


#endif //WHEREISWALDOVR_SCENE_H
