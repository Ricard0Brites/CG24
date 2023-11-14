#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>

#undef main

#pragma region Window Setup
#define WINDOW_RESOLUTION 1280, 720
#define WINDOW_TITLE "Computer Graphics Ricardo Brites 24"
#define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS

SDL_Window* Window = nullptr;
SDL_GLContext WindowContext= nullptr;
#pragma endregion


int main(int argc, char* argv[])
{
	#pragma region Setup Window
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
#pragma endregion

	bool IsRunning = true;

	while(IsRunning)
	{
		SDL_Event PollEvent;

		while(SDL_PollEvent(&PollEvent))
		{
			switch (PollEvent.type)
			{
			case SDL_QUIT:
				IsRunning = false;
				break;
			default:
				break;
			}
		}

		//Render
		SDL_GL_SwapWindow(Window);
	}

	// Destruction
	SDL_GL_DeleteContext(WindowContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();

	return 0;
}