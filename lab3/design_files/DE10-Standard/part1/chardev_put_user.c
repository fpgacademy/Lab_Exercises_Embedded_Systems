#include <linux/fs.h>               // struct file, struct file_operations
#include <linux/init.h>             // for __init, see code
#include <linux/module.h>           // for module init and exit macros
#include <linux/miscdevice.h>       // for misc_device_register and struct miscdev
#include <linux/uaccess.h>          // for copy_to_user, see code
#include <asm/io.h>                 // for mmap

/* Kernel character device driver. By default, this driver provides the text "Hello from 
 * chardev" when /dev/chardev is read (for example, cat /dev/chardev). The text can be changed 
 * by writing a new string to /dev/chardev (for example echo "New message" > /dev/chardev).
 * This version of the code uses put_user to send bytes to user-space */

static int device_open (struct inode *, struct file *);
static int device_release (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations chardev_fops = {
    .owner = THIS_MODULE,
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

#define SUCCESS 0
#define DEV_NAME "chardev"
#define MAX_SIZE 256    // we assume that no message longer than this will be used

static struct miscdevice chardev = { 
    .minor = MISC_DYNAMIC_MINOR, 
    .name = DEV_NAME,
    .fops = &chardev_fops,
    .mode = 0666
};

static int chardev_registered = 0;
static char chardev_msg[MAX_SIZE];    // the character array that can be read or written

static int __init start_chardev(void) {
    int err = misc_register (&chardev);
    if (err < 0) {
        printk (KERN_ERR "/dev/%s: misc_register() failed\n", DEV_NAME);
    }
    else {
        printk (KERN_INFO "/dev/%s driver registered\n", DEV_NAME);
        chardev_registered = 1;
    }
    strcpy (chardev_msg, "Hello from chardev\n"); /* initialize the message */

    return err;
}

static void __exit stop_chardev(void) {
    if (chardev_registered) {
        misc_deregister (&chardev);
        printk (KERN_INFO "/dev/%s driver de-registered\n", DEV_NAME);
    }
}

/* Called when a process opens chardev */
static int device_open(struct inode *inode, struct file *file) {
	return SUCCESS;
}

/* Called when a process closes chardev */
static int device_release(struct inode *inode, struct file *file) {
	return 0;
}

/* Called when a process reads from chardev. Returns number of bytes read; returns 0 when
 * the whole message has been sent, to indicate end-of-file. */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
	size_t bytes_read = 0;
	char *msg_Ptr = &(chardev_msg[*offset]);
	
	// Write to user buffer
	while (length && *msg_Ptr)  {
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}
	(*offset) = bytes_read;
	return bytes_read;
}

/* Called when a process writes to chardev */
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
	int i;
	for (i = 0; i < length; ++i)
		chardev_msg[i] = buffer[i];	// assume that data won't overflow
	chardev_msg[i] = '\0'; 				// NULL terminate
	return length;
}

MODULE_LICENSE("GPL");
module_init (start_chardev);
module_exit (stop_chardev);
