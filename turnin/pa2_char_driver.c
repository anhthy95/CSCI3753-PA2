

#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
static int device_open = 0;
static int device_close = 0;

static char device_buffer[BUFFER_SIZE];

ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	int device_buffer_size= strlen(device_buffer);
	if(device_buffer_size == 0){
	printk(KERN_ALERT "device buffer is empty");
	return 0;
	}
	printk(KERN_ALERT "Reading from device\n");
	int bytes = length;
	copy_to_user(buffer, device_buffer+*offset, bytes);
	*offset = pfile->f_pos;
	printk(KERN_ALERT "Read %d bytes from device. Offset value is %d",bytes,*offset);


	return 0;
}



ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */

	/* If the user space buffer is empty, there's nothing to write to return 0 */
	if(length==0){
	printk(KERN_ALERT "user buffer is empty");
	return 0;
	}
	int device_buffer_size=strlen(device_buffer);
	/* Write from the user space buffer to the device buffer */
	printk(KERN_ALERT "Writing to device\n");
	copy_from_user(device_buffer+*offset, buffer,length);
	/* Print out the amount of bytes that the user wrote */
	int bytes = length;
	*offset = pfile->f_pos+bytes;
	printk(KERN_ALERT "wrote %d bytes to device. Offest value is %d",bytes,*offset);
	return length;
}


int pa2_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	device_open++;
	printk(KERN_ALERT "file is opened %d times",device_open);

	return 0;
}

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	device_close++;
        printk(KERN_ALERT "file is closed %d times",device_close);

	return 0;
}

loff_t llseek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	loff_t temp_pos = 0;
	
	/*use temp_pos so can verfiy does not exceed BOF or EOF before updating pfile->f_pos*/
	switch(whence){
	
	case 0: temp_pos = offset;
	break;
	case 1: temp_pos= pfile->f_pos + offset;
	break;
	case 2: temp_pos= BUFFER_SIZE - offset;
	break;
	}//switch

	if(temp_pos> BUFFER_SIZE){
	printk(KERN_ALERT "ERROR, exeeded buffer size");
	temp_pos= pfile->f_pos;
}
	if(temp_pos< 0){

	printk(KERN_ALERT "ERROR, preceeded BOF");
	temp_pos=pfile->f_pos;
}
	pfile->f_pos = temp_pos;
	printk(KERN_ALERT "offset value is: %d", temp_pos);

	return temp_pos;
}

struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	.open	 = pa2_char_driver_open,					
	.release = pa2_char_driver_close,				
	.read	 = pa2_char_driver_read,				
	.write	 = pa2_char_driver_write,					
	.llseek	 = llseek		/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	//allocate memory for device_buffer

	//device_buffer= kmalloc(BUFFER_SIZE,GFP_KERNEL);
	/* register the device */
	register_chrdev(252, "pa2_char_driver", &pa2_char_driver_file_operations);
	return 0;
}

static void pa2_char_driver_exit(void)
	{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);

	//free kmalloc memory
	//kfree(device_buffer);
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(252, "pa2_char_driver");

	}


module_init(pa2_char_driver_init);
module_exit(pa2_char_driver_exit);
/* add module_init and module_exit to point to the corresponding init and exit function*/
