#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/time.h>
#include <linux/atmioc.h>

#define BUFSIZE 1024
static char mem[BUFSIZE] = {0};

struct device_help {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev m_cdev;
    struct class *m_class;
    struct device *m_device;
    char buf[32];
};

struct device_help dev1;


static int cdev_test_open(struct inode *inode, struct file *file) {
    
    file -> private_data = &dev1;
    printk("In cdev_test_open\n");


    return 0;
}

static ssize_t cdev_test_read (struct file* file, char __user* buf, size_t size, loff_t* loff) {
    
    loff_t pos = loff;
    size_t count = size;

    if(pos > BUFFSIZE) {
        return 0;
    }
    if(count > BUFFSIZE - p) {
        count = BUFFSIZE - p;
    }

    // unsigned int counter = 0;
    // struct device_help *dev_pri = (struct device_help *)file -> private_data;

    if(copy_to_user(buf, mem + pos, count)!= 0) {
        printk("copy_to_usr failure\n");
        return -1;
    }

    for(int i = 0; i < 20; ++i) {
        printk("buf[%d] is %c\n", i, mem[i]);
    }
    printk("mem is %s, pos is %lu, count is %d\n", mem + pos, pos, count);
    *loff += count;
    return count;
}


static ssize_t cdev_test_write (struct file *file, const char __user *buf, size_t size, loff_t *loff) {

    loff_t pos = loff;
    size_t count = size;

    if(pos > BUFFSIZE) {
        return 0;
    }
    if(count > BUFFSIZE - p) {
        count = BUFFSIZE - p;
    }

    struct device_help *dev_pri = (struct device_help *)file -> private_data;

    if(copy_from_user(mem + pos, buf, count) != 0)) {
        printk("copy_to_usr failure\n");
        return -1;
    }
    printk("mem is %s, pos is %lu", mem + pos, pos);

    *loff += count;
    return count;
}

static int cdev_test_release (struct inode *inode, struct file *file) {

    return 0;
}


loff_t  cdev_test_llseek(struct file * file, loff_t offdet, int whence) {
    loff_t new_offset;
    switch(whence) {
        case SEEK_SET:
            if(offset < 0 || offset > BUFSIZE) {
                return -EINVAL;
            }
            new_offset = offset;
        break;
        case SEEK_CUR:
            if((file -> f_pos + offset) > BUFFSIZE || (file -> f_pos + offset) < 0) {
                return -EINVAL;
            }
            new_offset = file -> f_pos + offset;
        break;
        case SEEK_END:
            if((file -> f_pos + offset) < 0) {
                return -EINVAL;
            }
            new_offset = BUFSIZE + offset;
        break;
        default:
            return -EINVAL;
    }
    file -> f_pos = new_offset;
    return new_offset;
}


static struct file_operations cdev_ops = {
    .owner      = THIS_MODULE,
    .open       = cdev_test_open,
    .read       = cdev_test_read,
    .write      = cdev_test_write,
    .release    = cdev_test_release,
    .llseek     = cdev_test_llseek
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


    printk("uninstall driver\n");
}

module_init(m_model_init);
module_exit(m_model_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");

