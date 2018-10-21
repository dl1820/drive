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
    MYSQL_RES * real_gps;
    int num;
} data;

void finish_with_error(MYSQL *conn);
data real_gps(char nodes[]);
data mysql_select(MYSQL *, char query[]);

int main(void)
{
	data d = real_gps("155M10640204 155M10720204");
	MYSQL_ROW row;
	return 0;	
}

void finish_with_error(MYSQL *conn)
{
    printf("%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
}

data real_gps(char nodes[])
{
	char query[1024] = "select linkid from a3_link where tonode = ";

    MYSQL *conn = mysql_init(NULL);

	char *ptr = strtok(nodes, " ");
	while(ptr != NULL){
		printf("%s\n", ptr);
		ptr = strtok(NULL, " ");
	}

    if( conn == NULL)
    {
        printf("%s\n", mysql_error(conn));
        exit(1);
    }

    if( mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
    {
        finish_with_error(conn);
    }
    /***
	sprintf(query, "%s'%s'", query, )

    MYSQL_ROW row;
	data d = mysql_select(conn,query);
	
	while(row = mysql_fetch_row(d.real_gps))
	{
		if(a == 3){
			sprintf(query3, "%s'%s' or ono = ", query3, row[1]);
			sprintf(query3, "%s'%s'", query3, row[2]);
		} else {
			sprintf(query3, "%s'%s' or ono = ", query3, row[1]);
			sprintf(query3, "%s'%s' or ono = ", query3, row[2]);
		}
		a++;
    }
	***/
    mysql_close(conn);
    return d;
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
