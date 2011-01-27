#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include "vector.h"

#define MAX_TIME 200

class Fire
{
typedef linear_math::Vector<3> Vector;
public:
	Fire() {}
	void Initialize()
	{
		startPosition.Zero();
		
		glGenTextures( 1, &texture );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture(GL_TEXTURE_2D, texture);

		for( int i(0); i < 256*256*4; i += 4)
		{
			int x = (i/4) % (256);
			int y = (i/4) / (256);

			if( x > 256 / 2 )
				x = 256 - x;
			if( y > 256 / 2 )
				y = 256 - y;

			int c = (x+y)/2;
			particleTexture[i] = c; // Red
			particleTexture[i+1] = c/3; // Green
			particleTexture[i+2] = c/20; // Blue
			particleTexture[i+3] = c; //Alpha
		}

		// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &particleTexture);


		particleDisplayList = glGenLists(1);

		glNewList( particleDisplayList, GL_COMPILE );
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f,  0.1f,  0.1f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f,  0.1f,  0.1f);

		glEnd();
		glEndList();
	}
	struct Particle
	{
		Vector position;
		Vector direction;
		int time;
	};
	inline Particle Create( Vector& direction )
	{
		Particle particle;
		particle.position = startPosition;
		particle.direction = Vector( direction[0] * 3200 + Random() - 16000,
					     direction[1] * 3200 + Random() - 16000,
					     direction[2] * 3200 + Random() - 16000 );
		particle.direction.Normalize();
		particle.direction /= 100;
		particle.time = Random() / MAX_TIME;
		return particle;
	}
	static inline bool Remove( Particle& particle )
		{ return particle.time <= 0 ? true : false; }
	inline void Prepare()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	static inline void Update( Particle& particle )
		{ particle.position += particle.direction; --particle.time;}
	inline void Draw( Particle& particle )
	{
		glPushMatrix();
		glColor4f( 1, 1, 1, (float)particle.time/MAX_TIME );
		glTranslatef( particle.position[0],
			      particle.position[1],
			      particle.position[2] );
		glCallList( particleDisplayList );
		glPopMatrix();
	}
protected:
	Vector startPosition;
private:
	int Random()
	{
		static unsigned int seed = 0;
		seed = seed * 0x343FD + 0x269EC3; 
		return seed >> 16 & 0x7FFF; 
	}
	GLuint texture;
	GLuint particleDisplayList;
	BYTE particleTexture[256 * 256 * 4];
};

#undef MAX_TIME
