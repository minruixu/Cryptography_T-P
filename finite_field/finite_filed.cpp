//
// Created by yirui on 2019/9/18.
//
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
using namespace std;
class finiteFiled{
private:
    int _m;
    vector <int> _irr_poly;
public:
    finiteFiled(int m,vector<int> irr_poly){
        _m = m+1;
        _irr_poly = irr_poly;
    }
    int myxor(int a,int b){
        if(a==b) return 0;
        else return 1;
    }
    vector<int> myshiff(vector<int> a,int n){
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
    vector <int> mod(vector <int> a){
        // 对不可约多项式求模
        vector <int> result;
        for(int i = a.size()-1;i>=_m;i--){
            if(a[i]==0) continue;
            vector <int> ve = myshiff(_irr_poly,i-_m+1);
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
            for(int i = 0;i<2*_m;i++){
                cout << result[i]<<' ';
            }
            cout << endl;
        }
        //取mod
        result = mod(result);
        return result;
    }
    vector <int> inverse(vector <int> a){

    }
    vector <int> pow(vector <int> a,int p){
        vector <int> result = a;
        for(int i = 0;i<p-1;i++){
            result = multiply(result,a);
        }
        return result;
    }
};
int main(){
//    vector <int> irr_poly(128,0);
//    irr_poly[0] = 1;irr_poly[1] = 1;irr_poly[127] = 1;

    int m = 4;
//    cin >> m;
    vector <int> irr_poly(m,0);
    irr_poly[0]=1;irr_poly[1] = 1;irr_poly[3] = 1;
    finiteFiled f = finiteFiled(m-1,irr_poly);
    vector <int> a(m,0);
    vector<int> b(m,0);
    a[0]=0;a[1] =1;a[2]=1;
    b[0]=0;b[1] =1;b[2]=1;
    vector <int> re = f.multiply(a,b);
    for(int i = 0;i<m;i++){
        cout << re[i]<<' ';
    }
    cout << endl;
    //f.pow(17340023,20190911);
}
