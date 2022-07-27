#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/errno.h>
#include<linux/device.h>
#include<linux/mutex.h>
#include<linux/i2c.h>
#include<linux/delay.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>

#define DEVICE_NAME	"mcp9808"
#define MCP_MAGIC	'M'
#define SHUTDWN_NO	0x01
#define MCP_SHUTDWN	_IO(MCP_MAGIC,SHUTDWN_NO)
#define WAKEUP_NO       0x02
#define MCP_WKEUP       _IO(MCP_MAGIC,WAKEUP_NO)

static unsigned int mcp_major;
static unsigned int mcp_minor;

struct mcp_data {
    struct cdev dev;
    unsigned char *data;
    struct i2c_client *client;
};

MODULE_AUTHOR("Jaggu");
MODULE_LICENSE("GPL");

struct class *mcp_class = NULL;

static const struct of_device_id mcp98xx_of[] = {
    {.compatible = "microchip,mcp9808",},
    { /*sentinel*/ },
};


int mcp_open(struct inode *inode,struct file *filp)
{
    struct mcp_data *mcp = NULL;

    mcp = container_of(inode->i_cdev,struct mcp_data,dev);

    if(mcp == NULL){
	pr_err("container_of did not found valid data\n");
	return -ENODEV;
    }
    
    filp->private_data = mcp;
    mcp->data = kmalloc(2,GFP_KERNEL);
    if(mcp->data == NULL){
	pr_err("Error allocating memory\n");
	return -ENOMEM;
    }
    return 0;

}

int mcp_release(struct inode *inode,struct file *filp)
{
    struct mcp_data *mcp = filp->private_data;
    if(mcp->data != NULL){
	kfree(mcp->data);
	mcp->data = NULL;
    }
    return 0;
}

ssize_t mcp_read(struct file *filp,char __user *buf,size_t count,loff_t *f_pos)
{
   u8 reg_addr;
   struct i2c_msg msg[2];
   struct mcp_data *mcp = filp->private_data;
   u8 rawtmp[2];

   /* Temperature raw value register address */
   reg_addr = 0x05;
    msg[0].addr = mcp->client->addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &reg_addr;

    msg[1].addr = mcp->client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].len = 2;
    msg[1].buf = rawtmp;

    if(i2c_transfer(mcp->client->adapter,msg,2) < 0){
	pr_err("i2c_transfer fail in %s\n",__func__);
    }

    /* conversion */
    mcp->data[0] = ((rawtmp[0] & 0x0f) << 4) | (rawtmp[1] >> 4);
    mcp->data[1] = rawtmp[1] & 0x0f;

    pr_info("Temprature value = %d.%d\n",mcp->data[0],mcp->data[1]);
    
    /* copy from kernel to user space */
    if(copy_to_user(buf,mcp->data,2) != 0){
	pr_err("copy_to_user fail\n");
	return -EIO;
    }
    return 2;
}

static int mcp_shutdown(struct mcp_data *mcp)
{
   u8 buff[3];
   struct i2c_msg msg[2];

   buff[0] = 0x01;
   buff[1] = 0x01;
   buff[2] = 0x00;
    msg[0].addr = mcp->client->addr;
    msg[0].flags = 0;
    msg[0].len = 3;
    msg[0].buf = buff;

    if(i2c_transfer(mcp->client->adapter,msg,1) < 0){
	pr_err("i2c_transfer fail in %s\n",__func__);
	return -1;
    }

    return 0;
}

static int mcp_wakeup(struct mcp_data *mcp)
{
   u8 buff[3];
   struct i2c_msg msg[2];

   buff[0] = 0x01;
   buff[1] = 0x00;
   buff[2] = 0x00;
    msg[0].addr = mcp->client->addr;
    msg[0].flags = 0;
    msg[0].len = 3;
    msg[0].buf = buff;

    if(i2c_transfer(mcp->client->adapter,msg,1) < 0){
	pr_err("i2c_transfer fail in %s\n",__func__);
	return -1;
    }

    return 0;
}

static long mcp_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{

    switch(cmd){
	case MCP_SHUTDWN:
	    mcp_shutdown(filp->private_data);
	    break;
	case MCP_WKEUP:
	    mcp_wakeup(filp->private_data);
	    break;
    }
    return 0;
}


static struct file_operations mcp_fops = {
    .owner = THIS_MODULE,
    .open = mcp_open,
    .release = mcp_release,
    .read = mcp_read,
    .unlocked_ioctl = mcp_ioctl,
};

static int mcp9808_probe(struct i2c_client *client, const struct i2c_device_id *id)
{

    struct i2c_msg msg[2];
    unsigned char reg_addr = 0x06;
    u8 data[2];
    int err;
    dev_t dev_no;
    struct device *mcp_device=NULL;
    struct mcp_data *mcp;

    pr_info("%s\n",__func__);
    if(!i2c_check_functionality(client->adapter,I2C_FUNC_SMBUS_BYTE_DATA)){
	pr_err("i2c_check_functionality\n");
    }

    /* Verify the identity of  device */
    msg[0].addr = client->addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &reg_addr;

    msg[1].addr = client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].len = 2;
    msg[1].buf = data;

    if(i2c_transfer(client->adapter,msg,2) < 0){
	pr_err("i2c_transfer fail\n MCP9808 not found\n");
    }
    pr_info("MCP9808 Manufacture ID = %x\n",data[0]<<8|data[1]);

    /* Allocate the major and minor number */
    err = alloc_chrdev_region(&dev_no,0,1,DEVICE_NAME);
    if(err < 0){
	pr_err("alloc_chrdev_region fail\n");
	return err;
    }
    mcp_major = MAJOR(dev_no);
    mcp_minor = MINOR(dev_no);
    pr_info("%s major num = %d -- minor num = %d\n",DEVICE_NAME,mcp_major,mcp_minor);

    /* Create the sysfs class */
    mcp_class = class_create(THIS_MODULE,DEVICE_NAME);
    if(IS_ERR(mcp_class)){
	err = PTR_ERR(mcp_class);
	goto fail;
    }

    /* Allocating memory for device specific data */
    mcp = kmalloc(sizeof(struct mcp_data),GFP_KERNEL);
    if(mcp == NULL){
	pr_err("kmalloc fail\n");
	err = -ENOMEM;
	return err;
    }

    mcp->data = NULL;
    mcp->client = client;
    
    /* Initilize and register character device with kernel */
    cdev_init(&mcp->dev,&mcp_fops);
    mcp->dev.owner = THIS_MODULE;
    err = cdev_add(&mcp->dev,dev_no,1);

    if(err){
	pr_err("Adding cdev fail\n");
	goto fail;
    }

    /* create the device so that user can access the slave device from user space */
    mcp_device = device_create(mcp_class,
		NULL,
		dev_no,
		NULL,
		DEVICE_NAME);

    if(IS_ERR(mcp_device)) {
	err = PTR_ERR(mcp_device);
	pr_err("Device creation fail\n");
	cdev_del(&mcp->dev);
	goto fail;
    }
    i2c_set_clientdata(client,mcp);
   return 0; 

fail:
   /* if anything fails undo everything */
    if(mcp_class != NULL){
	class_destroy(mcp_class);
    }
    unregister_chrdev_region(MKDEV(mcp_major,mcp_minor),1);
    if(mcp != NULL){
	kfree(mcp);
    }
    return err;

}

static int mcp9808_remove(struct i2c_client *client)
{
    struct mcp_data *mcp = i2c_get_clientdata(client);
    device_destroy(mcp_class,MKDEV(mcp_major,mcp_minor));
    kfree(mcp);
    class_destroy(mcp_class);
    unregister_chrdev_region(MKDEV(mcp_major,mcp_minor),1);
    return 0;
}

static const struct i2c_device_id mcp98xx_id[] = {
    {"mcp9808",0},
    {},
};

static struct i2c_driver mcp9808_i2c_driver = {
    .driver = {
	.owner = THIS_MODULE,
	.name = "mcp9808",
	.of_match_table = of_match_ptr(mcp98xx_of),
    },
    .probe = mcp9808_probe,
    .remove = mcp9808_remove,
    .id_table = mcp98xx_id,

};

module_i2c_driver(mcp9808_i2c_driver);
