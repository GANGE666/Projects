# coding=utf-8

import random
import json


class KeyManager:

    def __init__(self):

        self.KEY_FILE_PATH = "keyfile.json"

        self.keys = {"public_key": 11111, "private_key": 11111, "n": 11111}
        self.sm4key = None
        # self.sm4key = 218704240496475236270021651107770293820
        return

    def fetch_local_key(self):
        try:
            with open(self.KEY_FILE_PATH, "r") as f:
                self.keys = json.load(f)
        except:
            print("keyfile not exist or broken")

    def dump_local_key(self):
        with open(self.KEY_FILE_PATH, "w") as f:
            json.dump(self.keys, f)
            print("[*] dump keys succ!")

    def new_rsa_key(self):
        pass

    def new_SM4_key(self, keylen=128):
        self.sm4key = random.randint(0, 1 << keylen)
        return self.sm4key

    def get_rsa_public_key(self):
        return self.keys["public_key"]

    def get_rsa_private_key(self):
        return self.keys["private_key"]

    def get_rsa_n(self):
        return self.keys["n"]

    def get_sm4_key(self):
        if self.sm4key == None:
            self.new_SM4_key()
        print(self.sm4key)
        return self.sm4key


if __name__ == "__main__":
    m = KeyManager()
    m.keys = {"public_key": 690168106656618040363564980207720808173548644742090959344731597233406438873659640773006169470585788386185706975041358242806911670242192040328957861436269,
              "private_key": 6345123150999433290055459257694242694285757852953109410409097639278332519713287523014246876590980410002685259388087970737294629981941700870641726541762841,
              "n": 8191869101213257702214782028182612150271480980815795020417711429874405709482398679692592313772199651759234395055812164164720152614507221054169965684557701}
    m.dump_local_key()
    m.fetch_local_key()
