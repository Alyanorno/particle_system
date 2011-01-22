#pragma once
#include "vector.h"

template < int size, class Policy >
class ParticleSystem : private Policy
{
typedef typename Policy::Particle Particle;
typedef typename linear_math::Vector<3> Vector;
public:
	ParticleSystem() : _size(0) {}
	void Initialize() 
		{ Polizy::Initialize(); }
	void Clear() { _size = 0 }
	void Emit( int amount, Vector direction ) 
	{
		if( _size + amount > size )
			amount = size - _size;
		for( int i(0); i < amount; i++ )
			_particles[i+_size] = Policy::Create();
		_size += amount;
	}
	void Update() 
	{
		for( int i(0); i < _size; i++ )
			if( !Policy::Remove( _particles[i] ) )
				Policy::Update( _particles[i] );
			else
				_particles[i] = _particles[(_size--)-1];
	}
	void Draw() // NEEDS WORK
	{
		//Set up everything correctly

		for( int i(0); i < _size; i++ )
			Policy::Draw( _particles[i] );

		//Reset everything correctly
	}
private:
	Particle _particles[size];
	int _size;
};
