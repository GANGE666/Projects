
import pymysql

# db_config = {
#     'host': '************',
#     'username': 'root',
#     'password': '****************',
#     'database': 'help'
# }

db_config = {
    'host': '127.0.0.1',
    'username': 'admin',
    'password': '*************',
    'database': 'help'
}


class DB:

    # 初始化数据库连接
    def __init__(self, config=db_config):

        # try:
        self.db = pymysql.connect(config['host'], config['username'], config['password'], config['database'], charset='utf8')
        self.cursor = self.db.cursor()
        try:
            pass
        except Exception as e:
            print(e)
            exit(0)

    def __del__(self):
        # self.db.close()
        pass

    # 插入数据, 接受参数 data = {'pic' = xxx, 'name' = xxx, .....}
    def InsertData(self, data):

        try:

            sql = """INSERT INTO help_record(pic, name, phone, location_t, location_l, sendto)
                        VALUES (%(pic)s, %(name)s, %(phone)s, %(location_t)s, %(location_l)s, %(sendto)s);
                        """
            self.cursor.execute(sql, data)      # 防止sql注入
            self.db.commit()
            return True

        except Exception as e:
            self.db.rollback()
            print(e)

            return e.args[0]

    # 插入数据, 接受参数 data = {'pic' = xxx, 'name' = xxx, .....}
    def InsertData_(self, location_t, location_l, pic='0', name='user', phone='1234567890', sendto='0987654321'):

        data = {}
        data['pic'] = str(pic)
        data['name'] = str(name)
        data['phone'] = str(phone)
        data['location_t'] = str(location_t)
        data['location_l'] = str(location_l)
        data['sendto'] = str(sendto)

        try:

            sql = """INSERT INTO help_record(pic, name, phone, location_t, location_l, sendto)
                        VALUES (%(pic)s, %(name)s, %(phone)s, %(location_t)s, %(location_l)s, %(sendto)s);
                        """
            self.cursor.execute(sql, data)  # 防止sql注入
            self.db.commit()
            return True

        except Exception as e:
            self.db.rollback()
            print(e)

            return e.args[0]

if __name__ == "__main__":
    db = DB()
    print(db)




