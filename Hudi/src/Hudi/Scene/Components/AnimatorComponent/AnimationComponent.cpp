#include "hdpch.h"
#include "AnimationComponent.h"

namespace Hudi {
#ifdef ANIMATION
	void Animation::Update(float dt)
	{
		if (!filePath || m_NumberOfFrames == 0)
			return;

		if (m_CurrentFrame == 0 && m_TimePassed == 0.0f)
		{
			if (m_AnimationEvents[m_CurrentFrame])
				m_AnimationEvents[m_CurrentFrame]();
		}

		if (m_TimePassed >= m_FrameTimes[m_CurrentFrame])
		{
			// Current frame is the last frame
			if (m_CurrentFrame + 1 == m_NumberOfFrames)
			{
				if (loopTime)
				{
					m_CurrentFrame = 0;
					m_TimePassed = 0.0f;
				}
			}
			// Current frame is not the last frame
			else 
			{
				if (m_AnimationEvents[m_CurrentFrame])
					m_AnimationEvents[m_CurrentFrame]();

				m_CurrentFrame++;
			}
		}
		m_SpriteRenderer->SetSourceRect(m_FrameSize.x * m_CurrentFrame, 0, m_FrameSize.x, m_FrameSize.y);

		m_TimePassed += 0; // Time::deltaTime()* speed;
	}

	void Animation::GetSpriteRenderer()
	{
		m_SpriteRenderer = GetComponentByRef<SpriteRenderer>();
		m_SpriteRenderer->SetFile(filePath);
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

	void Animation::SetFrameTimes(const std::vector<float>& times)
	{
		m_NumberOfFrames = (uint8_t)times.size();
		m_FrameTimes = times;
		for (int i = 1; i < m_NumberOfFrames; i++)
		{
			m_FrameTimes[i] += m_FrameTimes[i - 1];
		}

		m_AnimationEvents.clear();
		m_AnimationEvents = std::vector<std::function<void(void)>>(m_NumberOfFrames, nullptr);
	}

	void Animation::SetFrameSize(int w, int h)
	{
		m_FrameSize.x = w;
		m_FrameSize.y = h;
	}
#endif

}