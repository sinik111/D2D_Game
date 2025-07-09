#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scripts;

	std::vector<Script*> m_scriptsForPendingInitialize;
	std::vector<Script*> m_scriptsForInitialize;
	std::vector<Script*> m_scriptsForStart;
	std::vector<Script*> m_scriptsForFixedUpdate;
	std::vector<Script*> m_scriptsForUpdate;
	std::vector<Script*> m_scriptsForLateUpdate;

	// �ڽ� Ŭ�������� override ���� �ʾ��� ��� �����ϱ� ���� �ӽ÷� �����ϴ� �����̳�
	std::vector<Script*> m_pendingUnregisterForFixedUpdate;
	std::vector<Script*> m_pendingUnregisterForUpdate;
	std::vector<Script*> m_pendingUnregisterForLateUpdate;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterFixedUpdate(Script* script);
	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);

public:
	void CallInitialize();
	void CallStart();
	void CallFixedUpdate();
	void CallUpdate();
	void CallLateUpdate();
};