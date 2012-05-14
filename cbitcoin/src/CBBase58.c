//
//  CBBase58.c
//  cbitcoin
//
//  Created by Matthew Mitchell on 28/04/2012.
//  Copyright (c) 2012 Matthew Mitchell
//  
//  This file is part of cbitcoin.
//
//  cbitcoin is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  cbitcoin is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with cbitcoin.  If not, see <http://www.gnu.org/licenses/>.

#include "CBBase58.h"

CBBigInt CBDecodeBase58(char * str){
	// ??? Quite likely these functions can be improved
	CBBigInt bi;
	bi.data = malloc(1);
	bi.data[0] = 0;
	bi.length = 1;
	u_int8_t temp[189];
	for (u_int8_t x = strlen(str) - 1;; x--){ // Working backwards
		// Get index in alphabet array
		int alphaIndex = str[x];
		if (alphaIndex != 49){ // If not 1
			if (str[x] < 58){ // Numbers
				alphaIndex -= 49;
			}else if (str[x] < 73){ // A-H
				alphaIndex -= 56;
			}else if (str[x] < 79){ // J-N
				alphaIndex -= 57;
			}else if (str[x] < 91){ // P-Z
				alphaIndex -= 58;
			}else if (str[x] < 108){ // a-k
				alphaIndex -= 64;
			}else{ // m-z
				alphaIndex -= 65;
			}
			CBBigInt bi2 = CBBigIntFromPowUInt8(58, strlen(str) - 1 - x);
			memset(temp, 0, bi2.length + 1);
			CBBigIntEqualsMultiplicationByUInt8(&bi2, alphaIndex, temp);
			CBBigIntEqualsAdditionByCBBigInt(&bi,&bi2);
			free(bi2.data);
		}
		if (!x) 
			break;
	}
	// Got CBBigInt from base-58 string. Add zeros on end.
	u_int8_t zeros = 0;
	for (u_int8_t x = 0; x < strlen(str); x++)
		if (str[x] == '1')
			zeros++;
		else
			break;
	if (zeros) {
		bi.length += zeros;
		realloc(bi.data, bi.length);
		memset(bi.data + bi.length - zeros, 0, zeros);
	}
	return bi;
}
void CBDecodeBase58Checked(u_int8_t * bytes,char * str){
	
}
void CBEncodeBase58(char * str, u_int8_t * bytes, u_int8_t len){
	// ??? Improvement?
	u_int8_t x = 0;
	// Zeros
	for (u_int8_t y = len - 1;; y--)
		if (!bytes[y]){
			str[x] = '1';
			x++;
			if (!y)
				break;
		}else
			break;
	u_int8_t zeros = x;
	// Make CBBigInt
	CBBigInt bi;
	bi.data = bytes;
	bi.length = len;
	CBBigIntNormalise(&bi);
	// Make temporary data store
	u_int8_t * temp = malloc(len);
	// Encode
	u_int8_t mod;
	for (;CBBigIntCompareToUInt8(bi, 58) >= 0;x++) {
		mod = CBBigIntModuloWithUInt8(bi, 58);
		str[x] = base58Characters[mod];
		CBBigIntEqualsSubtractionByUInt8(&bi, mod);
		memset(temp, 0, len);
		CBBigIntEqualsDivisionByUInt8(&bi, 58,temp);
	}
	str[x] = base58Characters[bi.data[bi.length-1]];
	x++;
	// Reversal
	for (u_int8_t y = zeros; y < x / 2; y++) {
		char temp = str[y];
		str[y] = str[x-y-1+zeros];
		str[x-y-1+zeros] = temp;
	}
	str[x] = '\0';
	// Cleanup
	free(temp);
}
