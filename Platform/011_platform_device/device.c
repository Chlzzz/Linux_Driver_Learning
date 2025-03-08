#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

// 平台总线模型： 将驱动分为device和driver；

static struct resource my_device_resource[] = {
    [0] = {
        .start = 0xFDD60000,
        .end = 0xFDD60004,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 13,
        .end = 13,
        .flags = IORESOURCE_IRQ,
    },
};

void mydevice_release(struct device *dev) {
    printk("In device release func\n");
}

// 定义结构体，描述硬件资源
struct platfrom_device platfrom_device_test = {
    .name = "mydevice",
    .id = -1,
    .resource = my_device_resource,
    .num_resource = ARRAY_SIZE(my_device_resource),
    .dev = {
        .release = mydevice_release,
    },
};

static int platform_device_init(void) {
    printk("platform_device_init\n");
    platform_device_register(&platfrom_device_test);
    int ret;
}

static void platform_device_exit(void) {
    printk("platform_device_exit\n");
    platform_device_unregister(&platfrom_device_test);
}

module_init(platform_device_init);
module_exit(platform_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

