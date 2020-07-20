import database
import json
import requests
import time
from twilio.rest import Client

SENDTO = "+86********"

# 调用API获取文字的位置
def genarate_location_info(loc_str):
    print(loc_str)
    ak = "*******************************"
    url = "http://api.map.baidu.com/reverse_geocoding/v3/?ak="+ak+"&output=json&coordtype=wgs84ll&location=" + loc_str
    # print(url)
    res = requests.get(url)
    # print(res.text)
    return res.json()

# 发送短信 https://www.twilio.com/console
def send_message(res_json):

    print(res_json)

    current_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    current_loc = res_json['result']['addressComponent']['country'] + res_json['result']['formatted_address'] + res_json['result']['business'] + res_json['result']['addressComponent']['street']
    jindu = res_json['result']['location']['lat']
    weidu = res_json['result']['location']['lng']

    message = "Help me in %s (%f,%f)" % (current_loc,jindu, weidu)
    #message = "[Warning] Please help me! I'm in Latitude: %f ,Longitude: %f " % (jindu, weidu)
    # message = "[Warning]"
    print(message)

    account_sid = "***************************"
    auth_token = "**************************"
    # client = Client(account_sid, auth_token)
    # message_ = client.messages.create(
    #     to=SENDTO,
    #     from_="+**********",
    #     body=message)
    # print(message_.sid)
    # print(message_.body)

    return current_loc


class SendMessage:
    def __init__(self):
        self.db = database.DB()

    # 处理从手机上返回的数据
    def handle_recv(self, data):
        data = data.decode().split("..")[0]

        try:
            data_dict = json.loads(data)
        except Exception:
            print("json parse error")
            print(data)
            return

        location_str = data_dict['location_l'].split('(')[1].split(')')[0]
        map_res_json = genarate_location_info(str(location_str))
        location_t = send_message(map_res_json)

        data_dict['location_t'] = location_t
        data_dict['sendto'] = SENDTO
        data_dict['pic'] = '1'

        print(self.db.InsertData(data_dict))








