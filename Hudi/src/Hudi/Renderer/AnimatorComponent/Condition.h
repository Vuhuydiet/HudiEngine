#pragma once

namespace Hudi {

	struct Condition
	{
		Condition(const char* name)
			: name(name) {}
		virtual ~Condition() = default;

		virtual const char* Type() const = 0;
		const char* name;
	};

	struct IntCondition : public Condition
	{
		IntCondition(const char* name)
			: Condition(name), value(0) {}
		IntCondition(const char* name, int value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "int"; }

		int value;
	};

	struct FloatCondition : public Condition
	{
		FloatCondition(const char* name)
			: Condition(name), value(0.0f) {}
		FloatCondition(const char* name, float value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "float"; }

		float value;
	};

	struct BoolCondition : public Condition
	{
		BoolCondition(const char* name)
			: Condition(name), value(false) {}
		BoolCondition(const char* name, bool value)
			: Condition(name), value(value) {}

		const char* Type() const override { return "bool"; }

		bool value;
	};

	struct TriggerCondition : public Condition
	{
		TriggerCondition(const char* name)
			: Condition(name) {}

		const char* Type() const override { return "trigger"; }
	};
}