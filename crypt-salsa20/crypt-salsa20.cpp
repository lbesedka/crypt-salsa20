#include <iomanip>
#include <sstream>
#include <Windows.h>
#include <vector>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <iostream>
#define rotate_left(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
using namespace std;

vector<unsigned int> quarterround(vector<unsigned int> y) {
    unsigned int z1 = y[1] ^ rotate_left((y[0] + y[3]), 7);
    unsigned int z2 = y[2] ^ rotate_left((z1 + y[0]), 9);
    unsigned int z3 = y[3] ^ rotate_left((z2 + z1), 13);
    unsigned int z0 = y[0] ^ rotate_left((z3 + z2), 18);
    return vector<unsigned int>{z0, z1, z2, z3};
}

vector<vector<unsigned int>> rowround(vector<vector<unsigned int>> y) {
    vector<unsigned int> z[4];
    vector<unsigned int> res[4]{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    
    z[0] = quarterround(vector<unsigned int> {y[0][0], y[0][1], y[0][2], y[0][3]});
    res[0] = z[0];

    z[1] = quarterround(vector<unsigned int> {y[1][1], y[1][2], y[1][3], y[1][0]});
    res[1] = { z[1][3], z[1][0], z[1][1],  z[1][2]};

    z[2] = quarterround(vector<unsigned int> {y[2][2], y[2][3], y[2][0], y[2][1]});
    res[2] = {z[2][2], z[2][3],  z[2][0],  z[2][1]};

    z[3] = quarterround(vector<unsigned int> {y[3][3], y[3][0], y[3][1], y[3][2]});
    res[3] = {z[3][1], z[3][2],  z[3][3],  z[3][0]};

    return vector<vector<unsigned int>> {res[0], res[1], res[2], res[3]};
}

vector<vector<unsigned int>> columnround(vector<vector<unsigned int>> y) {
    vector<unsigned int> z[4];
    vector<unsigned int> res[4]{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    z[0] = quarterround(vector<unsigned int> {y[0][0], y[1][0], y[2][0], y[3][0]});
    z[1] = quarterround(vector<unsigned int> {y[1][1], y[2][1], y[3][1], y[0][1]});
    z[2] = quarterround(vector<unsigned int> {y[2][2], y[3][2], y[0][2], y[1][2]});
    z[3] = quarterround(vector<unsigned int> {y[3][3], y[0][3], y[1][3], y[2][3]});
    res[0] = {z[0][0], z[1][3], z[2][2], z[3][1] };
    res[1] = { z[0][1], z[1][0], z[2][3], z[3][2] }; 
    res[2] = { z[0][2], z[1][1], z[2][0], z[3][3] };
    res[3] = { z[0][3], z[1][2], z[2][1], z[3][0] };
    return vector<vector<unsigned int>> {res[0], res[1], res[2], res[3]};
}

vector<vector<unsigned int>> doubleround(vector<vector<unsigned int>> y) {
    return rowround(columnround(y)); 
}

unsigned int littleendian(vector<unsigned char> b) {
    return (b[0] + b[1] * (1 << 8) + b[2] * (1 << 16) + b[3] * (1 << 24));
}

vector<unsigned char> littleendian_1(unsigned int b) {
    vector<unsigned char> result = { 0, 0, 0, 0 };
    result[0] = b;
    result[1] = b >> 8;
    result[2] = b >> 16;
    result[3] = b >> 24;
    return result;
}


vector<vector<unsigned char>> salsa20(vector<unsigned char> b) {
    vector<vector<unsigned int>> z = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
    vector<vector<unsigned char>> result = { { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 },
                                             { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 },
                                             { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 },
                                             { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0,0,0,0 } };
    unsigned char vedro[4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            vector<unsigned char> seq = { b[i * 16 + j * 4], b[i * 16 + j * 4 + 1],
                b[i * 16 + j * 4 + 2], b[i * 16 + j * 4 + 3] };
            z[i][j] = littleendian(seq);
        }
    vector<vector<unsigned int>> x = z;
    for (int i = 0; i < 10; i++)
        z = doubleround(z);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
        {
            z[i][j] += x[i][j];
            result[i*4 + j] = littleendian_1(z[i][j]);
        }


    return result; 
}


int main()
{
   /* vector<vector<unsigned int>> A = { {0x00000001, 0x00000000, 0x00000000, 0x00000000},
    {0x00000000, 0x00000000, 0x00000000, 0x00000000 },
        {0x00000000, 0x00000000, 0x00000000, 0x00000000},
        {0x00000000, 0x00000000, 0x00000000, 0x00000000} };
    vector<vector<unsigned int>> B = doubleround(A);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << B[i][j] << endl; 
        }
    }*/
    /*vector<unsigned char> A = {86, 75, 30, 9};
    unsigned int B = littleendian(A);
    cout << B << endl; */
    /*vector<unsigned int> A = {0x00000001, 0x00000000, 0x00000000, 0x00000000 };
    vector<unsigned int> B = quarterround(A);
    for (int i = 0; i < 4; i++) {
        cout << B[i] << endl;
    }*/
    vector<unsigned char> A = { 211,159, 13,115, 76, 55, 82,183, 3,117,222, 37,191,187,234,136,
        49,237,179, 48, 1,106,178,219,175,199,166, 48, 86, 16,179,207,
        31,240, 32, 63, 15, 83, 93,161,116,147, 48,113,238, 55,204, 36,
        79,201,235, 79, 3, 81,156, 47,203, 26,244,243, 88,118,104, 54 };
    vector<vector<unsigned char>> B = salsa20(A);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 4; j++) {
            cout << (int)B[i][j] << " ";
        }
        cout << endl;
    }

}

