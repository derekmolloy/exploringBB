/**
 * @file   button.c
 * @author Derek Molloy
 * @date   16 April 2015
 * @brief  A kernel module for controlling a button (or any signal) that is connected to
 * a GPIO. It has full support for interrupts and for sysfs entries so that the an
 * interface can be created to the button or the button can be configured from Linux
 * userspace (See: /sys/ebb/)
 * @see http://www.derekmolloy.ie/
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kobject.h>
#include <linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux GPIO Button LKM for the BBB");
MODULE_VERSION("0.1");

static bool isRising = 1;                   /// Rising edge is the default IRQ property
module_param(isRising, bool, S_IRUGO);      /// Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(isRising, " Rising edge = 1 (default), Falling edge = 0");  /// parameter description

static unsigned int gpioButton = 115;       /// Default GPIO is 115
module_param(gpioButton, uint, S_IRUGO);    /// Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(gpioButton, " GPIO Button number (default=115)");  /// parameter description

static unsigned int gpioLED = 49;           /// Default GPIO is 49
module_param(gpioLED, uint, S_IRUGO);       /// Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(gpioLED, " GPIO LED number (default=49)");         /// parameter description

static char            gpioName[8] = "gpioXXX";      /// Null terminated
static int	       irqNumber;
static int	       numberPresses = 0;
static bool	       ledOn = 0;
static struct          timespec ts_last, ts_current, ts_diff;

/// The prototype functions for the IRQ handler
irq_handler_t  gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs);

static ssize_t numberPresses_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", numberPresses);
}

static ssize_t numberPresses_store(struct kobject *kobj, struct kobj_attribute *attr,
                                   const char *buf, size_t count){
	sscanf(buf, "%du", &numberPresses);
	return count;
}

static ssize_t ledOn_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", ledOn);
}

static ssize_t lastTime_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%.2lu:%.2lu:%.2lu:%.9lu \n", (ts_last.tv_sec/3600)%24,
			(ts_last.tv_sec/60) % 60, ts_last.tv_sec % 60, ts_last.tv_nsec );
}

static ssize_t diffTime_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%lu.%.9lu\n", ts_diff.tv_sec, ts_diff.tv_nsec);
}

static struct kobj_attribute count_attr = __ATTR(numberPresses, 0666, numberPresses_show, numberPresses_store);
static struct kobj_attribute ledon_attr = __ATTR_RO(ledOn);
static struct kobj_attribute time_attr  = __ATTR_RO(lastTime);
static struct kobj_attribute diff_attr  = __ATTR_RO(diffTime);

static struct attribute *ebb_attrs[] = {
	&count_attr.attr,
	&ledon_attr.attr,
	&time_attr.attr,
	&diff_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.name  = gpioName,
	.attrs = ebb_attrs,
};

static struct kobject *ebb_kobj;

// The LKM initialization function -- The static keyword is used so that
// it is not visible outside this file. The __init token is a hint to the
// kernel that the function is used only at initialization time

static int __init ebbButton_init(void){
	int result = 0;
	unsigned long IRQflags = IRQF_TRIGGER_RISING;

	printk(KERN_INFO "EBB Button: Initializing the EBB Button LKM\n");
	sprintf(gpioName, "gpio%d", gpioButton);
	ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent);  /// kernel_kobj points to /sys/kernel
	if(!ebb_kobj){
		printk(KERN_ALERT "EBB Button: failed to create kobject\n");
		return -ENOMEM;
	}
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if(result) {
		printk(KERN_ALERT "EBB Button: failed to create sysfs group\n");
		kobject_put(ebb_kobj);
		return result;
	}

	getnstimeofday(&ts_last);
	ts_diff = timespec_sub(ts_last, ts_last);

	gpio_request(gpioLED, "sysfs");
	gpio_request(gpioButton, "sysfs");
	gpio_direction_output(gpioLED, 1);
	ledOn = true;
	gpio_direction_input(gpioButton);
	gpio_export(gpioLED, false);  // causes gpio49 to appear in /sys/class/gpio
				      // the second argument prevents the direction from being changed
	gpio_export(gpioButton, false);
	irqNumber = gpio_to_irq(gpioButton);
	printk(KERN_INFO "EBB Button: The button state is currently: %d\n", gpio_get_value(gpioButton));
	printk(KERN_INFO "EBB Button: The button is mapped to IRQ: %d\n", irqNumber);

	if(!isRising){
		IRQflags = IRQF_TRIGGER_FALLING;
	}
	result = request_irq(irqNumber, (irq_handler_t) gpio_irq_handler, IRQflags, "gpio_handler", NULL);
	return result;
}

// The LKM cleanup function -- make sure to destroy all devices etc.
static void __exit ebbButton_exit(void){
	printk(KERN_INFO "EBB Button: The button was pressed %d times\n", numberPresses);
	kobject_put(ebb_kobj);
	gpio_set_value(gpioLED, 0);
	gpio_unexport(gpioLED);
	free_irq(irqNumber, NULL);
	gpio_unexport(gpioButton);
	gpio_free(gpioLED);
	gpio_free(gpioButton);
	printk(KERN_INFO "EBB Button: Goodbye from the EBB Button LKM!\n");
}

/** @brief This is IRQ handler function.
 *  @param irq     The irq number
 *  @param *dev_id The Device ID
 *  @param *regs   Who knows?
 *  @retrun returns a message to indicate that the IRQ has been handled
 */
irq_handler_t gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs){
	ledOn = !ledOn;                                // invert the LED state -- used to store state
	gpio_set_value(gpioLED, ledOn);
	getnstimeofday(&ts_current);
	ts_diff = timespec_sub(ts_current, ts_last);
	ts_last = ts_current;
	printk(KERN_INFO "EBB Button: The button state is currently: %d\n", gpio_get_value(gpioButton));
	numberPresses++;
	return (irq_handler_t) IRQ_HANDLED;
}

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(ebbButton_init);
module_exit(ebbButton_exit);
