#include <stdio.h>
#include </usr/include/mysql/mysql.h>
#include <string.h>
#include <stdlib.h>
#include "azimuth.h"

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1"
#define DB_NAME "drive2"
#define CHOP(x) x[strlen(x) - 1] = ""

typedef struct _data{
    MYSQL_RES * gps;
    int num;
} data;

typedef struct _now_location{
	char linkid[12];
	char curb[32786];
} now_location;

void finish_with_error(MYSQL *conn);
now_location gps(float lat, float lon, int hd);
data mysql_select(MYSQL *, char query[]);

void finish_with_error(MYSQL *conn)
{
    printf("%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
}

now_location gps(float lat, float lon, int hd)
{
	// SELECT id , (6371*acos(cos(radians(35.838223))*cos(radians(gps.lat))*cos(radians(gps.lon) - radians(128.682269)) + sin(radians(35.838223))*sin(radians(gps.lat)))) AS distance , lat, lon FROM gps group by id order by distance limit 10;
    char query[1024];
	char query2[1024] = "select hdufid,fromnode, tonode from a3_link where hdufid = ";
	char query3[1024] = "select * from gps where ono = ";
	char query4[1024] = "select linkid from a3_link where tonode = ";
	char query5[1024];
	int tri[4];
	double latlon[4];
	char *tonode[4];
	char *curb;
    sprintf(query, "SELECT id , MIN(6371*acos(cos(radians(%0.7f))*cos(radians(gps.lat))*cos(radians(gps.lon) - radians(%0.7f)) + sin(radians(%0.7f))*sin(radians(gps.lat)))) AS distance , lat, lon FROM gps where id like 'A3%%' group by id order by distance limit 4;", lat, lon, lat);
	sprintf(query5, "select * from gps where id = (select id from gps where id like 'A1%%' and barrier = '04' order by (6371*acos(cos(radians(%0.7f))*cos(radians(gps.lat))*cos(radians(gps.lon) - radians(%0.7f)) + sin(radians(%0.7f))*sin(radians(gps.lat)))) limit 1);", lat, lon, lat);
    MYSQL *conn = mysql_init(NULL);

    if( conn == NULL)
    {
        printf("%s\n", mysql_error(conn));
        exit(1);
    }

    if( mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
    {
        finish_with_error(conn);
    }
    
    MYSQL_ROW row;
	MYSQL_ROW row1;
	data d = mysql_select(conn,query);
	
	for(int i = 0; i < 4; i++){
		row = mysql_fetch_row(d.gps);
		if(i == 3){
			sprintf(query2, "%s'%s'", query2, row[0]);		
		} else {
			sprintf(query2, "%s'%s' or hdufid = ", query2, row[0]);
		}
	}

	d = mysql_select(conn, query2);
	int a = 0;
	while(row = mysql_fetch_row(d.gps))
	{
		if(a == 3){
			sprintf(query3, "%s'%s' or ono = ", query3, row[1]);
			sprintf(query3, "%s'%s'", query3, row[2]);
		} else {
			sprintf(query3, "%s'%s' or ono = ", query3, row[1]);
			sprintf(query3, "%s'%s' or ono = ", query3, row[2]);
		}

//		for(int i = 0; i < d.num; i++)
//       {
//			printf("%s  ", row[i] ? row[i] : "NULL");
//        }
//       printf("\n");
		a++;
    }
//	printf("%s\n", query3);
	d = mysql_select(conn, query3);
	
	int j = 0;
	int tri_index = 0;

	while(row = mysql_fetch_row(d.gps))
	{
		int l;
//		for(int i = 0; i < d.num; i++)
//		{
//			printf("%s  ", row[i] ? row[i] : "NULL");
//		}
//		printf("\n");
    	if( j%2 == 0){
			latlon[0] = atof(row[2]);
			latlon[1] = atof(row[3]);
		} else{
			latlon[2] = atof(row[2]);
			latlon[3] = atof(row[3]);
			tri[tri_index] = bearingG1toG2(latlon[0], latlon[1], latlon[2],latlon[3]);
//			printf("%d\n", tri[tri_index]);
			tri_index++;
			l = strlen(row[5]);
			if ( l > 0 ){
				char* newstrptr = (char*)malloc(sizeof(char) * (l + 1));
				strcpy(newstrptr, row[5]);
				tonode[j/2] = newstrptr;
			}
		}
		j++;
	}


	int hd_cnt = 0;
	for(int i = 0; i < 4; i++){
		if ( hd > tri[i] - 20 || hd < tri[i] + 20){
			sprintf(query4, "%s'%s'", query4, tonode[i]);
			i = 5;
		} 
	}

	d = mysql_select(conn, query4); 
	row1 = mysql_fetch_row(d.gps);

//	printf("%s\n", row1[0]);

	d = mysql_select(conn, query5);	

	now_location wl;
	sprintf(wl.linkid, "%s",row1[0]);
	while(row = mysql_fetch_row(d.gps)){
		sprintf(wl.curb, "%s%s%s", wl.curb, row[2], row[3]);
	}

    mysql_close(conn);
    return wl;
}

data mysql_select(MYSQL *conn, char query[]){
	if( mysql_query(conn, query) )
    {
        finish_with_error(conn);
    }

    // mysql_store_result() 함수를 사용해서 결과셋을 가져옴 
    MYSQL_RES *result = mysql_store_result(conn);
	MYSQL_ROW row;
    if( result == NULL)
    {
        finish_with_error(conn);
    }
	int num_fields = mysql_num_fields(result);
    data d = {result, num_fields};

	return d;
}
