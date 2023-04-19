#include "hdpch.h"
#include "AnimatorComponent.h"

namespace Hudi {
#ifdef ANIMATION
	Animator::Animator()
	{
		//Init();
	}

	void Animator::Init()
	{
		AddAnimation(world->CreateComponent<Animation>("Entry"));
		AddAnimation(world->CreateComponent<Animation>("AnyState"));
		m_CurrentAnimation = "Entry";

		AddComponent<Animation>(m_Animations["Entry"]);
	}

	void Animator::Awake()
	{
		m_Triggers.clear();
	}

	void Animator::Update(float dt)
	{
		TransferAnimation(m_CurrentAnimation);
		TransferAnimation("AnyState");
		if (m_CurrentAnimation == "Entry")
			return;
	}

	void Animator::TransferAnimation(const std::string& cur_anim)
	{
		for (auto& transition : m_AnimationTransitions[cur_anim])
		{
			if (transition.IsSatisfied(m_Ints, m_Floats, m_Bools, m_Triggers))
			{
				m_CurrentAnimation = transition.m_NextAnimation;

				AddComponent<Animation>(m_Animations[m_CurrentAnimation]);
				GetComponent<Animation>().GetSpriteRenderer();
				return;
			}
		}
	}
		
	void Animator::AddAnimation(Ref<Animation> anim)
	{
		m_Animations[anim->name] = anim;

		if (m_Animations.size()==3)
		{
			Add_Bool_Parameter("default");
			SetBool("default", true);
			MakeTransition("Entry", anim->name);
			GetTransition("Entry", anim->name).AddCondition("default", true);
		}
	}

	void Animator::MakeTransition(const std::string& anim_1, const std::string& anim_2)
	{
		if (m_Animations.find(anim_1) == m_Animations.end() ||
			m_Animations.find(anim_2) == m_Animations.end())
			return;

		m_AnimationTransitions[anim_1].push_back(Transition(anim_2, m_Parameters));
	}

	Transition& Animator::GetTransition(const std::string& anim_1, const std::string& anim_2)
	{
		for (auto& transition : m_AnimationTransitions[anim_1])
		{
			if (transition.m_NextAnimation == anim_2)
				return transition;
		}

		HD_CORE_WARN("Unexisted transition. Returning default transition.");
		static Transition unexist("unknown", m_Parameters);
		return unexist;
	}

	void Animator::Add_Int_Parameter(const std::string& name)
	{
		m_Parameters[name] = NewRef<IntCondition>(name); 
		m_Ints[name] = 0;
	}
	void Animator::Add_Float_Parameter(const std::string& name) 
	{ 
		m_Parameters[name] = NewRef<FloatCondition>(name);
		m_Floats[name] = 0.0f; 
	}
	void Animator::Add_Bool_Parameter(const std::string& name) 
	{ 
		m_Parameters[name] = NewRef<BoolCondition>(name); 
		m_Bools[name] = false;
	}
	void Animator::Add_Trigger_Parameter(const std::string& name)
	{
		m_Parameters[name] = NewRef<TriggerCondition>(name);
	}

	void Animator::SetInt(const std::string& name, int in_int) 
	{ 
		if (m_Ints.find(name) == m_Ints.end())
			return;
		m_Ints[name] = in_int;
	}
	void Animator::SetFloat(const std::string& name, float in_float) 
	{ 
		if (m_Floats.find(name) == m_Floats.end())
			return;
		m_Floats[name] = in_float;
	}
	void Animator::SetBool(const std::string& name, bool in_bool)
	{ 
		if (m_Bools.find(name) == m_Bools.end())
			return;
		m_Bools[name] = in_bool;
	}
	void Animator::SetTrigger(const std::string& name) 
	{
		if (m_Triggers.find(name) == m_Triggers.end())
			return;
		m_Triggers.insert(name);
	}

	int Animator::GetInt(const std::string& name)
	{
		if (m_Ints.find(name) == m_Ints.end())
		{
			HD_CORE_ERROR("No \"{0}\" int found, returning default value \"{1}\".", name, 0);
			return 0;
		}
		return m_Ints[name];
	}
	float Animator::GetFloat(const std::string& name)
	{
		if (m_Floats.find(name) == m_Floats.end())
		{
			HD_CORE_ERROR("No \"{0}\" float found, returning default value \"{1}\".", name, 0.0f);
			return 0.0f;
		}
		return m_Floats[name];
	}
	bool Animator::GetBool(const std::string& name)
	{
		if (m_Bools.find(name) == m_Bools.end())
		{
			HD_CORE_ERROR("No \"{0}\" bool found, returning default value \"{1}\".", name, false);
			return false;
		}
		return m_Bools[name];
	}
	bool Animator::GetTrigger(const std::string& name)
	{
		if (m_Triggers.find(name) == m_Triggers.end())
		{
			HD_CORE_ERROR("No \"{0}\" trigger found, returning default value \"{1}\".", name, false);
			return false;
		}
		return m_Triggers.find(name) != m_Triggers.end();
	}
#endif

}