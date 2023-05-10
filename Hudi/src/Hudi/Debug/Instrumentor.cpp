#include "hdpch.h"
#include "Instrumentor.h"

namespace Hudi {

	void Instrumentor::BeginSession(const std::string& name, const std::string& filepath)
	{
		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession)
		{
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.
			if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
			{
				HD_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
			}
			InternalEndSession();
		}
		m_OutputStream.open(filepath);

		if (m_OutputStream.is_open())
		{
			m_CurrentSession = new InstrumentationSession({ name });
			WriteHeader();
		}
		else
		{
			if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
			{
				HD_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
			}
		}
	}

	void Instrumentor::EndSession()
	{
		std::lock_guard lock(m_Mutex);
		InternalEndSession();
	}

	void Instrumentor::WriteProfile(const ProfileResult& result)
	{
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession)
		{
			m_OutputStream << json.str();
			m_OutputStream.flush();
		}
	}

}