#pragma once


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else


#include <GL/glew.h>
#include <iostream>


#endif


#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif


#ifndef _SCREEN_H_
#define _SCREEN_H_


#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "glm/gtx/vector_angle.hpp"

struct DrawPackage { 
  glm::mat4 &projection;
  glm::mat4 &view;
  ovrEyeType eyeType;
  RenderingOption_X xStatus;
  glm::vec3 sizeScalingVector;
};


struct anglePackage {
	int wallID;
	glm::vec4 lookingVec;


};
// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.
const GLfloat vertices[] = {
  -1.0f, 1.0f, 0.0f,
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  1.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f,
};

const GLfloat textCoords[] = {
  0.0f, 1.0f,
  0.0f, 0.0f,
  1.0f, 0.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,
0.0f, 1.0f,
};


const unsigned int TEXTURE_WIDTH{ 1344 };

const unsigned int TEXTURE_HEIGHT{ 1344 };


class Quad {
public:

  glm::mat4 toWorld;


  // These variables are needed for the shader program
  GLuint vertexBuffer, normalBuffer, textureBuffer;


  GLuint uProjection, uModelview, uTexture, uDimmingFactor;

 // GLuint uWallId, uLookingVec, 


  GLuint VAO, VBO;

  //GLfloat vertices[];


  Quad() {

    //vertices = verts;

    toWorld = glm::mat4( 1.0f );

    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &vertexBuffer );
    //glGenBuffers(1, &normalBuffer);

    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray( VAO );

    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer(
      0,
      // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
      3,
      // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
      GL_FLOAT, // What type these components are
      GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
      3 * sizeof( GLfloat ),
      // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
      ( GLvoid* ) 0 );
    // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.



    glGenBuffers( 1, &textureBuffer );
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer( GL_ARRAY_BUFFER, textureBuffer );
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData( GL_ARRAY_BUFFER, sizeof( textCoords ), textCoords, GL_STATIC_DRAW );
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer(
      1,
      // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
      2,
      // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
      GL_FLOAT, // What type these components are
      GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
      2 * sizeof( GLfloat ),
      // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
      ( GLvoid* ) 0 );
    // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

  }


  Quad( const glm::vec3 scaleVec ) {

    //vertices = verts;

    toWorld = glm::scale( glm::mat4( 1.0f ), scaleVec );

    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &vertexBuffer );
	//glGenBuffers(1, &normalBuffer);

    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray( VAO );

    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer(
      0,
      // This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
      3,
      // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
      GL_FLOAT, // What type these components are
      GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
      3 * sizeof( GLfloat ),
      // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
      ( GLvoid* ) 0 );
    // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.



	glGenBuffers(1, &textureBuffer);
		// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(textCoords), textCoords, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		2,
		// This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		2 * sizeof(GLfloat),
		// Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0);
	// Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
  }


  ~Quad() {
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &vertexBuffer );
    //glDeleteBuffers( 1, &normalBuffer );
  }


  void draw( GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view ) {
    glUseProgram( shaderProgram );
    glDisable( GL_CULL_FACE );
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = view * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation( shaderProgram, "projection" );
    uModelview = glGetUniformLocation( shaderProgram, "modelview" );
    uTexture = glGetUniformLocation( shaderProgram, "texFramebuffer" );
    glm::vec4 normal = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) * toWorld;

    // Now send these values to the shader program
    glUniformMatrix4fv( uProjection, 1, GL_FALSE, &projection[0][0] );
    glUniformMatrix4fv( uModelview, 1, GL_FALSE, &modelview[0][0] );
    glUniform1i( uTexture, 0 );

    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray( VAO );
    // Tell OpenGL to draw with triangles
    glDrawArrays( GL_TRIANGLES, 0,
                  3 * 2 ); // 3 vertices per triangle, 2 triangles per face, 1 face
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray( 0 );
  }


  void draw( GLuint shaderProgram, const glm::mat4 &projection, const glm::mat4 &view,
             anglePackage pkg ) {
    glUseProgram( shaderProgram );
    glDisable( GL_CULL_FACE );
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = view * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation( shaderProgram, "projection" );
    uModelview = glGetUniformLocation( shaderProgram, "modelview" );
    uTexture = glGetUniformLocation( shaderProgram, "texFramebuffer" );
    uDimmingFactor = glGetUniformLocation( shaderProgram, "dimmingFactor" );
    glm::vec4 normal = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) * toWorld;
    float angleToNormalInRadians = glm::angle( normal, glm::normalize( pkg.lookingVec ) );

    float dimmingFactor = glm::cos( angleToNormalInRadians );
    if( glm::abs( angleToNormalInRadians ) > 3.1415926f / 2.0f ) {
      //	dimmingFactor = 0;
    }
    //std::cout << angleToNormalInRadians << "        " <<dimmingFactor << endl;
    // Now send these values to the shader program
    glUniformMatrix4fv( uProjection, 1, GL_FALSE, &projection[0][0] );
    glUniformMatrix4fv( uModelview, 1, GL_FALSE, &modelview[0][0] );
    glUniform1i( uTexture, 0 );
    glUniform1f( uDimmingFactor, dimmingFactor );


    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray( VAO );
    // Tell OpenGL to draw with triangles
    glDrawArrays( GL_TRIANGLES, 0,
                  3 * 2 ); // 3 vertices per triangle, 2 triangles per face, 1 face
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray( 0 );


  }


  void update() {
    spin( 1.0f );
  }


  void spin( float ) {
    toWorld = toWorld * glm::rotate( glm::mat4( 1.0f ), 1.0f / 180.0f * glm::pi <float>(),
                                     glm::vec3( 0.0f, 1.0f, 0.0f ) );
  }


};


#endif
