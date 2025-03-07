#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

int irq;

// 动态创建
struct tasklet_struct m_tasklet;

// 中断下文服务函数，处理耗时任务
void mtasklet_func(insigned long data) {
    printk("data = %d\n", data);
}

// 中断下文， 静态
// DECLARE_TASKLET(m_tasklet, mtasklet_func, 1)

// 中断上文, 调度下文函数
irqreturn_t itr_service_func(int irq, void *args) {
    printk("In itr service function\n");

    tasklet_schedule(&m_tasklet);
    return IRQ_RETVAL(IRQ_HANDLED);
}

// 每个外设的外部中断使用irqdesc结构体进行描述，【中断描述符】 
// 管理中断描述符：1.静态  struct irq_desc irq_desc[NR_IRQS] 2.动态, radix tree
// CONFIG_SPARSE_IRQ选择 1 or 2

// 从中断号找到对应的（硬件）中断描述符  映射为 （软件）中断描述符 

// irq_desc{ irqaction }, irqaction { handler}

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

    // 动态创建tasklet
    tasklet_init(&m_tasklet, mtasklet_func, 1);


    return 0;
}

static void interrupt_irq(void) {
    
    free_irq(irq, NULL);
    tasklet_enable(&m_tasklet);  // 下次 是使能的
    tasklet_kill(&m_tasklet);
    printk("uninstall driver\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

