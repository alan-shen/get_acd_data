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

#define IMEI_LEN                (15)
#define BTMAC_ADDRESS_LEN       (6)
#define ACER_UUID_LEN           (IMEI_LEN+BTMAC_ADDRESS_LEN+20)
int main(int argc, char **args)
{
        char uuid[ACER_UUID_LEN];
        /* Get IMEI, because there is no IMEI in ducati, so we set it all zero here */
        char *vIMEIno = "000000000000000";    

        /* Get BT Mac addr from ACD field */
        const unsigned char NullBTMacAddr[BTMAC_ADDRESS_LEN] = { 0, 0, 0, 0, 0, 0 }; 

        uint8_t vAcdIndex = ACD_BT_MAC_ADDR_FIELD_INDEX;
        unsigned char *ChaabiBTMacAddr = NULL;

        if ( get_customer_data( vAcdIndex, (void ** const) &ChaabiBTMacAddr) != BTMAC_ADDRESS_LEN ){
                ChaabiBTMacAddr = (unsigned char *) malloc(BTMAC_ADDRESS_LEN);
                memcpy(ChaabiBTMacAddr, NullBTMacAddr, BTMAC_ADDRESS_LEN);
                printf("[%s:%d] Chaabi BTMac not found!\n", __func__, __LINE__);
        }    

        int num = get_customer_data( vAcdIndex, (void ** const) &ChaabiBTMacAddr);
	printf("CHAABI Read Count: %d\n", num);
        printf("CHAABI BTMac adr is %02x:%02x:%02x:%02x:%02x:%02x\n", ChaabiBTMacAddr[0], ChaabiBTMacAddr[1], ChaabiBTMacAddr[2],
                                                                ChaabiBTMacAddr[3], ChaabiBTMacAddr[4], ChaabiBTMacAddr[5]);

        /* Set the property "ro.aceruuid" */
	int i, j;
	char vBTMac[20];
	for ( i=0,j=0; i<BTMAC_ADDRESS_LEN*2,j<BTMAC_ADDRESS_LEN; i+=2,j++ ){
		sprintf( &vBTMac[i],   "%x", (ChaabiBTMacAddr[j]&0xf0)>>4 );
		sprintf( &vBTMac[i+1], "%x", (ChaabiBTMacAddr[j]&0x0f) );
		printf("%02x:%c%c\n", ChaabiBTMacAddr[j], vBTMac[i], vBTMac[i+1]  );
	}

	printf(">>>btmac: %s\n", vBTMac);
        property_set( "shen.acerbtmac", (const char *)vBTMac );

        strcpy( uuid, vIMEIno );
	strcat( uuid, vBTMac );
	printf(">>> uuid: %s\n", uuid);
        property_set( "shen.aceruuid", (const char *)uuid );

        return 0;
}

