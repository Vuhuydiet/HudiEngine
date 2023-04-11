#pragma once
#include "hdpch.h"
#include "Condition.h"

namespace Hudi {

	struct Transition
	{
	public:
		Transition(const char* nextAnim, std::unordered_map<const char*, Ref<Condition>>& parameters)
			: m_NextAnimation(nextAnim), m_Parameters(parameters) {}

		void AddCondition(const char* name, int value);
		void AddCondition(const char* name, float value);
		void AddCondition(const char* name, double value);
		void AddCondition(const char* name, bool value);
		void AddCondition(const char* name);

	private:
		bool IsSatisfied(
			std::unordered_map<const char*, int>& ints,
			std::unordered_map<const char*, float>& floats,
			std::unordered_map<const char*, bool>& bools,
			std::unordered_set<const char*>& triggers
		);
	private:
		std::vector<Ref<Condition>> m_Conditions;
		const char* m_NextAnimation = nullptr;
		std::unordered_map<const char*, Ref<Condition>>& m_Parameters;
	private:
		friend class Animator;
	};


	// Definitions
	inline void Transition::AddCondition(const char* name, int value)
	{
		if (m_Parameters.find(name) == m_Parameters.end() ||
			m_Parameters[name]->Type() != "int")
			return;
		
		Ref<IntCondition> cdn = NewRef<IntCondition>(*std::static_pointer_cast<IntCondition>(m_Parameters[name]));
		cdn->value = value;
		m_Conditions.push_back(cdn);
	}
	inline void Transition::AddCondition(const char* name, float value)
	{
		if (m_Parameters.find(name) == m_Parameters.end() ||
			m_Parameters[name]->Type() != "float")
			return;

		Ref<FloatCondition> cdn = NewRef<FloatCondition>(*std::static_pointer_cast<FloatCondition>(m_Parameters[name]));
		cdn->value = value;
		m_Conditions.push_back(cdn);
	}
	inline void Transition::AddCondition(const char* name, double value)
	{
		AddCondition(name, (float)value);
	}
	inline void Transition::AddCondition(const char* name, bool value)
	{
		if (m_Parameters.find(name) == m_Parameters.end() ||
			m_Parameters[name]->Type() != "bool")
			return;

		Ref<BoolCondition> cdn = NewRef<BoolCondition>(*std::static_pointer_cast<BoolCondition>(m_Parameters[name]));
		cdn->value = value;
		m_Conditions.push_back(cdn);
	}
	inline void Transition::AddCondition(const char* name)
	{
		if (m_Parameters.find(name) == m_Parameters.end() ||
			m_Parameters[name]->Type() != "trigger")
			return;

		Ref<TriggerCondition> cdn = NewRef<TriggerCondition>(*std::static_pointer_cast<TriggerCondition>(m_Parameters[name]));
		m_Conditions.push_back(cdn);
	}

	inline bool Transition::IsSatisfied(
		std::unordered_map<const char*, int>& ints,
		std::unordered_map<const char*, float>& floats,
		std::unordered_map<const char*, bool>& bools,
		std::unordered_set<const char*>& triggers)
	{
		for (auto& cdn : m_Conditions)
		{
			const char* type = cdn->Type();
			if (type == "int")
			{
				IntCondition int_cdn = *std::static_pointer_cast<IntCondition>(cdn);
				if (ints.find(cdn->name)==ints.end() || ints[cdn->name] != int_cdn.value)
					return false;
			}
			else if (type == "float")
			{
				FloatCondition float_cdn = *std::static_pointer_cast<FloatCondition>(cdn);
				if (floats.find(cdn->name)==floats.end() || floats[cdn->name]!=float_cdn.value)
					return false;
			}
			else if (type == "bool")
			{
				BoolCondition bool_cdn = *std::static_pointer_cast<BoolCondition>(cdn);
				if (bools.find(cdn->name) == bools.end() || bools[cdn->name] != bool_cdn.value)
					return false;
			}
			else if (type == "trigger")
			{
				TriggerCondition trigger_cdn = *std::static_pointer_cast<TriggerCondition>(cdn);
				if (triggers.find(cdn->name)==triggers.end())
					return false;
			}
		}
		return true;
	}

	/*if (m_Parameters.find(name) == m_Parameters.end())
		return;

	Ref<Condition> in_cdn = m_Parameters[name];
	const char* type = m_Parameters[name]->Type();
	if (type == "int")
	{
		Ref<IntCondition> cdn = NewRef<IntCondition>(*std::static_pointer_cast<IntCondition>(in_cdn));
		cdn->value = static_cast<int>(value);
		conditions.push_back(cdn);
	}
	else if (type == "float")
	{
		Ref<FloatCondition> cdn = NewRef<FloatCondition>(*std::static_pointer_cast<FloatCondition>(in_cdn));
		cdn->value = static_cast<float>(value);
		conditions.push_back(cdn);
	}
	else if (type == "bool")
	{
		Ref<BoolCondition> cdn = NewRef<BoolCondition>(*std::static_pointer_cast<BoolCondition>(in_cdn));
		cdn->value = static_cast<bool>(value);
		conditions.push_back(cdn);
	}
	else if (type == "trigger")
	{
		Ref<TriggerCondition> cdn = NewRef<TriggerCondition>(*std::static_pointer_cast<TriggerCondition>(in_cdn));
		conditions.push_back(cdn);
	}*/
}