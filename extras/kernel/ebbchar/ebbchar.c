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
static char   message[1024] = {0};
static short  position;
static int    numberOpens = 0;
static struct class*  ebbcharClass  = NULL;
static struct device* ebbcharDevice = NULL;

// The prototype functions for the driver
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/**
 * This structure lists the callback functions
 * char devices usually implement open, read, write and release system calls.
 */
static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

// The LKM initialization function -- The static keyword is used so that
// it is not visible outside this file. The __init token is a hint to the
// kernel that the function is used only at initialization time

static int __init ebbchar_init(void){
	printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM\n");

	// Try to dynamically allocate a major number for the device -- more difficult but worth it
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "EBBChar failed to register device\n");
		return majorNumber;
	}
	printk(KERN_INFO "EBBChar: registered correctly with major number %d\n", majorNumber);

	ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ebbcharClass)){
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(ebbcharClass);
	}
	printk(KERN_INFO "EBBChar: device class registered correctly\n");

	ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ebbcharDevice)){
		class_destroy(ebbcharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(ebbcharDevice);
	}
	printk(KERN_INFO "EBBChar: device class created correctly\n");
	return 0;
}

// The LKM cleanup function -- make sure to destroy all devices etc.

static void __exit ebbchar_exit(void){
	device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
	class_unregister(ebbcharClass);
	class_destroy(ebbcharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "EBBChar: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "EBBChar: Device has been opened %d times\n", numberOpens);
	return 0;
}

// This function is called when data is sent from the device to the user
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	short error_count = 0;

	// copy_to_user has the format ( * to, *from, n) and returns 0 on success
	error_count = copy_to_user(buffer, message, position);

	if (error_count==0){
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", position);
//		*offset+=position;
		position=0;
		return position;
	}
	else {
		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
		return 0;
	}
}

// This function is called when data is sent to the device from the user
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	position = 0;
	memset(message,0,100);
	while(position<len){
		message[position] = buffer[position];
		position++;
	}
	printk(KERN_INFO "EBBChar: Received %d characters from the user\n", position);
	return position;
}

static int dev_release(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "EBBChar: Device successfully closed\n");
	return 0;
}

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(ebbchar_init);
module_exit(ebbchar_exit);
