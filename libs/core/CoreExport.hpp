#pragma once

#ifdef _WIN32
	#ifdef BUILDING_DLL
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#define CORE_API
#endif