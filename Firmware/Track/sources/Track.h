#ifndef _LSTIME_H_
#define _LSTIME_H_

#include <time.h>

enum Month_e { January, February, March, April, May, June, July, August, September, October, November, December};

struct tm DateTime(int mday, int month, int year, int hour,  int minute, int seconds);
double Days(int mday, int month, int year, int hour,  int minute, int seconds);
double GreenwichSiderealTime(double aTimestamps);
double LocalSiderealTime(double aTtimestamp, double aLongitude );
double JulienDay(int mday, int month, int year, int hour,  int minute, int seconds);

#endif
