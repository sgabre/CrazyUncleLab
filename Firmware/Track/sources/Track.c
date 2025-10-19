#include <stdio.h>

#include "Coordinates.h"

#include "Track.h"

#include <math.h>

static int INT(double value)
{
	return (int)value;
}

void Track(HorizontalCoordonites_ts* aHorizontal, double aLocalSiderealTime, GeographicCoordonites_ts* aSpot, CelestialCoordonites_ts* aCelestial )
{
	//Geographic_ts* aSpot,
	double aLocalHourAngle = aLocalSiderealTime - aCelestial->Ascension;
	
	aHorizontal->Elevation = asin(sin(aCelestial->Ascension) * sin(aSpot->Longitude) - cos(aCelestial->Ascension) * cos(aSpot->Longitude) * cos(aLocalHourAngle));
	
	double alpha =  asin( (cos(aCelestial->Declination) *sin(aLocalHourAngle) ) /cos(aHorizontal->Elevation) );
	
	
	aHorizontal->Azimuth = acos( (sin(aCelestial->Declination) - sin(aSpot->Longitude)*sin(aHorizontal->Elevation))/cos(aSpot->Longitude) *cos(aHorizontal->Elevation));
	
	if(alpha > 0)
	{
		aHorizontal->Azimuth = -aHorizontal->Azimuth;
	}
}
/*
GeoCoordinates_ts*  Track (Geographic_ts* aSpot, RadarData_ts* aData, double aLocalSideralTime)
{
	GeoCoordinates_ts Return;
	//Topocentric-Horizon Position
	TopoVector_ts aTopoPositionVector = {0};
	aTopoPositionVector.S = -aData->Range * cos(aData->Elevation) * cos(aData->Azimuth);
	aTopoPositionVector.E =  aData->Range * cos(aData->Elevation) * sin(aData->Azimuth);
	aTopoPositionVector.Z =  aData->Range * sin(aData->Elevation);
	
	//Topocentric-Horizon Velocity
	TopoVector_ts aTopoVelocityVector = {0};
	aTopoVelocityVector.S = -aData->Range * cos(aData->Elevation) * cos(aData->Azimuth);
	aTopoVelocityVector.E =  aData->Range * cos(aData->Elevation) * sin(aData->Azimuth);
	aTopoVelocityVector.Z =  aData->RangeRate * sin(aData->Elevation) + aData->Range *  cos(aData->Elevation) * aData-> ElevationRate;
	
	//Transformation Matrix
	double D11 = sin(aSpot->Longitude)  * cos(aLocalSideralTime);
	double D12 = -sin(aLocalSideralTime);
	double D13 = cos(aSpot->Longitude) * cos(aLocalSideralTime);
	
	double D21 = sin(aSpot->Longitude)  * sin(aLocalSideralTime);
	double D22 = cos(aLocalSideralTime);
	double D23 = cos(aSpot->Longitude)  * sin(aLocalSideralTime);
	
	double D31 = -cos(aSpot->Longitude); 
	double D32 = 0;
	double D33 = sin(aSpot->Longitude);
	
	//Geocentric Position Conversion
	Return.Position.I = D11 * aTopoPositionVector.S + D12 * aTopoPositionVector.E +  D13 * aTopoPositionVector.Z;
	Return.Position.J = D22 * aTopoPositionVector.S + D22 * aTopoPositionVector.E +  D23 * aTopoPositionVector.Z;
	Return.Position.K = D31 * aTopoPositionVector.S + D32 * aTopoPositionVector.E +  D33* aTopoPositionVector.Z;
	
	//Geocentric Velocity Conversion
	Return.Velocity.I = D11 * aTopoVelocityVector.S + D12 * aTopoVelocityVector.E +  D13 * aTopoVelocityVector.Z;
	Return.Velocity.J = D22 * aTopoVelocityVector.S + D22 * aTopoVelocityVector.E +  D23 * aTopoVelocityVector.Z;
	Return.Velocity.K = D31 * aTopoVelocityVector.S + D32 * aTopoVelocityVector.E +  D33* aTopoVelocityVector.Z;
	
	
	return &Return;
}
*/

/*
GeoVector_ts Site(double aLatitude, double aAltitude, double aLocalSiderealTime)
{
GeoVector_ts Return;
//X-Parameter
double H = 10;
double X = fabs( ((BODY_E_RADIUS)/( sqrt( 1 - pow(BODY_ECCENTRICITY,2) * sin(aLatitude)* sin(aLatitude) ) )) + aAltitude ) * cos(aLatitude);
double Z = fabs( ((BODY_E_RADIUS)/( sqrt( 1 - pow(BODY_ECCENTRICITY,2) * sin(aLatitude)* sin(aLatitude) ) )) + aAltitude ) * sin(aLatitude);

Return.I = X * cos(aLocalSiderealTime);
Return.J = X * sin(aLocalSiderealTime);
Return.K = Z;

return Return;
}
*/