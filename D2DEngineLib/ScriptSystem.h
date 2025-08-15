#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scriptsForInitialize;
	std::vector<Script*> m_scriptsForStart;
	std::vector<Script*> m_scriptsForFixedUpdate;
	std::vector<Script*> m_scriptsForUpdate;
	std::vector<Script*> m_scriptsForLateUpdate;
	std::vector<Script*> m_scriptsForOnRender;

	// 자식 클래스에서 override 하지 않았을 경우 삭제하기 위해 임시로 보관하는 컨테이너
	std::vector<Script*> m_pendingUnregisterForFixedUpdate;
	std::vector<Script*> m_pendingUnregisterForUpdate;
	std::vector<Script*> m_pendingUnregisterForLateUpdate;
	std::vector<Script*> m_pendingUnregisterForOnRender;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterFixedUpdate(Script* script);
	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);
	void UnregisterOnRender(Script* script);

public:
	void CallStart();
	void CallFixedUpdate();
	void CallUpdate();
	void CallLateUpdate();
	void CallOnRender();
};