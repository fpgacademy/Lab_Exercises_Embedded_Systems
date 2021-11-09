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
 * This version of the code uses put_user to send bytes to user-space */

static int device_open (struct inode *, struct file *);
static int device_release (struct inode *, struct file *);
static ssize_t device_read (struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"

static dev_t dev_no = 0;
static struct cdev *chardev_cdev = NULL;
static struct class *chardev_class = NULL;
static char chardev_msg[256];						// the string that can be read or written

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
	err = alloc_chrdev_region (&dev_no, 0, 1, DEVICE_NAME);
	if (err < 0) {
		printk (KERN_WARNING "chardev driver: alloc_chrdev_region() failed\n");
		return err;
	}
	
	chardev_class = class_create (THIS_MODULE, DEVICE_NAME);
	
	// Allocate and initialize the char device
	chardev_cdev = cdev_alloc (); 
	chardev_cdev->ops = &fops; 
	chardev_cdev->owner = THIS_MODULE; 
   
	// Add the character device to the kernel
	err = cdev_add (chardev_cdev, dev_no, 1);
	if (err < 0) {
		printk (KERN_WARNING "chardev driver: cdev_add() failed\n");
		return err;
	}
	
	device_create (chardev_class, NULL, dev_no, NULL, DEVICE_NAME );
	strcpy (chardev_msg, "Hello from chardev\n"); 

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

/* Called when a process reads from chardev. Returns number of bytes read; returns 0 when
 * the whole message has been sent, to indicate end-of-file. */
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
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
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
	int i;
	for (i = 0; i < length; ++i)
		chardev_msg[i] = buffer[i];	// assume that data won't overflow
	chardev_msg[i] = '\0'; 				// NULL terminate
	return length;
}

MODULE_LICENSE("GPL");
module_init (start_chardev);
module_exit (stop_chardev);
