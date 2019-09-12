#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;
// 使用重合检测法验证这个m是否正确

double all_fqc[26] = {0.082,0.015,0.028,0.043,0.127,0.022,0.020,0.061,0.070,0.002,0.008,0.040,0.024,0.067,0.075,0.019,0.001,0.060,0.063,0.091,0.028,0.010,0.023,0.001,0.020,0.001};

class VigenereCipher{
private:
    int _z;
    vector <int> _k;
public:
    VigenereCipher(){
        _z = 0;
    }
    int gcd(int x, int y){
        if(y == 0) return x;
        if(x < y) return gcd(y,x);
        else  return gcd(y, x%y);
    }
    int get_gcd(vector<int> ve){
        int d = ve[0];
        for(int i = 1;i<ve.size();i++){
            d = gcd(d,ve[i]);
        }
        return d;
    }
    double check_m(string en,int m){
        double exp = 0.065;
        vector <string> sstr;
        for(int i =0;i<m;i++){
            sstr.push_back("");
        }
        for(int i = 0;i<en.length();i++){
            sstr[i%m] += en[i];
        }
        double all_fre = 0;
        for(int i = 0;i<m;i++){
            // 计算频率
            for(char c = 'A';c<='Z';c++){
                all_fre += pow(count(sstr[i].begin(),sstr[i].end(),c)/double(sstr[i].length()),2);
            }
        }
        return all_fre/m;
    }
    int find_m(string en,string k_en){
        // 寻找周期的长度，如果找到就返回周期，如果没找到就返回-1
        string c_en = en;
        string s_re = "";
        for(int i = 0;i<k_en.length();i++){
            s_re += '-';
        }
        vector <int> pos;
        while(en.find(k_en)!=en.npos){
            pos.push_back(en.find(k_en));
            en = en.replace(pos[pos.size()-1],k_en.length(),s_re);
        }
        vector <int> dis;
        for(int i = 0;i<pos.size()-1;i++){
            dis.push_back(pos[i+1]-pos[i]);
        }
        if(dis.size() == 0) return -1;
        int m = get_gcd(dis);
        return m;
    }
    vector <int> get_key(string en,int m){
        vector <int> key;
        vector <string> sstr;
        for(int i =0;i<m;i++){
            sstr.push_back("");
        }
        for(int i = 0;i<en.length();i++) {
            sstr[i % m] += en[i];
        }
        for(int i = 0;i<m;i++){
            // 计算频率
            int b_k = 0;
            double k_loss = 1;
            for(int k = 0;k<26;k++){
                double all_fre = 0;
                for(char c = 'A';c<='Z';c++){
                    all_fre += all_fqc[c-'A']*(count(sstr[i].begin(),sstr[i].end(),char((c-'A'+k)%26+'A'))/double(sstr[i].length()));
                }
                if(abs(all_fre-0.065)<k_loss){
                    b_k = k;
                    k_loss = abs(all_fre-0.065);
                }
            cout << all_fre<< " ";
            }
        cout << endl;
            key.push_back(b_k);
        }
        return key;
    }
    bool analysis(string en){
        map <string,int> has;
        int m = -1;
        double m_loss = 1;
        int b_m = -1;
        int b_i = -1;
        for(int i = 0;i<en.length()-3+1;i++){
            string k_str = en.substr(i,3);
            if(has.count(k_str)) continue;
            has[k_str] = 1;
            m = find_m(en,k_str);
            if(m!=-1&&m_loss > check_m(en,m)){
                m_loss = check_m(en,m);
                b_m = m;
                b_i = i;
            }
            // 找一个最接近0.065 的
        }
        if(b_m==-1) return false;
        else{
//            cout << "字符串:" << en.substr(b_i,3)<<endl;
//            cout << "重合指数:" << check_m(en,b_m)<<endl;
            _z = b_m;
            _k = get_key(en,b_m);
            return true;
        }
    }
    string decipher(string en){
        string plaintext = "";
        for(int i = 0;i<en.length();i++){
            plaintext += char((en[i]-'A'+26-_k[i%_z])%26+'A');
        }
        return plaintext;
    }
    void print_key(){
        cout << "密钥的长度是:" <<_z<<endl;
        cout << "密钥为:(";
        for(int i = 0;i<_z;i++){
            cout << _k[i];
            if(i!=_z-1) cout << ",";
        }
        cout << ")"<<endl;
    }
};

int main(){
    VigenereCipher c = VigenereCipher();
    string ciphertext = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
    // 获得m后，计算密钥
    if(c.analysis(ciphertext))
    cout << c.decipher(ciphertext) << endl;
    else cout << "failed";
    c.print_key();
}
