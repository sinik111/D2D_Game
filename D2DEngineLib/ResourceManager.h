#pragma once

#include "BitmapResource.h"
#include "AnimationData.h"
#include "SpriteData.h"

class ResourceManager
{
private:
	Microsoft::WRL::ComPtr<ID2D1DeviceContext7> m_d2d1DeviceContext;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_wicImagingFactory;

	std::unordered_map<std::wstring, std::weak_ptr<BitmapResource>> m_bitmapResources;
	std::unordered_map<std::wstring, std::weak_ptr<SpriteSheet>> m_spriteSheets;
	std::unordered_map<std::wstring, std::weak_ptr<AnimationClip>> m_animationClips;

	//std::queue<std::pair<float, std::shared_ptr<BitmapResource>>> m_shortCachedBitmapResources;

	std::wstring m_resourcePath;

	float m_resourceTimer = 0.0f;

private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	~ResourceManager() = default;

public:
	static ResourceManager& Get();

public:
	HRESULT Initialize(Microsoft::WRL::ComPtr<ID2D1DeviceContext7> deviceContext,
		const std::wstring& modulePath,
		const std::wstring& binaryFolderName,
		const std::wstring& resourceFolderName);

	void Update();
	void Release();
	void ReleaseResources();

public:
	std::shared_ptr<BitmapResource> CreateBitmapResource(const std::wstring& filePath);
	std::shared_ptr<SpriteSheet> CreateSpriteSheet(const std::wstring& filePath);
	std::shared_ptr<AnimationClip> CreateAnimationClip(const std::wstring& filePath,
		std::unordered_map<std::wstring, std::shared_ptr<SpriteSheet>>& spriteSheets);

private:
	HRESULT LoadSpriteSheet(const std::wstring& filePath, std::shared_ptr<SpriteSheet>& spriteSheet);
	HRESULT LoadAnimationClip(const std::wstring& filePath,
		std::shared_ptr<AnimationClip>& animationClip,
		std::unordered_map<std::wstring, std::shared_ptr<SpriteSheet>>& spriteSheets);
};