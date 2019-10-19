//
// Created by 406 on 2019/10/15.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

typedef bitset<8> byte;
typedef bitset<32> word;
byte S_Box[16][16] =
    { /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, /*0*/
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, /*1*/
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, /*2*/
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, /*3*/
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, /*4*/
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, /*5*/
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, /*6*/
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, /*7*/
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, /*8*/
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, /*9*/
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, /*a*/
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, /*b*/
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, /*c*/
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, /*d*/
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, /*e*/
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16  /*f*/
};
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000};
string byte2string(byte a){
    string re;
    byte a1 = a>>4;
    byte a2 = (a << 4)>>4;
    int i1 = a1.to_ulong();
    int i2 = a2.to_ulong();
    if(i1<10) re = re + char('0'+i1);
    else re = re + char('a'+i1-10);
    if(i2<10) re = re + char('0'+i2);
    else re = re + char('a'+i2-10);
    return re;
}
class AES{
private:
    string _key;
    vector <word> _w;
    byte _shift;
    byte IV;
public:
    AES(string key,byte shift){
        _key = key;
        _shift = shift;
        _w.resize(44);
        KeyExpansion(_key);
    }
    word Word(byte& k1, byte& k2, byte& k3, byte& k4){
        //将四个字节合并为一个word
        word result(0x00000000);
        word temp;
        temp = k1.to_ulong();
        temp <<= 24;
        result |= temp;
        temp = k2.to_ulong();
        temp <<= 16;
        result |= temp;
        temp = k3.to_ulong();
        temp <<= 8;
        result |= temp;
        temp = k4.to_ulong();
        result |= temp;
        return result;
    }
    word RotWord(word rw) {
        word high = rw << 8;
        word low = rw >> 24;
        return high | low;
    }
    word SubWord(word sw){
        word temp;
//        cout << "+"<<endl;
//        cout << hex<<sw.to_ulong()<<endl;
        for(int i=0; i<32; i+=8)
        {
            int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];
            int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i];
            byte val = S_Box[row][col];
            for(int j=0; j<8; ++j)
                temp[i+j] = val[j];
        }
        return temp;
    }
    void KeyExpansion(string skey)
    {
        byte key[16];
        // 输入一个string 的key，生成轮密钥
        for(int i = 0;i<16;i++){
            if(i<skey.length()){
                key[i] = skey[i];
            }
            else key[i] = 0x00;
        }
        word temp;
        int i = 0;
        // w[]的前4个就是输入的key
        for(i =0;i<4;i++){
            _w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
        }
        for(i=4;i<44;i++){
            temp = _w[i-1]; // 记录前一个word
            if(i % 4 == 0)
                _w[i] = _w[i-4] ^ SubWord(RotWord(temp)) ^ Rcon[i/4-1];
            else
                _w[i] = _w[i-4] ^ temp;
        }
        for(int i = 0;i<44;i++){
            if(i%4 == 0) cout << endl;
            cout << hex << _w[i].to_ulong();
        }
    }

    vector<byte> SubBytes(vector <byte> mtx){
        /*
         * input: 长度为16byte 的vector
         * 进行字节替换，输出长度为16byte的vector
         */
        for(int i=0; i<16; ++i){
            int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];
            int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];
            mtx[i] = S_Box[row][col];
        }
        return mtx;
    }
    vector <byte> ShiftRows(vector <byte> mtx){
        // 第二行循环左移一位
        byte temp = mtx[4];
        for(int i=0; i<3; i++)
            mtx[i+4] = mtx[i+5];
        mtx[7] = temp;
        // 第三行循环左移两位
        for(int i=0; i<2; i++){
            temp = mtx[i+8];
            mtx[i+8] = mtx[i+10];
            mtx[i+10] = temp;
        }
        // 第四行循环左移三位
        temp = mtx[15];
        for(int i=3; i>0; i--)
            mtx[i+12] = mtx[i+11];
        mtx[12] = temp;
        return mtx;
    }
    byte GFMul(byte a, byte b) {
        byte p = 0; // 结果
        byte hi_bit_set;
        for (int counter = 0; counter < 8; counter++) {
            if ((b & byte(1)) ==1) {
                p ^= a;
            }
            hi_bit_set = (byte) (a & byte(0x80)); // a and 10000000
            a <<= 1; // a 向上移一位
            if (hi_bit_set != 0) {
                a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
            }
            b >>= 1;
        }
        return p;
    }
    vector <byte> MixColumns(vector <byte> mtx){
        byte t[4];
        for(int i=0; i<4; i++){ // 对第i列进行混合
            for(int j=0; j<4; ++j)
                t[j] = mtx[i+j*4];
            mtx[i] = GFMul(0x02, t[0]) ^ GFMul(0x03, t[1]) ^ t[2] ^ t[3];
            mtx[i+4] = GFMul(0x02, t[1]) ^ GFMul(0x03, t[2]) ^ t[0]  ^ t[3] ;
            mtx[i+8] = GFMul(0x02, t[2]) ^ GFMul(0x03, t[3]) ^ t[0] ^ t[1];
            mtx[i+12] = GFMul(0x02, t[3]) ^ GFMul(0x03, t[0]) ^ t[1] ^ t[2];
        }
        return mtx;
    }
    vector<byte> AddRoundKey(vector <byte> mtx, vector <word> k){
        for(int i=0; i<4; ++i){
            word k1 = k[i] >> 24;
            word k2 = (k[i] << 8) >> 24;
            word k3 = (k[i] << 16) >> 24;
            word k4 = (k[i] << 24) >> 24;

            mtx[i] = mtx[i] ^ byte(k1.to_ulong());
            mtx[i+4] = mtx[i+4] ^ byte(k2.to_ulong());
            mtx[i+8] = mtx[i+8] ^ byte(k3.to_ulong());
            mtx[i+12] = mtx[i+12] ^ byte(k4.to_ulong());
        }
        return mtx;
    }

    string encryption(string message){
        //128位 每次加密16个字符
        string result;
        int N = (16-message.length()%16)%16;
        vector <byte> msg;
        // 将string 的msg转变为a的
        for(int i = 0;i<message.length();i++){
            byte tmp = message[i];
            msg.push_back(tmp);
        }
        for(int i = 0;i<N;i++){
            byte tmp = N;
            msg.push_back(N);
        }
//        for(int i = 0;i<16;i++) msg.push_back(0x00);
        cout << "plain";
        for(int i = 0;i<msg.size();i++){
            cout << byte2string(msg[i]);
        }
        cout << endl;
        // 对msg进行加密
        int m = msg.size()/16;
        vector <byte> byte_result;
        for(int i = 0;i<m;i++){
            vector <byte> pt_msg(16);
            for(int j = 0;j<4;j++){
                for(int k = 0;k<4;k++){
                    pt_msg.push_back(msg[i*16 + k*4 + j]);
                }
            }
            vector <byte> tmp = encry(vector <byte> (msg.begin()+16*i,msg.begin()+16+16*i));
            for(int j = 0;j<4;j++){
                for(int k = 0;k<4;k++){
                     byte_result.push_back(tmp[j+k*4]);
                }
            }
        }
        cout << byte_result.size()<<endl;
        cout << "encipher:";
        for(int i = 0;i<byte_result.size();i++){
            cout << byte2string(byte_result[i]);
        }
        cout << endl;
        //将用byte表示的密文转化为十六进制和字符串。
        return result;
    }
    vector <byte> encry(vector <byte> x){
//        for(int i = 0;i<16;i++) cout << char('a'+x[i].to_ulong());
        // 对一个16byte的x进行加密，返回加密的结果
        vector <word> round_key;
        round_key = vector<word>{_w[0],_w[11],_w[22],_w[33]};
        x = AddRoundKey(x,vector <word> (_w.begin(),_w.begin()+4));
        cout << "add"<<endl;
        for(int i = 0;i<x.size();i++){
            cout << byte2string(x[i]);
        }
        cout << endl;
        for(int i = 1;i<=10;i++){
            x = SubBytes(x);
            cout << "sub"<<endl;
            for(int i = 0;i<x.size();i++){
                cout << byte2string(x[i]);
            }
            cout << endl;
            x = ShiftRows(x);
            cout << "shift"<<endl;
            for(int i = 0;i<x.size();i++){
                cout << byte2string(x[i]);
            }
            cout << endl;
            if(i!=10) x = MixColumns(x);
            cout << "mix"<<endl;
            for(int i = 0;i<x.size();i++){
                cout << byte2string(x[i]);
            }
            cout << endl;
            //round_key = vector<word>{_w[i],_w[11+i],_w[22+i],_w[33+i]};
            x = AddRoundKey(x,vector <word> (_w.begin()+i*4,_w.begin()+4+i*4));
            cout << "add"<<endl;
            for(int i = 0;i<x.size();i++){
                cout << byte2string(x[i]);
            }
            cout << endl;
        }
        return x;
    }
};

int main(){
//    string message = "ilearnedhowtocalculatetheamountofpaperneededforaroomwheniwasatschoolyoumultiplythesquarefootageofthewallsbythecubiccontentsofthefloorandceilingcombinedanddoubleityouthenallowhalfthetotalforopeningssuchaswindowsanddoorsthenyouallowtheotherhalfformatchingthepatternthenyoudoublethewholethingagaintogiveamarginoferrorandthenyouorderthepaper";
    string message = "b";
    string key = "aaaaaaaaaaaaaaaa";
    byte shift_byte = 123; //  CBC 模式下的初始向量
    AES a = AES(key,shift_byte);
    a.encryption(message);
}
