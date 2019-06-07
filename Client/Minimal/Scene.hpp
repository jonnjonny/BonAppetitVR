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

#include <unordered_map>


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
  //std::vector<std::string> props;




  GLuint textureShaderID, textureId, textureId2;


  GLint uniform_texture, attribute_texture;


  //processing bar specs
  Model* processingBar;


  GLuint processingBarShaderID; //shader id for skyboxB

  std::vector <std::string> textureFileNames;


  std::vector <GLuint> textureIds;


  Model* recipeBookOpened;


  Model* recipeBookClosed;


  Quad* screen;


  GLuint screenShaderID;


  GLuint screenFbo, screenRbo, screenTexture;

  
  unordered_map<char, Model*> letters;

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
    desertbox->toWorld = glm::scale( glm::mat4( 1.0f ), glm::vec3( 5.0f ) );

    ///controller
    sphere = new Model( "./Models/sphere.obj");

    populatingTables();

    loadingModels();

    processingBar = new Model( "./Models/cube.obj" );


	///for screen appearing from opening book
    screen = new Quad();
	screen->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
                  //   * glm::rotate( glm::mat4( 1.0f ), glm::radians( 45.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) )
                    // * glm::scale( glm::mat4( 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) );
    //populateInFrameRenderingBuffers();


    player1 = new Player();
    player2 = new Player();


    desk = new Model( "./Models/woodLog.obj", 1.0f );
	recipeBookOpened = new Model("./Models/RecipeBookOpened.obj");
	recipeBookClosed = new Model("./Models/RecipeBookClosed.obj");

	loadTextureFiles();

	populateLetterModels();
  }


  void populateLetterModels() {
	  letters = unordered_map<char, Model*>();
	  for (int i = 0; i < 1; i++) {
		  Model* upperCaseModel = new Model((std::string("./Models/Letters/") + (char)(ascii_A+i) + std::string(".obj")).c_str());
		  letters.insert({ (char)ascii_A, upperCaseModel });
		 // Model* lowerCaseModel = new Model((std::string("./Models/Letters/") + (char)(ascii_a+i) + std::string(".obj")).c_str());
		 // letters.insert({ (char)ascii_A, lowerCaseModel });
	  }
	  
  }
  void populateInFrameRenderingBuffers() {
    ///Framebuffers
    screenFbo = 0;
    glGenFramebuffers( 1, &screenFbo );
    glBindFramebuffer( GL_FRAMEBUFFER, screenFbo );


    screenTexture = 0;
    glGenTextures( 1, &screenTexture );
    glBindTexture( GL_TEXTURE_2D, screenTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, NULL );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0 );


    ///Renderbuffers
    screenRbo = 0;
    glGenRenderbuffers( 1, &screenRbo );
    glBindRenderbuffer( GL_RENDERBUFFER, screenRbo );
    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );


    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screenRbo );

  }


  void loadTextureFiles() {
	textureFileNames.push_back(std::string("RecipeBookClosed_Cover_diffuse"));

	textureFileNames.push_back(std::string("pages"));

	textureFileNames.push_back(std::string("ChoppingBoard"));
	textureFileNames.push_back(std::string("woodLog"));


    textureIds = std::vector <GLuint>( 31 );
    std::cout << textureIds.size() << std::endl;

    int width, height, numChannels;
    unsigned char* data;
	uniform_texture = glGetUniformLocation(textureShaderID, "texFramebuffer");

    for( GLuint i = 0; i < 4; i++ ) {
      //Setting up textures
      std::string fileName = "./JPG/" + textureFileNames[i] + ".jpg";
      data = stbi_load( fileName.c_str(), &width, &height, &numChannels, 0 );
      if( !data ) {
        throw std::runtime_error( "Cannot load JPG file" );
      }
      else {
        std::cout << textureFileNames[i] + " loaded successfully." << " id = " << i << std::endl;
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

    BoundingBox controller;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    xmin = ymin = zmin = -0.01f;
    xmax = ymax = zmax = 0.01f;

    controller.v1 = glm::vec3( xmin, ymax, zmax );
    controller.v2 = glm::vec3( xmin, ymax, zmin );
    controller.v3 = glm::vec3( xmax, ymax, zmin );
    controller.v4 = glm::vec3( xmax, ymax, zmax );
    controller.v5 = glm::vec3( xmin, ymin, zmax );
    controller.v6 = glm::vec3( xmin, ymin, zmin );
    controller.v7 = glm::vec3( xmax, ymin, zmin );
    controller.v8 = glm::vec3( xmax, ymin, zmax );

    output.controller = controller;

    output.cuttingBoard = props.at( 0 )->getObjectSpaceBoundingBox();


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


  void render( const glm::mat4 &projection, const glm::mat4 &view, const int playerNumber ) {

///screen in-framebuffer rendering
    //glBindFramebuffer( GL_FRAMEBUFFER, screenFbo);
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glEnable( GL_DEPTH_TEST );
    //set the view port ready for the texture scene
   // glViewport(0, 0, 1344, 1344);
    //TODO: render whatever in the screen later
	//letters.at('A')->Draw(woodShaderID, projection, view);
    //re-bind to default
    //glBindFramebuffer( GL_FRAMEBUFFER, 1 );
	//screen->draw(screenShaderID, projection, view);

/*
    if (eyeType == ovrEyeType::ovrEye_Left) {
      glViewport(0, 0, 1344, 1600);
    }
    else if (eyeType == ovrEyeType::ovrEye_Right) {
      glViewport(1344, 0, 1344, 1600);
    }
*/




    desertbox->draw( skyBoxShaderID, projection, view );


    ///controller
    // renderController(projection, view, controllerPosition);


    glDisable( GL_CULL_FACE );

    //tables
    render11Tables( projection, view );


/*
	  props.at((int)propsID::KNIFE)->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.02, 0)) *glm::translate(glm::mat4(1.0f), table_center_positions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))* glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, 1));
	  props.at((int)propsID::KNIFE)->Draw(woodShaderID, projection, view, 2);

	  props.at((int)propsID::SINGLE_EGG)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[1]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.005, 0.005, 0.005))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	  props.at((int)propsID::SINGLE_EGG)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::STAND_MIXER)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[2]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01))* glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	  props.at((int)propsID::STAND_MIXER)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::BARREL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[11]) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
	  props.at((int)propsID::BARREL)->Draw(textureShaderID, projection, view, 2);

	  props.at((int)propsID::SUGAR_BOWL)->toWorld = glm::translate(glm::mat4(1.0f), table_center_positions[4]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05))* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0, 1, 0));
	  props.at((int)propsID::SUGAR_BOWL)->Draw(textureShaderID, projection, view, 2);
 */
    player1->draw( shaderID, projection, view, playerNumber == 0 );
    player2->draw( shaderID, projection, view, playerNumber == 1 );


    renderProcessingBar( projection, view, 0.75f );




	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture, 0);
    recipeBookClosed->toWorld = glm::translate( glm::mat4( 1.0f ), table_center_positions[3] + glm::vec3( 0, 0.03, 0 ) );
	recipeBookClosed->Draw( textureShaderID, projection, view );
	
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture, 1);
	recipeBookOpened->toWorld = glm::translate( glm::mat4( 1.0f ), table_center_positions[2] + glm::vec3( 0, 0.03, 0 ) );
	recipeBookOpened->Draw( textureShaderID, projection, view );

	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture, 2); 
	//rendering props, order matters, add after existing lines!!!!!! Make sure matching the enum class propsID
	props.at((int)propsID::CHOPPING_BOARD)->toWorld =
		glm::translate(glm::mat4(1.0f), table_center_positions[0]) *
		glm::scale(glm::mat4(1.0f), glm::vec3(0.01, 0.01, 0.01)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0));
	props.at((int)propsID::CHOPPING_BOARD)->Draw(textureShaderID, projection, view, true, boundingBoxShaderID);
	
	glUseProgram(textureShaderID);
	glUniform1i(uniform_texture, 3);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 1, 0.5));
    desk->toWorld = glm::translate( glm::mat4( 1.0f ), table_positions[12] - glm::vec3( 0, 0.5, 0 ) ) *scaleMatrix;
	desk->Draw(textureShaderID, projection, view);

	//glUniform1i(uniform_texture, 0);

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
    props.push_back( new Model( "./Models/SingleEgg.obj" ) );
    props.push_back( new Model( "./Models/StandMixer.obj" ) );
    props.push_back( new Model( "./Models/teapot_s0.obj" ) );
    props.push_back( new Model( "./Models/SugarBowl.obj" ) );

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
    //std::cout << tables.size() << std::endl;


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
