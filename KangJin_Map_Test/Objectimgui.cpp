#include "../D2DEngineLib/framework.h"
#include "../D2DEngineLib/imgui_impl_dx11.h"
#include "../D2DEngineLib/GameObject.h"
#include "../D2DEngineLib/BitmapRenderer.h"
#include "../D2DEngineLib/Circle.h"
#include "../D2DEngineLib/CircleCollider.h"
#include "Structure.h"
#include "Objectimgui.h"

void Objectimgui::DrawImgui()
{
	ImGui::Begin("Structure Setting");
	Vector2 objectPosition = m_GameObject->GetTransform()->GetLocalPosition();
	Vector2 objectPivot = m_GameObject->GetComponent<BitmapRenderer>()->GetPivot();
	
	ImGui::DragFloat2("Object Position", &objectPosition.x, 1, -16280, 16280);
	ImGui::DragFloat2("Object Pivot", &objectPivot.x, 0.001f, 0, 1);
	ImGui::NewLine();
	Structure* structure = m_GameObject->GetComponent<Structure>();
	for (int i = 0; i < structure->index; ++i)
	{
		std::string offsetname = "CircleColliderOffset" + std::to_string(i);
		std::string Radiusname = "CircleColliderRadius" + std::to_string(i);

		Vector2 circleoffset = structure->m_Colliders[i]->GetOffset();
		float circleRadius = structure->m_Colliders[i]->GetCircle().radius;

		ImGui::DragFloat2(offsetname.c_str(), &circleoffset.x, 1, -10000, 10000);
		ImGui::DragFloat(Radiusname.c_str(), &circleRadius, 1, 1, 1000);
		ImGui::NewLine();

		m_GameObject->GetComponent<Structure>()->m_Colliders[i]->SetOffset(circleoffset);
		m_GameObject->GetComponent<Structure>()->m_Colliders[i]->SetRadius(circleRadius);
	}
	m_GameObject->GetTransform()->SetLocalPosition(objectPosition);
	m_GameObject->GetComponent<BitmapRenderer>()->SetPivot(objectPivot);
} 