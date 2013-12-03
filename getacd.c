#include <stdio.h>                                                                                                                           
#include <stdlib.h>                                                                                                                          
#include <string.h>                                                                                                                          
#include <unistd.h>                                                                                                                          
#include <fcntl.h>                                                                                                                           
#include <ctype.h>                                                                                                                           
#include <signal.h>                                                                                                                          
#include <sys/wait.h>                                                                                                                        
#include <sys/mount.h>                                                                                                                       
#include <sys/stat.h>                                                                                                                        
#include <sys/poll.h>                                                                                                                        
#include <errno.h>                                                                                                                           
#include <stdarg.h>                                                                                                                          
#include <mtd/mtd-user.h>                                                                                                                    
#include <sys/types.h>                                                                                                                       
#include <sys/socket.h>                                                                                                                      
#include <sys/un.h>                                                                                                                          
#include <sys/personality.h>

#include <linux/kd.h>                                                                                                                        
                                                                                                                                             
#ifdef HAVE_SELINUX                                                                                                                          
#include <selinux/selinux.h>                                                                                                                 
#include <selinux/label.h>                                                                                                                   
#include <selinux/android.h>                                                                                                                 
#endif                                                                                                                                       
                                                                                                                                             
#include <libgen.h>                                                                                                                          
                                                                                                                                             
#include <cutils/list.h>                                                                                                                     
#include <cutils/sockets.h>                                                                                                                  
#include <cutils/iosched_policy.h>                                                                                                           
#include <private/android_filesystem_config.h>                                                                                               
#include <termios.h>                                                                                                                         
                                                                                                                                             
#include <sys/system_properties.h>                                                                                                           
                                                                                                                                             
#include "umip_access.h"

#define IMEI_LEN                (15)
#define BTMAC_ADDRESS_LEN       (6)
#define ACER_UUID_LEN           (IMEI_LEN+BTMAC_ADDRESS_LEN+20)
int main(int nargs, char **args)
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
        strcpy( uuid, vIMEIno );

	int i, j;
	char vBTMac[20];
	for ( i=0,j=0; i<BTMAC_ADDRESS_LEN*2,j<BTMAC_ADDRESS_LEN; i+=2,j++ ){
		sprintf( &vBTMac[i],   "%x", (ChaabiBTMacAddr[j]&0xf0)>>4 );
		sprintf( &vBTMac[i+1], "%x", (ChaabiBTMacAddr[j]&0x0f) );
		printf("%02x:%c%c\n", ChaabiBTMacAddr[j], vBTMac[i], vBTMac[i+1]  );
	}

	printf("btmac: %s\n", vBTMac);
	printf(" uuid: %s\n", uuid);

        //property_set( "shen.acerbtmac", (char *)ChaabiBTMacAddr );
        property_set( "shen.acerbtmac", (char *)vBTMac );
	strcat( uuid, vBTMac );
        property_set( "shen.aceruuid", uuid );

        return 0;
}


