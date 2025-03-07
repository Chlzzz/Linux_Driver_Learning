#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>

int irq;

struct work_struct m_queuework;

// 自定义任务队列
struct workqueue_struct *test_workqueue;
// struct delayed_work m_delayed_work;

void test_work(struct work_struct *work) {
    printk("In workerqueue service function\n");
}

// 中断上文, 调度下文函数
irqreturn_t itr_service_func(int irq, void *args) {
    printk("In itr service function\n");

    queue_work(test_workqueue, &m_queuework);
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

    // 创建并法任务队列
    test_workqueue = alloc_workqueue("test_workqueue", WQ_UNBOUND, 0);
    INIT_WORK(&test_workqueue, m_queuework);

    return 0;
}

static void interrupt_irq(void) {
    
    free_irq(irq, NULL);
    cancel_work_sync(&m_queuework);
    flush_workqueue(test_workqueue);
    destory_workqueue(test_workqueue);

    printk("uninstall driver\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

