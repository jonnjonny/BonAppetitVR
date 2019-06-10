//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "shader.h"
#include "rpc/client.h"
#include "Player.h"
#include "SceneGraph.hpp"
#include "Skybox.h"
#include "BoundingBox.hpp"

#include <random>
#include <stdlib.h>     /* srand, rand */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"


#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>

#include <vector>
#include "shader.h"


#include "enums.h"


#include "Quad.hpp"


//#include "text.h"

// a class for building and rendering cubes
class Scene {

	const unsigned int ascii_A{ 65 };
	const unsigned int ascii_Z{ 90 };
	const unsigned int ascii_a{ 97 };
	const unsigned int ascii_z{ 122 };
	const unsigned int ascii_comma{ 65 };
	const unsigned int ascii_period{ 65 };
	const unsigned int ascii_exclamation{ 65 };

  //Model* sphere;

  Player* player1;


  Player* player2;


  Skybox* desertbox; //background box


  GLuint shaderID; //shader id for skyboxB
  GLuint woodShaderID;


  GLuint skyBoxShaderID;


  GLuint boundingBoxShaderID;


  ///controller specific
  Model* sphere;


  glm::vec3 controllerPosition; //controller position passed from exampleApp
  GLuint controllerShaderID;


  ////table specs
  std::vector <glm::vec3> table_positions;


  std::vector <glm::vec3> table_center_positions;


  std::vector <TexturedCube*> tables;


  std::vector <Model*> desks;


  Model* desk;


  ///collection of props that constantly updating positions (maybe)
  std::vector <std::string> modelFileNames;


  std::vector <Model*> props;
  std::vector <Model*> ingredients;
  //std::vector<std::string> props;




  GLuint textureShaderID, textureId, textureId2;


  GLint uniform_texture_from_picture, attribute_texture, uniform_texture_from_screen;


  //processing bar specs
  Model* processingBar;


  GLuint processingBarShaderID; //shader id for skyboxB

  std::vector <std::string> textureFileNames;


  std::vector <GLuint> textureIds;


  Model* recipeBookOpened;


  Model* recipeBookClosed;


  Quad* screen;


  GLuint screenShaderID;


  GLuint screenFbos[2], screenRbos[2], screenTextures[2];

  
  unordered_map<char, Model*> letters;

  Model* hands[2];

public:
  Scene() {
    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );
    woodShaderID = LoadShaders( "woodShader.vert", "woodShader.frag" );
    skyBoxShaderID = LoadShaders( "skybox.vert", "skybox.frag" );
    processingBarShaderID = LoadShaders( "processingBarShader.vert", "processingBarShader.frag" );
    boundingBoxShaderID = LoadShaders( "boundingbox.vert", "boundingbox.frag" );
    screenShaderID = LoadShaders( "screenShader.vert", "screenShader.frag" );
    controllerShaderID = LoadShaders( "shader.vert", "shader.frag" );    
	textureShaderID = LoadShaders( "textureFromPictureShader.vert","textureFromPictureShader.frag" );

	textureId2 = LoadShaders("textureFromPictureShader.vert", "textureFromPictureShader.frag");

    //for desert box
    desertbox = new Skybox( "desertbox" );
    desertbox->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))*glm::scale( glm::mat4( 1.0f ), glm::vec3( 5.0f ) );

    ///controller
    sphere = new Model( "./Models/sphere.obj");

    populatingTables();

    loadingModels();

    processingBar = new Model( "./Models/cube.obj" );


	//hands[0] = new Model("./Models/Hand_left.obj");
	//hands[1] = new Model("./Models/Hand_right.obj");



	///for screen appearing from opening book
    screen = new Quad();
	screen->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
                  //   * glm::rotate( glm::mat4( 1.0f ), glm::radians( 45.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) )
                    // * glm::scale( glm::mat4( 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) );
    populateInFrameRenderingBuffers();


    player1 = new Player();
    player2 = new Player();


    desk = new Model( "./Models/Oven.obj", 1.0f );
	recipeBookOpened = new Model("./Models/RecipeBookOpened.obj");
	recipeBookClosed = new Model("./Models/RecipeBookClosed.obj");

	//Props
	props.at((int)propsID::KNIFE)->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.02, 0)) *
		glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))* 
		glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));

	props.at((int)propsID::STAND_MIXER)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[2]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	/*props.at((int)propsID::STAND_MIXER_BOWL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[6]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2))* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1, 0, 0));
	props.at((int)propsID::STAND_MIXER_MACHINE)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[2]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));*/

	props.at((int)propsID::BARREL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[11]) * 
		glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));

	props.at((int)propsID::SUGAR_BOWL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[4]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5,0.5,0.5 ))* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 1, 0));

	props.at((int)propsID::EGG_CRATE)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[10]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));

	props.at((int)propsID::FLOUR_SACK)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[7]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	//props.at((int)propsID::CHOCOLATE)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[9]) *
	//	glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	//props.at((int)propsID::STRAWBERRY)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[8]) *
	//	glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));

	//Ingredients
	ingredients.at((int)ingredientsID::SINGLE_EGG)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.001, 0.001, 0.001))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::CRACKED_EGG)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::SUGAR_CUBE)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::FLOUR)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	/*ingredients.at((int)ingredientsID::WATER)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::CHOCOLATE)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	ingredients.at((int)ingredientsID::STRAWBERRY)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5))*
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));*/
	loadTextureFiles();

	populateLetterModels();
  }


  void populateLetterModels() {
	  letters = unordered_map<char, Model*>();
	  for (int i = 0; i < 1; i++) {
		  Model* upperCaseModel = new Model((std::string("./Models/Letters/") + (char)(ascii_A+i) + std::string(".obj")).c_str());
		  upperCaseModel->toWorld = //glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
		      // glm::rotate( glm::mat4( 1.0f ), glm::radians( 45.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) )*
			glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.1f, 0.1f, 0.1f ) );
		  letters.insert({ (char)ascii_A, upperCaseModel });
		 // Model* lowerCaseModel = new Model((std::string("./Models/Letters/") + (char)(ascii_a+i) + std::string(".obj")).c_str());
		 // letters.insert({ (char)ascii_A, lowerCaseModel });
	  }
	  
  }

  void populateInFrameRenderingBuffers() {

	  ovr::for_each_eye([&](ovrEyeType eye)
	  {
		  ///Framebuffers
		  screenFbos[eye] = 0;
		  glGenFramebuffers(1, &screenFbos[eye]);
		  glBindFramebuffer(GL_FRAMEBUFFER, screenFbos[eye]);


		  screenTextures[eye] = 0;
		  glActiveTexture(GL_TEXTURE0);
		  glGenTextures(1, &screenTextures[eye]);
		  glBindTexture(GL_TEXTURE_2D, screenTextures[eye]);
		  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB,
			  GL_UNSIGNED_BYTE, NULL);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTextures[eye], 0);


		  ///Renderbuffers
		  screenRbos[eye] = 0;
		  glGenRenderbuffers(1, &screenRbos[eye]);
		  glBindRenderbuffer(GL_RENDERBUFFER, screenRbos[eye]);
		  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		  glBindRenderbuffer(GL_RENDERBUFFER, 0);



		  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screenRbos[eye] );
	  });
    


  }


  void loadTextureFiles() {
	textureFileNames.push_back(std::string("RecipeBookClosed_Cover_diffuse"));

	textureFileNames.push_back(std::string("pages"));

	textureFileNames.push_back(std::string("woodLog"));
	textureFileNames.push_back(std::string("ChoppingBoard"));
	textureFileNames.push_back(std::string("Knife_metal"));
	textureFileNames.push_back(std::string("Sugar_cubes"));
	textureFileNames.push_back(std::string("StandMixer_diffuse"));
	textureFileNames.push_back(std::string("Hand"));


    textureIds = std::vector <GLuint>( 31 );
    std::cout << textureIds.size() << std::endl;

    int width, height, numChannels;
    unsigned char* data;
	uniform_texture_from_picture = glGetUniformLocation(textureShaderID, "texFramebuffer");

    for( GLuint i = 1; i < textureFileNames.size()+1; i++ ) { //skipping index 0
      //Setting up textures
      std::string fileName = "./JPG/" + textureFileNames[i-1] + ".jpg";
      data = stbi_load( fileName.c_str(), &width, &height, &numChannels, 0 );
      if( !data ) {
        throw std::runtime_error( "Cannot load JPG file" );
      }
      else {
        std::cout << textureFileNames[i-1] + " loaded successfully." << " id = " << i << std::endl;
      }

      glActiveTexture( GL_TEXTURE0 + i );
      glGenTextures( 1, &( textureIds[i] ) );
      glBindTexture( GL_TEXTURE_2D, textureIds[i] );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );

	  
    }


  }


  InitialData getInitialData() {

    InitialData output;

    float xmin, xmax, ymin, ymax, zmin, zmax;

	output.controller = player1->leftHand->getObjectSpaceBoundingBox();

	//Props
    output.cuttingBoard = props.at((int)propsID::CHOPPING_BOARD)->getObjectSpaceBoundingBox();
	output.knife = props.at((int)propsID::KNIFE)->getObjectSpaceBoundingBox();
	output.standMixer = props.at((int)propsID::STAND_MIXER)->getObjectSpaceBoundingBox();
	output.barrel = props.at((int)propsID::BARREL)->getObjectSpaceBoundingBox();
	output.sugarBowl = props.at((int)propsID::SUGAR_BOWL)->getObjectSpaceBoundingBox();
	output.eggCrate = props.at((int)propsID::EGG_CRATE)->getObjectSpaceBoundingBox();
	output.flourSack = props.at((int)propsID::FLOUR_SACK)->getObjectSpaceBoundingBox();

	//Ingredients
	output.singleEgg = ingredients.at((int)ingredientsID::SINGLE_EGG)->getObjectSpaceBoundingBox();
	output.crackedEgg = ingredients.at((int)ingredientsID::CRACKED_EGG)->getObjectSpaceBoundingBox();
	output.sugarCube = ingredients.at((int)ingredientsID::SUGAR_CUBE)->getObjectSpaceBoundingBox();
	output.flour = ingredients.at((int)ingredientsID::FLOUR)->getObjectSpaceBoundingBox();
	//output.water = ingredients.at((int)ingredientsID::WATER)->getObjectSpaceBoundingBox();
	/*output.chocolate = ingredients.at((int)ingredientsID::CHOCOLATE)->getObjectSpaceBoundingBox();
	output.strawberry = ingredients.at((int)ingredientsID::STRAWBERRY)->getObjectSpaceBoundingBox();*/

	BoundingBox table;
	xmin = ymin = zmin = -1.0f;
	xmax = ymax = zmax = 1.0f;

	table.v1 = glm::vec3(xmin, ymax, zmax);
	table.v2 = glm::vec3(xmin, ymax, zmin);
	table.v3 = glm::vec3(xmax, ymax, zmin);
	table.v4 = glm::vec3(xmax, ymax, zmax);
	table.v5 = glm::vec3(xmin, ymin, zmax);
	table.v6 = glm::vec3(xmin, ymin, zmin);
	table.v7 = glm::vec3(xmax, ymin, zmin);
	table.v8 = glm::vec3(xmax, ymin, zmax);

	output.table = table;

	return output;
  }


  void render( const glm::mat4 &projection, const glm::mat4 &view, const int playerNumber, ovrEyeRenderDesc eyeRenderDesc) {
	  glm::mat4 _view = view;
///screen in-framebuffer rendering
	  glm::mat4 iView = glm::mat4(1.0f);
		  glBindFramebuffer(GL_FRAMEBUFFER, screenFbos[eyeRenderDesc.Eye]);
		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		  glEnable(GL_DEPTH_TEST);
		  //set the view port ready for the texture scene
		  glViewport(0, 0, 1344, 1344);
		  //TODO: render whatever in the screen later
		  //desertbox->draw( skyBoxShaderID, projection, view );
		  //glClearColor(153.0f/255.0f, 255.0f / 255.0f, 153.0f / 255.0f, 1.0f);
		  glUseProgram(woodShaderID);
		  letters.at('A')->Draw(woodShaderID, projection, iView);
		  //re-bind to default
		  glBindFramebuffer(GL_FRAMEBUFFER, 1);
		  glUseProgram(screenShaderID);


	if (eyeRenderDesc.Eye == ovrEyeType::ovrEye_Left) {
		glViewport(0, 0, 1344, 1600);
	}
	else if (eyeRenderDesc.Eye == ovrEyeType::ovrEye_Right) {
		glViewport(1344, 0, 1344, 1600);
	}

		  screen->draw(screenShaderID, projection, iView);

	  

    desertbox->draw( skyBoxShaderID, projection, view );
	//glClearColor(255.0f/255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);


    ///controller
    // renderController(projection, view, controllerPosition);


    glDisable( GL_CULL_FACE );

    //tables
    render11Tables( projection, view );


	glUseProgram(shaderID);

    player1->draw( shaderID, projection, view, playerNumber == 0 );
    //player2->draw(textureShaderID, projection, view, playerNumber == 1 );


   // renderProcessingBar( projection, view, 0.75f );

	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 8);
	if(player1->leftHandVisible) player1->leftHand->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	if(player1->rightHandVisible) player1->rightHand->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);


	
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 4);
	//rendering props, order matters, add after existing lines!!!!!! Make sure matching the enum class propsID
	props.at((int)propsID::CHOPPING_BOARD)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 5);
	props.at((int)propsID::KNIFE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);

	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 7);
	//props.at((int)propsID::STAND_MIXER)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	//props.at((int)propsID::STAND_MIXER_BOWL)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	//props.at((int)propsID::STAND_MIXER_MACHINE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	
	props.at((int)propsID::BARREL)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);

	//glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 6);
	props.at((int)propsID::SUGAR_BOWL)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);

	props.at((int)propsID::EGG_CRATE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	props.at((int)propsID::FLOUR_SACK)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);

	//props.at((int)propsID::CHOCOLATE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	//props.at((int)propsID::STRAWBERRY)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);

	//Ingredients
	if (ingredients.at((int)ingredientsID::SINGLE_EGG)->isVisible) {
		ingredients.at((int)ingredientsID::SINGLE_EGG)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}

	if (ingredients.at((int)ingredientsID::CRACKED_EGG)->isVisible) {
		ingredients.at((int)ingredientsID::CRACKED_EGG)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}

	if (ingredients.at((int)ingredientsID::SUGAR_CUBE)->isVisible) {
		ingredients.at((int)ingredientsID::SUGAR_CUBE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}

	if (ingredients.at((int)ingredientsID::FLOUR)->isVisible) {
		ingredients.at((int)ingredientsID::FLOUR)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}

	/*if (ingredients.at((int)ingredientsID::WATER)->isVisible) {
		ingredients.at((int)ingredientsID::WATER)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}
	
	if (ingredients.at((int)ingredientsID::CHOCOLATE)->isVisible) {
		ingredients.at((int)ingredientsID::CHOCOLATE)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}
	if (ingredients.at((int)ingredientsID::STRAWBERRY)->isVisible) {
		ingredients.at((int)ingredientsID::STRAWBERRY)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	}*/

	/*
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 1);
    recipeBookClosed->toWorld = glm::translate( glm::mat4( 1.0f ), table_center_positions[3] + glm::vec3( 0, 0.03, 0 ) );
	//recipeBookClosed->Draw( textureShaderID, projection, view );
	
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 2);
	recipeBookOpened->toWorld = glm::translate( glm::mat4( 1.0f ), table_center_positions[2] + glm::vec3( 0, 0.03, 0 ) );
	//recipeBookOpened->Draw( textureShaderID, projection, view );


*/	
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture_from_picture, 3);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 1, 0.5));
	desk->toWorld = glm::translate(glm::mat4(1.0f), table_positions[12] - glm::vec3(0, 0.5, 0));// *scaleMatrix;
	desk->Draw(textureShaderID, projection, view);


  }


  void
  renderProcessingBar( const glm::mat4 &projection, const glm::mat4 &view, float percentage ) {//

    GLint uPercentage = glGetUniformLocation( processingBarShaderID, "percentDone" );
    // std::cout << uPercentage << std::endl;
    // std::cout << percentage << std::endl;
    glm::mat4 cubeToBar = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.4, 0.1, 0.01 ) );

    ///testing
    //  processingBar->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.25, 0))*glm::translate(glm::mat4(1.0f), table_positions[5]) * cubeToBar;



    glUseProgram( processingBarShaderID );
    glUniform1f( 1, percentage );


    //	processingBar->Draw(processingBarShaderID, projection, view);

    for( int i = 0; i < 20; ++i ) {
      if( i == 9 || i == 8 || i == 2 || i == 3 || i == 15 || i == 16 ) {
        processingBar->toWorld = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 1.25, 0 ) ) *
                                 glm::translate( glm::mat4( 1.0f ), table_positions[i] ) *
                                 glm::rotate( glm::mat4( 1.0f ), glm::radians( 90.0f ),
                                              glm::vec3( 0, 1, 0 ) ) * cubeToBar;
      }
      else {
        processingBar->toWorld = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 1.25, 0 ) ) *
                                 glm::translate( glm::mat4( 1.0f ), table_positions[i] ) *
                                 cubeToBar;

      }   // std::cout << i << std::endl;

      processingBar->Draw( processingBarShaderID, projection, view );
    }
  }


  void loadingModels() {
    props.push_back( new Model( "./Models/ChoppingBoard.obj" ) );
    props.push_back( new Model( "./Models/Knife.obj" ) );
    props.push_back( new Model( "./Models/StandMixer.obj" ) );
	props.push_back(new Model("./Models/kettle.obj"));
	props.push_back(new Model("./Models/Sugar_cubes.obj"));
	props.push_back(new Model("./Models/EggCrate.obj"));
	props.push_back(new Model("./Models/flour_sack.obj"));
	//props.push_back(new Model("./Models/StandMixer_bowl.obj"));
	//props.push_back(new Model("./Models/StandMixer_machine.obj"));
	//props.push_back(new Model("./Models/water.obj"));
	//props.push_back(new Model("./Models/water.obj"));

	ingredients.push_back(new Model("./Models/SingleEgg.obj",false));
	ingredients.push_back(new Model("./Models/CrackedEgg.obj", false));
	ingredients.push_back(new Model("./Models/Sugar_cube.obj", false));
	ingredients.push_back(new Model("./Models/flourpile.obj", false));
	//ingredients.push_back(new Model("./Models/water.obj", false));
	//ingredients.push_back(new Model("./Models/water.obj", false));
	//ingredients.push_back(new Model("./Models/water.obj", false));


  }


  void render11Tables( const glm::mat4 &projection, const glm::mat4 &view ) {
    glm::mat4 scaleMatrix = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25, 0.5, 0.25 ) );
    for( int i = 0; i < tables.size(); ++i ) {
      tables[i]->toWorld = glm::translate( glm::mat4( 1.0f ), table_positions[i] ) * scaleMatrix;
      if( i != 12 )
        tables[i]->draw( skyBoxShaderID, projection, view );
    }
  }


  void populatingTables() {
    for( int i = 0; i < 20; ++i ) {
      tables.push_back( new TexturedCube( "Martini" ) );

      //std::cout << i << std::endl;
    }

    table_positions.push_back( glm::vec3( 0.25, -1, -0.75 ) );//0
    table_positions.push_back( glm::vec3( 0.75, -1, -0.75 ) );//1
    table_positions.push_back( glm::vec3( 0.75, -1, -0.25 ) );//2
    table_positions.push_back( glm::vec3( 0.75, -1, 0.25 ) );//3
    table_positions.push_back( glm::vec3( 0.75, -1, 0.75 ) );//4
    table_positions.push_back( glm::vec3( 0.25, -1, 0.75 ) );//5
    table_positions.push_back( glm::vec3( -0.25, -1, 0.75 ) );//6
    table_positions.push_back( glm::vec3( -0.75, -1, 0.75 ) );//7
    table_positions.push_back( glm::vec3( -0.75, -1, 0.25 ) );//8
    table_positions.push_back( glm::vec3( -0.75, -1, -0.25 ) );//9
    table_positions.push_back( glm::vec3( -0.75, -1, -0.75 ) );//10
    table_positions.push_back( glm::vec3( -0.25, -1, -0.75 ) );//11
    table_positions.push_back( glm::vec3( 1.25, -1, -0.75 ) );//12
    table_positions.push_back( glm::vec3( 1.75, -1, -0.75 ) );//13
    table_positions.push_back( glm::vec3( 2.25, -1, -0.75 ) );//14
    table_positions.push_back( glm::vec3( 2.25, -1, -0.25 ) );//15
    table_positions.push_back( glm::vec3( 2.25, -1, 0.25 ) );//16
    table_positions.push_back( glm::vec3( 2.25, -1, 0.75 ) );//17
    table_positions.push_back( glm::vec3( 1.75, -1, 0.75 ) );//18
    table_positions.push_back( glm::vec3( 1.25, -1, 0.75 ) );//19


    table_center_positions.push_back( glm::vec3( 0.25, -0.495, -0.75 ) );//0
    table_center_positions.push_back( glm::vec3( 0.75, -0.495, -0.75 ) );//1
    table_center_positions.push_back( glm::vec3( 0.75, -0.495, -0.25 ) );//2
    table_center_positions.push_back( glm::vec3( 0.75, -0.495, 0.25 ) );//3
    table_center_positions.push_back( glm::vec3( 0.75, -0.495, 0.75 ) );//4
    table_center_positions.push_back( glm::vec3( 0.25, -0.495, 0.75 ) );//5
    table_center_positions.push_back( glm::vec3( -0.25, -0.495, 0.75 ) );//6
    table_center_positions.push_back( glm::vec3( -0.75, -0.495, 0.75 ) );//7
    table_center_positions.push_back( glm::vec3( -0.75, -0.495, 0.25 ) ); //8
    table_center_positions.push_back( glm::vec3( -0.75, -0.495, -0.25 ) );//9
    table_center_positions.push_back( glm::vec3( -0.75, -0.495, -0.75 ) );//10
    table_center_positions.push_back( glm::vec3( -0.25, -0.495, -0.75 ) );//11

    glm::mat4 scaleMatrix = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25, 0.5, 0.25 ) );

    for( int i = 0; i < tables.size(); ++i ) {
      tables[i]->toWorld = glm::translate( glm::mat4( 1.0f ), table_positions[i] ) * scaleMatrix;
    }
  }



  void update( SceneGraph s ) {

    player1->updateState( s.player1 );
    player2->updateState( s.player2 );
    props.at( ( int ) propsID::CHOPPING_BOARD )->toWorld = (
      glm::translate( glm::mat4( 1.0 ), s.cuttingBoard.position ) *
      glm::mat4_cast( s.cuttingBoard.orientation )
      * glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.01, 0.01, 0.01 ) ));
	props.at((int)propsID::KNIFE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.knife.position) *
		glm::mat4_cast(s.knife.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02)));
	/*props.at((int)propsID::STAND_MIXER)->toWorld = (
		glm::translate(glm::mat4(1.0), s.standMixer.position) *
		glm::mat4_cast(s.standMixer.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01)));
	props.at((int)propsID::STAND_MIXER_MACHINE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.standMixer.position) *
		glm::mat4_cast(s.standMixer.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01)));*/
	props.at((int)propsID::BARREL)->toWorld = (
		glm::translate(glm::mat4(1.0), s.barrel.position) *
		glm::mat4_cast(s.barrel.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
	props.at((int)propsID::SUGAR_BOWL)->toWorld = (
		glm::translate(glm::mat4(1.0), s.sugarBowl.position) *
		glm::mat4_cast(s.sugarBowl.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)));
	props.at((int)propsID::EGG_CRATE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.eggCrate.position) *
		glm::mat4_cast(s.eggCrate.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));
	props.at((int)propsID::FLOUR_SACK)->toWorld = (
		glm::translate(glm::mat4(1.0), s.flourSack.position) *
		glm::mat4_cast(s.flourSack.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.005, 0.005, 0.005)));
	/*props.at((int)propsID::CHOCOLATE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.chocolate.position) *
		glm::mat4_cast(s.chocolate.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));
	props.at((int)propsID::STRAWBERRY)->toWorld = (
		glm::translate(glm::mat4(1.0), s.strawberry.position) *
		glm::mat4_cast(s.strawberry.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));*/
	

	//Ingredients
	ingredients.at((int)ingredientsID::SINGLE_EGG)->isVisible = s.singleEgg.visible;
	ingredients.at((int)ingredientsID::SINGLE_EGG)->toWorld = (
		glm::translate(glm::mat4(1.0), s.singleEgg.position) *
		glm::mat4_cast(s.singleEgg.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.001, 0.001, 0.001)));

	ingredients.at((int)ingredientsID::CRACKED_EGG)->isVisible = s.crackedEgg.visible;
	ingredients.at((int)ingredientsID::CRACKED_EGG)->toWorld = (
		glm::translate(glm::mat4(1.0), s.crackedEgg.position) *
		glm::mat4_cast(s.crackedEgg.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)));

	ingredients.at((int)ingredientsID::SUGAR_CUBE)->isVisible = s.sugarCube.visible;
	ingredients.at((int)ingredientsID::SUGAR_CUBE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.sugarCube.position) *
		glm::mat4_cast(s.sugarCube.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)));

	ingredients.at((int)ingredientsID::FLOUR)->isVisible = s.flour.visible;
	ingredients.at((int)ingredientsID::FLOUR)->toWorld = (
		glm::translate(glm::mat4(1.0), s.flour.position) *
		glm::mat4_cast(s.flour.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));

	/*ingredients.at((int)ingredientsID::WATER)->isVisible = s.water.visible;
	ingredients.at((int)ingredientsID::FLOUR)->toWorld = (
		glm::translate(glm::mat4(1.0), s.water.position) *
		glm::mat4_cast(s.water.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));
	
	ingredients.at((int)ingredientsID::CHOCOLATE)->isVisible = s.chocolate.visible;
	ingredients.at((int)ingredientsID::CHOCOLATE)->toWorld = (
		glm::translate(glm::mat4(1.0), s.chocolate.position) *
		glm::mat4_cast(s.chocolate.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));

	ingredients.at((int)ingredientsID::STRAWBERRY)->isVisible = s.strawberry.visible;
	ingredients.at((int)ingredientsID::STRAWBERRY)->toWorld = (
		glm::translate(glm::mat4(1.0), s.strawberry.position) *
		glm::mat4_cast(s.strawberry.orientation)
		* glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));*/

  }


  void renderController( const glm::mat4 &projection, const glm::mat4 &view,
                         const glm::vec3 &controllerPosition ) {
    sphere->toWorld =
      glm::translate( glm::mat4( 1.0f ), controllerPosition ) *
      glm::scale( glm::mat4( 1.0f ), vec3( 0.0175, 0.0175, 0.0175 ) );
    // sphere->Draw(controllerShaderID, projection, view, 2);
  }


};


#endif //BONAPPETITE_SCENE_H
