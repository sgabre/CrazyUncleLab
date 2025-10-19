#ifndef _COORDINATES_SYSTEM_H_
#define _COORDINATES_SYSTEM_H_


typedef struct GeographicCoordonites_ts
{
	double Latitude;
	double Longitude;
	double Altitude;
}GeographicCoordonites_ts;

typedef struct CelestialCoordonites_ts
{
	double Ascension;//right ascension,
	double Declination;
}CelestialCoordonites_ts;

typedef struct HorizontalCoordonites_ts
{
	double Azimuth;		
	double Elevation;	
}HorizontalCoordonites_ts;

#endif
