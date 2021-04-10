
int Weight_Matrix[4][32];

void init_weight_matrix() {
Weight_Matrix[0][0]  = 0b1111111111010101;
Weight_Matrix[0][1]  = 0b1111111110110110;
Weight_Matrix[0][2]  = 0b0000000001001011;
Weight_Matrix[0][3]  = 0b0000000000111100;
Weight_Matrix[0][4]  = 0b1111111110100101;
Weight_Matrix[0][5]  = 0b0000000001111101;
Weight_Matrix[0][6]  = 0b1111111100110010;
Weight_Matrix[0][7]  = 0b0000000001001101;
Weight_Matrix[0][8]  = 0b0000000000111010;
Weight_Matrix[0][9]  = 0b0000000001000000;
Weight_Matrix[0][10] = 0b1111111111001001;
Weight_Matrix[0][11] = 0b1111111111000101;
Weight_Matrix[0][12] = 0b1111111111111000;
Weight_Matrix[0][13] = 0b0000000000001000;
Weight_Matrix[0][14] = 0b0000000011011000;
Weight_Matrix[0][15] = 0b0000000010100111;
Weight_Matrix[0][16] = 0b0000000000100111;
Weight_Matrix[0][17] = 0b0000000010000000;
Weight_Matrix[0][18] = 0b0000000010100101;
Weight_Matrix[0][19] = 0b1111111111101001;
Weight_Matrix[0][20] = 0b1111111111011001;
Weight_Matrix[0][21] = 0b0000000001100110;
Weight_Matrix[0][22] = 0b0000000001100110;
Weight_Matrix[0][23] = 0b0000000001111100;
Weight_Matrix[0][24] = 0b1111111111100111;
Weight_Matrix[0][25] = 0b0000000001000011;
Weight_Matrix[0][26] = 0b0000000001001000;
Weight_Matrix[0][27] = 0b1111111110001000;
Weight_Matrix[0][28] = 0b1111111110010101;
Weight_Matrix[0][29] = 0b0000000001011000;
Weight_Matrix[0][30] = 0b0000000011101011;
Weight_Matrix[0][31] = 0b1111111110110001;
Weight_Matrix[1][0]  = 0b1111111011010111;
Weight_Matrix[1][1]  = 0b0000000010100110;
Weight_Matrix[1][2]  = 0b1111111110101100;
Weight_Matrix[1][3]  = 0b1111111100101000;
Weight_Matrix[1][4]  = 0b0000000010111110;
Weight_Matrix[1][5]  = 0b1111111010110000;
Weight_Matrix[1][6]  = 0b1111111110110100;
Weight_Matrix[1][7]  = 0b1111111110010100;
Weight_Matrix[1][8]  = 0b0000000011111111;
Weight_Matrix[1][9]  = 0b1111111100111100;
Weight_Matrix[1][10] = 0b1111111111111100;
Weight_Matrix[1][11] = 0b1111111110110000;
Weight_Matrix[1][12] = 0b0000000001001010;
Weight_Matrix[1][13] = 0b0000000011010110;
Weight_Matrix[1][14] = 0b1111111100010010;
Weight_Matrix[1][15] = 0b0000000100011001;
Weight_Matrix[1][16] = 0b0000000000011111;
Weight_Matrix[1][17] = 0b1111111100000010;
Weight_Matrix[1][18] = 0b1111110111111010;
Weight_Matrix[1][19] = 0b0000000010110100;
Weight_Matrix[1][20] = 0b0000000010001100;
Weight_Matrix[1][21] = 0b1111111111111010;
Weight_Matrix[1][22] = 0b0000000001001010;
Weight_Matrix[1][23] = 0b0000000010010010;
Weight_Matrix[1][24] = 0b1111111001111000;
Weight_Matrix[1][25] = 0b0000000000011000;
Weight_Matrix[1][26] = 0b1111111111110010;
Weight_Matrix[1][27] = 0b1111111101011111;
Weight_Matrix[1][28] = 0b1111111011100111;
Weight_Matrix[1][29] = 0b0000000011000100;
Weight_Matrix[1][30] = 0b1111110110001100;
Weight_Matrix[1][31] = 0b0000000010100011;
Weight_Matrix[2][0]  = 0b0000000010000101;
Weight_Matrix[2][1]  = 0b0000000000111110;
Weight_Matrix[2][2]  = 0b1111111110011111;
Weight_Matrix[2][3]  = 0b0000000101010010;
Weight_Matrix[2][4]  = 0b0000000001111110;
Weight_Matrix[2][5]  = 0b1111111101111000;
Weight_Matrix[2][6]  = 0b0000000011010111;
Weight_Matrix[2][7]  = 0b1111111110001101;
Weight_Matrix[2][8]  = 0b1111111100101000;
Weight_Matrix[2][9]  = 0b0000000000001100;
Weight_Matrix[2][10] = 0b0000000001010010;
Weight_Matrix[2][11] = 0b0000000100001111;
Weight_Matrix[2][12] = 0b0000000000011000;
Weight_Matrix[2][13] = 0b1111111110011110;
Weight_Matrix[2][14] = 0b1111111101111100;
Weight_Matrix[2][15] = 0b0000000000100001;
Weight_Matrix[2][16] = 0b0000000001001100;
Weight_Matrix[2][17] = 0b1111111110101001;
Weight_Matrix[2][18] = 0b0000000000001101;
Weight_Matrix[2][19] = 0b0000000000101111;
Weight_Matrix[2][20] = 0b1111111111110010;
Weight_Matrix[2][21] = 0b0000000000101101;
Weight_Matrix[2][22] = 0b0000000000101110;
Weight_Matrix[2][23] = 0b1111111011110110;
Weight_Matrix[2][24] = 0b0000000100010110;
Weight_Matrix[2][25] = 0b1111111110100001;
Weight_Matrix[2][26] = 0b1111111111011100;
Weight_Matrix[2][27] = 0b0000000100000100;
Weight_Matrix[2][28] = 0b0000000001011001;
Weight_Matrix[2][29] = 0b1111111011101100;
Weight_Matrix[2][30] = 0b0000000000011101;
Weight_Matrix[2][31] = 0b0000000000101000;
Weight_Matrix[3][0]  = 0b1111111100101110;
Weight_Matrix[3][1]  = 0b0000000010010110;
Weight_Matrix[3][2]  = 0b0000000000000110;
Weight_Matrix[3][3]  = 0b1111111011110111;
Weight_Matrix[3][4]  = 0b0000000100101111;
Weight_Matrix[3][5]  = 0b1111111111110001;
Weight_Matrix[3][6]  = 0b1111111110011010;
Weight_Matrix[3][7]  = 0b1111111110100101;
Weight_Matrix[3][8]  = 0b0000000010000011;
Weight_Matrix[3][9]  = 0b1111111110110111;
Weight_Matrix[3][10] = 0b1111111110100000;
Weight_Matrix[3][11] = 0b1111111101011001;
Weight_Matrix[3][12] = 0b1111111111011110;
Weight_Matrix[3][13] = 0b1111111100101010;
Weight_Matrix[3][14] = 0b0000000000101100;
Weight_Matrix[3][15] = 0b1111111111110010;
Weight_Matrix[3][16] = 0b0000000000000001;
Weight_Matrix[3][17] = 0b1111111111011010;
Weight_Matrix[3][18] = 0b1111111101100110;
Weight_Matrix[3][19] = 0b1111111111101001;
Weight_Matrix[3][20] = 0b1111111111100011;
Weight_Matrix[3][21] = 0b1111111101000101;
Weight_Matrix[3][22] = 0b0000000000001000;
Weight_Matrix[3][23] = 0b1111111111100000;
Weight_Matrix[3][24] = 0b1111111011110111;
Weight_Matrix[3][25] = 0b0000000000100110;
Weight_Matrix[3][26] = 0b1111111110110101;
Weight_Matrix[3][27] = 0b1111111111110001;
Weight_Matrix[3][28] = 0b0000000011000110;
Weight_Matrix[3][29] = 0b0000000011010001;
Weight_Matrix[3][30] = 0b0000000101101000;
Weight_Matrix[3][31] = 0b0000000001000100;
}
