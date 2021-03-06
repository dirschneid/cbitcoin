//
//  testCBInventoryBroadcast.c
//  cbitcoin
//
//  Created by Matthew Mitchell on 11/07/2012.
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

#include <stdio.h>
#include "CBInventoryBroadcast.h"
#include <time.h>
#include "stdarg.h"

void onErrorReceived(CBError a,char * format,...);
void onErrorReceived(CBError a,char * format,...){
	va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
	printf("\n");
}

int main(){
	unsigned int s = (unsigned int)time(NULL);
	s = 1337544566;
	printf("Session = %ui\n",s);
	srand(s);
	// Test deserialisation
	uint8_t data[109] = {
		0x03, // Three items
		0x01,0x00,0x00,0x00, // Type transaction
		0xFB,0x30,0xB1,0x9B,0x3A,0x0F,0x82,0x4A,0xF1,0x2B,0x6E,0xA4,0x72,0xBA,0x9B,0x3A,0x67,0xBA,0xF2,0xD8,0x2E,0x18,0x06,0x9D,0x4A,0x1B,0x54,0xA3,0xD8,0x9C,0x43,0xCF, // Hash
		0x02,0x00,0x00,0x00, // Type block
		0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA, // Hash
		0x00,0x00,0x00,0x00, // Type error
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // Hash
	};
	CBByteArray * bytes = CBNewByteArrayWithDataCopy(data, 109, onErrorReceived);
	CBInventoryBroadcast * invBroadcast = CBNewInventoryBroadcastFromData(bytes, onErrorReceived);
	if(CBInventoryBroadcastDeserialise(invBroadcast) != 109){
		printf("DESERIALISATION LEN FAIL\n");
		return 1;
	}
	if (invBroadcast->itemNum != 3) {
		printf("DESERIALISATION NUM FAIL\n");
		return 1;
	}
	if (invBroadcast->items[0]->type != CB_INVENTORY_ITEM_TRANSACTION) {
		printf("DESERIALISATION FIRST TYPE FAIL\n");
		return 1;
	}
	if (memcmp(CBByteArrayGetData(invBroadcast->items[0]->hash),(uint8_t []){0xFB,0x30,0xB1,0x9B,0x3A,0x0F,0x82,0x4A,0xF1,0x2B,0x6E,0xA4,0x72,0xBA,0x9B,0x3A,0x67,0xBA,0xF2,0xD8,0x2E,0x18,0x06,0x9D,0x4A,0x1B,0x54,0xA3,0xD8,0x9C,0x43,0xCF},32)) {
		printf("DESERIALISATION FIRST HASH FAIL\n0x");
		uint8_t * d = CBByteArrayGetData(invBroadcast->items[0]->hash);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (uint8_t []){0xFB,0x30,0xB1,0x9B,0x3A,0x0F,0x82,0x4A,0xF1,0x2B,0x6E,0xA4,0x72,0xBA,0x9B,0x3A,0x67,0xBA,0xF2,0xD8,0x2E,0x18,0x06,0x9D,0x4A,0x1B,0x54,0xA3,0xD8,0x9C,0x43,0xCF};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (invBroadcast->items[1]->type != CB_INVENTORY_ITEM_BLOCK) {
		printf("DESERIALISATION SECOND TYPE FAIL\n");
		return 1;
	}
	if (memcmp(CBByteArrayGetData(invBroadcast->items[1]->hash),(uint8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA},32)) {
		printf("DESERIALISATION SECOND HASH FAIL\n0x");
		uint8_t * d = CBByteArrayGetData(invBroadcast->items[1]->hash);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (uint8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	if (invBroadcast->items[2]->type != CB_INVENTORY_ITEM_ERROR) {
		printf("DESERIALISATION THIRD TYPE FAIL\n");
		return 1;
	}
	if (memcmp(CBByteArrayGetData(invBroadcast->items[2]->hash),(uint8_t []){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},32)) {
		printf("DESERIALISATION THIRD HASH FAIL\n0x");
		uint8_t * d = CBByteArrayGetData(invBroadcast->items[2]->hash);
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		d = (uint8_t []){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		for (int x = 0; x < 32; x++) {
			printf("%.2X",d[x]);
		}
		return 1;
	}
	// Test serialisation with timestamps
	memset(CBByteArrayGetData(bytes), 0, 109);
	CBReleaseObject(invBroadcast->items[0]->hash);
	invBroadcast->items[0]->hash = CBNewByteArrayWithDataCopy((uint8_t []){0xFB,0x30,0xB1,0x9B,0x3A,0x0F,0x82,0x4A,0xF1,0x2B,0x6E,0xA4,0x72,0xBA,0x9B,0x3A,0x67,0xBA,0xF2,0xD8,0x2E,0x18,0x06,0x9D,0x4A,0x1B,0x54,0xA3,0xD8,0x9C,0x43,0xCF}, 32, onErrorReceived);
	CBReleaseObject(invBroadcast->items[1]->hash);
	invBroadcast->items[1]->hash = CBNewByteArrayWithDataCopy((uint8_t []){0xFA,0x30,0xB1,0x9B,0x35,0x0F,0x82,0x4A,0x92,0x2B,0x6E,0xA4,0x72,0xBA,0x9C,0x3A,0x67,0xBA,0xF2,0x18,0x12,0x18,0xF2,0x9D,0x4A,0x1C,0x54,0xA3,0xD8,0x9C,0x43,0xBA}, 32, onErrorReceived);
	CBReleaseObject(invBroadcast->items[2]->hash);
	invBroadcast->items[2]->hash = CBNewByteArrayWithDataCopy((uint8_t []){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, 32, onErrorReceived);
	if (CBInventoryBroadcastSerialise(invBroadcast,true) != 109){
		printf("SERIALISATION LEN FAIL\n");
		return 1;
	}
	if (memcmp(data, CBByteArrayGetData(bytes), 109)) {
		printf("SERIALISATION FAIL\n0x");
		uint8_t * d = CBByteArrayGetData(bytes);
		for (int x = 0; x < 109; x++) {
			printf("%.2X",d[x]);
		}
		printf("\n!=\n0x");
		for (int x = 0; x < 109; x++) {
			printf("%.2X",data[x]);
		}
		return 1;
	}
	CBReleaseObject(invBroadcast);
	CBReleaseObject(bytes);
	return 0;
}