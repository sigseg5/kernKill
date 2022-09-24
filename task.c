#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
 
#define TIMEOUT 1000    //milliseconds
static struct timer_list etx_timer;
static unsigned int count = 0;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
 
//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{
    pr_info("Timer Callback function Called [%d]\n",count++);
    
    /*
       Re-enable timer. 
    */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}

/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
    /* timer callback my_timer_callback */
    timer_setup(&etx_timer, timer_callback, 0);
 
    /* setup timer interval */
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));
 
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
    /* remove kernel timer when unloading module */
    del_timer(&etx_timer);
    pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL3");
MODULE_AUTHOR("sigseg5 <sigseg5@protonmail.com>");
MODULE_DESCRIPTION("kernKill");
MODULE_VERSION("0.1");