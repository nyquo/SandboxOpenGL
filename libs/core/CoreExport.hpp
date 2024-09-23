#pragma once

//#ifndef CORE_API
//	#ifdef CORE_API_EXPORT
//		#ifdef _WIN32
//			#define CORE_API __declspec(dllexport)
//		#else
//			#define CORE_API
//		#endif
//	#else
//		#ifdef _WIN32
//			#define CORE_API __declspec(dllimport)
//		#else
//			#define CORE_API
//		#endif
//	#endif
//#endif

#ifdef _WIN32
	#define CORE_API
	/*#ifdef CORE_API_EXPORT
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif*/
#else
	#define CORE_API
#endif