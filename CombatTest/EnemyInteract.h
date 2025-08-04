#pragma once
#include <iostream>

#include "../D2DEngineLib/Script.h"
#include "../D2DEngineLib/FSM.h"
#include "../D2DEngineLib/FSMContext.h"


class EnemyBase;
class Player;

class EnemyInteract
{
public:
	EnemyInteract() {}

	EnemyInteract(EnemyBase* script) 
	{
		m_owner = script;	
	}	
	~EnemyInteract()
	{
		m_owner = nullptr;	
	}

private:
	EnemyBase* m_owner = nullptr;

public:	
	
	//EnemyBase가 피격당하는 기능 관련---------------------------------------------------------------------------------

	// EnemyBase 피격 - Player 공격 통합 함수. Player의 트리거 충돌 함수 안에 넣어서 사용.
	// 
	//	사용 예시	 - 공격 오브젝트 객체는 자기 자신을 만들어낸 Player 객체를 가리키는 포인터 m_player를 갖고 있다고 가정. 갖고 있어야 함.
	// 
	//	if (collision.otherGameObject->GetName() == L"EnemyBase" )
	//	{
	//		auto comp = collision.otherGameObject->GetComponent<EnemyBase>();
	//		comp->EnemyIA()->HitEnemy(collision, m_player, this->GetGameObject()->GetName());
	//	
	//		//EnemyBase객체는 자신의 멤버 EnemyInteraction객체를 반환해주는 EnemyIA() 함수를 가짐
	//	}

public:
	void HitEnemy(const Collision& collision, const Player* pPlayer, const std::wstring& objName);
	void Parried(const Collision& collision, const Player* pPlayer, const std::wstring& objName);


private:
	//피격 시, EnemyBase의 m_isUnderAttack = true로 바꿈. 0.5초 이후 false가 되며, 피격시마다 다시 true, 타이머 0으로 초기화.
	void EnemyUnderAttack();	

	//넉다운 수치 누적
	void AddKnockdownValue(const int& value);

	//넉백, 넉다운 체크 /  넉다운 누적 >= 넉백 저항 =  넉백  /  넉다운 누적 >= 넉다운 저항 = 넉다운
	void CheckKnock();		

	// 적 피격시, 넉백 트리거 함수. 밀려날 거리와 속도를 매개변수로 넘김. 매개변수 없을 시 기본값 200.0f, 5.0f
	void KnockBack(const float& dist, const float& speed);

	//데미지 전달
	void DealDmg(const int& value);

};


