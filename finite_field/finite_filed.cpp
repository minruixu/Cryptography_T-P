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
    int decode(vector <int> a){
        int result = 0;
        for(int i = 0;i<_m/4;i++){
            vector <int> t;
            t.assign(a.begin()+(4*i),a.begin()+(4*i)+4);
            result += pow(10,i)*decode_int(t);
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
        vector <int> result(_m,0);
        for(int i = 0;i<_m;i++){
            result[i] = myxor(a[i],b[i]);
        }
        return result;
    }
    int find_high(vector<int> a){
        int t = 0;
        for(int i = 0;i<a.size();i++) if(a[i] == 1) t=i;
        return t;
    }
    vector <int> mod(vector <int> a,vector <int> b){
        //计算 a mod b
        vector <int> result;
        int a_high = find_high(a);
        int b_high = find_high(b);
        for(int i = a_high;i>=b_high;i--){
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
        result.assign(a.begin(),a.begin()+_m);
        return result;
    }
    vector <int> multiply(vector <int> a,vector <int> b){
        vector <int> result(2*_m,0);
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                int t = a[i]*b[j];
                result[i+j] = myxor(result[i+j],t);
            }
//            for(int i = 0;i<2*_m;i++){
//                cout << result[i]<<' ';
//            }
//            cout << endl;
        }
        //取mod
        result = mod(result,_irr_poly);
        return result;
    }
    vector <int> divide_mod(vector <int> a,vector <int> b){
        //a >= b, 大的除小地，返回一个数。
        vector <int> result(_m,0);
        int a_high = find_high(a);
        int b_high = find_high(b);
        for(int i = a_high;i>b_high;i--){
            if(a[i]==0) continue;
            vector <int> ve = myshift(b,i-b_high);
            for(int j = 0;j<=i;j++){
                a[j] = myxor(a[j],ve[j]);
            }
            result[i-b_high] = 1;
        }
        for(int i = b_high;i>=0;i--){
            if(a[i]>b[i]){
                result[0] = 1;
                break;
            }
            else if(a[i]<b[i]){
                break;
            }
        }
        return result;
    }
    vector <int> inverse(vector <int> r1,vector <int> r2,vector <int> v1=_v1,vector <int> v2=_v0,vector <int> w1=_v0,vector <int> w2=_v1){
        //使用欧几里得算法求逆
        if(decode(r1)==0 || decode(r2) == 1) return w2;
        vector <int> q3 = divide_mod(r1,r2);
        vector <int> r3 = mod(r1,r2);
        // 计算v3
        vector <int> v3 = add(v1,multiply(q3,v2));
        // 计算w3
        vector <int> w3 = add(w1,multiply(q3,w2));
        return inverse(r2,r3,v2,v3,w2,w3);
    }
    vector <int> finite_pow(vector <int> a,int p){
        vector <int> result = a;
        for(int i = 0;i<p-1;i++){
            result = multiply(result,a);
        }
        return result;
    }
};
int vec[128] = {0};
vector<int> finiteFiled::_v0(vec,vec+128);
int vec1[128] = {1};
vector<int> finiteFiled::_v1(vec1,vec1+128);
int main(){
//    vector <int> irr_poly(128,0);
//    irr_poly[0] = 1;irr_poly[1] = 1;irr_poly[127] = 1;
    int m = 4;
//    cin >> m;
    vector <int> irr_poly(m,0);
    irr_poly[0]=1;irr_poly[1] = 1;irr_poly[3] = 1;
    finiteFiled f = finiteFiled(m,irr_poly);
    vector <int> a(m,0);
//    vector<int> b(m,0);
    a[0]=1;a[1] =0;a[2]=0;
//    b[0]=0;b[1] =1;b[2]=1;
//    vector <int> re = f.multiply(a,b);
    vector <int> div_re = f.inverse(irr_poly,a);
    for(int i = 0;i<m;i++){
        cout << div_re[i]<<' ';
    }
    cout << endl;
//    vector<int> t = f.encode(17340023);
//    cout << f.decode(t) << endl;
    //f.pow(17340023,20190911);
}
