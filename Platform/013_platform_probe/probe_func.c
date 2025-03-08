#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

struct resource *myresource;

// probe函数必须要有, 匹配成功后自动调用
// driver通过probe获取硬件资源
int mydriver_probe(struct platform_device *dev) {
    printk("platform_device_probe\n");

    // 方法一
    printk("IRQ = %lld\n", dev -> resource[1].start);

    // 方法二
    myresource = platform_get_resource(dev, IORESOURCE_MEM, 0);
    printk("MEM = %lld\n", myresource -> start());
    myresource = platform_get_resource(dev, IORESOURCE_IRQ, 0);
    printk("IRQ = %lld\n", myresource -> start());

    return 0;
}

int mydriver_remove(struct platform_device *dev) {
    printk("platform_device_remove\n");
    return 0;
}

const struct platform_device_id mydriver_id_table = {
    .name = "mydevice",
}

strcut platform_driver platform_griver_test {
    .probe = mydriver_probe,
    .remove = mydriver_remove,
    .driver = {
        .name = "mydevice",
        .owner = THIS_MODULE,
    },
    .id_table = &mydriver_id_table,
};

static int platform_driver_init(void) {
    printk("platform_device_init\n");
    platform_driver_register(&platfrom_driver_test);
    int ret;
}

static void platform_driver_exit(void) {
    printk("platform_device_exit\n");
    platform_driver_unregister(&platfrom_driver_test);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

