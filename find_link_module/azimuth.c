#include <math.h>

#define PHI 3.141592

short bearingG1toG2(double G1_latitude, double G1_longitude, double G2_latitude, double G2_longitude)
{
	// chage to ladian
	double cur_lat_radian = G1_latitude * (PHI / 180);
	double cur_lon_radian = G1_longitude * (PHI / 180);

	double dest_lat_radian = G2_latitude * (PHI / 180);
	double dest_lon_radian = G2_longitude * (PHI / 180);

	// ladian distance
	double radian_distance = 0;
	radian_distance = acos(sin(cur_lat_radian) * sin(dest_lat_radian) + cos(cur_lat_radian) * cos(dest_lat_radian) * cos(cur_lon_radian - dest_lon_radian));
	
	double radian_bearing = acos((sin(dest_lat_radian) - sin(cur_lat_radian) * cos(radian_distance)) / (cos(cur_lat_radian) * sin(radian_distance)));
	
	double true_bearing = 0;
	
	if ( sin(dest_lon_radian - cur_lon_radian) < 0){
		true_bearing = radian_bearing * (180 / PHI);
		true_bearing = 360 - true_bearing;
	} else {
		true_bearing = radian_bearing * (180 / PHI);
	}
	return (short) true_bearing;
}
