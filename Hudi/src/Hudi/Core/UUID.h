#pragma once

#include <xhash>

namespace Hudi {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);

		operator uint64_t() const { return m_UUID; }
		bool operator< (const UUID& other) const { return this->m_UUID < other.m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template <>
	struct hash<Hudi::UUID>
	{
		std::size_t operator()(const Hudi::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}