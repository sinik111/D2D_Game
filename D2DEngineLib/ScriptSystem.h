#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scripts;

	std::vector<Script*> m_scriptsForPendingInitialize;
	std::vector<Script*> m_scriptsForInitialize;
	std::vector<Script*> m_scriptsForStart;
	std::vector<Script*> m_scriptsForUpdate;
	std::vector<Script*> m_scriptsForLateUpdate;

	// 자식 클래스에서 Update, LateUpdate를 override 하지 않았을 경우
	// m_scriptsForUpdate, m_scriptsForLateUpdate에서 삭제하기 위해
	// 임시로 보관하는 컨테이너
	std::vector<Script*> m_pendingUnregisterForUpdate;
	std::vector<Script*> m_pendingUnregisterForLateUpdate;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);

public:
	void Update();

private:
	void CallInitialize();
	void CallStart();
	void CallUpdate();
	void CallLateUpdate();
};