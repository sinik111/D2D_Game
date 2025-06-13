#pragma once

#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/Image.h"

class Camera;
class D2DRenderer;

class SpaceObject
	: public GameObject
{
private:
	Image* m_image;
	Camera* m_camera;
	D2DRenderer* m_renderer;

	float m_speed;

public:
	SpaceObject(Camera* camera, D2DRenderer* renderer, float speed);
	~SpaceObject();

	Image* GetImage();

public:
	void Update() override;
	void Render() override;
};