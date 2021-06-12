#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major = 0;
static char str[BUF_LEN] = "Test string\0";

ssize_t my_read(struct file *fd, char __user *buff, size_t size, loff_t *off)
{
    size_t rc;

    rc = simple_read_from_buffer(buff, size, off, str, BUF_LEN);

    return rc;
}

ssize_t my_write(struct file *fd, const char __user *buff, size_t size, loff_t *off)
{
    size_t rc = 0;

    if (rc > BUF_LEN)
        return -EINVAL;

    rc = simple_write_to_buffer(str, BUF_LEN, off, buff, size);

    return rc;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = my_read,
    .write = my_write,
};

static int __init my_chrdev_init(void)
{
    pr_info("My module loaded\n");

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_info("Registering the character device failed with %d\n", major);
        return major;
    }

    pr_info("Registering the character device with Major %d\n", major);

	return 0;
}

static void __exit my_chrdev_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("My module unloaded\n");
}

module_init(my_chrdev_init);
module_exit(my_chrdev_exit);

MODULE_LICENSE("GPLv2");
MODULE_DESCRIPTION("Simple char dev kernel module\n");
