#放弃c++
import time
import hashlib
import random
class SHA1:
    def __init__(self):
        # SHA1的pyhton实现
        pass

class RSA:
    def __init__(self,p,q,prikey):
        self.p = p
        self.q = q
        self.n = p * q
        self.phin = (p-1)*(q-1)
        print("Phin:",self.phin)
        self.prikey = prikey
        print("prikey:",self.prikey)
        self.pubkey = self.inverse(self.prikey,self.phin)
        print("pubkey:",self.pubkey)

    def inverse(self,a,m):
        # 扩展欧几里得算法求逆
        u1,u2,u3 = 1,0,a
        v1,v2,v3 = 0,1,m
        while v3!=0:
            q = u3//v3
            v1,v2,v3,u1,u2,u3 = (u1-q*v1),(u2-q*v2),(u3-q*v3),v1,v2,v3
        return u1%m

    def hash(self,input):
        # 对SHA1函数的扩展
        # 输入的是1024bit的字符串
        s = hashlib.sha1()
        re = b""
        s.update(input)
        round = s.digest()
        re = round + re
        for i in range(5):
            s.update(round)
            round = s.digest()
            re = round + re
        for i in range(64):
            re = b"\x00" + re
        return re
    def ra(self):
        # 生成一个1024bit的随机数
        re = bytearray(128)
        for i in range(64):
            re[127-i] ^= random.randint(0,255)
        return re
    def encodeOAEP(self,plaintext):
        # 输入明文的字符串，返回密文的比特串
        P1 = bytearray(128)
        P2 = bytearray(128)
        re = bytearray(256)
        M = b""
        for i in plaintext:
            M = M + bytes(i,encoding="utf-8")
        l = 128-len(plaintext)
        for i in range(0,l):
            M = b"\x00" + M
        r = self.ra()
        print("encode(r):",r)
        hr = self.hash(r)
        for i in range(128):
            P1[i] = M[i] ^ hr[i]
            re[i+128] = P1[i]
        hp1  = self.hash(P1)
        for i in range(128):
            P2[i] = r[i] ^ hp1[i]
            re[i] = P2[i]
        # re[0] = re[0] ^ int(b"\x01".hex())
        return re
    def decodeOAEP(self,ciphertext):
        # 输入密文的额比特串，返回明文的比特串
        P1 = bytearray(128)
        P2 = bytearray(128)
        for i in range(128):
            P1[i] = ciphertext[i+128]
        for i in range(128):
            P2[i] = ciphertext[i]
        hp1 = self.hash(P1)
        r = bytearray(128)
        for i in range(128):
            r[i] = hp1[i] ^ P2[i]
        print("decode(r):",r)
        hr = self.hash(r)
        M = bytearray(128)
        for i in range(128):
            M[i] = P1[i] ^ hr[i]
        return M
    def i2b(self,t):
        b = ""
        num = [1,2,4,8,16,32,64,128]
        for i in range(8):
            if t >= num[7-i]:
                t -= num[7-i]
                b = b+"1"
            else:
                b = b+"0"
        return b
    def quick_pow(self,a,m):
        re = 1
        a = a %self.n
        while(m!=0):
            if m&1:
                re = (re * a) % self.n
            a = (a * a) % self.n
            m >>= 1
        return re
    def RSAencode(self,encode_plain):
        # 输入的是一个256位的bytearray
        # 将输入的bytearray转化为二进制字符串
        binary_str = ""
        for i in range(256):
            binary_str = binary_str + self.i2b(encode_plain[i])
        print("binary(encode)",binary_str)
        p = 0
        num = 1
        for i in range(2048):
            if binary_str[2047-i] == "1":
                p += num
            num *= 2
        # 公钥加密,返回一个数
        print("p:",p%self.n)
        encode = self.quick_pow(p,self.pubkey)
        print("encode:",encode)
        return encode
    def RSAdecode(self,encode_cipher):
        # 私钥解密，返回一个256的bytearray
        print("encode_cipher:",encode_cipher)
        plain = self.quick_pow(encode_cipher,self.prikey)
        print("plain",plain)
        r = 1
        arr = []
        for i in range(2048):
            arr.append(r)
            r *= 2
        # 把数字处理成为二进制字符串
        plain_bin = ""
        for i in range(2048):
            if plain>=arr[2047-i]:
                plain -= arr[2047-i]
                plain_bin = plain_bin + "1"
            else:
                plain_bin = plain_bin + "0"
        print("plain_bin:",plain_bin)
        # 把二进制字符串转化为bytearray
        plain_byte = bytearray(256)
        for i in range(256):
            k = 0
            tb = plain_bin[i * 8:(i + 1) * 8]
            for j in range(8):
                if tb[7-j] == "1":
                    k += 2**j
            plain_byte[i] = k
        print("plain_byte:",plain_byte[-1])
        return plain_byte

if __name__ == '__main__':
    s = hashlib.sha1()
    s.update(b"\x61"+b"\x61")
    m = s.digest()
    print()
    p = 74829638746533240988779487685519857207853614853369269125737224926836190844584386477252351678694220847326413610447912126732461551667746605631104998257474529620520790296706975377558703885069067389851079532628142311782993846028093717798601985881211947182951877707743021688050533047297428267547207193214872407179
    q = 107296245469105708553885694146097228957924134383498411644481264273897635331338716573880625961872125593436197056329996041839918538287799741950000667523364412951312104942279975769261920125078391971701830788146273511314474716785909534480336382568039624262497906015374016000442339021334156275769032915698038588739
    prikey = 17340031
    plain = "Sun Yat-sen University"
    r = RSA(p,q,prikey)
    test = r.encodeOAEP(plain)
    print("OAEP(encode):",test)
    en = r.RSAencode(test)
    de = r.RSAdecode(en)
    final = r.decodeOAEP(de)
    print(final)
    print("test power")
    tt = 32317006071311007300714876688669951960444102669715484032130345427524655138867890893197201411522913463688717960921898019494119559150490921095088152386448283120630877367300996091750197750389652106796057638384067568276792218642619756161838094338476170470581645852036305042887575891541065808607552399123930385521914333389668342420684974786564569494856176035326322058077805659331026192708460314150258592864177116725943603718461857357598351152301645904403697613233287231227125684710820209725157101726931323469678542580656697935045997268352998638215525166389437335543602135433229604645318478604952148193555853611059596230656
    print(tt)
    ttt = r.quick_pow(tt,r.pubkey)
    print(ttt)
    print(r.quick_pow(ttt,r.prikey))
