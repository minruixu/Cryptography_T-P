//
// Created by 406 on 2019/11/2.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bitset>
#include<time.h>
using namespace std;

typedef bitset<8> byte;
typedef bitset<32> word;
string byte2string(byte a){
    string re;
    byte a1 = a>>4;
    byte a2 = (a << 4)>>4;
    int i1 = a1.to_ulong();
    int i2 = a2.to_ulong();
    if(i1<10) re = re + char('0'+i1);
    else re = re + char('a'+i1-10);
    if(i2<10) re = re + char('0'+i2);
    else re = re + char('a'+i2-10);
    return re;
}
string word2string(word a){
    string re;
    word b = a >> 24;
    re = re+ byte2string(byte(b.to_ulong()));
    b = (a << 8) >> 24;
    re = re+ byte2string(byte(b.to_ulong()));
    b = (a << 16) >> 24;
    re = re+ byte2string(byte(b.to_ulong()));
    b = (a<< 24) >> 24;
    re = re+ byte2string(byte(b.to_ulong()));
    return re;
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
    vector <word> hash(string msg){
        string msgb = "";
        // 把msg变成二进制形式的
        for(int i = 0;i<msg.length();i++){
            byte tmp = msg[i];
            msgb = msgb + tmp.to_string();
        }
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
int main(){
    clock_t start,finish;
    SHA1 s = SHA1();
    vector <word> result;
    string str = "asfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhasasfhlkashdfklashbfljkhas";
    cout << "The length of the string is:"<<endl;
    cout << str.length()<<endl;
    start = clock();
    result = s.hash(str);
    finish = clock();
    cout << "Time for encoding is: "<<double(finish-start)/CLOCKS_PER_SEC <<"s"<<endl;
    cout << "The result of hash is:" <<endl;
    for(int i =0;i<5;i++){
        cout << word2string(result[i]);
    }
    cout << endl;
    cin >> str;
}
