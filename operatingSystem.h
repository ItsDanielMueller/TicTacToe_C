#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H


#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif



int whichOperatingSystem(const char *path);

#endif

