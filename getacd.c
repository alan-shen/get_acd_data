#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/system_properties.h>
#include <cutils/properties.h>

#include "umip_access.h"

/*
 * PATH: prebuilts/intel/hardware/intel/prebuilts/redhookbay/chaabi/Lib/umip_access.h
 * DESP:
 * 
 */

/*
 * PATH: prebuilts/intel/hardware/intel/prebuilts/redhookbay/chaabi/Lib/acd_reference.h
 * DESP: Android Customer Data field indices
 * 
 * #define ACD_WV_KEYBOX_FIELD_INDEX               1
 * #define ACD_RESERVED_2_FIELD_INDEX              2
 * #define ACD_RESERVED_3_FIELD_INDEX              3
 * #define ACD_RESERVED_4_FIELD_INDEX              4
 * #define ACD_RESERVED_5_FIELD_INDEX              5
 * #define ACD_WV_DEVICE_ID_FIELD_INDEX            6
 * #define ACD_PRODUCT_ID_FIELD_INDEX              7
 * #define ACD_SOFT_PLATFORM_ID_FIELD_INDEX        8
 * #define ACD_CUSTOMER_RESERVED_9_FIELD_INDEX     9
 * #define ACD_WLAN_MAC_ADDR_FIELD_INDEX           10
 * #define ACD_BT_MAC_ADDR_FIELD_INDEX             11
 * #define ACD_CUSTOMER_RESERVED_15_FIELD_INDEX    15
 * #define ACD_CUSTOMER_RESERVED_16_FIELD_INDEX    16
 * #define ACD_CUSTOMER_RESERVED_17_FIELD_INDEX    17
 * #define ACD_CUSTOMER_RESERVED_18_FIELD_INDEX    18
 *  
 **/


int getacd_string(uint8_t index)
{
        unsigned char *ChaabiDataAddr = NULL;
        int length = get_customer_data( index, (void ** const) &ChaabiDataAddr);
	int i;

	printf("\tCHAABI Read ACD Index %d, Count: %4d, Data: %s", index, length, ChaabiDataAddr);
	printf("\n");

	return 0;
}

int getacd_hex(uint8_t index)
{
        unsigned char *ChaabiDataAddr = NULL;
        int length = get_customer_data( index, (void ** const) &ChaabiDataAddr);
	int i;

	printf("\tCHAABI Read ACD Index %d, Count: %4d, Data: ", index, length);
	for( i=0; i<length; i++){
		printf("%02x", ChaabiDataAddr[i]);
		if( i != length-1){
			printf(":");
		}
	}
	printf("\n");

#if 0
	int i, j;
	char vBTMac[20];
	for ( i=0,j=0; i<BTMAC_ADDRESS_LEN*2,j<BTMAC_ADDRESS_LEN; i+=2,j++ ){
		sprintf( &vBTMac[i],   "%x", (ChaabiBTMacAddr[j]&0xf0)>>4 );
		sprintf( &vBTMac[i+1], "%x", (ChaabiBTMacAddr[j]&0x0f) );
		printf("%02x:%c%c\n", ChaabiBTMacAddr[j], vBTMac[i], vBTMac[i+1]  );
	}	
#endif

	return 0;
}

int main(int argc, char **args)
{
	getacd_hex( ACD_BT_MAC_ADDR_FIELD_INDEX );
	getacd_hex( ACD_WLAN_MAC_ADDR_FIELD_INDEX );

	getacd_string( ACD_CUSTOMER_RESERVED_15_FIELD_INDEX );

        return 0;
}

