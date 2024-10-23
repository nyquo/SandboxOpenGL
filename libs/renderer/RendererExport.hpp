#pragma once

//#ifndef RENDERER_API
//	#ifdef RENDERER_API_EXPORT
//		#ifdef _WIN32
//			#define RENDERER_API __declspec(dllexport)
//		#else
//			#define RENDERER_API
//		#endif
//	#else
//		#ifdef _WIN32
//			#define RENDERER_API __declspec(dllimport)
//		#else
//			#define RENDERER_API
//		#endif
//	#endif
//#endif

#ifdef _WIN32
#define RENDERER_API
/*#ifdef RENDERER_API_EXPORT
        #define RENDERER_API __declspec(dllexport)
#else
        #define RENDERER_API __declspec(dllimport)
#endif*/
#else
#define RENDERER_API
#endif
