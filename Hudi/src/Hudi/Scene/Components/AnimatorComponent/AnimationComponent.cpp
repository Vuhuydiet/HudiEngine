#include "hdpch.h"
#include "AnimationComponent.h"

namespace Hudi {

#ifdef SINGLE_SUPPORTED
	void Animation::AddFrame(const std::filesystem::path& path, float timePoint)
	{
		if (type != Type::Single)
			return;
		m_ImagePaths.push_back(path);
		m_FrameTimes.push_back(timePoint);
		m_CycleTime = hd_max(m_CycleTime, timePoint);
	}
#endif

	void Animation::LoadFrames(const std::filesystem::path& path, uint8_t frameNum, float frameDuration)
	{
		if (type != Type::Multiple)
			return;
		
		m_ImagePath = path;
		
		m_FrameNum = frameNum;
		for (int i = 0; i < frameNum; i++) {
			m_TimePoints.push_back(i * frameDuration);
		}
		m_CycleTime = frameNum * frameDuration;
	}

	void Animation::Init(SpriteRenderer* sprite)
	{
		m_SpriteRenderer = sprite;
		m_SpriteRenderer->SetFilePath(m_ImagePath);
	}

	void Animation::Update(float dt)
	{
		if (m_ImagePath.empty() || m_FrameNum == 0)
			return;

		bool changed = true;
		if (m_TimePassed > m_CycleTime)
		{
			m_TimePassed = 0.0f;
			m_CurrentFrame = 0;
		}
		// current frame is not the last frame && time passed exceeded the time point of the next frame
		else if (m_CurrentFrame < m_FrameNum - 1 && m_TimePassed >= m_TimePoints[m_CurrentFrame + 1])
		{
			m_CurrentFrame++;
		}
		else 
		{
			changed = false;
		}

		UpdateSpriteRenderer(changed);

		m_TimePassed += speed * dt;
	}

	void Animation::Stop()
	{
		speed = 0.0f;
	}

	void Animation::Reset()
	{
		speed = 1.0f;
		m_CurrentFrame = 0;
		m_TimePassed = 0.0f;
	}

	void Animation::UpdateSpriteRenderer(bool changed)
	{
		if (type == Type::Multiple)
		{
			Ref<Texture2D> texture = m_SpriteRenderer->texture;
			uint32_t texWidth = texture->GetWidth();
			float frameWidth = texWidth * 1.0f / m_FrameNum;
			texture->lowerBound.x = m_CurrentFrame * frameWidth;
			texture->upperBound.x = (m_CurrentFrame + 1) * frameWidth;
		}
	}


}