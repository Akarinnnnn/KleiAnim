#pragma once
#ifdef _WIN32
#if KLEIANIM_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif // KLEIANIM_EXPORTS
#else
#define EXPORT_API
#endif