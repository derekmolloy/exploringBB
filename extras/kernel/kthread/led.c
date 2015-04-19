/**
 * @file   led.c
 * @author Derek Molloy
 * @date   16 April 2015
 * @brief  A kernel module for controlling a simple LED (or any signal) that is connected to
 * a GPIO. It is fully threaded.
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
#include <linux/kthread.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Derek Molloy");
MODULE_DESCRIPTION("A simple Linux LED driver for the BBB");
MODULE_VERSION("0.1");

static unsigned int gpioLED = 49;           /// Default GPIO is 49
module_param(gpioLED, uint, S_IRUGO);       /// Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(gpioLED, " GPIO LED number (default=49)");         /// parameter description

static unsigned int blinkPeriod = 1000;
module_param(blinkPeriod, uint, S_IRUGO);       /// Param desc. S_IRUGO can be read/not changed
MODULE_PARM_DESC(blinkPeriod, " LED blink period in ms (default=1000)");         /// parameter description

static char            ledName[7] = "ledXXX";      /// Null terminated
static bool	       ledOn = 0;

static ssize_t ledOn_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
	return sprintf(buf, "%d\n", ledOn);
}

//static struct kobj_attribute count_attr = __ATTR(numberPresses, 0666, numberPresses_show, numberPresses_store);
static struct kobj_attribute ledon_attr = __ATTR_RO(ledOn);

static struct attribute *ebb_attrs[] = {
	&ledon_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.name  = ledName,
	.attrs = ebb_attrs,
};

static struct kobject *ebb_kobj;
static struct task_struct *task;

static int flash(void *arg){
	printk(KERN_INFO "EBB LED: Thread has started running \n");
	while(!kthread_should_stop()){
		schedule();
		set_current_state(TASK_INTERRUPTIBLE);
	}
	printk(KERN_INFO "EBB LED: Thread has run to completion \n");
	return 0;
}

// The LKM initialization function -- The static keyword is used so that
// it is not visible outside this file. The __init token is a hint to the
// kernel that the function is used only at initialization time

static int __init ebbLED_init(void){
	int result = 0;
//	unsigned long IRQflags = IRQF_TRIGGER_RISING;

	printk(KERN_INFO "EBB LED: Initializing the EBB LED LKM\n");
	sprintf(ledName, "led%d", gpioLED);
	ebb_kobj = kobject_create_and_add("ebb", kernel_kobj->parent);  /// kernel_kobj points to /sys/kernel
	if(!ebb_kobj){
		printk(KERN_ALERT "EBB LED: failed to create kobject\n");
		return -ENOMEM;
	}
	result = sysfs_create_group(ebb_kobj, &attr_group);
	if(result) {
		printk(KERN_ALERT "EBB LED: failed to create sysfs group\n");
		kobject_put(ebb_kobj);
		return result;
	}

	gpio_request(gpioLED, "sysfs");
	gpio_direction_output(gpioLED, 1);
	ledOn = true;
	gpio_export(gpioLED, false);  // causes gpio49 to appear in /sys/class/gpio
				      // the second argument prevents the direction from being changed

	task = kthread_run(flash, NULL, "flash_thread");
	if(IS_ERR(task)){
		printk(KERN_ALERT "EBB LED: failed to create the task\n");
		return PTR_ERR(task);
	}

//	irqNumber = gpio_to_irq(gpioButton);
//	printk(KERN_INFO "EBB LED: The button state is currently: %d\n", gpio_get_value(gpioButton));

//	result = request_irq(irqNumber, (irq_handler_t) gpio_irq_handler, IRQflags, "gpio_handler", NULL);
	return result;
}

// The LKM cleanup function -- make sure to destroy all devices etc.
static void __exit ebbLED_exit(void){
	kthread_stop(task);
	kobject_put(ebb_kobj);
	gpio_set_value(gpioLED, 0);
	gpio_unexport(gpioLED);
//	free_irq(irqNumber, NULL);
	gpio_free(gpioLED);
	printk(KERN_INFO "EBB LED: Goodbye from the EBB LED LKM!\n");
}

/** @brief This is IRQ handler function.
 *  @param irq     The irq number
 *  @param *dev_id The Device ID
 *  @param *regs   Who knows?
 *  @retrun returns a message to indicate that the IRQ has been handled
 */

/*
irq_handler_t gpio_irq_handler(int irq, void *dev_id, struct pt_regs *regs){
	ledOn = !ledOn;                                // invert the LED state -- used to store state
	gpio_set_value(gpioLED, ledOn);
	getnstimeofday(&ts_current);
	ts_diff = timespec_sub(ts_current, ts_last);
	ts_last = ts_current;
	printk(KERN_INFO "EBB Button: The button state is currently: %d\n", gpio_get_value(gpioButton));
	numberPresses++;
	return (irq_handler_t) IRQ_HANDLED;
}*/

// This next calls are  mandatory -- they identify the initialization function
// and the cleanup function (as above).
module_init(ebbLED_init);
module_exit(ebbLED_exit);
