#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of_fdt.h>

#define BOARD_TYPE_NANOPI_DUO 	   (4)
#define BOARD_TYPE_NANOPI_NEO_CORE (5)

static unsigned int sunxi_get_board_vendor_id(void)
{
    u32 vid_cnt;
    u32 i, pin_val, vid_val;
    int gpio[2] = {(32*2+4), (32*2+7)};             // GPIOC4,GPIOC7

    vid_cnt = 2;
    vid_val = 0;
    for (i=0; i<vid_cnt; i++) {
        if (gpio_is_valid(gpio[i])) {
            gpio_request(gpio[i], NULL);
            gpio_direction_input(gpio[i]);
            pin_val = gpio_get_value(gpio[i]);
        } else {
            printk(KERN_ERR"%s %d is invalid\n", __func__, gpio[i]);
            return -1;
        }
        vid_val |= (pin_val<<i);
    }
    return vid_val;
}

ssize_t sys_info_show(struct class *class, struct class_attribute *attr, char *buf)
{
    int databuf[4];
    size_t size = 0;

    /* platform */
    size += sprintf(buf + size, "sunxi_platform    : Sun8iw7p1/Sun50iw2p1\n");

    /* secure */
    size += sprintf(buf + size, "sunxi_secure      : normal\n");

    /* chipid */
    size += sprintf(buf + size, "sunxi_chipid      : unsupported\n");

    /* chiptype */
    size += sprintf(buf + size, "sunxi_chiptype    : unsupported\n");

    /* socbatch number */
    size += sprintf(buf + size, "sunxi_batchno     : unsupported\n");

    /* Board vendor id*/
    if (!strcasecmp("FriendlyARM NanoPi Duo", dt_machine_name))
    	size += sprintf(buf + size, "sunxi_board_id    : %d(0)\n", BOARD_TYPE_NANOPI_DUO);
    else if (!strcasecmp("FriendlyARM NanoPi NEO Core", dt_machine_name))
    	size += sprintf(buf + size, "sunxi_board_id    : %d(0)\n", BOARD_TYPE_NANOPI_NEO_CORE);
    else {
    	databuf[0] = sunxi_get_board_vendor_id();
    	size += sprintf(buf + size, "sunxi_board_id    : %d(%d)\n", (databuf[0]<0)?(-1):(databuf[0]&~(0xe0)), (databuf[0]<0)?(-1):((databuf[0]>>5)&0x01));
	}
	
    /*  Board manufacturer  */
    size += sprintf(buf + size, "board_manufacturer: FriendlyElec\n");

	/* Board name */
    size += sprintf(buf + size, "board_name        : %s\n", dt_machine_name);
    return size;
}

static struct class_attribute info_class_attrs[] = {
    __ATTR(sys_info, 0644, sys_info_show, NULL),
    __ATTR_NULL,
};

static struct class info_class = {
    .name           = "sunxi_info",
    .owner          = THIS_MODULE,
    .class_attrs    = info_class_attrs,
};

static int __init sunxi_info_init(void)
{
    int status;

    status = class_register(&info_class);
    if(status < 0)
        pr_err("%s err, status %d\n", __func__, status);
    else
        pr_debug("%s success\n", __func__);

    return status;
}

static void __exit sunxi_info_exit(void)
{
    class_unregister(&info_class);
}

module_init(sunxi_info_init);
module_exit(sunxi_info_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyElec");