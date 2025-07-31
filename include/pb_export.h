#ifndef PB_EXPORT_H
#define PB_EXPORT_H

// PB 模块导出符号定义
// 当前支持的模块：common、detection、perception

// 检查编译器和平台
#ifdef _WIN32
// Windows平台
#ifdef COMMON_EXPORTS
#define COMMON_API __declspec(dllexport)
#else
#define COMMON_API __declspec(dllimport)
#endif

#ifdef DETECTION_EXPORTS
#define DETECTION_API __declspec(dllexport)
#else
#define DETECTION_API __declspec(dllimport)
#endif

#ifdef PERCEPTION_EXPORTS
#define PERCEPTION_API __declspec(dllexport)
#else
#define PERCEPTION_API __declspec(dllimport)
#endif

#define PB_LOCAL

#else
// Linux/Unix平台
#if __GNUC__ >= 4
#ifdef COMMON_EXPORTS
#define COMMON_API __attribute__((visibility("default")))
#else
#define COMMON_API __attribute__((visibility("default")))
#endif

#ifdef DETECTION_EXPORTS
#define DETECTION_API __attribute__((visibility("default")))
#else
#define DETECTION_API __attribute__((visibility("default")))
#endif

#ifdef PERCEPTION_EXPORTS
#define PERCEPTION_API __attribute__((visibility("default")))
#else
#define PERCEPTION_API __attribute__((visibility("default")))
#endif

#define PB_LOCAL __attribute__((visibility("hidden")))
#else
#define COMMON_API
#define DETECTION_API
#define PERCEPTION_API
#define PB_LOCAL
#endif
#endif

// 通用宏，用于简化不同模块的使用
#define DECLARE_PB_EXPORT(MODULE) MODULE##_API

#endif // PB_EXPORT_H
