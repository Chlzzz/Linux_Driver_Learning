#include <linux/time.h>  

static void timer_callback(struct timer_list* t);

// 初始化
DEFINE_TIMER(test_timer, timer_callback); 

//  超时处理函数
static void timer_callback(struct timer_list* t) {
    printk("Timer timeout");
    mod_timer(t, jiffies_64 + msecs_to_jiffies(1000));
}

static int timer_init(void) {
    printk("driver init\n");
    test_timer.expires = jiffies_64 + msecs_to_jiffies(1000);   // 64位系统

    // 注册
    add_time(&test_timer);
}

static void timer_exit(void) {
    printk("driver exit\n");
    //  删除
    del_timer(&test_timer);
}

module_init(timer_init);
module_exit(timer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0")