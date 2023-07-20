#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

namespace Hudi {

	struct Condition
	{
		enum class Type { None, Int, Float, Bool, Trigger };
		Condition(const std::string& _name, Type _type)
			: name(_name), type(_type) {}

		std::string name;
		Type type = Type::None;
		
		union Value {
			int Int;
			float Float;
			bool Bool;
		} value;

	};

	struct Transition
	{
	public:
		Transition(const std::string& nextAnim)
			: m_NextAnimation(nextAnim) {}

		void AddCondition(const Condition& cdn);
		void Add_IntCondition(const std::string& name, int value);
		void Add_FloatCondition(const std::string& name, float value);
		void Add_BoolCondition(const std::string& name, bool value);
		void Add_TriggerCondition(const std::string& name);

	private:
		bool IsSatisfied(
			const std::unordered_map<std::string, int>& ints,
			const std::unordered_map<std::string, float>& floats,
			const std::unordered_map<std::string, bool>& bools,
			const std::unordered_set<std::string>& triggers
		);
	private:
		std::vector<Condition> m_Conditions;
		std::string m_NextAnimation;
	private:
		friend class Animator;
	};


	// Definitions
	inline void Transition::AddCondition(const Condition& cdn)
	{
		m_Conditions.push_back(cdn);
	}

	inline void Transition::Add_IntCondition(const std::string& name, int value)
	{
		Condition cdn(name, Condition::Type::Int);
		cdn.value.Int = value;
		AddCondition(cdn);
	}
	inline void Transition::Add_FloatCondition(const std::string& name, float value)
	{
		Condition cdn(name, Condition::Type::Float);
		cdn.value.Float = value;
		AddCondition(cdn);
	}
	inline void Transition::Add_BoolCondition(const std::string& name, bool value)
	{
		Condition cdn(name, Condition::Type::Bool);
		cdn.value.Bool = value;
		AddCondition(cdn);
	}
	inline void Transition::Add_TriggerCondition(const std::string& name)
	{
		Condition cdn(name, Condition::Type::Trigger);
		AddCondition(cdn);
	}

	inline bool Transition::IsSatisfied(
		const std::unordered_map<std::string, int>& ints,
		const std::unordered_map<std::string, float>& floats,
		const std::unordered_map<std::string, bool>& bools,
		const std::unordered_set<std::string>& triggers)
	{
		for (const auto& cdn : m_Conditions)
		{
			Condition::Type type = cdn.type;
			switch (type)
			{
			case Condition::Type::Int:
				if (ints.find(cdn.name) == ints.end() || ints.at(cdn.name) != cdn.value.Int)
					return false;
				break;
			case Condition::Type::Float:
				if (floats.find(cdn.name) == floats.end() || floats.at(cdn.name) != cdn.value.Float)
					return false;
				break;
			case Condition::Type::Bool:
				if (bools.find(cdn.name) == bools.end() || bools.at(cdn.name) != cdn.value.Bool)
					return false;
				break;
			case Condition::Type::Trigger:
				if (triggers.find(cdn.name) == triggers.end())
					return false;
				break;
			}
		}
		return true;
	}

}