import math
class DLP:
    def __init__(self,p):
        self._p = p

    def f(self,x,a,b):
        if x % 3==1:
            return (self._beta*x)%self._p,a,(b+1)%self._p
        if x % 3==0:
            return (x*x)%self._p,2*a%self._p,2*b%self._p
        if x % 3==2:
            return self._alpha*x % self._p,(a+1)%self._p,b

    def inverse(self,a,m):
        # 扩展欧几里得算法求逆
        u1,u2,u3 = 1,0,a
        v1,v2,v3 = 0,1,m
        while v3!=0:
            q = u3//v3
            v1,v2,v3,u1,u2,u3 = (u1-q*v1),(u2-q*v2),(u3-q*v3),v1,v2,v3
        return u1%m

    def rank(self):
        pass
    def pollard_rho(self,alpha,beta):
        self._alpha = alpha
        self._beta = beta
        x,a,b = self.f(1,0,0)
        xt,at,bt = self.f(x,a,b)
        while x != xt:
            print(x)
            x,a,b = self.f(x,a,b)
            xt,at,bt = self.f(xt,at,bt)
            xt,at,bt = self.f(xt,at,bt)
        if math.gcd(bt-b,self._p) != 1:
            return "failure"
        else:
            return (a-at)*self.inverse(bt-b,self._p)%self._p

if __name__ == '__main__':
    # p = 5682549022748424631339131913370125786212509227588493537874673173634936008725904358935442101466555561124455782847468955028529037660533553941399408331331403379
    p = 809
    dlp = DLP(p)
    # g = 2060700868429783091611633355344180229561388849150035417527475314898771386496110567969592724791964544803380427287644059398886410683651686798605735244265072881
    g = 89
    # ya = 3825058671964099517205471223234846245194588471680037949514700806335021383393529724195692131003753889127671117390431517299962378605270757342283339240245427141
    ya = 618
    # yb = 2302635109511322529771741748647406080202335756806042374177023626219665963400570136543353658409439849520553114777775877601809512683048696443723398973290837487
    xa = dlp.pollard_rho(g,ya)
    print("+")
    # xb = dlp.pollard_rho(g,yb)
    print(xa)
    # print(xb)
