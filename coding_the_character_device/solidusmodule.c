#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> 			//file_operations structure - which ò course allows use to open/close, read/write to device.
#include <linux/cdev.h>			//this is a char driver; makes cdev available
#include <linux/semaphore.h> 		//used to access semaphores; sychronization behaviors.
#include <asm/uaccess.h>


//(1) create a structure for our fake device
struct facke_device{
	char data[100];
	struct semaphore sem;
} virtual_device; 


//(2) to later register our device we need a cdev object and some other variables.
struct cdev *mcdev; 
int major_number;			//will store our major numer -extracted from dev_t using macro - mknod /directory/file c major, minor.
int ret; 				//so declaring variables all over the pass in our module functions eats up the stack very fast 
dev_t dev_num;				//will hold major number that kernel give us
					//name -> appears in /proc/devices


#define		DEVICE_NAME	"solidusdevice"

//HERE
//(6) Tell the kernel which functions to call when user operates on our device file
struct file_operations fops = {
	.owner	= THIS_MODULE,		//prevent unloading of this module when operations are in use
	.open	= device_open,		//points to the method to call when opening the device
	.release= device_close,		//points to the method to call when closing the device
	.write	= device_write,		//points to the method to call when writing the device
	.read	= device_read		//points to the method to call when reading	 the device
}



static int driver_entry(void) {
	
	//(3) Register our device with the system: a two step process.
	//step(1) use dynamic allocation to assign our device
	//	a major number-- alloc_chrdev_region (dev_t*, uint count, char* name)
	ret = alloc_chrdev_region(&dev_num, 0,1, DEVICE_NAME);
	if(ret < 0) { //at time kernel functions return negatives , there is an error
		printk(KERN_ALERT "soliduscode: failed to allocate a major number");
		return ret; //propagate error
	}
	
	major_number = MAJOR(dev_num); //extracts the major number and store in our variable (MACRO)
	printk(KERN_INFO "soliduscode: major number is %d", major_number);
	printk(KERN_INFO "\tuse\"mknod /dev/%s c %d 0 \" for device file", DEVICE_NAME, major_number);


	//step(2)
	mcdev = cdev_alloc();
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;

	//now that we created cdev, we have to add it to the kernel
	//int cdev_add(struct cdev* dev, dev_t num, unsigned int count)
	ret = cdev_add(mcdev, dev_num, 1);
	if(ret < 0){
		printk(KERN_ALERT "soliduscode: unable to add cdev to kernel");
		return ret;
	}

	//(4) initialize our semaphore.
	sema_init(&virtual_device.sem, 1); //initial value of one.

	return 0;
}

static void driver_exit(void) {
	//(5)	unregister everything in reverse order
	//(a) 	
	cdev_del(mcdev);
	
	//(b)
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "soliduscode: unloaded module"); 
}
