#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;
// 使用重合检测法验证这个m是否正确

double all_fqc[26] = {0.082,0.015,0.028,0.043,0.127,0.022,0.020,0.061,0.070,0.002,0.008,0.040,0.024,0.067,0.075,0.019,0.001,0.060,0.063,0.091,0.028,0.010,0.023,0.001,0.020,0.001};
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
    cout << "+"<<endl;
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
//            cout << all_fre<< " ";
        }
//        cout << endl;
        key.push_back(b_k);
    }
    return key;
}

//class VigenereCipher{
//private:
//    string _ciphertext;
//    string _plaintext;
//    int _len;
//    double _z = z;
//
//public:
//    VigenereCipher(string en,int len,double z){
//        _ciphertext = en;
//        _len = len;
//        _z = z;
//    }
//    vector <int> decipher(string en,double z){
//
//    }
//};

int main(){
    string en = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
    map <string,int> has;
    int m = -1;
    double m_loss = 1;
    int b_m = -1;
    for(int j = 3;j<4;j++)
    for(int i = 0;i<en.length()-j+1;i++){
        string k_str = en.substr(i,j);
        if(has.count(k_str)) continue;
        has[k_str] = 1;
        m = find_m(en,k_str);
        if(m!=-1&&m_loss > check_m(en,m)){
            m_loss = check_m(en,m);
            b_m = m;
        }
        // 找一个最接近0.065 的
	}
    cout << b_m << endl;
	// 获得m后，计算密钥
	vector <int> key = get_key(en,b_m);
	for(int i = 0;i<key.size();i++){
//	    cout << key[i]<<endl;
	    cout << char(key[i]+'A');
	}
	cout <<endl;
	// 解密
    string plaintext = "";
	for(int i = 0;i<en.length();i++){
	    plaintext += char((en[i]-'A'+26-key[i%b_m])%26+'A');
	}
	cout <<plaintext<<endl;
}
