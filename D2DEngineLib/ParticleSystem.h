#pragma once

class Particle;

class ParticleSystem
{
private:
	std::vector<Particle*> m_particles;

public:
	void RegisterParticle(Particle* particle);
	void UnregisterParticle(Particle* particle);

public:
	void Update();
};