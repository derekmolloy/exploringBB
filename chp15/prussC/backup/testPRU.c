

volatile char shared_buf[100] __attribute__((cregister("EXT_MEM")));
int local_data;
volatile register unsigned int __R31;

int main()
{
   for (;;)
   {        
      while ((__R31 & 0x1) == 0);
      shared_buf[++local_data] = 1;
   }
}
