
/*************************************
 *               Include             *
 *************************************/
#include <linux/module.h>  /* Define functions macro as module_init and module_exit */
#include <linux/fs.h>      /* Define functions allocate major & minor number */
#include <linux/device.h>  /* Define functions class, devides */
#include <linux/cdev.h>    /* Define functions cdev_init/cdev_add */
#include <linux/uaccess.h> /* Define functions copy_to_user(), copy_from_user() */
#include <linux/slab.h>    /* Define functions kfree() */

/*************************************
 *               Define              *
 *************************************/
#define DRIVER_AUTHOR "Raiii quoctoan24898@gmail.com"
#define DRIVER_DESC "Hello world kernel module"
#define DRIVER_VERS "1.0"
#define NPAGES 1

/*************************************
 *       Definition variable         *
 *************************************/
struct m_dev
{
    int32_t size;
    char *kmalloc_ptr;
    dev_t dev_numb;
    struct class *dev_class;
    struct device *dev;
    struct cdev m_cdev;
} mdev;

/*************************************
 *       Function Prototypes         *
 *************************************/
static int __init m_dev_init(void);
static void __exit m_dev_exit(void);
static int m_open(struct inode *inode, struct file *file);
static int m_release(struct inode *inode, struct file *file);
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t m_write(struct file *filp, const char *user_buf, size_t size, loff_t *offset);
static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .read = m_read,
        .write = m_write,
        .open = m_open,
        .release = m_release,
};

/* This function will be called when we open the Device file */
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");
    return 0;
}

/* This function will be called when we close the Device file */
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");
    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_read = (size > mdev.size - *offset) ? (mdev.size - *offset) : size;

    /* Copy from mapped area to user buffer */
    if (copy_to_user(user_buf, mdev.kmalloc_ptr + *offset, to_read))
        return -EFAULT;

    *offset += to_read;
    printk("%s\n", mdev.kmalloc_ptr);

    return to_read;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    size_t to_write;

    pr_info("System call write() called...!!!\n");

    /* Check size doesn't exceed our mapped area size */
    to_write = (size + *offset > NPAGES * PAGE_SIZE) ? (NPAGES * PAGE_SIZE - *offset) : size;

    /* Copy from user buffer to mapped area */
    memset(mdev.kmalloc_ptr, 0, NPAGES * PAGE_SIZE);
    if (copy_from_user(mdev.kmalloc_ptr + *offset, user_buf, to_write) != 0)
        return -EFAULT;

    pr_info("Data from usr: %s", mdev.kmalloc_ptr);

    *offset += to_write;
    mdev.size = *offset;

    return to_write;
}

/*************************************
 *            Constructor            *
 *************************************/
static int __init m_dev_init(void)
{
    // 1. Dynamic allocation for read or write
    mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL);
    if (!mdev.kmalloc_ptr)
    {
        pr_err("Failed to allocate memory using kmalloc\n");
        return -ENOMEM;
    }

    // 2. Allocation device number
    if (alloc_chrdev_region(&mdev.dev_numb, 0, 1, "my_device") < 0)
    {
        printk("Failed to register device number dynamically\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_numb), MINOR(mdev.dev_numb));

    // 3. Creat struct class
    if ((mdev.dev_class = class_create(THIS_MODULE, "my_class")) == NULL)
    {
        pr_err("Cannot create the struct class for my device\n");
        goto failed_creat_class;
    }
    // 4. Creat device in class
    if ((device_create(mdev.dev_class, NULL, mdev.dev_numb, NULL, "my_device")) == NULL)
    {
        pr_err("Cannot create my device\n");
        goto failed_creat_device;
    }
    // 5.1. Creat character device
    cdev_init(&mdev.m_cdev, &fops);
    // 5.2. Add device to system
    if ((cdev_add(&mdev.m_cdev, mdev.dev_numb, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto failed_cdev;
    }

    return 0;

failed_creat_class:
    class_destroy(mdev.dev_class);
failed_creat_device:
    device_destroy(mdev.dev_class, mdev.dev_numb);
failed_cdev:
    cdev_del(&mdev.m_cdev);
    return -1;
}
/*************************************
 *          Destructor               *
 *************************************/
static void __exit m_dev_exit(void)
{
    cdev_del(&mdev.m_cdev);                        /* 05 */
    device_destroy(mdev.dev_class, mdev.dev_numb); /* 04 */
    class_destroy(mdev.dev_class);                 /* 03 */
    unregister_chrdev_region(mdev.dev_numb, 1);    /* 02 */
    kfree(mdev.kmalloc_ptr);                       /* 01 */
    printk("Exit driver\n");
}
// Macro to register function m_dev_init
module_init(m_dev_init);
// Macro to register function m_dev_exit
module_exit(m_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);
