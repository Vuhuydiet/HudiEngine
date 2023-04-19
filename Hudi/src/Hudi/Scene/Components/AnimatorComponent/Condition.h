#pragma once

namespace Hudi {

	struct Condition
	{
		Condition(const std::string& name)
			: name(name) {}
		virtual ~Condition() = default;

		virtual const char* Type() const = 0;
	public:
		std::string name;
	};

	struct IntCondition : public Condition
	{
		IntCondition(const std::string& name)
			: Condition(name), value(0) {}
		IntCondition(const std::string& name, int value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "int"; }

		int value;
	};

	struct FloatCondition : public Condition
	{
		FloatCondition(const std::string& name)
			: Condition(name), value(0.0f) {}
		FloatCondition(const std::string& name, float value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "float"; }

		float value;
	};

	struct BoolCondition : public Condition
	{
		BoolCondition(const std::string& name)
			: Condition(name), value(false) {}
		BoolCondition(const std::string& name, bool value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "bool"; }

		bool value;
	};

	struct TriggerCondition : public Condition
	{
		TriggerCondition(const std::string& name)
			: Condition(name) {}

		const char* Type() const override { return "trigger"; }
	};
}