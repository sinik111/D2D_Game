#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scriptsForStart;
	std::vector<Script*> m_scriptsForFixedUpdate;
	std::vector<Script*> m_scriptsForUpdate;
	std::vector<Script*> m_scriptsForLateUpdate;

	std::vector<Script*> m_pendingUnregisteredScripts;
	std::vector<Script*> m_pendingUnregisteredFixedUpdate;
	std::vector<Script*> m_pendingUnregisteredUpdate;
	std::vector<Script*> m_pendingUnregisteredLateUpdate;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterFixedUpdate(Script* script);
	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);

public:
	void CallStart();
	void CallFixedUpdate();
	void CallUpdate();
	void CallLateUpdate();

private:
	void UnregisterScripts();
	void UpdateScriptsList();
};