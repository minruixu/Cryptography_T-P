//
// Created by yirui on 19-9-5.
//

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
using namespace std;
class affineHill{
private:
    int _m;
    vector <vector<int> > _Lmatrix;
    vector <int> _b;
public:
    affineHill(int m){
        _m = m;
        _b = vector<int> (m,0);
        _Lmatrix = vector <vector<int> > (m);
        for(int i = 0;i<m;i++){
            _Lmatrix[i].resize(m);
        }
    }
    vector<vector<int> > getL(){
        return _Lmatrix;
    }
    vector<int> getb(){
        return _b;
    }
    /*
    int computeDet(vector<vector<int> > ma,int n){
        //按第一行展开计算|A|
        if(n==1) return ma[0][0];
        int det = 0;
        vector<vector<int> > temp(n);
        int i,j,k;
        for(i=0;i<n;i++)
        {
            for(j=0;j<n-1;j++)
            {
                for(k=0;k<n-1;k++)
                {
                    temp[j][k] = ma[j+1][(k>=i)?k+1:k];
                }
            }
            double t = computeDet(ma,n-1);
            if(i%2==0)
            {
                det += ma[0][i]*t;
            }
            else
            {
                det -=  ma[0][i]*t;
            }
        }
    }


    vector<vector<int> > computeAdjoint(vector<vector<int> > ma,int n){
        if(n==1)
        {
            vector<vector<int> ans(1);
            ans[0][0] = 1;
            return ans;
        }
        int i,j,k,t;
        vector<vector<int> temp(n);
        vector<vector<int> ans(n);
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                for(k=0;k<n-1;k++)
                {
                    for(t=0;t<n-1;t++)
                    {
                        temp[k][t] = ma[k>=i?k+1:k][t>=j?t+1:t];
                    }
                }
                ans[j][i]  =  getA(temp,n-1);
                if((i+j)%2 == 1)
                {
                    ans[j][i] = (26 - ans[j][i]) %26;
                }
            }
        }
    }
整数矩阵的LU分解失败
    vector <vector<int> > getInverse(vector <vector<int> > a,int n){
        //使用LU分解求矩阵的逆
        vector <vector<int> > L(n),l(n);
        vector <vector<int> > U(n),u(n);
        vector <vector<int> > out(n);
        for(int i = 0;i<n;i++){
            U[i].resize(n);
            u[i].resize(n);
            L[i].resize(n);
            l[i].resize(n);
            out[i].resize(n);
        }
        int flag = 1;
        int i,j,k;
        int s=0,t=0;

        // U矩阵的第一行
        for(i = 0;i<n;i++) a[0][i] = a[0][i];
        // L矩阵的第一列
        for(i = 1;i<n;i++) {
            a[i][0] = getMultiEqual(a[0][0],a[i][0]);
        }
        for(k = 1;k<n;k++){
            for(j = k;j<n;j++){
                s = 0;
                for(i = 0;i<k;i++)
                    s = s+a[k][i]*a[i][j]; //累加
                a[k][j] = (a[k][j] - s)%26;// 计算U矩阵的其他元素
            }
            for(i = k+1;i<n;i++){
                t = 0;
                for(j=0;j<k;j++)
                    t = t + a[i][j]*a[j][k];//累加
                    a[i][k] = getMultiEqual(a[i][k]-t,a[k][k]);
                    //a[i][k] = ((a[i][k]-t)/a[k][k])%26; //计算L矩阵的其他元素
            }
        }
        for(int i = 0;i<3;i++){
            for(int j = 0;j<3---;j++){
                cout << a[i][j]<<" ";
            }
            cout << endl;
        }
        for(i = 0;i<n;i++) {
            for (j = 0; j < n; j++) {
                if (i > j) {
                    //如果i>j,计算矩阵的下三角部分，得出L
                    L[i][j] = a[i][j];
                    U[i][j] = 0;
                } else {
                    //计算矩阵的shang三角部分，得出U
                    U[i][j] = a[i][j];
                    if (i == j)
                        L[i][j] = 1;
                    else
                        L[i][j] = 0;
                }
            }
        }
        cout << "L" <<endl;
        for(int i = 0;i<2;i++){
            for(int j = 0;j<2;j++){
                cout << L[i][j]<<" ";
            }
            cout << endl;
        }
        cout << "U" <<endl;
        for(int i = 0;i<3;i++){
            for(int j = 0;j<3;j++){
                cout << U[i][j]<<" ";
            }
            cout << endl;
        }
        for(int i = 0;i<n;i++){
            if(U[i][i] == 0) {flag = 0;
            cout << "逆矩阵不存在"<<endl;}
        }
        if(flag ==1){
            //求L和U的逆
            for(i = 0;i<n;i++){
                //求矩阵u的逆
                u[i][i] = 1/U[i][i];
                for(k = i-1;k>=0;k--){
                    s = 0;
                    for(j=k+1;j<=i;j++) s = s+U[k][j]*u[j][i];
                    //迭代计算，按列倒序依次取每一个值
                    u[k][i] = - s/U[k][k];
                }
            }
            for(i = 0;i<n;i++){
                //求L矩阵的逆
                l[i][i] = 1;
                for(k=i+1;k<n;k++){
                    for(j = i;j<=k;j++) l[k][i] = l[k][i] - L[k][j]*l[j][i];
                }
            }
        }
        //将r u 相乘，得到逆矩阵
        for(int i = 0;i<n;i++){
            for(int j = 0;j<n;j++){
                for(int k = 0;k<n;k++){
                    out[i][j] += u[i][k]*l[k][j];
                }
            }
        }
        return out;
    }*/
    int addInv(int a){
        return 26-a;
    }
    int multiInv(int a){
        for(int i = 1;i<=26;i++){
            if(a*i%26==1) return i;
        }
        //没有乘逆
        return -1;
    }
    int findXishu(int a,int b){
        for(int i = 1;i<=26;i++){
            if(a*i%26==b) return i;
        }
        //没有乘逆
        return -1;
    }
    int gcd(int x, int y){
        if(y == 0) return x;
        if(x < y) return gcd(y,x);
        else  return gcd(y, x%y);
    }
// 初等行变换
    vector <vector<int> > getInvLinear(vector<vector<int> > a,int n){
        vector<vector<int> > e(n);
        for(int i = 0;i<n;i++) e[i].resize(2*n);
        for(int i = 0;i<n;i++){
            for(int j = 0;j<n;j++){
                e[i][j] = a[i][j];
            }
            e[i][i+n] = 1;
        }
        //进行初等行变换
        int xishu = 1;
        for(int i = 0;i<n;i++){
            if(e[i][i] == 0||gcd(e[i][i],26)!=1){
                //如果对焦点上的元素为0
                if(i==n-1) {
                    cout << "矩阵不可逆"<<endl;
                    return vector<vector<int>>(1);
                }
                //对第i行以后的各行进行判断，找到第i个元素不为0的行，并进行交换
                for(int j = 1;j<n;j++){
                    if(e[j][i]!=0){
                        vector <int> temp = e[j];
                        e[j] = e[i];
                        e[i] = temp;
                        break;
                    }
                }
            }
            //进行初等变换
            for(int j = 0;j<n;j++){
                //对其他行的列进行计算,
                if(j!=i){
                    if(e[j][i] !=0){
                        xishu = findXishu(e[i][i],26-e[j][i]);
                        for(int k = i;k<2*n;k++){
                            e[j][k] = (e[j][k] + xishu*e[i][k]) %26;
                        }
                    }
                }
            }
            //将本行的所有列除对角线上的值，将牵绊部分化成单位矩阵
            xishu = multiInv(e[i][i]);
//            cout << i<<"row"<<xishu<<endl;
            for(int j = i;j<2*n;j++){
                e[i][j] = (e[i][j]*xishu)%26;
            }
        }
        //finish
        vector<vector<int> > out(n);
        for(int i = 0;i<n;i++){
            out[i].resize(n);
            for(int j = 0;j<n;j++){
                out[i][j] = (e[i][j+n]+26)%26;
            }
        }
        //求得一个矩阵的逆
        return out;
    }
    bool analysis(string plaintext,string ciphertext){
        // use 明文和密文更新矩阵
        vector <vector<int> > x1(_m),x2(_m),y1(_m),y2(_m);
        for(int i = 0;i<_m;i++){
            x1[i].resize(_m);
            y1[i].resize(_m);
            x2[i].resize(_m);
            y2[i].resize(_m);
        }
        if(plaintext.length()<2*(_m*_m)||ciphertext.length()<2*(_m*_m)) return false;
        //将字符串转发成数字装入数组中
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                x1[i][j] = plaintext[i*_m+j]-'a';
                x2[i][j] = plaintext[(_m*_m)+i*_m+j]-'a';
                y1[i][j] = ciphertext[i*_m+j]-'A';
                y2[i][j] = ciphertext[(_m*_m)+i*_m+j]-'A';
            }
        }
//        for(int i = 0;i<_m;i++){
//            for(int j = 0;j<_m;j++){
//                cout << x2[i][j]<<" ";
//            }
//            cout << endl;
//        }
//        for(int i = 0;i<_m;i++){
//            for(int j = 0;j<_m;j++){
//                cout << y2[i][j]<<" ";
//            }
//            cout << endl;
//        }
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                x1[i][j] = (x1[i][j] + addInv(x2[i][j]))%26;
                y1[i][j] = (y1[i][j] + addInv(y2[i][j]))%26;
            }
        }
        x1 = getInvLinear(x1,_m);
        cout <<"+"<<endl;
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                int t = 0;
                for(int k = 0;k<_m;k++){
                    t += x1[i][k] * y1[k][j];
                }
                _Lmatrix[i][j] = t%26;
            }
        }
        //用x2,y2得到b
        x1 = getInvLinear(x1,_m);
//        for(int i = 0;i<_m;i++){
//            for(int j = 0;j<_m;j++){
//                cout << x1[i][j]<<" ";
//            }
//            cout << endl;
//        }
        for(int i = 0;i<_m;i++){
            int t = 0;
            for(int j = 0;j<_m;j++){
                t += x2[0][j] * _Lmatrix[j][i];
            }
            _b[i] = (y2[0][i] + addInv(t%26))%26;
        }
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                cout << _Lmatrix[i][j]<<" ";
            }
            cout << endl;
        }
        for(int j = 0;j<_m;j++){
            cout << _b[j]<<" ";
        }
        cout << endl;
        return true;
    }
};
int main(){
    string plaintext = "adisplayedequation";
    string ciphertext = "DSRMSIOPLXLJBZULLM";
    int m =  3;
    affineHill a = affineHill(m);
    //vector <vector<int> > mm = {{10,5,12},{3,14,21},{8,9,11}};
//    vector <vector<int> > mm = {{11,8},{3,7}};
    a.analysis(plaintext,ciphertext);
}
