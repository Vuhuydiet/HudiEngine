#include "pch.h"

#include "Interface.h"

namespace hd_api {

	HD_API Behaviour* Instantiate() {
		return new Behaviour();
	}

}