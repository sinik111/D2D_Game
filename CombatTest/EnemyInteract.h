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
	
	//EnemyBase�� �ǰݴ��ϴ� ��� ����---------------------------------------------------------------------------------

	// EnemyBase �ǰ� - Player ���� ���� �Լ�. Player�� Ʈ���� �浹 �Լ� �ȿ� �־ ���.
	// 
	//	��� ����	 - ���� ������Ʈ ��ü�� �ڱ� �ڽ��� ���� Player ��ü�� ����Ű�� ������ m_player�� ���� �ִٰ� ����. ���� �־�� ��.
	// 
	//	if (collision.otherGameObject->GetName() == L"EnemyBase" )
	//	{
	//		auto comp = collision.otherGameObject->GetComponent<EnemyBase>();
	//		comp->EnemyIA()->HitEnemy(collision, m_player, this->GetGameObject()->GetName());
	//	
	//		//EnemyBase��ü�� �ڽ��� ��� EnemyInteraction��ü�� ��ȯ���ִ� EnemyIA() �Լ��� ����
	//	}

public:
	void HitEnemy(const Collision& collision, const Player* pPlayer, const std::wstring& objName);
	void Parried(const Collision& collision, const Player* pPlayer, const std::wstring& objName);


private:
	//�ǰ� ��, EnemyBase�� m_isUnderAttack = true�� �ٲ�. 0.5�� ���� false�� �Ǹ�, �ǰݽø��� �ٽ� true, Ÿ�̸� 0���� �ʱ�ȭ.
	void EnemyUnderAttack();	

	//�˴ٿ� ��ġ ����
	void AddKnockdownValue(const int& value);

	//�˹�, �˴ٿ� üũ /  �˴ٿ� ���� >= �˹� ���� =  �˹�  /  �˴ٿ� ���� >= �˴ٿ� ���� = �˴ٿ�
	void CheckKnock();		

	// �� �ǰݽ�, �˹� Ʈ���� �Լ�. �з��� �Ÿ��� �ӵ��� �Ű������� �ѱ�. �Ű����� ���� �� �⺻�� 200.0f, 5.0f
	void KnockBack(const float& dist, const float& speed);

	//������ ����
	void DealDmg(const int& value);

};


