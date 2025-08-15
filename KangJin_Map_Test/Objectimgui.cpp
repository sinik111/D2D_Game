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
	ImGui::Begin(Name.c_str());
	for (int i = 0; i < m_GameObjectlist.size(); ++i)
	{
		Vector2 objectPosition = m_GameObjectlist[i]->GetTransform()->GetLocalPosition();
		Vector2 objectPivot = m_GameObjectlist[i]->GetComponent<BitmapRenderer>()->GetPivot();
		ImGui::DragFloat2("Object Position", &objectPosition.x, 1, -16280, 16280);
		ImGui::NewLine();
		ImGui::DragFloat2("Object pivot", &objectPivot.x, 0.001f, -1, 1);
		ImGui::NewLine();
		Structure* structure = m_GameObjectlist[i]->GetComponent<Structure>();
		if (nullptr != structure)
		{
			for (int j = 0; j < structure->m_Colliders.size(); ++j)
			{
				std::string offsetname = "CircleColliderOffset" + std::to_string(j);
				std::string Radiusname = "CircleColliderRadius" + std::to_string(j);

				Vector2 circleoffset = structure->m_Colliders[j]->GetOffset();
				float circleRadius = structure->m_Colliders[j]->GetCircle().radius;

				ImGui::DragFloat2(offsetname.c_str(), &circleoffset.x, 1, -10000, 10000);
				ImGui::DragFloat(Radiusname.c_str(), &circleRadius, 1, 1, 1000);
				ImGui::NewLine();


				m_GameObjectlist[i]->GetComponent<Structure>()->m_Colliders[j]->SetOffset(circleoffset);
				m_GameObjectlist[i]->GetComponent<Structure>()->m_Colliders[j]->SetRadius(circleRadius);
			}
		}
		
		m_GameObjectlist[i]->GetTransform()->SetLocalPosition(objectPosition);
		m_GameObjectlist[i]->GetComponent<BitmapRenderer>()->SetPivot(objectPivot);
	}
	
	ImGui::End();
} 