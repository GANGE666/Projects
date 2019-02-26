import hashlib
import struct

inputstr = "123456"
salt = "12345678900987654321"
xor_key = 0xAA

file_size = 17

# 计算压码
ym = 0
for i in inputstr:
    ym += ord(i)

ym &= 0xFF

inputstr += chr(ym)

saltstr = inputstr + salt
# saltstr = salt + inputstr

print(saltstr)

md5ed = hashlib.md5(saltstr.encode(encoding='UTF-8')).hexdigest()

print(md5ed)

# md5int = int(md5ed, base=16)

check_code = 0

file_content = ""
for i in range(int(len(md5ed)/2)):
    file_content += hex(xor_key^int(md5ed[2*i:2*i+2],base=16)).replace("0x", "")
    # 密钥文件压码
    check_code += int(md5ed[2*i:2*i+2],base=16)
    print("%s\t%2x" % (md5ed[2*i:2*i+2], check_code))

# 将压码加入文件中
file_content += hex(xor_key ^ (check_code & 0xFF)).replace("0x", "")

print(file_content)

file_content_int = int(file_content, base=16)

f = open('keyfile_salt_inputstr', 'wb')
# f.write(md5ed.encode(encoding='utf-8'))
f.write(file_content_int.to_bytes(file_size, byteorder='big'))



f.write((0).to_bytes(1, byteorder='big'))
f.close()

