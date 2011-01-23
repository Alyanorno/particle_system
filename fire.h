#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "vector.h"

class Fire
{
typedef linear_math::Vector<3> Vector;
public:
	Fire() {}
	void Initialize()
	{
		startPosition.Zero();
		
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

		// Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, &particleTexture);
	}
	struct Particle
	{
		Vector position;
		Vector direction;
		int time;
	};

	int Random()
	{
		static unsigned seed = 0;
		seed = seed * 0x343FD + 0x269EC3; 
		return seed >> 16 & 0x7FFF; 
	}
	inline Particle Create(/*Vector& direction*/)
	{
		Particle particle;
		particle.position = startPosition;
		particle.direction = Vector( Random(), Random(), -Random() );
		particle.direction.Normalize();
		particle.direction /= 100;
		particle.time = 1000;
		return particle;
	}
	static inline bool Remove( Particle& particle )
		{ return particle.time <= 0 ? true : false; }
	static inline void Update( Particle& particle )
		{ particle.position += particle.direction; --particle.time;}
	static inline void Draw( Particle& particle )
	{
		glPushMatrix();
		glTranslatef( particle.position[0],
			      particle.position[1],
			      particle.position[2] );
 		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.1f,  0.1f);  // Bottom Left Of The Texture
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.1f,  0.1f);  // Bottom Right Of The Texture
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f,  0.1f,  0.1f);  // Top Right Of The Texture
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f,  0.1f,  0.1f);  // Top Left Of The Texture

		glEnd();
		glPopMatrix();
	}
private:
	Vector startPosition;
	GLuint texture;
	BYTE particleTexture[256 * 256 * 3];
};
