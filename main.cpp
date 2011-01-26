#include "particle_system.h"
#include "fire.h"
#include "vector.h"

#include <Windows.h>
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

bool done = false;
ParticleSystem< 1000, Fire > fireParticles;

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
	
	glDisable(GL_DEPTH_TEST); 
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	glEnable( GL_TEXTURE_2D );

	glViewport (0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	
	glMatrixMode (GL_PROJECTION);
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW);

	fireParticles.Initialize();
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
	static bool inputE = false;

	SDL_Event event;
	while( SDL_PollEvent(&event) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = true;
		else if(event.key.keysym.sym == 'e')
			inputE = inputE ? false: true;

	if( inputE )
		fireParticles.Emit( 10 );
}

void Update( void )
{
	fireParticles.Update();
}

void Draw( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	fireParticles.Draw();

	SDL_GL_SwapBuffers();
}
