#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <gl\GL.h>

#pragma region Window Setup
#define WINDOW_RESOLUTION 500, 500
#define WINDOW_TITLE "Computer Graphics Ricardo Brites 24"
#define WINDOW_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS

SDL_Window* Window = nullptr;
SDL_GLContext WindowContext= nullptr;
#pragma endregion

float vertices[] = 
{ 
	-0.5f, -.5f,0.f,
	.5f, -.5f, 0.f,
	0.f, .5f, 0.f 
};


#pragma region Vertex Shader
const char* VertexShaderCode =
"#version 330 core\n"
"in vec3 position;"
"void main()"
"{"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);"
"}";
#pragma endregion

#pragma region Fragment Shader
const char* FragmentShaderCode = 
"#version 330 core\n"
"out vec4 color;"
"void main()"
"{"
"	color = vec4(1, 1, 1, 1);"
"}";
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		#pragma region Render
		//Clear Screen
		glClearColor(0.f, 0.f, 0.f, 1);
		glClear(GL_RGB);


		glBindVertexArray(VAO);
		glUseProgram(Shader);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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