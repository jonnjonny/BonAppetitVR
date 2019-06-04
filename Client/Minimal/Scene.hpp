//
// Created by Jonathan Leung May 2019.
//

#ifndef BONAPPETITE_SCENE_H
#define BONAPPETITE_SCENE_H


#include <vector>
#include "Core.h"
#include "shader.h"
#include "rpc/client.h"
#include "TexturedCube.h"
#include "Player.h"
#include "SceneGraph.hpp"


// a class for building and rendering cubes
class Scene {
  // Program
  GLuint shaderID;


  //Model* sphere;

  Player* player1;
  Player* player2;


  ////
  //std::vector <glm::mat4> table_positions;


  //std::vector <TexturedCube*> tables;


public:
  Scene() {

    // Shader Program
    shaderID = LoadShaders( "shader.vert", "shader.frag" );

	player1 = new Player();
	player2 = new Player();

    ///
    //populatingTables();

  }



  void render( const glm::mat4 &projection, const glm::mat4 &view, const int playerNumber) {
    ///test purpose only for index 0
	  player1->draw(shaderID, projection, view, playerNumber == 0);
	  player2->draw(shaderID, projection, view, playerNumber == 1);

  }



  /*void populatingTables() {
    for( int i = 0; i < 11; ++i ) {
      tables.push_back( new TexturedCube( "table_box" ) );
    }

    glm::mat4 currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.25, 0.25, 0.25 ) ) *
                        glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.25, 0.25, 0.25 ) );
    currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, -0.5 ) ); //0
    table_positions.push_back( currPos );

    currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.5, 0, 0 ) ); //1
    table_positions.push_back( currPos );

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, 0.5 ) ); //2,3,4
      table_positions.push_back( currPos );
    }

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( -0.5, 0, 0 ) ); //5,6,7
      table_positions.push_back( currPos );
    }

    for( int j = 0; j < 3; ++j ) {
      currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0, 0, -0.5 ) ); //8,9,10
      table_positions.push_back( currPos );
    }

    currPos = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.5, 0, 0 ) ); //11
    table_positions.push_back( currPos );

    ///finished calculating all the positions of tables

  }*/

  void updatePlayer(SceneGraph s) {

		  player1->updateState(s.player1);
		  player2->updateState(s.player2);

  }



};


#endif //BONAPPETITE_SCENE_H
