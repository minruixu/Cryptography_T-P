#include <iostream>
#include <string>
#include <vector>
#include <map>
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
    for(int i = i;i<ve.size();i++){
        d = gcd(d,ve[i]);
    }
    return d;
}
bool check_m(string en,int m){
    // 是u用  
    return true;
}
int find_m(string en,string k_en){
    // 寻找周期的长度，如果找到就返回周期，如果没找到就返回-1
    string c_en = en;
    vector <int> pos;
    while(en.find(k_en)!=en.npos){
        pos.push_back(en.find(k_en));
        en = en.replace(pos[pos.size()-1],3,"");
    }
    vector <int> dis;
    for(int i = 0;i<pos.size()-1;i++){
        dis.push_back(pos[i+1]-pos[i]);
    }
    if(dis.size() == 0) return -1;
    int m = get_gcd(dis);
    if (check_m(c_en,m)) return m;
    else return -1;
}
int get_key(string en,int m,int no){
	int g = 0;
	int g_pro = 0;
	double fqc[26] = {0};
	for(int i = no;i<en.length();i+=26){
		fqc[en[i]-'a'] += 1;
	}
	for(int i=0;i<26;i++){
	    fqc[i]/(en.length()/m);
	}
	return 0;
}


int main(){
    string en = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
    int m = -1;
    for(int i = 0;i<en.length()-3;i++){
        string k_str = en.substr(i,3);
        m = find_m(en,k_str);
        if(m!=-1) break;
	}
    cout << m << endl;
	// 获得m后，计算密钥
}
