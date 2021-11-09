#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>

/* Kernel character device driver. By default, this driver provides the text "Hello from 
 * chardev" when /dev/chardev is read (for example, cat /dev/chardev). The text can be changed 
 * by writing a new string to /dev/chardev (for example echo "New message" > /dev/chardev).
 * This version of the code uses copy_to_user and copy_from_user, to send data to, and receive
 * date from, user-space */

static int device_open (struct inode *, struct file *);
static int device_release (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"

static dev_t dev_no = 0;
static struct cdev *chardev_cdev = NULL;
static struct class *chardev_class = NULL;
#define MAX_SIZE 256	// we assume that no message longer than this will be used
static char chardev_msg[MAX_SIZE];	// the character array that can be read or written

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static int __init start_chardev(void)
{
	int err = 0;

	/* Get a device number. Get one minor number (0) */
	if ((err = alloc_chrdev_region (&dev_no, 0, 1, DEVICE_NAME)) < 0) {
		printk (KERN_ERR "chardev: alloc_chrdev_region() failed with return value %d\n", err);
		return err;
	}

	// Allocate and initialize the character device
	chardev_cdev = cdev_alloc (); 
	chardev_cdev->ops = &fops; 
	chardev_cdev->owner = THIS_MODULE; 
   
	// Add the character device to the kernel
	if ((err = cdev_add (chardev_cdev, dev_no, 1)) < 0) {
		printk (KERN_ERR "chardev: cdev_add() failed with return value %d\n", err);
		return err;
	}
	
	chardev_class = class_create (THIS_MODULE, DEVICE_NAME);
	device_create (chardev_class, NULL, dev_no, NULL, DEVICE_NAME );
	
	strcpy (chardev_msg, "Hello from chardev\n"); /* initialize the message */

	return 0;
}

static void __exit stop_chardev(void)
{
	device_destroy (chardev_class, dev_no);
	cdev_del (chardev_cdev);
	class_destroy (chardev_class);
	unregister_chrdev_region (dev_no, 1);
}

/* Called when a process opens chardev */
static int device_open(struct inode *inode, struct file *file)
{
	return SUCCESS;
}

/* Called when a process closes chardev */
static int device_release(struct inode *inode, struct file *file)
{
	return 0;
}

/* Called when a process reads from chardev. Provides character data from chardev_msg.
 * Returns, and sets *offset to, the number of bytes read. */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	bytes = strlen (chardev_msg) - (*offset);	// how many bytes not yet sent?
	bytes = bytes > length ? length : bytes;	// too much to send all at once?
	
	if (bytes)
		if (copy_to_user (buffer, &chardev_msg[*offset], bytes) != 0)
			printk (KERN_ERR "Error: copy_to_user unsuccessful");
	*offset = bytes;	// keep track of number of bytes sent to the user
	return bytes;
}

/* Called when a process writes to chardev. Stores the data received into chardev_msg, and 
 * returns the number of bytes stored. */
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	size_t bytes;
	bytes = length;

	if (bytes > MAX_SIZE - 1)	// can copy all at once, or not?
		bytes = MAX_SIZE - 1;
	if (copy_from_user (chardev_msg, buffer, bytes) != 0)
		printk (KERN_ERR "Error: copy_from_user unsuccessful");
	chardev_msg[bytes] = '\0';	// NULL terminate
	// Note: we do NOT update *offset; we just copy the data into chardev_msg
	return bytes;
}

MODULE_LICENSE("GPL");
module_init (start_chardev);
module_exit (stop_chardev);
