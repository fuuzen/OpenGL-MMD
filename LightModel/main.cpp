#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/assimploader.hpp>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Static Model", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// We would expect width and height to be 1024 and 768
	int windowWidth = 1024;
	int windowHeight = 768;
	// But on MacOS X with a retina screen it'll be 1024*2 and 768*2, so we get the actual framebuffer size:
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("mmd.vert", "mmd.frag");

	GLuint u_WV = glGetUniformLocation(programID, "u_WV");
	GLuint u_WVP = glGetUniformLocation(programID, "u_WVP");
	GLuint u_LightWVP = glGetUniformLocation(programID, "u_LightWVP");

	GLuint u_Ambient = glGetUniformLocation(programID, "u_Ambient");
	GLuint u_Diffuse = glGetUniformLocation(programID, "u_Diffuse");
	GLuint u_Specular = glGetUniformLocation(programID, "u_Specular");
	GLuint u_SpecularPower = glGetUniformLocation(programID, "u_SpecularPower");
	GLuint u_Alpha = glGetUniformLocation(programID, "u_Alpha");

	glm::vec3 m_ambient = glm::vec3(0.6f, 0.6f, 0.6f);
	glm::vec3 m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_specular = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_specularPower = 5.0f;
	float m_alpha = 1.0f;

	GLuint u_Tex = glGetUniformLocation(programID, "u_Tex");
	GLuint u_TexMode = glGetUniformLocation(programID, "u_TexMode");
	GLuint u_TexMulFactor = glGetUniformLocation(programID, "u_TexMulFactor");
	GLuint u_TexAddFactor = glGetUniformLocation(programID, "u_TexAddFactor");

	glm::vec4 m_textureMulFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_textureAddFactor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint u_SphereTex = glGetUniformLocation(programID, "u_SphereTex");
	GLuint u_SphereTexMode = glGetUniformLocation(programID, "u_SphereTexMode");

	GLuint u_ToonTex = glGetUniformLocation(programID, "u_ToonTex");
	GLuint u_ToonTexMode = glGetUniformLocation(programID, "u_ToonTexMode");
	GLuint u_ToonTexMulFactor = glGetUniformLocation(programID, "u_ToonTexMulFactor");
	GLuint u_ToonTexAddFactor = glGetUniformLocation(programID, "u_ToonTexAddFactor");

	glm::vec4 m_toonTextureMulFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 m_toonTextureAddFactor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint u_LightDir = glGetUniformLocation(programID, "u_LightDir");
	GLuint u_LightColor = glGetUniformLocation(programID, "u_LightColor");

	GLuint u_ShadowMapEnabled = glGetUniformLocation(programID, "u_ShadowMapEnabled");
	GLuint u_ShadowMap0 = glGetUniformLocation(programID, "u_ShadowMap0");
	GLuint u_ShadowMap1 = glGetUniformLocation(programID, "u_ShadowMap1");
	GLuint u_ShadowMap2 = glGetUniformLocation(programID, "u_ShadowMap2");
	GLuint u_ShadowMap3 = glGetUniformLocation(programID, "u_ShadowMap3");

	// Load the texture
	GLuint Texture1 = loadPNG("../model/hutao/tex/\xc3\xe6.png");
	GLuint Texture2 = loadPNG("../model/hutao/tex/\xb7\xa2.png");
	GLuint Texture3 = loadPNG("../model/hutao/tex/\xb7\xfe.png");

	GLuint Toon = loadBMP_custom("../model/hutao/skin.bmp");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID1 = glGetUniformLocation(programID, "myTextureSampler1");
	GLuint TextureID2 = glGetUniformLocation(programID, "myTextureSampler2");
	GLuint TextureID3 = glGetUniformLocation(programID, "myTextureSampler3");

	GLuint ToonID = glGetUniformLocation(programID, "myToonSampler");

	// Read our .pmx file
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texIndex;
	std::vector<std::string> texs;
	std::vector<std::string> toons;
	bool res = loadAssImp("../model/hutao/hutao.pmx", indices, vertices, uvs, normals, texIndex, texs, toons);

	// Load it into a VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint texIndexbuffer;
	glGenBuffers(1, &texIndexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texIndexbuffer);
	glBufferData(GL_ARRAY_BUFFER, texIndex.size() * sizeof(glm::vec2), &texIndex[0], GL_STATIC_DRAW);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		auto world = glm::mat4(1.0f);
		auto wv = ViewMatrix * world;
		auto wvp = ProjectionMatrix * ViewMatrix * world;

		glUniformMatrix4fv(u_WV, 1, GL_FALSE, &wv[0][0]);
		glUniformMatrix4fv(u_WVP, 1, GL_FALSE, &wvp[0][0]);

		glUniform3fv(u_Ambient, 1, &m_ambient[0]);
		glUniform3fv(u_Diffuse, 1, &m_diffuse[0]);
		glUniform3fv(u_Specular, 1, &m_specular[0]);
		glUniform1f(u_SpecularPower, m_specularPower);
		glUniform1f(u_Alpha, m_alpha);

		glActiveTexture(GL_TEXTURE0 + 0);
		glUniform1i(u_Tex, 0);
		glUniform1i(u_TexMode, 1);

		glUniform4fv(u_TexMulFactor, 1, &m_textureMulFactor[0]);
		glUniform4fv(u_TexAddFactor, 1, &m_textureAddFactor[0]);

		glActiveTexture(GL_TEXTURE0 + 1);
		glUniform1i(u_SphereTex, 1);
		glUniform1i(u_SphereTexMode, 0);

		glActiveTexture(GL_TEXTURE0 + 2);
		glUniform1i(u_ToonTex, 2);
		glUniform4fv(u_ToonTexMulFactor, 1, &m_toonTextureMulFactor[0]);
		glUniform4fv(u_ToonTexAddFactor, 1, &m_toonTextureAddFactor[0]);
		glUniform1i(u_ToonTexMode, 1);

		glm::vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 lightDir = vec3(-0.5f, -0.1f, -0.5f);
		glm::mat3 viewMat = glm::mat3(1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, 0.0, -1.0f);
		lightDir = viewMat * lightDir;
		glUniform3fv(u_LightDir, 1, &lightDir[0]);
		glUniform3fv(u_LightColor, 1, &lightColor[0]);

		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(programID);

		// Bind our texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture1);
		glUniform1i(TextureID1, 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glUniform1i(TextureID2, 1);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, Texture3);
		glUniform1i(TextureID3, 2);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, Toon);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glUniform1i(u_ToonTex, 3);	

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : uvs
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 4th attribute buffer : texIndex
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, texIndexbuffer);
		glVertexAttribPointer(
			3,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &texIndexbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture1);
	glDeleteTextures(1, &Texture2);
	glDeleteTextures(1, &Texture3);
	glDeleteTextures(1, &Toon);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}