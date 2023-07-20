#pragma once

#include "Hudi/Scene/Components/SpriteRendererComponent.h"

#include <string>
#include <vector>

namespace Hudi {

	class Animation
	{
	public:
		enum class Type 
		{
#ifdef SINGLE_SUPPORTED
			Single, 
#endif
			Multiple 
		};
	
	public:
		Animation() = default;
		Animation(const std::string& _name)
			: name(_name) {}
		Animation(const char* _name)
			: name(_name) {}

#ifdef SINGLE_SUPPORTED
		// Single
		void AddFrame(const std::filesystem::path& paths, float timePoint);
#endif
		// Multiple
		void LoadFrames(const std::filesystem::path& path, uint8_t frameNum, float frameDuration);

		void Init(SpriteRenderer* sprite);
		void Update(float dt);
		void Stop();
		void Reset();

	private:
		void UpdateSpriteRenderer(bool changed);

	public:
		std::string name;
		Type type = Type::Multiple;

		float speed = 1.0f;
		bool loop = true;
	private:
		std::filesystem::path m_ImagePath;

		int m_FrameNum = 0;
		std::vector<float> m_TimePoints;
		float m_CycleTime = 0.0f;

		float m_TimePassed = 0.0f;
		uint8_t m_CurrentFrame = 0;

	private:
		SpriteRenderer* m_SpriteRenderer = nullptr;
		friend class Animator;
	};

}