#pragma once
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "vector.h"

template < int size, class Policy >
class ParticleSystem : private Policy
{
typedef typename Policy::Particle Particle;
typedef typename linear_math::Vector<3> Vector;
public:
	ParticleSystem() : Policy(), _size(0) {}
	void Initialize()
		{ Policy::Initialize(); }
	void Clear() { _size = 0 }
	void Emit( int amount, Vector direction = Vector( 0, 0, 0 ) )
	{
		if( _size + amount > size )
			amount = size - _size;
		for( int i(0); i < amount; i++ )
			_particles[i+_size] = Policy::Create( direction );
		_size += amount;
	}
	void SetPosition( float x, float y, float z )
	{
		Policy::startPosition[0] = x;
		Policy::startPosition[1] = y;
		Policy::startPosition[2] = z;
	}
	void Update() 
	{
		for( int i(0); i < _size; i++ )
			if( !Policy::Remove( _particles[i] ) )
				Policy::Update( _particles[i] );
			else
				_particles[i] = _particles[--_size];
	}
	void Draw()
	{
		glPushMatrix();

		float view[16];
		glGetFloatv(GL_MODELVIEW_MATRIX , view);

		for( int i(0); i<3; i++ )
			for( int j(0); j<3; j++ )
				if ( i==j )
					view[i*4+j] = 1.0;
				else
					view[i*4+j] = 0.0;

		glLoadMatrixf(view);

		Policy::Prepare();
		for( int i(0); i < _size; i++ )
			Policy::Draw( _particles[i] );

		glPopMatrix();
	}
private:
	Particle _particles[size];
	int _size;
};
