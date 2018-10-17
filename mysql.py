import pymysql



con = pymysql.connect(host='localhost', user='root', password='1', db='drive2', charset='utf8')

curs = con.cursor()

curs.execute(sql)
rows = curs.fetchall()
con.commit()
con.close()
