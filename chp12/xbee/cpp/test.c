#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xbee.h>

void myCB(struct xbee *xbee, struct xbee_con *con, struct xbee_pkt **pkt, void **data) {
        if ((*pkt)->dataLen > 0) {
                if ((*pkt)->data[0] == '@') {
                        xbee_conCallbackSet(con, NULL, NULL);
                        printf("*** DISABLED CALLBACK... ***\n");
                }
                printf("rx: [%s]\n", (*pkt)->data);
        }
        printf("tx: %d\n", xbee_conTx(con, NULL, "Hello\r\n"));
}

int main(void) {
        void *d;
        struct xbee *xbee;
        struct xbee_con *con;
        struct xbee_conAddress address;
        xbee_err ret;

        if ((ret = xbee_setup(&xbee, "xbeeZB", "/dev/ttyUSB0", 9600)) != XBEE_ENONE) {
                printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
                return ret;
        }

        memset(&address, 0, sizeof(address));
        address.addr64_enabled = 1;
        address.addr64[0] = 0x00;
        address.addr64[1] = 0x13;
        address.addr64[2] = 0xA2;
        address.addr64[3] = 0x00;
        address.addr64[4] = 0x40;
        address.addr64[5] = 0xC2;
        address.addr64[6] = 0x96;
        address.addr64[7] = 0xE6;
        if ((ret = xbee_conNew(xbee, &con, "Data", &address)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
                return ret;
        }

        if ((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conDataSet() returned: %d", ret);
                return ret;
        }

        if ((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conCallbackSet() returned: %d", ret);
                return ret;
        }

        /* kick off the chain reaction! */
        xbee_conTx(con, NULL, "Hello\r\n");

        for (;;) {
                void *p;

                if ((ret = xbee_conCallbackGet(con, (xbee_t_conCallback*)&p)) != XBEE_ENONE) {
                        xbee_log(xbee, -1, "xbee_conCallbackGet() returned: %d", ret);
                        return ret;
                }

                if (p == NULL) break;

                usleep(1000000);
        }

        if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
                xbee_log(xbee, -1, "xbee_conEnd() returned: %d", ret);
                return ret;
        }

        xbee_shutdown(xbee);

        return 0;
}
