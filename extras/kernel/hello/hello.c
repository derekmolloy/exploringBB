#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux driver for the BBB");

// An example argument -- default value is "world"
static char *name = "world";

module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "The name to output to the logs on initialization");

// The LKM initialization function
// The static keyword is used so that it is not visible outside this file. The
// __init token is a hint to the kernel that the function is used only at
// initialization time.

static int __init hello_init(void){
	printk(KERN_INFO "Hello %s from the BBB LKM!\n", name);
	return 0;
}

// The LKM cleanup function
static void __exit hello_exit(void){
	printk(KERN_INFO "Goodbye from the BBB LKM!\n");
}

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(hello_init);
module_exit(hello_exit);
