#pragma once
#include "hdpch.h"
#include "AnimationComponent.h"
#include "Condition.h"
#include "Transition.h"

#include <ECS.h>

namespace Hudi {

	class Animator : public Component
	{
	public:
		Animator();
		void Init() override;
		const char* ToString() const override { return "Animator"; }

		virtual void Awake() override;
		virtual void Update() override;

		void AddAnimation(const Animation& anim);

		void MakeTransition(
			const char* anim_1, const char* anim_2
		);
		Transition& GetTransition(const char* anim_1, const char* anim_2);

		void Add_Int_Parameter(const char* name);
		void Add_Float_Parameter(const char* name);
		void Add_Bool_Parameter(const char* name);
		void Add_Trigger_Parameter(const char* name);

		void SetInt(const char* name, int in_int);
		void SetFloat(const char* name, float in_float);
		void SetBool(const char* name, bool in_bool);
		void SetTrigger(const char* name);

		int GetInt(const char* name);
		float GetFloat(const char* name);
		bool GetBool(const char* name);
		bool GetTrigger(const char* name);

	private:
		void TransferAnimation(const char* cur_anim);

	private:
		std::unordered_map<const char*, Animation> m_Animations;
		const char* m_CurrentAnimation = nullptr;

		std::unordered_map<const char*, Ref<Condition>> m_Parameters;
		std::unordered_map<const char*, int> m_Ints;
		std::unordered_map<const char*, float> m_Floats;
		std::unordered_map<const char*, bool> m_Bools;
		std::unordered_set<const char*> m_Triggers;

		std::unordered_map<const char*, std::vector<Transition>> m_AnimationTransitions;
	};
}