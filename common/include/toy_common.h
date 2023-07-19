/**
 * @file toy_common.h
 * @brief
 *
 */

#ifndef TOY_COMMON_H_
#define TOY_COMMON_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TOY_VERSION_MAJOR 1 /**< TOY_VERSION_MAJOR */
#define TOY_VERSION_MINOR 1 /**< TOY_VERSION_MINOR */
#define TOY_VERSION_PATCH 6 /**< TOY_VERSION_PATCH */

//platform/compiler-specific instructions
#if defined(__linux__) || defined(__MINGW32__) || defined(__GNUC__)

/**
 * @def TOY_API
 * @brief @@???@@
 *
 */
#define TOY_API extern

#elif defined(_MSC_VER)

#ifndef TOY_EXPORT
#define TOY_API __declspec(dllimport)
#else
#define TOY_API __declspec(dllexport)
#endif

#else

#define TOY_API extern

#endif
#endif /* TOY_COMMON_H_ */

