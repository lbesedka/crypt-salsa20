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
    res[1] = { z[1][1], z[1][2], z[1][3],  z[1][0]};

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

int main()
{
    vector<vector<unsigned int>> A = { {0x00000001, 0x00000000, 0x00000000, 0x00000000 },
    { 0x00000001, 0x00000000, 0x00000000, 0x00000000 },
        { 0x00000001, 0x00000000, 0x00000000, 0x00000000},
        {0x00000001, 0x00000000, 0x00000000, 0x00000000 } };
    vector<vector<unsigned int>> B = columnround(A);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << B[i][j] << endl; 
        }
    }
    /*vector<unsigned int> A = {0x00000001, 0x00000000, 0x00000000, 0x00000000 };
    vector<unsigned int> B = quarterround(A);
    for (int i = 0; i < 4; i++) {
        cout << B[i] << endl;
    }*/

}

