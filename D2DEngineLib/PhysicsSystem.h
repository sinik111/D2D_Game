#pragma once

class RigidBody2D;
class BoxCollider2D;

class PhysicsSystem
{
private:
	std::vector<RigidBody2D*> m_dynamicRigidBodies;
	std::vector<RigidBody2D*> m_kinematicRigidBodies;
	std::vector<RigidBody2D*> m_staticRigidBodies;
	std::vector<BoxCollider2D*> m_boxColliders;

public:
	void RegisterRigidBody2D(RigidBody2D* rigidBody2d);
	void UnregisterRigidBody2D(RigidBody2D* rigidBody2d);
	void RegisterBoxCollider2D(BoxCollider2D* boxCollider2d);
	void UnregisterBoxCollider2D(BoxCollider2D* boxCollider2d);

public:
	void ProcessPhysics();

	void Interpolate();
	void UpdateColliders();
};