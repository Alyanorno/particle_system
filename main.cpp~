#include "particle_system.h"
#include "fire.h"

#include <Windows.h>
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

bool done = false;
ParticleSystem< 100, Fire > fireParticles;

void Initialize( void );
void Run( void );
void Deinit( void );
int main( int argc, char *argv[])
{
	Initialize();
	Run();
	Deinit();
	return 0;
}

void Initialize( void )
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_SetVideoMode(800, 600, 0, SDL_OPENGL | SDL_HWSURFACE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 2.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glViewport (0, 0, 800, 600);
	GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glMatrixMode (GL_PROJECTION);
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

void Deinit( void )
{
}

void Input( void );
void Update( void );
void Draw( void );
void Run( void )
{
	Uint32 time = SDL_GetTicks();
	Uint32 secondTime = SDL_GetTicks();
	while( !done )
	{
		while( time + 10 > secondTime)
		{
			SDL_Delay(1);
			secondTime = SDL_GetTicks();
		}

		Input();
		Update();
		Draw();
		time = SDL_GetTicks();
	}
}

void Input( void )
{
	SDL_Event event;
	while( SDL_PollEvent(&event) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = true;
}

void Update( void )
{
}

void Draw( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	SDL_GL_SwapBuffers();
}
