import pymysql
from math import *

con = pymysql.connect(host='localhost', user='root', password='1', db='drive2', charset='utf8')

curs = con.cursor()
query = "select fromnode, tonode, linkid from a3_link"

curs.execute(query)
rows = curs.fetchall()

def heading(G1_latitude, G1_longitude, G2_latitude, G2_longitude):
	PHI = 3.141582
	cur_lat_radian = float(G1_latitude) * PHI / 180
	cur_lon_radian = float(G1_longitude) * PHI / 180
	dest_lat_radian = float(G2_latitude) * PHI / 180
	dest_lon_radian = float(G2_longitude) * PHI / 180
	radian_distance = acos(sin(cur_lat_radian) * sin(dest_lat_radian) + cos(cur_lat_radian) * cos(dest_lat_radian)* cos(cur_lon_radian - dest_lon_radian))
	if ((cos(cur_lat_radian) * sin(radian_distance)) > 0):
		radian_bearing = acos((sin(dest_lat_radian) - sin(cur_lat_radian) * cos(radian_distance)) / (cos(cur_lat_radian) * sin(radian_distance)))
		if (sin(dest_lon_radian - cur_lon_radian) < 0):
			true_bearing = 360 - radian_bearing * (180/PHI)
		else :
			true_bearing = radian_bearing * (180 / PHI)
	else :
		true_bearing  = 9999
	return true_bearing

for row in rows:
	linkid = row[2]
	fromto = "select lat, lon, ono, id from gps where ono = '%s' or ono = '%s'"%(row[0], row[1])
	curs.execute(fromto)
	fromto_rows = curs.fetchall()
	fromnode = fromto_rows[0]
	for i in range(1,len(fromto_rows)):
		insertHeading = "UPDATE gps SET heading = %d where ono = '%s'"%(int(heading(float(fromnode[0]),float(fromnode[1]),fromto_rows[i][0],fromto_rows[i    ][1])), linkid)
		curs.execute(insertHeading)
#		print("heading : %f id : %s"%(heading(float(fromnode[0]),float(fromnode[1]),fromto_rows[i][0],fromto_rows[i][1]), linkid))
con.commit()
con.close()
