#pragma once

class Script;

class ScriptSystem
{
private:
	std::vector<Script*> m_scripts;

	std::vector<Script*> m_startScripts;
	std::vector<Script*> m_updateScripts;
	std::vector<Script*> m_lateUpdateScripts;

public:
	void Register(Script* script);
	void Unregister(Script* script);

	void UnregisterUpdate(Script* script);
	void UnregisterLateUpdate(Script* script);

public:
	void UpdateSystem();

private:
	void StartScripts();
	void UpdateScripts();
	void LateUpdateScripts();
};