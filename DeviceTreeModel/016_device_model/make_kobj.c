#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

struct kobject *mkobject01;
struct kobject *mkobject02;
struct kobject *mkobject03;

struct kobj_type mtype;


static int mkobj_init(void) {
    
    // 创建kobject，方法1
    // 创建的目录， 父节点
    mkobject01 = kobject_create_and_add("mkobject01", NULL);
    // 在mkobject01目录下创建mkobject02
    mkobject02 = kobject_create_and_add("mkobject02", mkobject01);

    // 创建kobject，方法2
    // 需要先分配内存
    mkobject03 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    int ret;
    ret = kobject_init_and_add(mkobject03, &mtype, NULL, "%s", "myobject03");
 
    return 0;
}

static void mkobj_exit(void) {
    kobject_put(mkobject01);
    kobject_put(mkobject02);
    kobject_put(mkobject03);
}


module_init(mkobj_init);
module_exit(mkobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chl");
MODULE_VERSION("v1.0");