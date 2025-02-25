#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/time.h>
#include <linux/atmioc.h>


struct device_help {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev m_cdev;
    struct class *m_class;
    struct device *m_device;
    char buf[32];
    atomic64_t counter;
};

struct device_help dev1;

static void m_timer_callback(struct timer_list* t);


DEFINE_TIMER(m_timer, m_timer_callback);


static void m_timer_callback(strcut timer_list* t) {
    
    printk("processing timer callback\n");
    mod_timer(&m_timer, jiffies_64 + msecs_to_jiffies(1000));
    atomic64_inc(&dev1.counter);
}



static int cdev_test_open(struct inode *inode, struct file *file) {
    
    file -> private_data = &dev1;
    printk("In cdev_test_open\n");

    m_timer.expires = jiffies_64 + msecs_to_jiffies(1000);
    add_timer(&m_timer);
    atomic64_set(&dev1.counter, 0);

    return 0;
}

static ssize_t cdev_test_read (struct file *file, char __user * buf, size_t size, loff_t * loff) {
    
    unsigned int counter = 0;
    struct device_help *dev_pri = (struct device_help *)file -> private_data;

    counter = atomic64_read(&dev_pri -> counter);
    if(copy_to_user(buf, &counter, strlen(counter)) != 0) {
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
    del_timer(&m_timer);
    return 0;
}

static struct file_operations cdev_ops = {
    .owner      = THIS_MODULE,
    .open       = cdev_test_open,
    .read       = cdev_test_read,
    .write      = cdev_test_write,
    .release    = cdev_test_release
};



static int m_model_init(void) {
    int ret;

    // 自动分配设备号
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "test_alloc");
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

    return 0;

err_device_create:
    class_destroy(dev1.m_class);
err_class_create:
    cdev_del(&dev1.m_cdev);
err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);
err_chrdev:
    return ret;
}

static void m_model_exit(void) {
    unregister_chrdev_region(dev1.dev_num, 1);
    cdev_del(&dev1.m_cdev);

    device_destroy(dev1.m_class, dev1.dev_num);
    class_destroy(dev1.m_class);

    del_timer(&m_timer);

    printk("uninstall driver\n");
}

module_init(m_model_init);
module_exit(m_model_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

