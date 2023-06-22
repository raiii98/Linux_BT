#include <linux/module.h> /* Thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h>     /* Thu vien nay dinh nghia cac ham allocate major & minor number */

#define DRIVER_AUTHOR "Raiii quoctoan24898@gmail.com"
#define DRIVER_DESC "Hello world kernel module"
#define DRIVER_VERS "1.0"
struct m_dev
{
    dev_t dev_numb;
} mdev;
/* Constructor */
static int __init m_dev_init(void)
{
    if (alloc_chrdev_region(&mdev.dev_numb, 0, 1, "my_device") < 0)
    {
        printk("Failed to register device number dynamically\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_numb), MINOR(mdev.dev_numb));
    printk("Initialize drive succesfully\n");
    return 0;
}

/* Destructor */
static void __exit m_dev_exit(void)
{
    unregister_chrdev_region(mdev.dev_numb, 1);
    printk("Exit driver\n");
}

module_init(m_dev_init);
module_exit(m_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
