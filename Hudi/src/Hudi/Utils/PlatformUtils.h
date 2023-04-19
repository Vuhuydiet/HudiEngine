#pragma once

#include <string>

namespace Hudi {

	namespace FileDialogs {

		std::string OpenFile(const char* filter);
		std::string SaveFile(const char* filter);

	}

}