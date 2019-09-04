#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
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
