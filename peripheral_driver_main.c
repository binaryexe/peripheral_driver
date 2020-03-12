/* Module.h has details about module specific function such as 
*/
#include  <linux/module.h>
/* */
#include <linux/version.h>
/* kernel.h has information about KERN_INFO */
#include <linux/kernel.h>

static int __init pdd_init(void)
{
    printk(KERN_INFO "PDD Init Called");
    return 0;
}

static void __exit pdd_exit(void)
{
    printk(KERN_INFO "PDD Exit Called")
}

module_init(pdd_init);
module_exit(pdd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("exebinary@gmail.com")
MODULE_DESCRIPTION("For self-learning LDD")