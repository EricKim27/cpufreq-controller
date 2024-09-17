#include <linux/printk.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/string.h>
#include "proccon.h"

/*
 ### How this module is going to work

 This module would make two entries in /sys/kernel interface.

 - /sys/kernel/freqcon/max
 - /sys/kernel/freqcon/min

 These two entries are going to be used to control
 the CPU's max/minimal frequency at all cores.
 
 It's important to note that the making of this module is solely for educational purposes.
*/

static int max = 0;
static int min = 0;
static struct kobject *freq_kobj;

void check_val()
{
    if(max > 0 && min > 0)
        limit_freq(max, min);
}

static ssize_t min_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", min);
}
static ssize_t min_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &min);
    check_val();
    return count;
}

static ssize_t max_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", max);
}
static ssize_t max_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &max);
    check_val();
    return count;
}
static struct kobj_attribute max_attr = __ATTR(max, 0660, max_show, max_store);
static struct kobj_attribute min_attr = __ATTR(min, 0660, min_show, min_store);

static struct attribute *attrs[] = {
    &max_attr.attr,
    &min_attr.attr,
    NULL
};
static struct attribute_group attr_group = {
    .attrs = attrs,
};

static int __init freqcon_init(void)
{
    int ret;
    freq_kobj = kobject_create_and_add("freqcon", kernel_kobj);
    if(!freq_kobj)
        return -ENOMEM;
    
    ret = sysfs_create_group(freq_kobj, &attr_group);
    if(ret)
        kobject_put(freq_kobj);

    printk(KERN_INFO "Added kobject freqcon\n");
    return 0;
}
static void __exit freqcon_exit(void)
{
    kobject_put(freq_kobj);
}

module_init(freqcon_init);
module_exit(freqcon_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Eric Kim");
MODULE_DESCRIPTION("A CPU max/min frequency controller module");