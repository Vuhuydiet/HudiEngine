#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Hudi/Core/Core.h"

#include "Condition.h"
#include "Transition.h"
#include "AnimationComponent.h"

namespace Hudi {
#ifdef ANIMATION
	class Animator : public Component
	{
	public:
		Animator();
		void Init() override;
		const char* ToString() const override { return "Animator"; }

		virtual void Awake() override;
		virtual void Update(float dt) override;

		void AddAnimation(Ref<Animation> anim);

		void MakeTransition(const std::string& anim_1, const std::string& anim_2);
		Transition& GetTransition(const std::string& anim_1, const std::string& anim_2);

		void Add_Int_Parameter(const std::string& name);
		void Add_Float_Parameter(const std::string& name);
		void Add_Bool_Parameter(const std::string&);
		void Add_Trigger_Parameter(const std::string&);

		void SetInt(const std::string&, int in_int);
		void SetFloat(const std::string&, float in_float);
		void SetBool(const std::string& e, bool in_bool);
		void SetTrigger(const std::string& name);

		int GetInt(const std::string& name);
		float GetFloat(const std::string& name);
		bool GetBool(const std::string& name);
		bool GetTrigger(const std::string& name);

	private:
		void TransferAnimation(const std::string& cur_anim);

	private:
		std::unordered_map<std::string, Ref<Animation>> m_Animations;
		std::string m_CurrentAnimation = nullptr;

		std::unordered_map<std::string, Ref<Condition>> m_Parameters;
		std::unordered_map<std::string, int> m_Ints;
		std::unordered_map<std::string, float> m_Floats;
		std::unordered_map<std::string, bool> m_Bools;
		std::unordered_set<std::string> m_Triggers;

		std::unordered_map<std::string, std::vector<Transition>> m_AnimationTransitions;
	};
#endif
}