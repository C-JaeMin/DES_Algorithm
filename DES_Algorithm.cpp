#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minwindef.h>
#define BLOCK_SIZE 64
#define DES_ROUND 16

UINT ip[64] = {
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17,9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7};

UINT iip[64] = {
40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41,9,49,17,57,25};

UINT E[64] = {
32,1,2,3,4,5,4,5,
6,7,8,9,8,9,10,11,
12,13,12,13,14,15,16,17,
16,17,18,19,20,21,20,21,
22,23,24,25,24,25,26,27,
28,29,28,29,30,31,32,1};

UINT s_box[8][4][16] = {
{	{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
    { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8}, 
    { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
    {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}  },
{	{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
    { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5}, 
    { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
    {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}  },
{	{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
    {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1}, 
    {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
    { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}  },
{	{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
    {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9}, 
    {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
    { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}  },
{	{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
    {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6}, 
    { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
    {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}  },
{	{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11}, 
    {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8}, 
    { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
    { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}  },
{	{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
    {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6}, 
    { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
    { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}  },
{	{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
   { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2}, 
   { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
   { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}  }
};

UINT P[32] = {
16,7,20,21,29,12,28,17,
1,15,23,26,5,18,31,10,
2,8,24,14,32,27,3,9,
19,13,30,6,22,11,4,25};

UINT PC_1[56] = {
57,49,41,33,25,17,9,1,
58,50,42,34,26,18,10,2,
59,51,43,35,27,19,11,3,
60,52,44,36,63,55,47,39,
31,23,15,7,62,54,46,38,
30,22,14,6,61,53,45,37,
29,21,13,5,28,20,12,4};

UINT PC_2[48] = {
14,17,11,24,1,5,3,28,
15,6,21,10,23,19,12,4,
26,8,16,7,27,20,13,2,
41,52,31,37,47,55,30,40,
51,45,33,48,44,49,39,56,
34,53,46,42,50,36,29,32};

void IP(BYTE *in, BYTE *out) {
	int i;
	BYTE index, bit;
	BYTE mask = 0x80;
	for ( i=0;i<64;i++ ) {
		index = (ip[i] - 1) / 8;
		bit = (ip[i] - 1) % 8;
		if (in[index] & (mask >> bit)) {
			out[i / 8] |= mask >> (i % 8);
		}
	}
}

void IN_IP(BYTE *in, BYTE *out) {
	int i;
	BYTE index,bit;
	BYTE mask = 0x80;
	for( i=0; i<64; i++ ) {
		index = (iip[i]-1)/8;
		bit=(iip[i]-1)%8;
		if(in[index]&(mask>>bit)) {
			out[i/8] |= mask>>(i%8);
		}
	}
}

void BtoW(BYTE *Plain64, UINT *Left32, UINT *Right32) {
	int i;
	for( i=0;i<8;i++ ) {
		if(i<4) {
			*Left32 |= (UINT)Plain64[i]<<(24-(i*8));
		}
		else {
			*Right32 |= (UINT)Plain64[i]<<(56-(i*8));
		}
	}
}

void EP(UINT r, BYTE* out) {
	int i;
	UINT mask = 0x80000000;
	UINT bit8_Mask=0x80;
	UINT bit32_Mask = 0x80000000;
	for (i = 0;i<48;i++) {
		if (r & (mask >> (E[i] - 1))) {
			out[i / 8] |= (BYTE)(0x80 >> (i % 8));
		}
	}
}

UINT S_box_Transfer(BYTE* in) {
	int i, row, column;
	int shift = 28;
	UINT temp = 0, result = 0, mask = 0x80;
	for (i = 0;i<48;i++) {
		if (in[i / 8]&(BYTE)(mask>>(i%8))) {
			temp |= 0x20 >> (i % 6);
		}
		if ((i + 1) % 6 == 0) {
			row = ((temp & 0x20) >> 4) + (temp & 0x01);
			column = (temp & 0x1E) >> 1;
			result += ((UINT)s_box[i / 6][row][column]<<shift);
			shift -= 4;
			temp = 0;
		}
	}
	return result;
}

UINT Permutation(UINT in) {
	int i;
	UINT out=0, mask=0x80000000;
	for(i=0;i<32;i++) {
		if(in&(mask>>(P[i]-1))) {
			out|=(mask>>i);
		}
		else
		;
	}
	return out;
}

UINT f(UINT r, BYTE* rkey) {
	int i;
	BYTE data[6] = { 0, };
	UINT out;
	EP(r, data);
	for (i = 0;i<6;i++) {
		data[i] = data[i]^rkey[i];
	}
	out = Permutation(S_box_Transfer(data));
	return out;
}

void swap(UINT *x, UINT *y) {
	UINT temp;
	temp=*x;
	*x=*y;
	*y=temp;
}

void WtoB(UINT Left32, UINT Right32,BYTE *out) {
	int i;
	UINT mask = 0xff000000;
	for( i=0;i<8;i++ ) {
		if(i<4) {
			out[i]|=(Left32 & (mask>>(i*8)))>>(24-(i*8));
		}
		else {
			out[i]|=(Right32 & (mask>>(i*8)))>>(56-(i*8));
		}
	}
}

void PC1(BYTE *Key_In, BYTE *Key_Out) {
	int i, index, bit;
	UINT mask = 0x00000080;
	for( i=0; i<56; i++ ) {
		index = (PC_1[i]-1)/8;
		bit = (PC_1[i]-1)%8;
		
		if(Key_In[index]&(BYTE)(mask>>bit)) {
			Key_Out[i/8] |= (BYTE)(mask>>(i%8));
		}
	}
}

void makeBit28(UINT *c, UINT *d, BYTE *Key_Out) {
	int i;
	BYTE mask = 0x80;
	for( i=0; i<56; i++ ) {
		if( i<28 ) {
			if( Key_Out[i/8]&(mask>>(i%8))) {
				*c|=0x08000000>>i;
			}
			else {
				;
			}
		}
		else {
			if( Key_Out[i/8]&(mask>>(i%8))) {
				*d|=0x08000000>>(i-28);
			} 
			else {
				;
			}
		}
	}
}

void PC2(UINT c, UINT d, BYTE *Key_Out) {
	int i;
	UINT mask = 0x80000000;
	for( i=0; i<48; i++ ) {
		if( PC_2[i]<28 ) {
			if( c&(mask>>(PC_2[i]-1))) {
				Key_Out[i/8] |= 0x80>>(i%8);
			}
			else {
				;
			}
		}
		else {
			if( d&(mask>>(PC_2[i]-1-28))) {
				Key_Out[i/8] |= 0x80>>(i%8);
			}
			else {
				;
			}
		}
	}
}

UINT cir_shift(UINT n, int r) {
	int n_shift[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };
	n = (n << n_shift[r]) + (n >> (28 - n_shift[r]));
	return n & 0xFFFFFFF;
}

void key_expansion(BYTE *key, BYTE round_key[16][6]) {
	int i;
	BYTE pc1_result[7] = { 0, };
	UINT c = 0, d = 0;
	PC1(key, pc1_result);
	makeBit28(&c, &d, pc1_result);
	for (i = 0;i<16;i++) {
		c = cir_shift(c, i);
		d = cir_shift(d, i);
		PC2(c, d, round_key[i]);
	}
}

void DES_Encryption(BYTE *p_text, BYTE *result, BYTE *key) {
	int i;
	BYTE data[BLOCK_SIZE] = { 0, };
	BYTE round_key[16][6] = { 0, };
	UINT left = 0;
	UINT right = 0;
	key_expansion(key, round_key);
	IP(p_text, data);
	BtoW(data, &left, &right);
	for (i=0; i<DES_ROUND; i++) {
		left = left ^ f(right, round_key[i]);
		if (i != DES_ROUND -1) {
			swap(&left, &right);
		}
	}
	WtoB(left, right, data);
	IN_IP(data, result);
}

int main() {
	int i;
	int msg_len=0, block_count=0;
	
	BYTE p_text[128] = {0,};
	BYTE c_text[128] = {0,};
	BYTE d_text[128] = {0,};
	BYTE key[9] = {0,};
	
	printf("Enter Plain Text : ");
	gets((char *)p_text);
	
	printf("Enter Key: ");
	scanf("%s",key);
	
	msg_len=(rsize_t)strlen((char *)p_text);
	block_count=(msg_len%BLOCK_SIZE)?(msg_len/BLOCK_SIZE+1):(msg_len/BLOCK_SIZE);
	
	for( i=0; i<block_count; i++ ) {
		DES_Encryption(&p_text[i*BLOCK_SIZE], &c_text[i*BLOCK_SIZE],key);
	}
	
	printf("\nCyper Text : ");
	for( i=0; i<(block_count*BLOCK_SIZE); i++ ) {
		printf("%c ",c_text[i]);
	}
	
	return 0;
}