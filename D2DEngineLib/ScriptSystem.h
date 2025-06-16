#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scripts;

	std::vector<Script*> m_scriptsForStart;
	std::vector<Script*> m_scriptsForUpdate;
	std::vector<Script*> m_scriptsForLateUpdate;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);

public:
	void UpdateSystem();

private:
	void CallStart();
	void CallUpdate();
	void CallLateUpdate();
};