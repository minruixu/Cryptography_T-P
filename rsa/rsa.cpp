//
// Created by 406 on 2019/11/15.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bitset>
#include<time.h>
using namespace std;
/*
 * 我决定了，用c++写
 * 好的
 * 快写
 *
 * 实现2048RSA加密并给出解密函数验证
 * 要求：
 * p 和 q 是两个1024的安全素数，根据学号最后一位的数字选择
 * p = 74829638746533240988779487685519857207853614853369269125737224926836190844584386477252351678694220847326413610447912126732461551667746605631104998257474529620520790296706975377558703885069067389851079532628142311782993846028093717798601985881211947182951877707743021688050533047297428267547207193214872407179
 * q = 74829638746533240988779487685519857207853614853369269125737224926836190844584386477252351678694220847326413610447912126732461551667746605631104998257474529620520790296706975377558703885069067389851079532628142311782993846028093717798601985881211947182951877707743021688050533047297428267547207193214872407179
 * 密钥是学号的下一个素数：17340031
 * 公钥就是生成的啦
 * 明文是中山大学的英文 (Sun Yat-sen University) 进行OAEP填充到2048位
 * 填充是使用两个Hash函数为实验4实现的SHA-1的扩展 hash六次，前面补0 到1024位
 *
 * p 和 q 一开始肯定是字符串了，然后呢，怎么变成一个大整数
 *
 */

typedef bitset<8> byte;
typedef bitset<32> word;
typedef bitset<1024> pmsg;
typedef bitset<2048> msg;

struct Wint:vector<int>{
    //用标准库vector做基类，完美解决位数问题，同时更易于实现
    //将低精度转高精度的初始化，可以自动被编译器调用
    //因此无需单独写高精度数和低精度数的运算函数，十分方便
    Wint(int n=0){//默认初始化为0，但0的保存形式为空
        push_back(n);
        check();
    }
    Wint(string s){//默认初始化为0，但0的保存形式为空
        for(int i=s.size()-1; i>=0; --i) push_back(s[i]-'0');
        check();
    }
    Wint& check(){//在各类运算中经常用到的进位小函数，不妨内置
        while(!empty()&&!back()) pop_back();//去除最高位可能存在的0
        if(empty())return *this;
        for(int i=1; i<size(); ++i)
        {
            (*this)[i]+=(*this)[i-1]/10;
            (*this)[i-1]%=10;
        }
        while(back()>=10)//最前一位
        {
            push_back(back()/10);
            (*this)[size()-2]%=10;
        }
        return *this;//为使用方便，将进位后的自身返回引用
    }
};

ostream& operator<<(ostream &os,Wint n)
{
    if(n.empty())os<<0;
    for(int i=n.size()-1; i>=0; --i)os<<n[i];
    return os;
}
bool operator!=(Wint a,Wint b)
{
    if(a.size()!=b.size())return 1;
    for(int i=a.size()-1; i>=0; --i)
        if(a[i]!=b[i])return 1;
    return 0;
}
bool operator==(Wint a,Wint b){
    return !(a!=b);
}
bool operator<(Wint a,Wint b)
{
    if(a.size()!=b.size())return a.size()<b.size();
    for(int i=a.size()-1; i>=0; --i)
        if(a[i]!=b[i])return a[i]<b[i];
    return 0;
}
bool operator>(Wint a,Wint b)
{
    return b<a;
}
bool operator<=(Wint a,Wint b)
{
    return !(a>b);
}
bool operator>=(Wint a,Wint b)
{
    return !(a<b);
}
Wint operator-=(Wint a,Wint b){
    if(a<b)swap(a,b);
    for(int i=0; i!=b.size(); a[i]-=b[i],++i)
        if(a[i]<b[i])//需要借位
        {
            int j=i+1;
            while(!a[j])++j;
            while(j>i)
            {
                --a[j];
                a[--j]+=10;
            }
        }
    return a.check();
}
Wint operator-(Wint a,Wint b)
{
    return a-=b;
}
Wint operator+=(Wint a,Wint b){
    if(a.size()<b.size())a.resize(b.size());
    for(int i=0; i!=b.size(); ++i)a[i]+=b[i];
    return a.check();
}
Wint operator+(Wint a,Wint b){
    return a+=b;
}
Wint operator*(Wint a,Wint b){
    Wint n(0);
    n.assign(a.size()+b.size()-1,0);
    for(int i=0; i!=a.size(); ++i)
        for(int j=0; j!=b.size(); ++j)
            n[i+j]+=a[i]*b[j];
    return n.check();
}
Wint operator%(Wint a,Wint b){
    Wint ans(0);
    for(int t=a.size()-b.size(); a>=b; --t){
        Wint d;
        d.assign(t+1,0);
        d.back()=1;
        Wint c=b*d;
        while(a>=c){
            a = a - c;
            ans+=d;
        }
    }
    return a;
}
Wint operator/(Wint a,Wint b){
    Wint ans;
    for(int t=a.size()-b.size(); a>=b; --t){
        Wint d;
        d.assign(t+1,0);
        d.back()=1;
        Wint c=b*d;
        while(a>=c){
            a = a - c;
            ans= ans + d;
        }
    }
    return ans;
}
Wint operator/=(Wint a,Wint b){
    return a/b;
}
Wint pow(Wint n,Wint k){
    if(k.empty())return 1;
    if(k==Wint("2")) return n*n;
    if(k.front()%2) return n*pow(n,k-1);
    return pow(pow(n,k/2),2);
}
msg Wint2msg(Wint w){
    msg result;
    Wint num = Wint("2");
    Wint num2047 = Wint("2047");
    vector <Wint> pow2;
    pow2.push_back(Wint(1));
    for(int i = 1;i<2048;i++){
        pow2.push_back(pow2[i-1]*2);
    }
    for(int i = 2047;i>=0;i--){
        if(w >= pow2[i]){
            result[i] = 1;
            w = w - pow2[i];
        }
    }
    cout << result.to_string() << endl;
    return result;
}
class SHA1{
private:
    string s;
public:
    SHA1(){
        // 输入为字节流，就是每一个字节都是8比特的
        // INPUT: 512 + 160
        // OUTPUT: 160
    }
    word getK(int t){
        if(t<=19) return 0x5A827999;
        else if(t<=39) return 0x6ED9EBA1;
        else if(t<=59) return 0x8F1BBCDC;
        else return 0xCA62C1D6;
    }
    word f(int t,word B,word C, word D){
        if(t<=19) return (B&C)|((~B)&D);
        else if(t<=39) return B^C^D;
        else if(t<=59) return (B&C)|(B&D)|(C&D);
        else return B^C^D;
    }
    word rotl(int n,word x){
        word b1 = x << n;
        word b2 = x >> (32-n);
        return b1|b2;
    }
    word add(word a, word b){
        unsigned int t = a.to_ulong() + b.to_ulong();
        return t;
    }
    vector <word> hash(string msgb){
        unsigned int l = msgb.length();
        //将msgb补充为512分组
        int d = (447 - l) % 512;
        msgb += "1";
        for(int i = 0;i<d;i++){
            msgb += "0";
        }
        // 将l添加到msgb中，64bits
        msgb = msgb + bitset<8*8>(l).to_string();
//        cout << msgb <<endl;
//        cout << msgb.length()<<endl;
        // 将msgb切分为32bit的word
        int n = msgb.length()/32;
        vector <word> msgw;
        for(int i =0;i<n;i++){
            word t;
            for(int j = 0;j<32;j++) t[31-j] = (msgb[i*32+j]=='1');
//            cout << t.to_string()<<endl;
            msgw.push_back(t);
        }
        n = msgw.size()/16;
        word h0 = 0x67452301;
        word h1 = 0xEFCDAB89;
        word h2 = 0x98BADCFE;
        word h3 = 0x10325476;
        word h4 = 0xC3D2E1F0;
        for(int i = 0;i<n;i++){
            vector <word> w = vector <word> (msgw.begin()+i*16,msgw.begin()+i*16+16);
            for(int t = 16;t<80;t++){
                w.push_back(rotl(1,w[t-3]^w[t-8]^w[t-14]^w[t-16]));
            }
            word A = h0;
            word B = h1;
            word C = h2;
            word D = h3;
            word E = h4;
            for(int t = 0;t<80;t++){
                word temp = add(add(add(rotl(5,A),f(t,B,C,D)),add(E,w[t])),getK(t));
                E = D;
                D = C;
                C = rotl(30,B);
                B = A;
                A = temp;
            }
            h0 = add(h0,A);
            h1 = add(h1,B);
            h2 = add(h2,C);
            h3 = add(h3,D);
            h4 = add(h4,E);
        }
//        cout << word2string(h0)<<endl;
        return vector <word> ({h0,h1,h2,h3,h4});
    }
};
class RSA{
private:
    msg _p,_q;
    msg _n,_phin;
    msg _priket,_pubkey;
    // a 和 b 可以表示为大整数形式
public:
    RSA(string p,string q,string prikey){
        // 将p ， q 和 prikey 的字符串 转为 bitset
        _p = Wint2msg(Wint(p));
        _q = Wint2msg(Wint(p));
        _prikey = Wint2msg(Wint(prikey));
    }
    // 对一个明文进行进行OAEP
    msg hash(msg input){
        msg output;
        SHA1 sha1 = SHA1();
        vector <word> re = sha1.hash(input.to_string());
        for(int i = 0;i<6;i++){
            cout << re[i].to_string();
        }
        cout << endl;
        return output;
    }
//    msg encodeOAEP(string str){
//
//    }
//    // 对密文进行OAEP
//    string decodeOAEP(msg decode){
//
//    }
//    // RSA 公钥加密
//    msg RSAencode(msg){
//
//    }
//    // RSA 私钥解密
//    msg RSAdecode(){
//
//    }
    // 在RSA中实现的大整数运算：pow(multiply,mod),inverse
};
int main(){
    // 1024bit的p和q
    string p = "74829638746533240988779487685519857207853614853369269125737224926836190844584386477252351678694220847326413610447912126732461551667746605631104998257474529620520790296706975377558703885069067389851079532628142311782993846028093717798601985881211947182951877707743021688050533047297428267547207193214872407179";
    string q = "74829638746533240988779487685519857207853614853369269125737224926836190844584386477252351678694220847326413610447912126732461551667746605631104998257474529620520790296706975377558703885069067389851079532628142311782993846028093717798601985881211947182951877707743021688050533047297428267547207193214872407179";
    Wint wq = Wint(q);
    Wint2msg(wq);
}
