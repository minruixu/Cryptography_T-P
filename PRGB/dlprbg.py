def quick_pow(a, m, p):
    re = 1
    a = a % p
    while (m != 0):
        if m & 1:
            re = (re * a) % p
        a = (a * a) % p
        m >>= 1
    return re

def dlPEBG(s0,p,alpha,l):
    # 生成一个l比特的随机数
    rand = ""
    s = s0
    p = p/2
    for i in range(l):
        s = quick_pow(alpha,s,p)
        if s > p:
            rand = rand + "1"
        else:
            rand = rand + "0"
    return rand

if __name__ == "__main__":
    p = 2135176579
    alpha = 7
    s0 = 17340023
    l = 200
    rand = dlPEBG(s0,p,alpha,l)
    print("the random number is(in bit):",rand)
    print("the random number is(int):",int(rand,2))
    num_1 = 0
    for i in range(l):
        if rand[i] == "1":
            num_1 += 1
    print("the numum of \"1\" in random number is",num_1)