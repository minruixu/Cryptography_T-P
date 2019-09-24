//
// Created by yirui on 2019/9/18.
//
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
//我要初始化0和1的vector
vector <int> encode_int(int a){
    //输入一个int，返回4位的数组
    vector <int> result(4);
    for(int i = 3;i>=0;i--){
        if(a>=pow(2,i)){
            result[i] = 1;
            a -= pow(2,i);
        }
    }
    return result;
}
int decode_int(vector<int> a){
    int result = 0;
    for(int i = 0;i<4;i++) result+= a[i]*pow(2,i);
    return result;
}
class finiteFiled{
private:
    int _m;
    vector <int> _irr_poly;
public:
    static vector<int> _v0;
    static vector<int> _v1;
    finiteFiled(int m,vector<int> irr_poly){
        _m = m+1;
        _irr_poly = irr_poly;
    }
    vector <int> encode(int a){
        //输入一个学号，返回一个数组
        vector <int> result(_m);
        int j = 0;
        while(a!=0){
            vector<int> t = encode_int(a%10);
            for(int i = 0;i<4;i++){
                result[i+j*4] = t[i];
            }
            j++;
            a /= 10;
        }
        return result;
    }
    vector <char> decode(vector <int> a){
        vector <char> result(_m/4);
        vector <char> c = {'0','1','2','3','4','5','6','7','8','9','0','A','B','C','D','E','F'};
        for(int i = 0;i<_m/4;i++){
            vector <int> t;
            t.assign(a.begin()+(4*i),a.begin()+(4*i)+4);
            result[i] = c[decode_int(t)];
        }
        return result;
    }
    int myxor(int a,int b){
        if(a==b) return 0;
        else return 1;
    }
    vector<int> myshift(vector<int> a,int n){
        for(int i = 0;i<n;i++)
            a.insert(a.begin(),0);
        return a;
    }
    vector <int> add(vector <int> a,vector <int> b){
        int mt = max(a.size(),b.size());
        vector <int> result(mt,0);
        for(int i = 0;i<a.size();i++){
            result[i] = myxor(a[i],result[i]);
        }
        for(int i = 0;i<b.size();i++){
            result[i] = myxor(b[i],result[i]);
        }
        return result;
    }
    int find_high(vector<int> a){
        int t = 0;
        for(int i = 0;i<a.size();i++) if(a[i] == 1) t=i;
        return t;
    }
    vector <int> new_mod(vector <int> a,vector <int> b){
        //计算 a mod b
        vector <int> result;
        int a_high = find_high(a);
        int b_high = find_high(b);
        b[b_high] = 0;
        while(a_high>=b_high){
            vector <int> bt = myshift(b,a_high-b_high);
            a[a_high]=0;
            a = add(a,bt);
            a_high=find_high(a);
        }
//        cout << "+";
//        for(int i = 0;i<_m;i++){
//            cout << a[i];
//        }
//        cout << endl;
        result.assign(a.begin(),a.begin()+_m);
        return result;
    }
    vector <int> mod(vector <int> a,vector <int> b){
        //计算 a mod b
        vector <int> result;
        int a_high = find_high(a);
        int b_high = find_high(b);
        for(int i = a_high;i>b_high;i--){
            if(a[i]==0) continue;
            vector <int> ve = myshift(b,i-b_high);
            for(int j = 0;j<=i;j++){
                a[j] = myxor(a[j],ve[j]);
            }
//            for(int m = 0;m<ve.size();m++){
//                cout << ve[m]<<' ';
//            }
//            cout << endl;
//            for(int m = 0;m<a.size();m++){
//                cout << a[m]<<' ';
//            }
//            cout << endl;
        }
        if(a_high>=b_high)
            for(int i = b_high;i>=0;i--){
                if(a[i]>b[i]){
                    for(int j = 0;j<=b_high;j++){
                        a[j] = myxor(a[j],b[j]);
                    }
                    break;
                }
                else if(a[i]<b[i]){
                    break;
                }
            }
//        for(int i = 0;i<_m;i++){
//            cout << a[i];
//        }
//        cout << endl;
        result.assign(a.begin(),a.begin()+_m);
        return result;
    }
    vector <int> multiply(vector <int> a,vector <int> b){
        vector <int> result(2*_m,0);
        for(int i = 0;i<_m;i++){
            if(a[i]==1) {
                vector<int> bt = myshift(b, i);
                for (int j = 0; j < bt.size(); j++) {
                    result[j] = myxor(result[j], bt[j]);
                }
            }
        }
        //取mod
        result = new_mod(result,_irr_poly);
        return result;
    }
    vector <int> divide_mod(vector <int> a,vector <int> b){
        //a >= b, 大的除小地，返回一个数。
        vector <int> result(_m,0);
        int a_high = find_high(a);
        int b_high = find_high(b);
//        cout <<"____________________"<<endl;
//        for(int i = 0;i<_m;i++){
//            cout << a[i]<<' ';
//        }
//        cout << endl;
//        for(int i = 0;i<_m;i++){
//            cout << b[i]<<' ';
//        }
//        cout << endl;
//        cout <<"____________________"<<endl;
        for(int i = a_high;i>b_high;i--){
            if(a[i]==0) continue;
            vector <int> ve = myshift(b,i-b_high);
            for(int j = 0;j<=i;j++){
                a[j] = myxor(a[j],ve[j]);
            }
            result[i-b_high] = 1;
        }
        if(a_high>=b_high)
        for(int i = b_high;i>=0;i--){
            if(a[i]>b[i]){
                result[0] = 1;
                break;
            }
            else if(a[i]<b[i]){
                break;
            }
        }
//        for(int i = 0;i<_m;i++){
//            cout << result[i]<<' ';
//        }
//        cout << endl;
        return result;
    }
    vector <int> inverse(vector <int> r1,vector <int> r2,vector <int> v1=_v1,vector <int> v2=_v0,vector <int> w1=_v0,vector <int> w2=_v1){
        //使用欧几里得算法求逆
        vector<char> rr2 = decode(r2);
        for(int i = rr2.size()-1;i>=0;i--){
            if(i!=0&&rr2[i]!='0') break;
            if(i==0)
                if(rr2[0]=='0' || rr2[0] == '1') return w2;
        }
        vector <int> q3 = divide_mod(r1,r2);
//        cout << "++";
        vector <int> r3 = mod(r1,r2);
        // 计算v3
        vector <int> v3 = add(v1,multiply(q3,v2));
        // 计算w3
        vector <int> w3 = add(w1,multiply(q3,w2));
//        cout << "+";
//        for(int i = 0;i<_m;i++){
//            cout << r3[i];
//        }
//        cout << endl;
        return inverse(r2,r3,v2,v3,w2,w3);
    }
    vector <int> finite_pow(vector <int> a,int p){
        vector <int> result = _v1;
        while (p>0){
            if(p%2==1) result = multiply(result,a);
            a = multiply(a,a);
            p = p >> 1;
        }
        return result;
    }
};
int vec[128] = {0};
vector<int> finiteFiled::_v0(vec,vec+128);
int vec1[128] = {1};
vector<int> finiteFiled::_v1(vec1,vec1+128);
int main(){
    int m = 128;
    vector <int> irr_poly(m,0);
    irr_poly[0]=1;irr_poly[1] = 1;irr_poly[127] = 1;
    finiteFiled f = finiteFiled(m,irr_poly);
    vector <int> a(m,0);
    vector <int> b(m,0);
    vector <int> re(m,0);
    vector <char> num(m,0);
    cout << "欢迎来到有限域的运算:"<<endl;
    cout << "本有限域的次数为127次，不可约多项式为:x^127+x+1"<<endl;
    cout << "验证加法(3+17340023):"<<endl;
    a = f.encode(3);
    b = f.encode(17340023);
    re = f.add(a,b);
    cout << "二进制的结果为："<<endl;
    for(int i = 0;i<m;i++){
        cout << re[m-i-1];
    }
    cout << endl;
    cout << "十六进制的结果为："<<endl;
    num = f.decode(re);
    for(int i = 0;i<num.size();i++){
        cout << num[num.size()-i-1];
    }
    cout << endl;
    cout << "验证乘法(3*17340023):"<<endl;
    a = f.encode(3);
    b = f.encode(17340023);
    re = f.multiply(a,b);
    cout << "二进制的结果为："<<endl;
    for(int i = 0;i<m;i++){
        cout << re[m-i-1];
    }
    cout << endl;
    cout << "十六进制的结果为："<<endl;
    num = f.decode(re);
    for(int i = 0;i<num.size();i++){
        cout << num[num.size()-i-1];
    }
    cout << endl;
    cout << "求逆(17340023):"<<endl;
    a = f.encode(17340023);
    re = f.inverse(irr_poly,a);
    cout << "二进制的结果为："<<endl;
    for(int i = 0;i<m;i++){
        cout << re[m-i-1];
    }
    cout << endl;
    cout << "十六进制的结果为："<<endl;
    num = f.decode(re);
    for(int i = 0;i<num.size();i++){
        cout << num[num.size()-i-1];
    }
    cout << endl;
    cout <<"验证求逆！"<<endl;
    cout << "二进制的结果为："<<endl;
    re = f.multiply(a,re);
    for(int i = 0;i<m;i++){
        cout << re[m-i-1];
    }
    cout << endl;
    cout << "十六进制的结果为："<<endl;
    num = f.decode(re);
    for(int i = 0;i<num.size();i++){
        cout << num[num.size()-i-1];
    }
    cout << endl;
    cout << "验证次方(17340023^20190911):"<<endl;
    a = f.encode(17340023);
    re = f.finite_pow(a,20190911);
    cout << "二进制的结果为："<<endl;
    for(int i = 0;i<m;i++){
        cout << re[m-i-1];
    }
    cout << endl;
    cout << "十六进制的结果为："<<endl;
    num = f.decode(re);
    for(int i = 0;i<num.size();i++){
        cout << num[num.size()-i-1];
    }
    cout << endl;
    cout << "自由检测时间,请输入一个int以内的数字(-_-*),-1退出:"<<endl;
    int test = 1;
    cin >> test;
    while(test!=-1){
        cout << "验证求逆(test):"<<endl;
        a = f.encode(test);
        re = f.inverse(irr_poly,a);
        cout << "二进制的结果为："<<endl;
        for(int i = 0;i<m;i++){
            cout << re[m-i-1];
        }
        cout << endl;
        cout << "十六进制的结果为："<<endl;
        num = f.decode(re);
        re = f.multiply(a,re);
        cout <<"验证求逆！"<<endl;
        cout << "二进制的结果为："<<endl;
        for(int i = 0;i<m;i++){
            cout << re[m-i-1];
        }
        cout << endl;
        cout << "十六进制的结果为："<<endl;
        num = f.decode(re);
        for(int i = 0;i<num.size();i++){
            cout << num[num.size()-i-1];
        }
        cout << endl;
        cout << "验证次方(test^20190911):"<<endl;
        a = f.encode(test);
        re = f.finite_pow(a,20190911);
        cout << "二进制的结果为："<<endl;
        for(int i = 0;i<m;i++){
            cout << re[m-i-1];
        }
        cout << endl;
        cout << "十六进制的结果为："<<endl;
        num = f.decode(re);
        for(int i = 0;i<num.size();i++){
            cout << num[num.size()-i-1];
        }
        cout << endl;
        cin >> test;
    }

}
