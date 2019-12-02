import math
class DLP:
    def __init__(self):
        self._p = 5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379
        # p-1 的素因子为：
        self._p1_prime_factors = [2,2154937333,481006989528686211893,602384082763980697559,1037605481820435550019,1524982628045073955591,1976916723258661210051,1206102098588167138789,1206102098588167138789]
        self._p1_factors = []
        for i in range(len(self._p1_prime_factors)):
            f = 1
            for j in range(i,len(self._p1_prime_factors)):
                f *= self._p1_prime_factors[j]
                self._p1_factors.append(f)
        self._p1_factors = sorted(self._p1_factors)

    def f(self,x,a,b):
        if x % 3==1:
            return (self._beta*x)%self._p, a, (b+1)%self._ra
        elif x % 3==0:
            return (x*x)%self._p, 2*a%self._ra, 2*b%self._ra
        else:
            return self._alpha*x % self._p, (a+1)%self._ra, b

    def inverse(self,a,m):
        # 扩展欧几里得算法求逆
        while math.gcd(a,m) != 1:
            a,m = a/math.gcd(a,m),m/math.gcd(a,m)
        u1,u2,u3 = 1,0,a
        v1,v2,v3 = 0,1,m
        while v3!=0:
            q = u3//v3
            v1,v2,v3,u1,u2,u3 = (u1-q*v1),(u2-q*v2),(u3-q*v3),v1,v2,v3
        return u1%m
    def quick_pow(self,a,m):
        re = 1
        a = a %self._p
        while(m!=0):
            if m&1:
                re = (re * a) % self._p
            a = (a * a) % self._p
            m >>= 1
        return re
    def rank(self,g):
        # 计算 g 的阶
        for i in range(len(self._p1_factors)):
            if self.quick_pow(g,self._p1_factors[i]) == 1:
                return self._p1_factors[i]
        # g 不是这个循环群的元素
        return -1
    def pollard_rho(self,alpha,beta,ra):
        self._ra = ra
        self._alpha = alpha
        self._beta = beta
        x,a,b = self.f(1,0,0)
        xt,at,bt = self.f(x,a,b)
        re = 0
        while x != xt:
            x,a,b = self.f(x,a,b)
            xt,at,bt = self.f(xt,at,bt)
            xt,at,bt = self.f(xt,at,bt)
        # print(x)
        print(a,at,b,bt)
        r = (ra+bt-b)%ra
        print(r)
        print("inverse",self.inverse(r,ra//2))
        print("Test inverse:",r*self.inverse(r,ra//2)%(ra//2))
        # print(math.gcd(abs(bt-b),ra))
        d = math.gcd((bt-b),ra)
        solution = (a-at)*self.inverse(bt-b,ra//2)%ra
        # 对解进行验证
        print("We get:",d,"solution(s).")
        for i in range(d):
            solution = (ra + solution + i * ra//d)%ra
            if self.quick_pow(alpha,solution)==beta:
                print(((a - at)*self.inverse(r,ra//2) % ra)*(bt-b)%ra,'==',(ra + a-at)%ra)
                return solution
        return (a - at)*self.inverse(r,ra//2) % ra

    def pohlig_hellman(self):
        pass
    def baoli(self,x,y,b):
        f = 1
        for i in range(b):
            print(i)
            f *= x
            if f == y:
                return i
        return -1

if __name__ == '__main__':
    # p = 5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379
    '''
    p-1 = 5 682549 022748 424631 339131 913370 125786 212509 227588 493537 874673 173634 936008 725904 358935 442101 466555 561124 455782 847468 955028 529037 660533 553941 399408 331331 403378 (157 digits) = 2 × 2154 937333 × 481 006989 528686 211893 × 602 384082 763980 697559 × 1037 605481 820435 550019 × 1524 982628 045073 955591 × 1976 916723 258661 210051 × 1206 102098 588167 1387892
    '''
    dlp = DLP()
    g = 2410497055970432881345493397846112198995088771364307195189734031205605186951241875096796459061741781667380437076874705300974836586165283741668656930807264789
    rg = dlp.rank(g)
    print("rank g:",rg)
    ya = 973768284341326272301553751114322685324340805902069613339667142187801529585352406975030927008752571917079716318221077758236884342662829402529734009607531649
    #ya = 618
    yb = 4149822765985031146554298777122732051870868431387323913747785791685310508836836283702926446817000114868007555717546362425841865173929670156568682745060708314
    xa = dlp.pollard_rho(g,ya,rg)
    # print(dlp.baoli(g,ya,rg))
    #print("+")
    xb = dlp.pollard_rho(g,yb,rg)
    print(xa)
    print(xb)
