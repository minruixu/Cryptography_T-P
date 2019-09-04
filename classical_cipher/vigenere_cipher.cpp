#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
// 使用重合检测法验证这个m是否正确

double all_fqc[26] = {0.082,0.015,0.028,0.043,0.127,0.022,0.020,0.061,0.070,0.002,0.008,0.040,0.024,0.067,0.075,0.019,0.001,0.060,0.063,0.091,0.028,0.010,0.023,0.001,0.020,0.001};
bool check_m(string en,int m){
    return true;
}

int find_m(string en){
	for(int i = 0;i<en.length();i++){
	    int m = 0; // 周期长度
		string c_en = en;
		string k_str = en.substr(i,3);
		while(c_en.find(k_str)!=-1){
		    int t = c_en.find(k_str);
		    if(t!=0)
		    cout << t << endl;
		    c_en[t] = '-';
	    }
	}
}
int get_key(string en,int m,int no){
	int g = 0;
	int g_pro = 0;
	double fqc[26] = {0};
	for(int i = no;i<en.length();i+=26){
		fqc[en[i]-'a'] += 1;
	}
	for(int i=0;i<26;i++){
	    fqc[i]/(en.length/m);
	}

}

int get_gcd(vector<int> ve){
	int d = ve[0];
	for(int i = i;i<ve.length();i++){
	    d = gcd(d,ve[i]);
	}
	return d;
}
int gcd(int x, int y){
     if(y == 0) return x;    
     if(x < y)      return gcd(y,x);    
     else        return gcd(y, x%y); 
}

int main(){
    string en = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST";
	for(int i = 0;i<en.length();i++){
	    int m = 0; // 周期长度
		string c_en = en;
		string k_str = en.substr(i,3);
		while(c_en.find(k_str)!=-1){
		    int t = c_en.find(k_str);
		    if(t!=0)
		    cout << t << endl;
		    c_en[t] = '-';
	    }
	}
	
}
