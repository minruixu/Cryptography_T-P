def bbs(s0,p,q,l):
    # 生成一个l比特的随机数
    n = p * q
    ct = 1
    rand = ""
    s0 = s0 * s0 % n
    for i in range(l):
        if s0 % 2 == 1:
            rand = rand + "1"
        else:
            rand = rand + "0"
        s0 = s0*s0 % n
        ct *= 2
    return rand

if __name__ == "__main__":
    s0 = 20749
    p = 383
    q = 503
    rand = bbs(s0,p,q,20)
    print(rand)
