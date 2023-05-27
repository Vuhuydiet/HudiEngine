#include <iostream>

#include "Header.h"

extern "C" SCRIPT_API hd_api::ScriptComponent* InstantiateScript()
{
	return new Movement();
}