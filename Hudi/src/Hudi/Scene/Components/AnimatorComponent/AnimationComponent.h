#pragma once
#include "hdpch.h"

#include "Hudi/Scene/Components/Component.h"

namespace Hudi {
#ifdef ANIMATION
	class Animation : public Component
	{
	public:
		Animation() = default;
		Animation(const std::string& name) : name(name) {}
		virtual const char* ToString() const override { return "Animation"; }

		virtual void Update(float dt) override;

		void Stop();
		void Reset();

		void SetFrameTimes(const std::vector<float>& times); //*
		void SetFrameSize(int w, int h); //*

		void AddAnimationEvent(uint8_t frame, std::function<void(void)> function) { m_AnimationEvents[frame] = function; }
	private:
		void GetSpriteRenderer();
	public:
		std::string name = "unknown"; //*

		const char* filePath = nullptr; //*
		float speed = 1.0f;

		bool loopTime = true;
	private:
		Vec2<int> m_FrameSize;

		uint8_t m_NumberOfFrames = 0;
		uint8_t m_CurrentFrame = 0;
		std::vector<float> m_FrameTimes;
		float m_TimePassed = 0.0f;

		std::vector<std::function<void(void)>> m_AnimationEvents;

	private:
		Ref<SpriteRenderer> m_SpriteRenderer = nullptr;
		friend class Animator;
	};
#endif

}