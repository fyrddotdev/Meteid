#ifndef GLOBAL_H
#define GLOBAL_H

#if defined(PLATFORM_ANDROID) || defined(__ANDROID__)
#define ASSETS_PATH ""
#else
#define ASSETS_PATH "assets/"
#endif

#endif