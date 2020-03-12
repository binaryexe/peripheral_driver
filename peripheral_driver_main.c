/* Module.h has details about module specific function such as 
*/
#include  <linux/module.h>
/* */
#include <linux/version.h>
/* kernel.h has information about KERN_INFO */
#include <linux/kernel.h>
/* file_operations structures are defined here */
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
static dev_t dev_num;
static struct class *cl; // Global variable for the device class
static struct cdev c_dev; // Global variable for the character device structure

static int ppdd_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "PPD Driver: open()\n");
    return 0;
}

static int ppdd_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "PPD Driver: close()\n");
    return 0;
}

static ssize_t ppdd_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "PPD Driver: read()\n");
    return 0;
}

static ssize_t ppdd_write(struct file *f, const char __user *buf, size_t len,
    loff_t *off)
{
    printk(KERN_INFO "PPD Driver: write()\n");
    return len;
}

static struct file_operations ppdd_fops =
{
    .owner = THIS_MODULE,
    .open = ppdd_open,
    .release = ppdd_close,
    .read = ppdd_read,
    .write = ppdd_write
    /* data */
};

static int __init ppdd_init(void)
{
    int ret = 0;
    printk(KERN_INFO "PPDD Init Called");
    /* The below API dynamically figures out a free major number and
     * registers the cnt number of device file numbers starting from
     * <the free major, firstminor>, with the name. */
    /* Registers a range of char device numbers */
    /* When you load driver and run the command 'cat /proc/devices'
     * you can see the driver name 'bx'
     */
    if ((ret = alloc_chrdev_region(&dev_num, 0, 3, "bx")) < 0)
    {
        return ret;
    }
    /* Kernel populates the appropriate device class & device info into
     * the /sys window for the device under consideration. */
    /* Create device class */
    if ((cl = class_create(THIS_MODULE, "ppd_drv")) == NULL)
    {
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }
    /* Create device info */
    if (device_create(cl, NULL, dev_num, NULL, "mynull") == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }
    /* Initialize the character device structure (struct cdev c_dev) 
     * with that, using cdev_init()*/  
    cdev_init(&c_dev, &ppdd_fops);
    
    /* to hand this structure to the VFS using the call cdev_add() */
     if (cdev_add(&c_dev, dev_num, 1) == -1)
    {
        device_destroy(cl, dev_num);
        class_destroy(cl);
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    return 0;
}

static void __exit ppdd_exit(void)
{
    printk(KERN_INFO "PPDD Exit Called");
    cdev_del(&c_dev);
    device_destroy(cl, dev_num);
    class_destroy(cl);
    unregister_chrdev_region(dev_num, 1);
}

module_init(ppdd_init);
module_exit(ppdd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("exebinary@gmail.com");
MODULE_DESCRIPTION("For self-learning LDD");