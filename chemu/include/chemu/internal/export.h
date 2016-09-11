#ifndef _EXPORT_H
#define _EXPORT_H

#if defined(_MSC_VER)

	#ifdef CHEMU_EXPORTS
		#define CHEMU_API __declspec(dllexport)
	#else
		#define CHEMU_API __declspec(dllimport)
	#endif

#else

	#define CHEMU_API 

#endif




#endif // _EXPORT_H