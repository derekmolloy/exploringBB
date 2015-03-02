#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux driver for the BBB");

// An example argument -- default value is "world"
static char  *name = "world";
static int   major = 92;            // Currently unused in the list
static char  message[100] = {0};
static short readPosition = 0;
static int   numberOpens = 0;

module_param(name, charp, 0000);
MODULE_PARM_DESC(name, "The name to output to the logs on initialization");

// The prototype functions for the driver
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// This structure lists the callback functions
static struct file_operations fops =
{
	.read = dev_read,
	.open = dev_open,
	.write = dev_write,
	.release = dev_release,
};

// The LKM initialization function
// The static keyword is used so that it is not visible outside this file. The
// __init token is a hint to the kernel that the function is used only at
// initialization time.

static int __init driver_init(void){
	int i;
	printk(KERN_INFO "Hello %s from the EBBTest LKM!\n", name);
	i = register_chrdev(major, "ebbtest", &fops);
	if (i<0) printk(KERN_ALERT "EBBTest device failed to register\n");
	else printk(KERN_INFO "EBBTest device registered correctly\n");
	return i;
}

// The LKM cleanup function
static void __exit driver_exit(void){
	printk(KERN_INFO "Goodbye from the EBBTest LKM!\n");
	unregister_chrdev(major, "ebbtest");
}

static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "EBBTest device has been opened %d times\n", numberOpens);
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "EBBTest device successfully closed\n");
	return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	short count = 0;
	while(len && (message[readPosition]!=0)){
		put_user(message[readPosition], buffer++);
		count++;
		len--;
		readPosition++;
	}
	return count;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	short i = len-1;
	short count = 0;
	memset(message,0,100);
	readPosition = 0;
	while(len>0){
		message[count++] = buffer[i--];
		len--;
	}
	return count;
}

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(driver_init);
module_exit(driver_exit);
