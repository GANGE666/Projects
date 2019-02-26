
change_hex = b'\x9F\xE5\x00\x20\xA0\xE3\x02\x10\xA0\xE1\x3F\x10\xC3\xE5'
target_hex = b'\x00\x00\xA0\xE1\x00\x00\xA0\xE1\x00\x00\xA0\xE1\x00\x00\xA0\xE1'

f = open("mylock", 'rb')

content = f.read()

f.close()

content_len = len(content)

counter = 0
mode = 0

f = open("mylock_patch", "wb")

# 一条arm指令4个字节 四条arm指令16字节
for i in range(int(content_len/16)):
    if content[16*i+2: 16*(i+1)] == change_hex:
        counter += 1
        if counter > 3:
            # content[16*i: 16*(i+1)] = target_hex
            print("Patch Program %d" % i*16)
            f.write(target_hex)
        else:
            f.write(content[16*i:16*(i+1)])
    else:
        counter = 0
        f.write(content[16*i:16*(i+1)])

f.write(content[int(content_len/16)*16:])

f.close()














