import time
import hashlib
import random

def gcd(a,b):
    if a < b:
        return gcd(b,a)
    elif a%b == 0:
        return b
    else :
        return gcd(b,a%b)

def inverse(a, m):
    # 扩展欧几里得算法求逆
    while gcd(a, m) != 1:
        a, m = a / math.gcd(a, m), m / math.gcd(a, m)
    u1, u2, u3 = 1, 0, a
    v1, v2, v3 = 0, 1, m
    while v3 != 0:
        q = u3 // v3
        v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3
    return u1 % m

def quick_pow(a, m, p):
    re = 1
    a = a % p
    while (m != 0):
        if m & 1:
            re = (re * a) % p
        a = (a * a) % p
        m >>= 1
    return re

def dsa_sig(x):
    p = 0x008746338ba0d0b67ec6dde878f14d624dfe86a4663aaf170208b02b020c09199af1dbaaf0ba5a70d52b3eb715f775b0989ff176d1d6c680042dab48d35d802c598036280c559ffb59ea4c82c01fae4a227847cb715e03511602f7dedd0d0f1556c5e63c9b181a4a7ffb1416a3d6ef69cb9b413746aab5e259c3b12bec3244a7e09f6718ecb721c528020a9ae09568f461512c526593563fad544106fb5fa90922aef456a0f110ccab7628d793ca3c5f6a4c6e22ee05227e1d3c395f99f52418c5dfdee1442d0aa3ca1e9154cae569c25497c3445f2bed1e8de3cada7e2f1825d7341e591234df4053ca5b33c92e3217967ca00211b68c1b0aafa5152b84557a81
    q = 0x00bdd727428445e6e4ad64978a4184de1f28ab3f298e79f11147bee060150040e1
    g = 0x75fd519cb49bcfd504dc7f8bfb100511dee71335ba07d7bf85e0b602441b121fc1b537f29b941254f85703d2aa41e2cf400e3be7d952ad9a32f461174873034631295c2bc86d5134779358e8f4c6d54e591edc645dbd389088f9a7dfa3927fbeedce1bc57fa1e6dd3908daac26914ead4f40e8acab8b57a303d1970a06393789c56decbb504f7ae3d9f8e2cd1850b9e77d4aa64f49218fd0f6c24ccb5aa6af9d97e4d041b0676dda90dfbba0177df89b730ab1b8db5ef19ebd2fed52a790826fc507647576bebed69e5a8a11ec25f89d445b56b738d2a7f713d74527a4e6ed99af43ebcdb2cfe2ccaee76fd543ea429ec6402c273b030a63e6a686b2d299d8d7
    k = random.randint(1,q-1)
    a = random.randint(1,q-1)
    print("alpha:",g)
    print("prikey:",a)
    beta = quick_pow(g,a,p)
    print("beta",beta)
    gamma = quick_pow(g,k,p) % q
    print("k:",k)
    print("gamma:",gamma)
    print("sha256(x):",hashlib.sha256(x.encode()).hexdigest())
    delta = (int(hashlib.sha256(x.encode()).hexdigest(),16) + a* gamma) * inverse(k,q)%q
    print("delta:",delta)
    return gamma,delta,g,beta
def dsa_ver(x,gamma,delta,alpha,beta):
    p = 0x008746338ba0d0b67ec6dde878f14d624dfe86a4663aaf170208b02b020c09199af1dbaaf0ba5a70d52b3eb715f775b0989ff176d1d6c680042dab48d35d802c598036280c559ffb59ea4c82c01fae4a227847cb715e03511602f7dedd0d0f1556c5e63c9b181a4a7ffb1416a3d6ef69cb9b413746aab5e259c3b12bec3244a7e09f6718ecb721c528020a9ae09568f461512c526593563fad544106fb5fa90922aef456a0f110ccab7628d793ca3c5f6a4c6e22ee05227e1d3c395f99f52418c5dfdee1442d0aa3ca1e9154cae569c25497c3445f2bed1e8de3cada7e2f1825d7341e591234df4053ca5b33c92e3217967ca00211b68c1b0aafa5152b84557a81
    q = 0x00bdd727428445e6e4ad64978a4184de1f28ab3f298e79f11147bee060150040e1
    e1 =int(hashlib.sha256(x.encode()).hexdigest(),16) * inverse(delta,q) % q
    e2 = gamma* inverse(delta,q) % q
    print("alpha:",alpha)
    print("beta:",beta)
    print("e1:",e1)
    print("e2",e2)
    print("vertify:",quick_pow(alpha,e1,p) * quick_pow(beta,e2,p) %p % q)
    return quick_pow(alpha,e1,p) * quick_pow(beta,e2,p) %p % q == gamma
if __name__ == "__main__":
    x = "SchoolofDataandComputerScience,Sunyat-senUniversity"
    gamma,delta,alpha,beta = dsa_sig(x)
    print("gamma",gamma)
    print("delta",delta)
    if dsa_ver(x,gamma,delta,alpha,beta):
        print("vertified!")
    else:
        print("fail!")
