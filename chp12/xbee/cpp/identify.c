#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbee.h>

void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
        xbee_err ret;
        char *ni;
        struct xbee_conAddress *addr;

        printf("An XBee joined the network!\n");
        if ((ret = xbee_pktDataGet(*pkt, "NI", 0, 0, (void**)&ni)) == XBEE_ENONE && ni != NULL) {
                printf("  It is called: [%s]\n", ni);
        } else {
                printf("  Error while retrieving its NI - %d (%s)\n", ret, xbee_errorToStr(ret));
        }
       
        /* you could also use 'Address (16-bit)' or 'Address (64-bit)' to get the raw byte arrays */
        if ((ret = xbee_pktDataGet(*pkt, "Address", 0, 0, (void**)&addr)) == XBEE_ENONE && addr != NULL) {
                printf("  It's address is:\n");
                if (addr->addr16_enabled) {
                        printf("   16-bit address:  0x%02X%02X\n", addr->addr16[0], addr->addr16[1]);
                } else {
                        printf("   16-bit address:  --\n");
                }
                if (addr->addr64_enabled) {
                        printf("   64-bit address:  0x%02X%02X%02X%02X 0x%02X%02X%02X%02X\n",
                        addr->addr64[0], addr->addr64[1], addr->addr64[2], addr->addr64[3],
                        addr->addr64[4], addr->addr64[5], addr->addr64[6], addr->addr64[7]);
                } else {
                        printf("   64-bit address:  --\n");
                }
        } else {
                printf("  Error while retrieving its Address - %d (%s)\n", ret, xbee_errorToStr(ret));
        }
}

int main(void) {
        void *d;
        struct xbee *xbee;
        struct xbee_con *con;
        unsigned char txRet;
        xbee_err ret;

        if ((ret = xbee_setup(&xbee, "xbee2", "/dev/ttyUSB0", 9600)) != XBEE_ENONE) {
                printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
                return ret;
        }
        if ((ret = xbee_conNew(xbee, &con, "Identify", NULL)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
                return ret;
        }

        if ((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
                return ret;
        }
        
        usleep(300000000);
        
        if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
                return ret;
        }

        xbee_shutdown(xbee);

        return 0;
}
