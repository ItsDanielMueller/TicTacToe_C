#ifndef LIBRARYS_H
#define LIBRARYS_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>  // für mkdir unter Linux/macOS
#endif

#endif

