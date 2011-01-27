#include "particle_system.h"
#include "fire.h"
#include "smoke.h"
#include "vector.h"

#include <Windows.h>
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

bool done = false;
ParticleSystem< 350, Fire > fireParticles;
ParticleSystem< 200, Smoke > smokeParticles;

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
	SDL_Init( SDL_INIT_VIDEO );

	SDL_SetVideoMode( 800, 600, 0, SDL_OPENGL | SDL_HWSURFACE );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	glDisable( GL_DEPTH_TEST ); 
	
	glHint( GL_POINT_SMOOTH_HINT,GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );

	glViewport (0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	
	glMatrixMode (GL_PROJECTION);
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW);

	fireParticles.Initialize();
	smokeParticles.Initialize();
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

	fireParticles.Emit( 5, linear_math::Vector<3>( 0, 25, 0 ) );
	smokeParticles.Emit( 1, linear_math::Vector<3>( 0, 20, 0 ) );
}

void Update( void )
{
	fireParticles.Update();
	smokeParticles.Update();
}

void Draw( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef( 0.0, 0.0, -3.0 );
	
	smokeParticles.Draw();
	fireParticles.Draw();

	SDL_GL_SwapBuffers();
}
