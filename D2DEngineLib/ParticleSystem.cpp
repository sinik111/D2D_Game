#include "pch.h"
#include "ParticleSystem.h"

#include "Particle.h"

void ParticleSystem::RegisterParticle(Particle* particle)
{
	m_particles.push_back(particle);
}

void ParticleSystem::UnregisterParticle(Particle* particle)
{
	Util::OptimizedErase(m_particles, particle);
}

void ParticleSystem::Update()
{
	for (const auto& particle : m_particles)
	{
		particle->Update();
	}
}
