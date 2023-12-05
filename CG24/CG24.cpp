#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <ext.hpp>

typedef glm::mat4 mat4;
typedef glm::vec3 vec3;

#pragma region Window Setup
#define WINDOW_RESOLUTION WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y
#define WINDOW_RESOLUTION_X 500
#define WINDOW_RESOLUTION_Y 500

#define WINDOW_TITLE "Computer Graphics Ricardo Brites 24"
#define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS

SDL_Window* Window = nullptr;
SDL_GLContext WindowContext= nullptr;
#pragma endregion

#pragma region Camera Setup
#define CAMERA_LOCATION_X 0.f
#define CAMERA_LOCATION_Y 0.f
#define CAMERA_LOCATION_Z -5.f
#pragma endregion


float vertices[] = 
{ 
	// Vertices						Color
	-0.8f,	 0.f,	 -0.5f,			1.f, 0.f, 0.f,
	0.f,	 0.f,	 1.f,			1.f, 0.f, 0.f,
	0.8f,	 0.f,	 -.5f,			1.f, 0.f, 0.f,

	0.f,	 0.f,	 1.f,			1.f, 1.f, 0.f,
	0.8f,	 0.f,	 -0.5f,			1.f, 1.f, 0.f,
	0.f,	 1.5f,	 0.f,			1.f, 1.f, 0.f,

	0.8f,	 0.f,	 -0.5f,			1.f, 0.5f, 0.f,
	-0.8f,	 0.f,	 -0.5f,			1.f, 0.5f, 0.f,
	0.f,	 1.5f,	 0.f,			1.f, 0.5f, 0.f,

	-0.8f,	0.f,	-0.5f,			1.f, 0.f, 1.f,
	0.f,	0.f,	1.f,			1.f, 0.f, 1.f,
	0.f,	1.5f,	0.f,			1.f, 0.f, 1.f
};


#pragma region Vertex Shader
const char* VertexShaderCode =
R"(
	#version 330 core
	
	in vec3 position;
	in vec3 color;
	out vec3 vertexColor;
	uniform mat4 ModelMatrix;
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;

	void main()
	{
		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.f);
		vertexColor = color;
	}
)";
#pragma endregion

#pragma region Fragment Shader
const char* FragmentShaderCode = 
R"(
	#version 330 core
	out vec4 color;

	in vec3 vertexColor;

	void main()
	{
		color = vec4(vertexColor, 1);
	}
)";
#pragma endregion


int main(int argc, char* argv[])
{
	#pragma region Init
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		return 1;

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	Window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_RESOLUTION, WINDOW_FLAGS);

	if (!Window)
	{
		SDL_Quit();
		return 1;
	}

	WindowContext = SDL_GL_CreateContext(Window);

	if (!WindowContext)
	{
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		SDL_GL_DeleteContext(WindowContext);
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
#pragma endregion
	
	#pragma region Shaders
	unsigned int VertexShader, FragmentShader, Shader;

	//Vertex Shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);

	//Fragment Shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);

	//Create
	Shader = glCreateProgram();
	glAttachShader(Shader, VertexShader);
	glAttachShader(Shader, FragmentShader);
	glLinkProgram(Shader);

	//Delete shaders
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	//Vertex Array (VAO)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertext Buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Pass info to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)12);
	glEnableVertexAttribArray(1);

#pragma endregion
	
	#pragma region Transformations
	float WindowAspectRatio = WINDOW_RESOLUTION_X / WINDOW_RESOLUTION_Y;
	mat4 PMatrix = glm::perspective(glm::radians(50.f), WindowAspectRatio, 0.1f, 100.f);

	mat4 VMatrix = mat4(1.f);
	//Translate the camera (quote unquote)
	
	//VMatrix = glm::translate(VMatrix, vec3(CAMERA_LOCATION_X, CAMERA_LOCATION_Y, CAMERA_LOCATION_Z));
	VMatrix = glm::lookAt(vec3(0.f, 3.f, -5.f), vec3(0,0,0), vec3(0,1,0));

	mat4 MMatrix = mat4(1.f);
	#pragma endregion

	bool IsRunning = true;
	while(IsRunning)
	{
		SDL_Event PollEvent;

		while(SDL_PollEvent(&PollEvent))
		{
			#pragma region Stop Execution
			switch (PollEvent.type)
			{
			case SDL_QUIT:
				IsRunning = false;
				break;
			default:
				break;
			}
#pragma endregion
		}
		#pragma region Clear Screen
		//Clear Screen
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		#pragma endregion

		glUseProgram(Shader);

		#pragma region Apply Transformations

		float ModelRotation = 0.f;
		vec3 ModelRotationAxis = vec3(0.f, 1.f, 0.f);

		//Rotate The Model
		if (ModelRotation > 360)
			ModelRotation -= 360;
		ModelRotation += 0.5f;
		MMatrix = glm::rotate(MMatrix, glm::radians(ModelRotation), ModelRotationAxis);

		//-----------------------------------------------------------------------------------------------------------------
		//-------									Communicate to the shader										-------
		//-----------------------------------------------------------------------------------------------------------------
		
		//Get Vertex Shader var locations
		unsigned int ProjectionMatrix, ViewMatrix, ModelMatrix;
		ProjectionMatrix = glGetUniformLocation(Shader, "ProjectionMatrix");
		ViewMatrix = glGetUniformLocation(Shader, "ViewMatrix");
		ModelMatrix = glGetUniformLocation(Shader, "ModelMatrix");

		// Send info to Vertex shader
		glUniformMatrix4fv(ProjectionMatrix, 1, GL_FALSE, glm::value_ptr(PMatrix));
		glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(VMatrix));
		glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(MMatrix));
	#pragma endregion

		#pragma region Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		SDL_GL_SwapWindow(Window);
		#pragma endregion
	}
	
	#pragma region Destruct
	SDL_GL_DeleteContext(WindowContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();
#pragma endregion

	return 0;
}