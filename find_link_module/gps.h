#include </usr/include/mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1"
#define DB_NAME "drive"
#define CHOP(x) x[strlen(x) - 1] = ""

//void connect_DB();
//void select_DB();
void finish_with_error(MYSQL *conn);

//MYSQL *connection=NULL, conn;
//MYSQL_RES *sql_result;
//MYSQL_ROW sql_row;
//int query_stat;

typedef struct _data{
	MYSQL_RES * gps;
	int num;
} data;

data gps(float lat, float lon)
{	
	char query[1024];
 	char q1[] = "select * from gps where id = (select id from gps where id like 'A3%' order by (SQRT(POW(lon - ";
	char q2[] = ",2) + POW(lat -";
	char q3[] = ",2))) limit 1);";

	sprintf(query, "%s%0.10f%s%0.10f%s", q1, lat, q2, lon, q3);	
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
	
	if( mysql_query(conn, query) )
	{
		finish_with_error(conn);
	}

	// mysql_store_result() 함수를 사용해서 결과셋을 가져옴	
	MYSQL_RES *result = mysql_store_result(conn);	

	if( result == NULL)
	{
		finish_with_error(conn);
	}

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;

//	while(row = mysql_fetch_row(result))
//	{
//		for(int i = 0; i < num_fields; i++)
//		{
//			printf("%s	", row[i] ? row[i] : "NULL");
//		}
//		printf("\n");
//	}

	data d = {result, num_fields};

	mysql_free_result(result);
	mysql_close(conn);
	return d;
}

void finish_with_error(MYSQL *conn)
{
	printf("%s\n", mysql_error(conn));
	mysql_close(conn);
	exit(1);
}

