#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#define  DEVICE_NAME "ebbchar"
#define  CLASS_NAME  "character"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux driver for the BBB");
MODULE_VERSION("0.1");

static int    majorNumber;
static char   message[100] = {0};
static short  readPosition = 0;
static int    numberOpens = 0;
static struct class* ebbcharClass = NULL;
static struct device* ebbcharDevice = NULL;

// The prototype functions for the driver
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
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

static int __init ebbchar_init(void){
	printk(KERN_INFO "Initializing the EBBChar LKM\n");

	// Try to dynamically allocate a major number for the device
        // more difficult but worth it
	majorNumber = register_chrdev(0, "ebbchar", &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "EBBChar failed to register device\n");
		return -1;
	}
	printk(KERN_INFO "EBBChar registered correctly with major number %d\n", majorNumber);
	ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ebbcharClass)){
		printk(KERN_ALERT "Failed to register device class\n");
		return -1;
	}
	printk(KERN_INFO "EBBChar device class registered correctly\n");
	ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ebbcharDevice)){
		printk(KERN_ALERT "Failed to create the device\n");
		return -1;
	}
	printk(KERN_INFO "EBBChar device class created  correctly\n");

	return 0;
}

// The LKM cleanup function -- make sure to destroy all devices
static void __exit ebbchar_exit(void){
	device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
	class_unregister(ebbcharClass);
	class_destroy(ebbcharClass);
	unregister_chrdev(majorNumber, "ebbchar");
	printk(KERN_INFO "Goodbye from the EBBChar LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "Device has been opened %d times\n", numberOpens);
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "Device successfully closed\n");
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
module_init(ebbchar_init);
module_exit(ebbchar_exit);
