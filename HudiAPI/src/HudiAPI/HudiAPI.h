#pragma once

#ifdef HD_API_EXPORTS
#define HD_API __declspec(dllexport)
#else
#define HD_API __declspec(dllimport)
#endif

