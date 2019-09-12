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
    void print_M(vector<vector<int> > m){
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                cout << m[i][j]<<" ";
            }
            cout << endl;
        }
    }
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
        cout << "x1："<<endl;
        print_M(x1);
        cout << "x2："<<endl;
        print_M(x2);
        cout << "y1："<<endl;
        print_M(y1);
        cout << "y2："<<endl;
        print_M(y2);
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                x1[i][j] = (x1[i][j] + addInv(x2[i][j]))%26;
                y1[i][j] = (y1[i][j] + addInv(y2[i][j]))%26;
            }
        }
        cout << "消去仿射矩阵b后得到名秘闻矩阵"<<endl;
        cout <<"X:"<<endl;
        print_M(x1);
        cout <<"Y:"<<endl;
        print_M(y1);
        x1 = getInvLinear(x1,_m);
        cout <<"对X求逆后,X的逆为:"<<endl;
        print_M(x1);
//        cout <<"+"<<endl;
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
        cout << "得到的加密矩阵为:"<<endl;
        for(int i = 0;i<_m;i++){
            for(int j = 0;j<_m;j++){
                cout << _Lmatrix[i][j]<<" ";
            }
            cout << endl;
        }
        cout << "得到的仿射矩阵为:"<<endl;
        for(int j = 0;j<_m;j++){
            cout << _b[j]<<" ";
        }
        cout << endl;
        return true;
    }
    string encipher(string plaintext){
        if(plaintext.length()%_m!=0) return "length error";
        string ciphertext = "";
        for(int i = 0;i<plaintext.length()/_m;i++){
            vector <int> t;
            for(int j = 0;j<_m;j++){
                t.push_back(plaintext[i*_m+j]-'a');
            }
            for(int j = 0;j<_m;j++){
                int num = 0;
                for(int k = 0;k<_m;k++){
                    num += t[k]*_Lmatrix[k][j];
                }
                num = (num+_b[j]) %26;
                ciphertext += char(num+'A');
            }
        }
        return ciphertext;
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
    cout << "使用分析得到的加密矩阵和仿射矩阵，对明文进行加密的结果为:"<<endl;
    cout << "明文:"<<plaintext<<endl;
    cout << "密文:" <<a.encipher(plaintext)<<endl;
}
