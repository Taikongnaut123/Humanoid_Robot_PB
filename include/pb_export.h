#ifndef PB_EXPORT_H
#define PB_EXPORT_H

// 通用的protobuf模块导出符号定义
// 支持多个模块：perception、calculate、control、imageprocessing等

// 检查编译器和平台
#ifdef _WIN32
// Windows平台
#ifdef PERCEPTION_PB_EXPORTS
#define PERCEPTION_PB_API __declspec(dllexport)
#else
#define PERCEPTION_PB_API __declspec(dllimport)
#endif

#ifdef IMAGEPROCESSING_PB_EXPORTS
#define IMAGEPROCESSING_PB_API __declspec(dllexport)
#else
#define IMAGEPROCESSING_PB_API __declspec(dllimport)
#endif

#ifdef DETECTION_EXPORTS
#define DETECTION_API __declspec(dllexport)
#else
#define DETECTION_API __declspec(dllimport)
#endif

#ifdef IMAGEPROCESSING_EXPORTS
#define IMAGEPROCESSING_API __declspec(dllexport)
#else
#define IMAGEPROCESSING_API __declspec(dllimport)
#endif

#ifdef CALCULATE_PB_EXPORTS
#define CALCULATE_PB_API __declspec(dllexport)
#else
#define CALCULATE_PB_API __declspec(dllimport)
#endif

#define PB_LOCAL

#else
// Linux/Unix平台and other compilers
#if __GNUC__ >= 4
#ifdef PERCEPTION_PB_EXPORTS
#define PERCEPTION_PB_API __attribute__((visibility("default")))
#else
#define PERCEPTION_PB_API __attribute__((visibility("default")))
#endif

#ifdef IMAGEPROCESSING_PB_EXPORTS
#define IMAGEPROCESSING_PB_API __attribute__((visibility("default")))
#else
#define IMAGEPROCESSING_PB_API __attribute__((visibility("default")))
#endif

#ifdef DETECTION_EXPORTS
#define DETECTION_API __attribute__((visibility("default")))
#else
#define DETECTION_API __attribute__((visibility("default")))
#endif

#ifdef IMAGEPROCESSING_EXPORTS
#define IMAGEPROCESSING_API __attribute__((visibility("default")))
#else
#define IMAGEPROCESSING_API __attribute__((visibility("default")))
#endif

#ifdef CALCULATE_PB_EXPORTS
#define CALCULATE_PB_API __attribute__((visibility("default")))
#else
#define CALCULATE_PB_API __attribute__((visibility("default")))
#endif

#define PB_LOCAL __attribute__((visibility("hidden")))
#else
#define PERCEPTION_PB_API
#define IMAGEPROCESSING_PB_API
#define DETECTION_API
#define IMAGEPROCESSING_API
#define CALCULATE_PB_API
#define PB_LOCAL
#endif
#endif

// C接口导出宏
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
#ifdef PERCEPTION_PB_EXPORTS
#define PERCEPTION_PB_C_API __declspec(dllexport)
#else
#define PERCEPTION_PB_C_API __declspec(dllimport)
#endif

#ifdef IMAGEPROCESSING_PB_EXPORTS
#define IMAGEPROCESSING_PB_C_API __declspec(dllexport)
#else
#define IMAGEPROCESSING_PB_C_API __declspec(dllimport)
#endif

#ifdef CALCULATE_PB_EXPORTS
#define CALCULATE_PB_C_API __declspec(dllexport)
#else
#define CALCULATE_PB_C_API __declspec(dllimport)
#endif
#else
#define PERCEPTION_PB_C_API __attribute__((visibility("default")))
#define IMAGEPROCESSING_PB_C_API __attribute__((visibility("default")))
#define CALCULATE_PB_C_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
}
#endif

// 通用宏，用于简化不同模块的使用
#define DECLARE_PB_EXPORT(MODULE) MODULE##_PB_API

#endif // PB_EXPORT_H
