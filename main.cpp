#include <Windows.h>
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

bool done = false;
const int number_of_particles = 100;
const int life_spawn = 1000;
Uint32 time;
Uint32 secondTime;
GLuint texture;
BYTE myTexture[256 * 256 * 3];

template <typename T>
struct Vector
{
	T x, y;
};

struct Particle
{
	Particle( void ) {}
	Particle( int _life, Vector<float> _speed, Vector<float> _position )
	{
		life = _life;
		speed = _speed;
		position = _position;
	}
	int life;
	Vector<float> speed;
	Vector<float> position;
};

class ParticleSystem
{
#define last_particle particles[sizeParticles]
public:
	ParticleSystem( Vector<float> _position ) : sizeParticles(0) { position = _position; }
	void add( void ) 
	{
		last_particle.life = life_spawn;
		last_particle.speed = speed;
		last_particle.position = position;
		++sizeParticles;
	}
	void update( void )
	{
		for( int i = 0; i < sizeParticles; i++ )
		{
			// Update them
		}
	}
	void draw( void )
	{
		for( int i = 0; i < sizeParticles; i++ )
		{
			// Draw them
		}
	}

private:
	Vector<float> speed;
	Vector<float> position;

	Particle particles[number_of_particles];
	int sizeParticles;
};


void Init( void );
void Run( void );
void Deinit( void );
int main( int argc, char *argv[])
{
	Init();
	Run();
	Deinit();
	return 0;
}

void Init( void )
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_SetVideoMode(800, 600, 0, SDL_OPENGL | SDL_HWSURFACE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // 1 1 1 1
	GLfloat mat_shininess[] = { 50.0 }; // 50
	GLfloat light_position[] = { 2.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        //spelar ingen roll vilken ordning de är i
	glViewport (0, 0, 800, 600);
	GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glShadeModel(GL_SMOOTH);  // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);  // Black Background
	glClearDepth(1.0f);  // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);  // Enables Depth Testing
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);	// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
	
	glMatrixMode (GL_PROJECTION); //ändrar vilken matrice som blir påverkad
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW); //ändrar vilken matrice som blir påverkad

	// 2D
	glGenTextures( 1, &texture );
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glBindTexture(GL_TEXTURE_2D, texture);

	for( int i(0), j(0); i < 256*256*3; i++,j++ )
	{
		if( j == 3)
			j = 0;

		// Röd
		if( j == 0 )
			myTexture[i] = 250;
		// Grön
		if( j == 1 )
			myTexture[i] = 0;
		// Blå
		if( j == 2 )
			myTexture[i] = 0;
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &myTexture);
}

void Deinit( void )
{
	// guess what
}

void Input( void );
void Update( void );
void Draw( void );
void Run( void )
{
	time = SDL_GetTicks();
	secondTime = SDL_GetTicks();
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
	static bool inputMouseLeft = false;

	SDL_Event event;
	while( SDL_PollEvent(&event) )
		if(event.key.keysym.sym == SDLK_ESCAPE)
			done = true;
		else if( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT )
			inputMouseLeft = true;

	if( inputMouseLeft == true )
		inputMouseLeft = false;
}

void Update( void )
{
}

void Draw( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

/*	glTranslatef( cubes[0].x , cubes[0].y ,  cubes[0].z );
	glRotatef( cubes[CAMERA].rotationX, 1.0, 0.0, 0.0 );
	glRotatef( cubes[CAMERA].rotationY, 0.0, 1.0, 0.0 );
	glRotatef( cubes[CAMERA].rotationZ, 0.0, 0.0, 1.0 ); */

	// 2D
	glLoadIdentity();
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );

	glBindTexture( GL_TEXTURE_2D, texture );

	glTranslatef( -1.0, 1.0, -5.0 );

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.2f,  0.2f);  // Bottom Left Of The Texture
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f, -0.2f,  0.2f);  // Bottom Right Of The Texture
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f,  0.2f,  0.2f);  // Top Right Of The Texture
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f,  0.2f,  0.2f);  // Top Left Of The Texture
	glEnd();

	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );

	SDL_GL_SwapBuffers();
}
