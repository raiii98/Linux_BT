#include <linux/module.h> /* Thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h>     /* Thu vien nay dinh nghia cac ham allocate major & minor number */
#include <linux/device.h> /* Thu vien dinh nghia class, devides */

#define DRIVER_AUTHOR "Raiii quoctoan24898@gmail.com"
#define DRIVER_DESC "Hello world kernel module"
#define DRIVER_VERS "1.0"
struct m_dev
{
    dev_t dev_numb;
    struct class *dev_class;
    struct device *dev;
} mdev;
/* Constructor */
static int __init m_dev_init(void)
{
    // 1. Cấp phát device number
    if (alloc_chrdev_region(&mdev.dev_numb, 0, 1, "my_device") < 0)
    {
        printk("Failed to register device number dynamically\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_numb), MINOR(mdev.dev_numb));

    // 2. Tạo struct class
    if ((mdev.dev_class = class_create(THIS_MODULE, "my_class")) == NULL)
    {
        pr_err("Cannot create the struct class for my device\n");
        goto failed_creat_class;
    }
    // 3. Tạo device trong class
    if ((device_create(mdev.dev_class, NULL, mdev.dev_numb, NULL, "my_device")) == NULL)
    {
        pr_err("Cannot create my device\n");
        goto failed_creat_device;
    }
    return 0;
failed_creat_class:
    class_destroy(mdev.dev_class);
failed_creat_device:
    device_destroy(mdev.dev_class, mdev.dev_numb);
    return -1;
}

/* Destructor */
static void __exit m_dev_exit(void)
{
    device_destroy(mdev.dev_class, mdev.dev_numb);
    class_destroy(mdev.dev_class);
    unregister_chrdev_region(mdev.dev_numb, 1);
    printk("Exit driver\n");
}

module_init(m_dev_init);
module_exit(m_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
