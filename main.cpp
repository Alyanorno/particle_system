#include <Windows.h>
#include "SDL/include/SDL.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

bool done = false;
const int max_particles = 100;
const int life_spawn = 1000;
Uint32 time;
Uint32 secondTime;
GLuint texture;
BYTE particleTexture[256 * 256 * 3];

template <typename T>
struct Vector
{
	Vector() :x(0), y(0) {}
	Vector(T _x, T _y) : x(_x), y(_y) {}
	T x, y;
	Vector operator+=(Vector<T> vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		return vector;
	}
};

struct Particle
{
	Particle( void ) {}
	Particle( int _life, Vector<float> _speed, Vector<float> _position )
	{
		life = _life;
		speed = _speed;
		pos = _position;
	}
	int life;
	Vector<float> speed;
	Vector<float> pos;
};

class ParticleSystem
{
#define i_par particles[i]
#define last_par particles[numParticles]
public:
	ParticleSystem( Vector<float> _position, Vector<float> _speed ) : numParticles(0) 
	{ pos = _position; speed = _speed; }
	void Add( void ) 
	{
		static float rotation = 0;
		last_par.life = life_spawn;
		last_par.speed.x = cos(rotation) * speed.x;
		last_par.speed.y = sin(rotation) * speed.y;
		last_par.pos = pos;
		++numParticles;
		if( numParticles >= max_particles )
			numParticles = 0;
		++rotation;
	}
	void Update( void )
	{
		for( int i = 0; i < max_particles; i++ )
		{
			i_par.pos += i_par.speed;
			--i_par.life;
		}
	}
	void Draw( void )
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.2f, -0.2f,  0.2f);  // Bottom Left Of The Texture
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.2f, -0.2f,  0.2f);  // Bottom Right Of The Texture
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.2f,  0.2f,  0.2f);  // Top Right Of The Texture
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.2f,  0.2f,  0.2f);  // Top Left Of The Texture
		glEnd();

		for( int i = 0; i < max_particles; i++ )
		{
 			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(i_par.pos.x - 0.2f, i_par.pos.y - 0.2f,  0.2f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(i_par.pos.x + 0.2f, i_par.pos.y - 0.2f,  0.2f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(i_par.pos.x + 0.2f, i_par.pos.y + 0.2f,  0.2f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(i_par.pos.x - 0.2f, i_par.pos.y + 0.2f,  0.2f);
			glEnd();
		}
	}

private:
	Vector<float> speed;
	Vector<float> pos;

	Particle particles[max_particles];
	int numParticles;
#undef last_par
#undef i_par
} particleSystem( Vector<float>(0, 0), Vector<float>(0.1, 0.1) );


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

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 2.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        //spelar ingen roll vilken ordning de är i
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
			particleTexture[i] = 250;
		// Grön
		if( j == 1 )
			particleTexture[i] = 0;
		// Blå
		if( j == 2 )
			particleTexture[i] = 0;
	}

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &particleTexture);
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
	particleSystem.Add();
	particleSystem.Update();
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

	particleSystem.Draw();

	glDisable( GL_TEXTURE_2D );
	glEnable( GL_LIGHTING );

	SDL_GL_SwapBuffers();
}
