/**
 * @file   ebbLED.c
 * @author Derek Molloy
 * @date   4 April 2015
 * @brief  A kernel module for controlling a GPIO LED/button pair. The device appears as
 * /dev/ebbLED and the LEDs are available via sysfs /sys/class/gpio/gpio115 and gpio49
 * This circuit for the BeagleBone assumes that an LED is attached to GPIO 49 which is
 * on P9_23 and the button is attached to GPIO 115 on P9_27.
 *
 * @see http://www.derekmolloy.ie/
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#define  DEVICE_NAME "ebbLED"
#define  CLASS_NAME  "character"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux LED driver for the BBB");
MODULE_VERSION("0.1");

static int    majorNumber;                  //!< Used to store the device number
static char   message[1024] = {0};
static short  position;
static int    numberOpens = 0;
static struct class*  ebbLEDClass  = NULL;
static struct device* ebbLEDDevice = NULL;

static unsigned int    gpioLED = 49;
static unsigned int    gpioButton = 115;
static int	       irqNumber;
static int	       numberPresses = 0;
static bool	       ledOn = 0;

// The prototype functions for the driver
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
irq_handler_t  gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs);

/**
 * @brief The file operations structure
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

static int __init ebbLED_init(void){
	int result = 0;
	printk(KERN_INFO "EBBLED: Initializing the EBBChar LKM\n");

	// Try to dynamically allocate a major number for the device -- more difficult but worth it
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "EBBLED failed to register device\n");
		return majorNumber;
	}
	printk(KERN_INFO "EBBLED: registered correctly with major number %d\n", majorNumber);

	ebbLEDClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(ebbLEDClass)){
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(ebbLEDClass);
	}
	printk(KERN_INFO "EBBLED: device class registered correctly\n");

	ebbLEDDevice = device_create(ebbLEDClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(ebbLEDDevice)){
		class_destroy(ebbLEDClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(ebbLEDDevice);
	}
	printk(KERN_INFO "EBBLED: device class created correctly\n");

	if (!gpio_is_valid(gpioLED)){
		printk(KERN_INFO "EBBLED: invalid LED gpio\n");
		return -1;
	}
	gpio_request(gpioLED, "sysfs");
	gpio_request(gpioButton, "sysfs");
	gpio_direction_output(gpioLED, 1);
	ledOn = true;
	gpio_direction_input(gpioButton);
//	gpio_set_value(gpioLED, 1);   // not required -- set by the line above
	gpio_export(gpioLED, false);  // causes gpio49 to appear in /sys/class/gpio
				      // the second argument prevents the direction from being changed
	gpio_export(gpioButton, false);
	irqNumber = gpio_to_irq(gpioButton);
	printk(KERN_INFO "The button state is currently: %d\n", gpio_get_value(gpioButton));
	printk(KERN_INFO "The button is mapped to IRQ: %d\n", irqNumber);

	result = request_irq(irqNumber, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "gpio_handler", NULL);
	printk(KERN_INFO "The interrupt request result is: %d\n", result);
	return result;
}

// The LKM cleanup function -- make sure to destroy all devices etc.
static void __exit ebbLED_exit(void){
	printk(KERN_INFO "The button state is currently: %d\n", gpio_get_value(gpioButton));
	printk(KERN_INFO "The button was pressed %d times\n", numberPresses);
	gpio_set_value(gpioLED, 0);
	gpio_unexport(gpioLED);
	free_irq(irqNumber, NULL);
	gpio_unexport(gpioButton);
	gpio_free(gpioLED);
	gpio_free(gpioButton);
	device_destroy(ebbLEDClass, MKDEV(majorNumber, 0));
	class_unregister(ebbLEDClass);
	class_destroy(ebbLEDClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	printk(KERN_INFO "EBBLED: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "EBBLED: Device has been opened %d times\n", numberOpens);
	return 0;
}

// This function is called when data is sent from the device to the user
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	short error_count = 0;
	// copy_to_user has the format ( * to, *from, n) and returns 0 on success
	error_count = copy_to_user(buffer, message, position);

	if (error_count==0){
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", position);
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
	printk(KERN_INFO "EBBLED: Received %d characters from the user\n", position);
	return position;
}

irq_handler_t gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs){
	ledOn = !ledOn;
	gpio_set_value(gpioLED, ledOn);
	printk(KERN_INFO "The button state is currently: %d\n", gpio_get_value(gpioButton));
	numberPresses++;
	return (irq_handler_t) IRQ_HANDLED;
}

static int dev_release(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "EBBLED: Device successfully closed\n");
	return 0;
}

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(ebbLED_init);
module_exit(ebbLED_exit);
