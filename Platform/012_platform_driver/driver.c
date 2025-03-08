#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

// 平台总线模型： 将驱动分为device和driver；

// probe函数必须要有
int mydriver_probe(struct platform_device *dev) {
    printk("platform_device_probe\n");
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

