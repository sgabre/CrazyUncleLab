#include <stdio.h>

#include "LSTime.h"

#include <math.h>

static int INT(double value)
{
	return (int)value;
}

struct tm DateTime(int mday, int month, int year, int hour,  int minute, int seconds)
{
	struct tm aDate = {0};
	
	aDate.tm_mday = mday;
	aDate.tm_mon = month;  
	aDate.tm_year = (year - 1900);  
	aDate.tm_hour = hour;
	aDate.tm_min = minute;
	aDate.tm_sec = seconds; 
	
	return aDate;
}

double Days(int mday, int month, int year, int hour,  int minute, int seconds)
{
	struct tm  aDate = DateTime(mday, month, year, hour, minute, seconds);
	return mktime( &aDate );
}

double GreenwichSiderealTime(double aTimestamps)
{
	struct tm _1JANUARY1968_DATE_TIME = DateTime(1, January, 1968, 0, 0, 0);
	time_t _1JANUARY1968 = mktime( &_1JANUARY1968_DATE_TIME );
	
	double ElapsedInSeconds = difftime(aTimestamps, _1JANUARY1968);
	double ElapsedInDay = ElapsedInSeconds/(24*60*60);
	
	double aGreenwichSiderealTimeAt1January1968InRad = 1.74046342;
	
	double aGreenwichSiderealTime = aGreenwichSiderealTimeAt1January1968InRad + 1.0027379093 * 2 * M_PI * ElapsedInDay;
	
	double Result =  fmod(aGreenwichSiderealTime, (2 * M_PI) );
	
	return Result;
}

double LocalSiderealTime(double aTtimestamp, double aLongitude )
{
	double Result =  GreenwichSiderealTime(aTtimestamp) + aLongitude;
	//double Result =  fmod(GreenwichSiderealTime(aTtimestamp) + aLongitude, (2 * M_PI) );
	
	return Result;
}


double JulienDay(int mday, int month, int year, int hour,  int minute, int seconds)
{
	int Month = month;
	int Year = year;
	
	if(Month < March)
		{
			Month = Month + 12;
			Year = Year - 1;
		}
	
	double A = INT( Year / 100 );
	double B = 2 - A + INT( A / 4 );
	double C = INT( 365.25 * Year );
	double D = INT( 30.6001 * ( Month + 1 ) );
	
	return  B + C + D + mday + 1720994.5;
	
	
	return 0.0;
}