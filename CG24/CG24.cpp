#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <ext.hpp>

#include"OBJ.h"
#include "Shader.h"
#include "Camera.h"

typedef glm::mat4 mat4;
typedef glm::vec3 vec3;

#pragma region Window Setup
#define WINDOW_RESOLUTION WINDOW_RESOLUTION_X, WINDOW_RESOLUTION_Y
#define WINDOW_RESOLUTION_X 1280
#define WINDOW_RESOLUTION_Y 720
#define ASPECTRATIO (float)(WINDOW_RESOLUTION_X / WINDOW_RESOLUTION_Y)

#define WINDOW_TITLE "Computer Graphics Ricardo Brites 24"
#define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS
#define WINDOWBACKGROUNDCOLOR 0.f, 0.f, 0.f

SDL_Window* Window = nullptr;
SDL_GLContext WindowContext = nullptr;
#pragma endregion

#pragma region Camera Setup
#define CAMERA_LOCATION_X 40.f
#define CAMERA_LOCATION_Y 0.f
#define CAMERA_LOCATION_Z 0.f

#define SENSITIVITYMULTIPLIER 0.1f
#pragma endregion

#define DEGTORAD(Deg) (Deg * (glm::pi<float>() / 180))
#define ROTATEMATRIX(ModelMatrix, DegToRotate, V3Axis) ModelMatrix = glm::rotate(ModelMatrix, DEGTORAD(DegToRotate), V3Axis);

#define MOVEMENTSPEED 0.025f

#define FILETOOPEN "Rabbit_Lowpoly_3.obj"

int main(int argc, char* argv[])
{
	#pragma region Load OBJ
		OBJ model;
		if (!model.LoadOBJ(FILETOOPEN))
			return 0;
		std::vector<float> vertices = model.GetVertices();
	#pragma endregion

	#pragma region Init Window
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			return 1;

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
		Shader shader;
		shader.InitShader();

		//Vertex Array (VAO)
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Vertext Buffer
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		//Vertex Locations
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
		glEnableVertexAttribArray(0);

		//Vertex Colors
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)12);
		glEnableVertexAttribArray(1);

		//Vertex Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)24);
		glEnableVertexAttribArray(2);
	#pragma endregion

	#pragma region Camera
		Camera camera;

		camera.SetLocation(vec3(CAMERA_LOCATION_X, CAMERA_LOCATION_Y, CAMERA_LOCATION_Z));
		camera.AddYaw(-90);
	#pragma endregion

	#pragma region Transformations
		//these are base values, the updatable values are implemented in the game loop
		mat4 PMatrix = glm::perspective(glm::radians(50.f), ASPECTRATIO, 0.1f, 100.f);
		mat4 MMatrix = mat4(1.f);
	#pragma endregion

	#pragma region Input
		const Uint8* keyStates = SDL_GetKeyboardState(nullptr);
		SDL_SetRelativeMouseMode(SDL_TRUE);

	#pragma endregion
	bool IsRunning = true;
	while (IsRunning) // game loop
	{
		#pragma region Events
		SDL_Event PollEvent;
		while (SDL_PollEvent(&PollEvent))
		{
			#pragma region Stop Execution
			switch (PollEvent.type)
						{
						case SDL_QUIT:
							SDL_SetRelativeMouseMode(SDL_FALSE);
							IsRunning = false;
							break;
						default:
							break;
						}
			#pragma endregion
			#pragma region Mouse Input
			if (SDL_GetWindowFlags(Window) & (SDL_WINDOW_INPUT_FOCUS))
			{
				if (PollEvent.type == SDL_MOUSEMOTION)
				{
					camera.AddPitch((float)PollEvent.motion.yrel * SENSITIVITYMULTIPLIER);
					camera.AddYaw((float)PollEvent.motion.xrel * SENSITIVITYMULTIPLIER);
				}
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region Keyboard Input
		int counter = 0;
		vec3 movementNormalVector = vec3(0);

		if(keyStates[SDL_SCANCODE_W])
		{
			movementNormalVector += camera.GetForwardVector();
			counter++;
		}
		if(keyStates[SDL_SCANCODE_A])
		{
			movementNormalVector -= camera.GetRightVector();
			counter++;
		}
		if(keyStates[SDL_SCANCODE_S])
		{
			movementNormalVector -= camera.GetForwardVector();
			counter++;
		}
		if(keyStates[SDL_SCANCODE_D])
		{
			movementNormalVector += camera.GetRightVector();
			counter++;
		}
		if(keyStates[SDL_SCANCODE_Q])
		{
			movementNormalVector -= camera.GetUpVector();
			counter++;
		}
		if(keyStates[SDL_SCANCODE_E])
		{
			movementNormalVector += camera.GetUpVector();
			counter++;
		}
		if(counter > 0)
		{
			camera.AddOffset(movementNormalVector / (float)counter, MOVEMENTSPEED);
			counter = 0;
		}
		#pragma endregion

		#pragma region Clear Screen
		//Clear Screen
		glClearColor(WINDOWBACKGROUNDCOLOR, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.UseShader();
		#pragma endregion		

		#pragma region Matrix Transformations
		shader.SetModelMatrix(MMatrix);
		shader.SetProjectionMatrix(PMatrix);
		shader.SetViewMatrix(camera.GetViewMatrix());
		#pragma endregion

		//this just keeps the mouse centered in the window so we dont loose window focus while
		// moving the camera arround
		SDL_WarpMouseInWindow(Window, WINDOW_RESOLUTION_X / 2, WINDOW_RESOLUTION_Y / 2); 
	
		#pragma region Render
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, model.GetNumOfVertices());
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