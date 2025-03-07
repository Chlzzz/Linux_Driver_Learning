#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>

int irq;

// *** 1、打包数据
struct work_data
{
    struct work_struct test_work;
    int a;
    int b; 
};

struct work_data test_workqueue_work;

// 自定义任务队列
struct workqueue_struct *test_workqueue;
// struct delayed_work m_delayed_work;

// *** 2、解释包数据
void test_work(struct work_struct *work) {
    printk("In workerqueue service function\n");
    struct work_data *pdata;
    pdata = container_of(work, struct work_data, test_work);

    printk("a = %d\n", pdata.a);
    printk("b = %d\n", pdata.b);
}

// 中断上文, 调度下文函数
irqreturn_t itr_service_func(int irq, void *args) {
    printk("In itr service function\n");

    queue_work(test_workqueue, &test_workqueue_work.test_work);
    return IRQ_RETVAL(IRQ_HANDLED);
}


static int interrupt_irq_init(void) {
    
    int ret;
    // eg: GPIO0_B5-----13  pin计算 bank[0] * 32 + number, number = group[B=1] * 8 + x[5]
    irq = gpio_to_irq(13);
    printk("irq is %d\n", irq);
     
    // 向内核申请中断
    ret = request_irq(irq, itr_service_func, IRQF_TRIGGER_RISING, "test", NULL);
    if(ret < 0) {
        printk("request failed\n");
    }

    // 创建任务队列
    test_workqueue = create_workqueue("test_workqueue");
    INIT_WORK(&test_workqueue_work.test_work, test_work);
    test_workqueue_work.a = 1;
    test_workqueue_work.b = 2;

    return 0;
}

static void interrupt_irq(void) {
    
    free_irq(irq, NULL);
    cancel_work_sync(&test_workqueue_work.test_work);
    flush_workqueue(test_workqueue);
    destory_workqueue(test_workqueue);

    printk("uninstall driver\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

