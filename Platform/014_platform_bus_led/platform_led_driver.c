#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/uaccess.h>
#include <linux/errno.h>
#include <io.h>

struct resource *myresource;

struct device_help {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev m_cdev;
    struct class *m_class;
    struct device *m_device;
    char buf[32];
    unsigned int *vir_gpio_dr;
};

struct device_help dev1;

static int cdev_test_open(struct inode *inode, struct file *file) {
    file -> private_data = &dev1;
    printk("In cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read (struct file *file, char __user * buf, size_t size, loff_t * loff) {
    
    struct device_help *dev_pri = (struct device_help *)file -> private_data;
    if(copy_to_user(buf, dev_pri -> buf, strlen(dev_pri -> buf)) != 0) {
        printk("copy_to_usr failure\n");
        return -1;
    }
    return 0;
}


static ssize_t cdev_test_write (struct file *file, const char __user * buf, size_t size, loff_t * off) {
    
    struct device_help *dev_pri = (struct device_help *)file -> private_data;
    if(copy_from_user(dev_pri -> buf, buf, size) != 0) {
        printk("copy_from_usr failure\n");
        return -1;
    }
    printk("In cdev_test_write, buf = %s\n", dev_pri -> buf);
    return 0;
}

static int cdev_test_release (struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations cdev_ops = {
    .owner      = THIS_MODULE,
    .open       = cdev_test_open,
    .read       = cdev_test_read,
    .write      = cdev_test_write,
    .release    = cdev_test_release
};


// probe函数必须要有, 匹配成功后自动调用
// driver通过probe获取硬件资源
int mydriver_probe(struct platform_device *dev) {
    
    printk("platform_device_probe\n");

    // 拿到硬件资源
    myresource = platform_get_resource(dev, IORESOURCE_MEM, 0);
    printk("MEM = %lld\n", myresource -> start());

    int ret;

    // 自动分配设备号
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "led_alloc");
    if(ret < 0) {
       goto err_chrdev;
    }

    // 获取主，从设备号
    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.minor);

    // 注册设备
    dev1.m_cdev.owner = THIS_MODULE;
    cdev_init(&dev1.m_cdev, &cdev_ops);
    ret = cdev_add(&dev1.m_cdev, dev1.dev_num, 1);
    if(ret < 0) {
        goto err_chr_add;
     }

    // 设备节点
    dev1.m_class = class_create(THIS_MODULE, "mtest");
    if(IS_ERR(dev1.m_class)) {          // 判断是否错误，（落在错误码范围内）
        ret = PTR_ERR(dev1.m_class);    // 转变为错误码
        goto err_class_create;
    }
    dev1.m_device = device_create(dev1.m_class, NULL, dev1.dev_num, NULL, "mtest");
    if(IS_ERR(dev1.m_device)) {          // 判断是否错误，（落在错误码范围内）
        ret = PTR_ERR(dev1.m_device);    // 转变为错误码
        goto err_device_create;
    }

    dev1.vir_gpio_dr = ioremap(myresource -> start(), 4)
    if(IS_ERR(dev1.vir_gpio_dr)) {          // 判断是否错误，（落在错误码范围内）
        ret = PTR_ERR(dev1.vir_gpio_dr);    // 转变为错误码
        goto err_ioremap;
    }

    return 0;
err_ioremap:
    iounmap(dev1.vir_gpio_dr);
err_device_create:
    class_destroy(dev1.m_class);
err_class_create:
    cdev_del(&dev1.m_cdev);
err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);
err_chrdev:
    return ret;

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
    unregister_chrdev_region(dev1.dev_num, 1);
    cdev_del(&dev1.m_cdev);

    device_destroy(dev1.m_class, dev1.dev_num);
    class_destroy(dev1.m_class);
    ioumap(dev1.vir_gpio_dr);
    platform_driver_unregister(&platfrom_driver_test);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

