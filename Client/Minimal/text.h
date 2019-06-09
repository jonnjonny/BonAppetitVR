#pragma once
class text
{
public:

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"

#include "Core.h"
#include <ft2build.h>

#include <unordered_map>

#include FT_FREETYPE_H


	using namespace std;


	class Text {
	public:

		GLuint uProjection, uModelview, ubooleanHighlight;
		FT_Library ft;
		FT_Face face;

		/*  Functions  */
		// constructor
		Text() {
			//load the library
			setUpFace();
			setUpTextModels();
			loadFirst128Chars();
			finishSetUpFace();

			// now that we have all the required data, set the vertex buffers and its attribute pointers.
			//setupMesh();


		}


		void RenderText(GLuint shaderId, std::string text) {
			// Activate corresponding render state
			//s.Use();

			glUseProgram(shaderId);

			//glUniform3f( glGetUniformLocation( s.Program, "textColor" ), color.x,
						 //color.y, color.z );
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);


			///TODO delete
			GLfloat x, GLfloat y, GLfloat z, GLfloat scale;
			x = 0; y = 5; z = 0;
			scale = 1;

			// Iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++) {
				Character ch = Characters[*c];

				GLfloat xpos = x + ch.Bearing.x * scale;
				//offset ypos below the baseline
				GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
				GLfloat zpos = z;

				GLfloat w = ch.Size.x * scale;
				GLfloat h = ch.Size.y * scale;
				// Update VBO for each character
				GLfloat vertices[6][4] = {
				  { xpos,     ypos + h, 0.0, 0.0 },
				  { xpos,     ypos,     0.0, 1.0 },
				  { xpos + w, ypos,     1.0, 1.0 },

				  { xpos,     ypos + h, 0.0, 0.0 },
				  { xpos + w, ypos,     1.0, 1.0 },
				  { xpos + w, ypos + h, 1.0, 0.0 }
				};
				// Render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.textureID);
				// Update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// Render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
				x += (ch.Advance >> 6) *
					scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}


	private:

		struct Character {
			GLuint TextureID;  // ID handle of the glyph texture
			glm::ivec2 Size;       // Size of glyph
			glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
			GLuint Advance;    // Offset to advance to next glyph
		};


		std::map <GLchar, Character> Characters;


		/*  Render data  */
		unsigned int VBO, EBO;


		/*  Functions    */
		void setUpFace() {

			//text


			if (FT_Init_FreeType(&ft))
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
				<< std::endl;

			if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

			FT_Set_Pixel_Sizes(face, 0, 48);

		}


		//load the first 128 characters of the ASCII character set.
		void loadFirst128Chars() {
			// Disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// For each character, we generate a texture and store its relevant data
			// into a Character struct that we add to the Characters map. This way,
			// all data required to render each character is stored for later use.
			for (GLubyte c = 0; c < 128; c++) {
				// Load character glyph
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// Generate texture
				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				// store each byte of the bitmap buffer as a texture's color value
				// by creating a texture where each byte corresponds to the texture
				// color's red component (first byte of its color vector).
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
					face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer);
				// Set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// Now store character for later use
				Character character = {
				  texture,
				  glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				  glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				  face->glyph->advance.x
				};
				Characters.insert(std::pair <GLchar, Character>(c, character));
			}
		}


		void finishSetUpFace() {
			//clear FreeType's resources once you're finished processing the glyphs
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}


		// initializes all the buffer objects/arrays
		void setUpTextModels() {
			// enable blending for text to work
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//TODO
			glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

			// Configure VAO/VBO for texture quads
			GLuint VAO, VBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//The 2D quad requires 6 vertices of 4 floats each so we reserve 6 * 4
			//floats of memory. Because we'll be updating the content of the VBO's
			//memory quite often we'll allocate the memory with GL_DYNAMIC_DRAW.
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL,
				GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

	};

};


