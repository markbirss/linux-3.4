/*
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/*
 * A V4L2 driver for OV16825 cameras.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>


#include "camera.h"


MODULE_AUTHOR("lwj");
MODULE_DESCRIPTION("A low-level driver for OV16825 sensors");
MODULE_LICENSE("GPL");

//for internel driver debug
#define DEV_DBG_EN      1 
#if(DEV_DBG_EN == 1)    
#define vfe_dev_dbg(x,arg...) printk("[OV16825]"x,##arg)
#else
#define vfe_dev_dbg(x,arg...) 
#endif
#define vfe_dev_err(x,arg...) printk("[OV16825]"x,##arg)
#define vfe_dev_print(x,arg...) printk("[OV16825]"x,##arg)

#define LOG_ERR_RET(x)  { \
                          int ret;  \
                          ret = x; \
                          if(ret < 0) {\
                            vfe_dev_err("error at %s\n",__func__);  \
                            return ret; \
                          } \
                        }

//define module timing
#define MCLK              (24*1000*1000)
#define VREF_POL          V4L2_MBUS_VSYNC_ACTIVE_HIGH
#define HREF_POL          V4L2_MBUS_HSYNC_ACTIVE_HIGH
#define CLK_POL           V4L2_MBUS_PCLK_SAMPLE_RISING
#define V4L2_IDENT_SENSOR 0x16825
int ov16825_sensor_vts;
//#define QSXGA_12FPS

//define the voltage level of control signal
#define CSI_STBY_ON     1
#define CSI_STBY_OFF    0
#define CSI_RST_ON      0
#define CSI_RST_OFF     1
#define CSI_PWR_ON      1
#define CSI_PWR_OFF     0
#define CSI_AF_PWR_ON   1
#define CSI_AF_PWR_OFF  0
#define regval_list reg_list_a16_d8


#define REG_TERM 0xfffe
#define VAL_TERM 0xfe
#define REG_DLY  0xffff

/*
 * Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE 30


/*
 * The ov16825 sits on i2c with ID 0x6c
 */
#define I2C_ADDR 0x6c
#define SENSOR_NAME "ov16825"
//static struct delayed_work sensor_s_ae_ratio_work;
static struct v4l2_subdev *glb_sd;

/*
 * Information we maintain about a known sensor.
 */
struct sensor_format_struct;  /* coming later */

struct cfg_array { /* coming later */
	struct regval_list * regs;
	int size;
};

static inline struct sensor_info *to_state(struct v4l2_subdev *sd)
{
  return container_of(sd, struct sensor_info, sd);
}


/*
 * The default register settings
 *
 */


static struct regval_list sensor_default_regs[] = {
	{0x0103, 0x01},
	//;delay 20ms
	{0xffff, 20},
	//;PLL
	{0x0100,0x00},

	{0x0300, 0x02},//0x02
	{0x0302, 0x64},
	{0x0305, 0x01},
	{0x0306, 0x00},
	{0x030b, 0x02},//0x02
	{0x030c, 0x14},
	{0x030e, 0x00},
	{0x0313, 0x02},//0x02
	{0x0314, 0x14},
	{0xffff, 20},
	{0x031f, 0x00},
	{0x3022, 0x01},
	{0x3032, 0x80},
	{0x3601, 0xf8},
	{0x3602, 0x00},
	{0x3605, 0x50},
	{0x3606, 0x00},
	{0x3607, 0x2b},
	{0x3608, 0x16},
	{0x3609, 0x00},
	{0x360e, 0x99},
	{0x360f, 0x75},
	{0x3610, 0x69},
	{0x3611, 0x59},
	{0x3612, 0x40},
	{0x3613, 0x89},
	{0x3615, 0x44},
	{0x3617, 0x00},
	{0x3618, 0x20},
	{0x3619, 0x00},
	{0x361a, 0x10},
	{0x361c, 0x10},
	{0x361d, 0x00},
	{0x361e, 0x00},
	{0x3640, 0x15},
	{0x3641, 0x54},
	{0x3642, 0x63},
	{0x3643, 0x32},
	{0x3644, 0x03},
	{0x3645, 0x04},
	{0x3646, 0x85},
	{0x364a, 0x07},
	{0x3707, 0x08},
	{0x3718, 0x75},
	{0x371a, 0x55},
	{0x371c, 0x55},
	{0x3733, 0x80},
	{0x3760, 0x00},
	{0x3761, 0x30},
	{0x3762, 0x00},
	{0x3763, 0xc0},
	{0x3764, 0x03},
	{0x3765, 0x00},
	{0x3823, 0x08},
	{0x3827, 0x02},
	{0x3828, 0x00},
	{0x3832, 0x00},
	{0x3833, 0x00},
	{0x3834, 0x00},
	{0x3d85, 0x17},
	{0x3d8c, 0x70},
	{0x3d8d, 0xa0},
	{0x3f00, 0x02},
	{0x4001, 0x83},
	{0x400e, 0x00},
	{0x4011, 0x00},
	{0x4012, 0x00},
	{0x4200, 0x08},
	{0x4302, 0x7f},
	{0x4303, 0xff},
	{0x4304, 0x00},
	{0x4305, 0x00},
	{0x4501, 0x30},
	{0x4603, 0x20},
	{0x4b00, 0x22},
	{0x4903, 0x00},
	{0x5000, 0x7f},
	{0x5001, 0x01},
	{0x5004, 0x00},
	{0x5013, 0x20},
	{0x5051, 0x00},

//	0x5040, 0xa4,	//test mode
	
	{0x5500, 0x01},
	{0x5501, 0x00},
	{0x5502, 0x07},
	{0x5503, 0xff},
	{0x5505, 0x6c},
	{0x5509, 0x02},
	{0x5780, 0xfc},
	{0x5781, 0xff},
	{0x5787, 0x40},
	{0x5788, 0x08},
	{0x578a, 0x02},
	{0x578b, 0x01},
	{0x578c, 0x01},
	{0x578e, 0x02},
	{0x578f, 0x01},
	{0x5790, 0x01},
	{0x5792, 0x00},
	{0x5980, 0x00},
	{0x5981, 0x21},
	{0x5982, 0x00},
	{0x5983, 0x00},
	{0x5984, 0x00},
	{0x5985, 0x00},
	{0x5986, 0x00},
	{0x5987, 0x00},
	{0x5988, 0x00},
	//;Because current regsiter number in group hold is more than 85 (default), change group1 and
	//group2 start address. Use group 0, 1, 2.
	{0x3201, 0x15},
	{0x3202, 0x2a},
	//;MIPI 4-Lane 4608x3456 10-bit 15fps setting
	//;don't change any PLL VCO in group hold
	{0x0305,0x01},
	{0x030e,0x01},
	{0x3018,0x7a},
	{0x3031,0x0a},// [4:0]8/10/12/16bit
	{0x3603,0x00},
	{0x3604,0x00},
	{0x360a,0x00},
	{0x360b,0x02},
	{0x360c,0x12},
	{0x360d,0x00},
	{0x3614,0x77},
	{0x3616,0x30},
	{0x3631,0x60},
	{0x3700,0x30},
	{0x3701,0x08},
	{0x3702,0x11},
	{0x3703,0x20},
	{0x3704,0x08},
	{0x3705,0x00},
	{0x3706,0x84},
	{0x3708,0x20},
	{0x3709,0x3c},
	{0x370a,0x01},
	{0x370b,0x5d},
	{0x370c,0x03},
	{0x370e,0x20},
	{0x370f,0x05},
	{0x3710,0x20},
	{0x3711,0x20},
	{0x3714,0x31},
	{0x3719,0x13},
	{0x371b,0x03},
	{0x371d,0x03},
	{0x371e,0x09},
	{0x371f,0x17},
	{0x3720,0x0b},
	{0x3721,0x18},
	{0x3722,0x0b},
	{0x3723,0x18},
	{0x3724,0x04},
	{0x3725,0x04},
	{0x3726,0x02},
	{0x3727,0x02},
	{0x3728,0x02},
	{0x3729,0x02},
	{0x372a,0x25},
	{0x372b,0x65},
	{0x372c,0x55},
	{0x372d,0x65},
	{0x372e,0x53},
	{0x372f,0x33},
	{0x3730,0x33},
	{0x3731,0x33},
	{0x3732,0x03},
	{0x3734,0x10},
	{0x3739,0x03},
	{0x373a,0x20},
	{0x373b,0x0c},
	{0x373c,0x1c},
	{0x373e,0x0b},
	{0x373f,0x80},
	{0x3800,0x00},
	{0x3801,0x20},
	{0x3802,0x00},
	{0x3803,0x0e},
	{0x3804,0x12},
	{0x3805,0x3f},
	{0x3806,0x0d},
	{0x3807,0x93},
	{0x3808,0x12},
	{0x3809,0x00},
	{0x380a,0x0d},
	{0x380b,0x80},
	{0x380c,0x05},
	{0x380d,0xf8},
	{0x380e,0x0d},
	{0x380f,0xa2},
	{0x3811,0x0f},
	{0x3813,0x02},
	{0x3814,0x01},
	{0x3815,0x01},
	{0x3820,0x00},
	{0x3821,0x06},
	{0x3829,0x00},
	{0x382a,0x01},
	{0x382b,0x01},
	{0x3830,0x08},
	{0x3f08,0x20},
	{0x4002,0x04},
	{0x4003,0x08},
	{0x4837,0x14},
	{0x3501,0xd9},
	{0x3502,0xe0},
	{0x3508,0x04},
	{0x3509,0xff},
	
	{0x3638,0x00}, //;activate 36xx
	
	//0x4800, 0x24, //bit5=1 gating clock 
	
	{0x0100,0x01},
};

//for capture                                                                         
static struct regval_list sensor_16M_regs[] = {
	//Slave_ID=0x6c;
	//@@ 0 20 MIPI 4-Lane 4608x3456 10-bit 15fps 800Mbps/lane
	//100 99 4608 3456
	//100 98 1 1
	//102 81 0 ffff
	//102 84 1 ffff
	//102 3601 5dc
	//102 3f00 da2
	//
	//RUN RedFPGA_MIPI
	//RUN GreenFPGA_MIPI
	//RUN GreenVerCFPGA_MIPI8
	//

	{0x0103, 0x01},
	//;delay 20ms
	{0xffff, 20},
	//;PLL
	{0x0100,0x00},

	{0x0300, 0x02},//0x02
	{0x0302, 0x64},
	{0x0305, 0x01},
	{0x0306, 0x00},
	{0x030b, 0x02},//0x02
	{0x030c, 0x14},
	{0x030e, 0x00},
	{0x0313, 0x02},//0x02
	{0x0314, 0x14},
	{0xffff, 20},
	{0x031f, 0x00},
	{0x3022, 0x01},
	{0x3032, 0x80},
	{0x3601, 0xf8},
	{0x3602, 0x00},
	{0x3605, 0x50},
	{0x3606, 0x00},
	{0x3607, 0x2b},
	{0x3608, 0x16},
	{0x3609, 0x00},
	{0x360e, 0x99},
	{0x360f, 0x75},
	{0x3610, 0x69},
	{0x3611, 0x59},
	{0x3612, 0x40},
	{0x3613, 0x89},
	{0x3615, 0x44},
	{0x3617, 0x00},
	{0x3618, 0x20},
	{0x3619, 0x00},
	{0x361a, 0x10},
	{0x361c, 0x10},
	{0x361d, 0x00},
	{0x361e, 0x00},
	{0x3640, 0x15},
	{0x3641, 0x54},
	{0x3642, 0x63},
	{0x3643, 0x32},
	{0x3644, 0x03},
	{0x3645, 0x04},
	{0x3646, 0x85},
	{0x364a, 0x07},
	{0x3707, 0x08},
	{0x3718, 0x75},
	{0x371a, 0x55},
	{0x371c, 0x55},
	{0x3733, 0x80},
	{0x3760, 0x00},
	{0x3761, 0x30},
	{0x3762, 0x00},
	{0x3763, 0xc0},
	{0x3764, 0x03},
	{0x3765, 0x00},
	{0x3823, 0x08},
	{0x3827, 0x02},
	{0x3828, 0x00},
	{0x3832, 0x00},
	{0x3833, 0x00},
	{0x3834, 0x00},
	{0x3d85, 0x17},
	{0x3d8c, 0x70},
	{0x3d8d, 0xa0},
	{0x3f00, 0x02},
	{0x4001, 0x83},
	{0x400e, 0x00},
	{0x4011, 0x00},
	{0x4012, 0x00},
	{0x4200, 0x08},
	{0x4302, 0x7f},
	{0x4303, 0xff},
	{0x4304, 0x00},
	{0x4305, 0x00},
	{0x4501, 0x30},
	{0x4603, 0x20},
	{0x4b00, 0x22},
	{0x4903, 0x00},
	{0x5000, 0x7f},
	{0x5001, 0x01},
	{0x5004, 0x00},
	{0x5013, 0x20},
	{0x5051, 0x00},

//	0x5040, 0xa4,	//test mode
	
	{0x5500, 0x01},
	{0x5501, 0x00},
	{0x5502, 0x07},
	{0x5503, 0xff},
	{0x5505, 0x6c},
	{0x5509, 0x02},
	{0x5780, 0xfc},
	{0x5781, 0xff},
	{0x5787, 0x40},
	{0x5788, 0x08},
	{0x578a, 0x02},
	{0x578b, 0x01},
	{0x578c, 0x01},
	{0x578e, 0x02},
	{0x578f, 0x01},
	{0x5790, 0x01},
	{0x5792, 0x00},
	{0x5980, 0x00},
	{0x5981, 0x21},
	{0x5982, 0x00},
	{0x5983, 0x00},
	{0x5984, 0x00},
	{0x5985, 0x00},
	{0x5986, 0x00},
	{0x5987, 0x00},
	{0x5988, 0x00},
	//;Because current regsiter number in group hold is more than 85 (default), change group1 and
	//group2 start address. Use group 0, 1, 2.
	{0x3201, 0x15},
	{0x3202, 0x2a},
	//;MIPI 4-Lane 4608x3456 10-bit 15fps setting
	//;don't change any PLL VCO in group hold
	{0x0305,0x01},
	{0x030e,0x01},
	{0x3018,0x7a},
	{0x3031,0x0a},// [4:0]8/10/12/16bit
	{0x3603,0x00},
	{0x3604,0x00},
	{0x360a,0x00},
	{0x360b,0x02},
	{0x360c,0x12},
	{0x360d,0x00},
	{0x3614,0x77},
	{0x3616,0x30},
	{0x3631,0x60},
	{0x3700,0x30},
	{0x3701,0x08},
	{0x3702,0x11},
	{0x3703,0x20},
	{0x3704,0x08},
	{0x3705,0x00},
	{0x3706,0x84},
	{0x3708,0x20},
	{0x3709,0x3c},
	{0x370a,0x01},
	{0x370b,0x5d},
	{0x370c,0x03},
	{0x370e,0x20},
	{0x370f,0x05},
	{0x3710,0x20},
	{0x3711,0x20},
	{0x3714,0x31},
	{0x3719,0x13},
	{0x371b,0x03},
	{0x371d,0x03},
	{0x371e,0x09},
	{0x371f,0x17},
	{0x3720,0x0b},
	{0x3721,0x18},
	{0x3722,0x0b},
	{0x3723,0x18},
	{0x3724,0x04},
	{0x3725,0x04},
	{0x3726,0x02},
	{0x3727,0x02},
	{0x3728,0x02},
	{0x3729,0x02},
	{0x372a,0x25},
	{0x372b,0x65},
	{0x372c,0x55},
	{0x372d,0x65},
	{0x372e,0x53},
	{0x372f,0x33},
	{0x3730,0x33},
	{0x3731,0x33},
	{0x3732,0x03},
	{0x3734,0x10},
	{0x3739,0x03},
	{0x373a,0x20},
	{0x373b,0x0c},
	{0x373c,0x1c},
	{0x373e,0x0b},
	{0x373f,0x80},
	{0x3800,0x00},
	{0x3801,0x20},
	{0x3802,0x00},
	{0x3803,0x0e},
	{0x3804,0x12},
	{0x3805,0x3f},
	{0x3806,0x0d},
	{0x3807,0x93},
	{0x3808,0x12},
	{0x3809,0x00},
	{0x380a,0x0d},
	{0x380b,0x80},
	{0x380c,0x05},
	{0x380d,0xf8},
	{0x380e,0x0d},
	{0x380f,0xa2},
	{0x3811,0x0f},
	{0x3813,0x02},
	{0x3814,0x01},
	{0x3815,0x01},
	{0x3820,0x00},
	{0x3821,0x06},
	{0x3829,0x00},
	{0x382a,0x01},
	{0x382b,0x01},
	{0x3830,0x08},
	{0x3f08,0x20},
	{0x4002,0x04},
	{0x4003,0x08},
	{0x4837,0x14},
	{0x3501,0xd9},
	{0x3502,0xe0},
	{0x3508,0x04},
	{0x3509,0xff},
	
	{0x3638,0x00}, //;activate 36xx
	
	//0x4800, 0x24, //bit5=1 gating clock 
	
	//0x0100,0x01,
	



	//;Sensor Setting
	//;group 0
	//;don't change any PLL VCO in group hold
	{0x0305, 0x01},
	{0x030e, 0x01},

	{0x3208, 0x00},
	
	{0x301a, 0xfb},
	
	
	{0x3018, 0x7a},
	{0x3031, 0x0a},// [4:0]8/10/12/16bit
	{0x3603, 0x00},
	{0x3604, 0x00},
	{0x360a, 0x00},
	{0x360b, 0x02},
	{0x360c, 0x12},
	{0x360d, 0x00},
	{0x3612, 0x40},
	{0x3614, 0x77},
	{0x3616, 0x30},
	{0x3631, 0x60},
	{0x3700, 0x30},
	{0x3701, 0x08},
	{0x3702, 0x11},
	{0x3703, 0x20},
	{0x3704, 0x08},
	{0x3705, 0x00},
	{0x3706, 0x84},
	{0x3708, 0x20},
	{0x3709, 0x3c},
	{0x370a, 0x01},
	{0x370b, 0x5d},
	{0x370c, 0x03},
	{0x370e, 0x20},
	{0x370f, 0x05},
	{0x3710, 0x20},
	{0x3711, 0x20},
	{0x3714, 0x31},
	{0x3719, 0x13},
	{0x371b, 0x03},
	{0x371d, 0x03},
	{0x371e, 0x09},
	{0x371f, 0x17},
	{0x3720, 0x0b},
	{0x3721, 0x18},
	{0x3722, 0x0b},
	{0x3723, 0x18},
	{0x3724, 0x04},
	{0x3725, 0x04},
	{0x3726, 0x02},
	{0x3727, 0x02},
	{0x3728, 0x02},
	{0x3729, 0x02},
	{0x372a, 0x25},
	{0x372b, 0x65},
	{0x372c, 0x55},
	{0x372d, 0x65},
	{0x372e, 0x53},
	{0x372f, 0x33},
	{0x3730, 0x33},
	{0x3731, 0x33},
	{0x3732, 0x03},
	{0x3734, 0x10},
	{0x3739, 0x03},
	{0x373a, 0x20},
	{0x373b, 0x0c},
	{0x373c, 0x1c},
	{0x373e, 0x0b},
	{0x373f, 0x80},
	
	{0x3800, 0x00},
	{0x3801, 0x20},
	{0x3802, 0x00},
	{0x3803, 0x0e},
	{0x3804, 0x12},
	{0x3805, 0x3f},
	{0x3806, 0x0d},
	{0x3807, 0x93},
	{0x3808, 0x12},
	{0x3809, 0x00},
	{0x380a, 0x0d},
	{0x380b, 0x80},
	{0x380c, 0x05},
	{0x380d, 0xf8},
	{0x380e, 0x0d},
	{0x380f, 0xa2},
	{0x3811, 0x0f},
	{0x3813, 0x02},
	{0x3814, 0x01},
	{0x3815, 0x01},
	{0x3820, 0x00},
	{0x3821, 0x06},
	{0x3829, 0x00},
	{0x382a, 0x01},
	{0x382b, 0x01},
	{0x3830, 0x08},	
	{0x3f08, 0x20},
	{0x4002, 0x04},
	{0x4003, 0x08},
	{0x4837, 0x14},	
	{0x3501, 0xd9},
	{0x3502, 0xe0},
	{0x3508, 0x04},
	{0x3509, 0xff},	
	{0x3638, 0x00}, //;activate 36xx	
	{0x301a, 0xf0},	
	{0x3208, 0x10},
	{0x3208, 0xa0},	
	{0x0100, 0x01},

};

//for video
static struct regval_list sensor_2160p_regs[] =
#if 0
{ 
	//@@MIPI 4-Lane 3840x2160 30fps 

	{0x0100, 0x00},
	//0x0103, 0x01,
	{0x0300, 0x02},
	{0x0302, 0x64},
	{0x0305, 0x01},
	{0x0306, 0x00},
	{0x030b, 0x02},
	{0x030c, 0x14},
	{0x030e, 0x00},
	{0x0313, 0x02},
	{0x0314, 0x14},
	{0x031f, 0x00},
	{0x3022, 0x01},
	{0x3032, 0x80},
	{0x3601, 0xf8},
	{0x3602, 0x00},
	{0x3605, 0x50},
	{0x3606, 0x00},
	{0x3607, 0x2b},
	{0x3608, 0x16},
	{0x3609, 0x00},
	{0x360e, 0x99},
	{0x360f, 0x75},
	{0x3610, 0x69},
	{0x3611, 0x59},
	{0x3612, 0x50},
	{0x3613, 0x89},
	{0x3615, 0x44},
	{0x3617, 0x00},
	{0x3618, 0x20},
	{0x3619, 0x00},
	{0x361a, 0x10},
	{0x361c, 0x10},
	{0x361d, 0x00},
	{0x361e, 0x00},
	{0x3631, 0x60},
	{0x3640, 0x15},
	{0x3641, 0x54},
	{0x3642, 0x63},
	{0x3643, 0x32},
	{0x3644, 0x03},
	{0x3645, 0x04},
	{0x3646, 0x85},
	{0x364a, 0x07},
	{0x3707, 0x08},
	{0x3718, 0x75},
	{0x371a, 0x55},
	{0x371c, 0x55},
	{0x3733, 0x80},
	{0x3734, 0x10},
	{0x3760, 0x00},
	{0x3761, 0x30},
	{0x3762, 0x00},
	{0x3763, 0xc0},
	{0x3764, 0x03},
	{0x3765, 0x00},
	{0x3823, 0x08},
	{0x3827, 0x02},
	{0x3828, 0x00},
	{0x3832, 0x00},
	{0x3833, 0x00},
	{0x3834, 0x00},
	{0x3d85, 0x17},
	{0x3d8c, 0x70},
	{0x3d8d, 0xa0},
	{0x3f00, 0x02},
	{0x4001, 0x83},
	{0x400e, 0x00},
	{0x4011, 0x00},
	{0x4012, 0x00},
	{0x4200, 0x08},
	{0x4302, 0x7f},
	{0x4303, 0xff},
	{0x4304, 0x00},
	{0x4305, 0x00},
	{0x4501, 0x30},
	{0x4603, 0x20},
	{0x4b00, 0x22},
	{0x4903, 0x00},
	{0x5000, 0x7f},
	{0x5001, 0x01},
	{0x5004, 0x00},
	{0x5013, 0x20},
	{0x5051, 0x00},
	{0x5500, 0x01},
	{0x5501, 0x00},
	{0x5502, 0x07},
	{0x5503, 0xff},
	{0x5505, 0x6c},
	{0x5980, 0x00},
	{0x5981, 0x21},
	{0x5982, 0x00},
	{0x5983, 0x00},
	{0x5984, 0x00},
	{0x5985, 0x00},
	{0x5986, 0x00},
	{0x5987, 0x00},
	{0x5988, 0x00},
	{0x3201, 0x15},
	{0x3202, 0x2a},
	{0x3208, 0x00},
	{0x301a, 0xfb},
	{0x0305, 0x01},
	{0x030e, 0x00},
	{0x3018, 0x72},
	{0x3031, 0x0a},
	{0x3106, 0x21},
	{0x3603, 0x00},
	{0x3604, 0x00},
	{0x360a, 0x00},
	{0x360b, 0x02},
	{0x360c, 0x12},
	{0x360d, 0x00},
	{0x3614, 0x77},
	{0x3616, 0x30},
	{0x3700, 0x30},
	{0x3701, 0x08},
	{0x3702, 0x11},
	{0x3703, 0x20},
	{0x3704, 0x08},
	{0x3705, 0x00},
	{0x3706, 0x84},
	{0x3708, 0x20},
	{0x3709, 0x3c},
	{0x370a, 0x01},
	{0x370b, 0x5d},
	{0x370c, 0x03},
	{0x370e, 0x20},
	{0x370f, 0x05},
	{0x3710, 0x20},
	{0x3711, 0x20},
	{0x3714, 0x31},
	{0x3719, 0x13},
	{0x371b, 0x03},
	{0x371d, 0x03},
	{0x371e, 0x09},
	{0x371f, 0x17},
	{0x3720, 0x0b},
	{0x3721, 0x18},
	{0x3722, 0x0b},
	{0x3723, 0x18},
	{0x3724, 0x04},
	{0x3725, 0x04},
	{0x3726, 0x02},
	{0x3727, 0x02},
	{0x3728, 0x02},
	{0x3729, 0x02},
	{0x372a, 0x25},
	{0x372b, 0x65},
	{0x372c, 0x55},
	{0x372d, 0x65},
	{0x372e, 0x53},
	{0x372f, 0x33},
	{0x3730, 0x33},
	{0x3731, 0x33},
	{0x3732, 0x03},
	{0x3739, 0x03},
	{0x373a, 0x20},
	{0x373b, 0x0c},
	{0x373c, 0x1c},
	{0x373e, 0x0b},
	{0x373f, 0x80},
	{0x3800, 0x01},
	{0x3801, 0xa0},
	{0x3802, 0x02},
	{0x3803, 0x96},
	{0x3804, 0x10},
	{0x3805, 0xbf},
	{0x3806, 0x0b},
	{0x3807, 0x0b},
	{0x3808, 0x0f},
	{0x3809, 0x00},
	{0x380a, 0x08},
	{0x380b, 0x70},
	{0x380c, 0x04},
	{0x380d, 0xc0},
	{0x380e, 0x08},
	{0x380f, 0x92},
	{0x3811, 0x0f},
	{0x3813, 0x02},
	{0x3814, 0x01},
	{0x3815, 0x01},
	{0x3820, 0x00},
	{0x3821, 0x06},
	{0x3829, 0x00},
	{0x382a, 0x01},
	{0x382b, 0x01},
	{0x3830, 0x08},
	{0x3f08, 0x20},
	{0x4002, 0x04},
	{0x4003, 0x08},
	{0x4837, 0x14},
	{0x3501, 0x88},
	{0x3502, 0xe0},
	{0x3508, 0x04},
	{0x3509, 0xff},
	{0x3638, 0x00}, //;activate 36xx
	{0x301a, 0xf0},
	{0x3208, 0x10},
	{0x3208, 0xa0},
	{0x0100, 0x01},
};
#else
{ 
	//@@MIPI 4-Lane 3840x2160 30fps 

	{0x0100, 0x00},
	{0x3612, 0x50},
		//;don't change any PLL VCO in group hold
	{0x0305, 0x01},
	{0x030e, 0x00},
	{0x3208, 0x00},
	{0x301a, 0xfb},
	{0x3018, 0x72},
	{0x3031, 0x0a},
	{0x3106, 0x21},
	{0x3603, 0x00},
	{0x3604, 0x00},
	{0x360a, 0x00},
	{0x360b, 0x02},
	{0x360c, 0x12},
	{0x360d, 0x00},
	{0x3614, 0x77},
	{0x3616, 0x30},
	{0x3631, 0x60},
	{0x3700, 0x30},
	{0x3701, 0x08},
	{0x3702, 0x11},
	{0x3703, 0x20},
	{0x3704, 0x08},
	{0x3705, 0x00},
	{0x3706, 0x84},
	{0x3708, 0x20},
	{0x3709, 0x3c},
	{0x370a, 0x01},
	{0x370b, 0x5d},
	{0x370c, 0x03},
	{0x370e, 0x20},
	{0x370f, 0x05},
	{0x3710, 0x20},
	{0x3711, 0x20},
	{0x3714, 0x31},
	{0x3719, 0x13},
	{0x371b, 0x03},
	{0x371d, 0x03},
	{0x371e, 0x09},
	{0x371f, 0x17},
	{0x3720, 0x0b},
	{0x3721, 0x18},
	{0x3722, 0x0b},
	{0x3723, 0x18},
	{0x3724, 0x04},
	{0x3725, 0x04},
	{0x3726, 0x02},
	{0x3727, 0x02},
	{0x3728, 0x02},
	{0x3729, 0x02},
	{0x372a, 0x25},
	{0x372b, 0x65},
	{0x372c, 0x55},
	{0x372d, 0x65},
	{0x372e, 0x53},
	{0x372f, 0x33},
	{0x3730, 0x33},
	{0x3731, 0x33},
	{0x3732, 0x03},
	{0x3734, 0x10},
	{0x3739, 0x03},
	{0x373a, 0x20},
	{0x373b, 0x0c},
	{0x373c, 0x1c},
	{0x373e, 0x0b},
	{0x373f, 0x80},
	{0x3800, 0x01},
	{0x3801, 0xa0},
	{0x3802, 0x02},
	{0x3803, 0x96},
	{0x3804, 0x10},
	{0x3805, 0xbf},
	{0x3806, 0x0b},
	{0x3807, 0x0b},
	{0x3808, 0x0f},
	{0x3809, 0x00},
	{0x380a, 0x08},
	{0x380b, 0x70},
	{0x380c, 0x04},
	{0x380d, 0xc0},
	{0x380e, 0x08},
	{0x380f, 0x92},
	{0x3811, 0x0f},
	{0x3813, 0x02},
	{0x3814, 0x01},
	{0x3815, 0x01},
	{0x3820, 0x00},
	{0x3821, 0x06},
	{0x3829, 0x00},
	{0x382a, 0x01},
	{0x382b, 0x01},
	{0x3830, 0x08},
	{0x3f08, 0x20},
	{0x4002, 0x04},
	{0x4003, 0x08},
	{0x4837, 0x14},
	{0x3501, 0x88},
	{0x3502, 0xe0},
	{0x3508, 0x04},
	{0x3509, 0xff},
	{0x3638, 0x00}, //;activate 36xx
	{0x301a, 0xf0},
	{0x3208, 0x10},
	{0x3208, 0xa0},
	{0x0100, 0x01},
};
#endif

static struct regval_list sensor_1080p_regs[] = { 
	//; XVCLK = 24Mhz, Sysclk = 80Mhz, 10bit, 60fps
	//;group 0
	{0x0103, 0x01},
	//;delay 20ms
	{0xffff, 20},
	//;PLL
	{0x0100,0x00},

	{0x0300, 0x02},//0x02
	{0x0302, 0x64},
	{0x0305, 0x01},
	{0x0306, 0x00},
	{0x030b, 0x02},//0x02
	{0x030c, 0x14},
	{0x030e, 0x00},
	{0x0313, 0x02},//0x02
	{0x0314, 0x14},
	{0x031f, 0x00},
	{0x3022, 0x01},
	{0x3032, 0x80},
	{0x3601, 0xf8},
	{0x3602, 0x00},
	{0x3605, 0x50},
	{0x3606, 0x00},
	{0x3607, 0x2b},
	{0x3608, 0x16},
	{0x3609, 0x00},
	{0x360e, 0x99},
	{0x360f, 0x75},
	{0x3610, 0x69},
	{0x3611, 0x59},
	{0x3612, 0x40},
	{0x3613, 0x89},
	{0x3615, 0x44},
	{0x3617, 0x00},
	{0x3618, 0x20},
	{0x3619, 0x00},
	{0x361a, 0x10},
	{0x361c, 0x10},
	{0x361d, 0x00},
	{0x361e, 0x00},
	{0x3640, 0x15},
	{0x3641, 0x54},
	{0x3642, 0x63},
	{0x3643, 0x32},
	{0x3644, 0x03},
	{0x3645, 0x04},
	{0x3646, 0x85},
	{0x364a, 0x07},
	{0x3707, 0x08},
	{0x3718, 0x75},
	{0x371a, 0x55},
	{0x371c, 0x55},
	{0x3733, 0x80},
	{0x3760, 0x00},
	{0x3761, 0x30},
	{0x3762, 0x00},
	{0x3763, 0xc0},
	{0x3764, 0x03},
	{0x3765, 0x00},
	{0x3823, 0x08},
	{0x3827, 0x02},
	{0x3828, 0x00},
	{0x3832, 0x00},
	{0x3833, 0x00},
	{0x3834, 0x00},
	{0x3d85, 0x17},
	{0x3d8c, 0x70},
	{0x3d8d, 0xa0},
	{0x3f00, 0x02},
	{0x4001, 0x83},
	{0x400e, 0x00},
	{0x4011, 0x00},
	{0x4012, 0x00},
	{0x4200, 0x08},
	{0x4302, 0x7f},
	{0x4303, 0xff},
	{0x4304, 0x00},
	{0x4305, 0x00},
	{0x4501, 0x30},
	{0x4603, 0x20},
	{0x4b00, 0x22},
	{0x4903, 0x00},
	{0x5000, 0x7f},
	{0x5001, 0x01},
	{0x5004, 0x00},
	{0x5013, 0x20},
	{0x5051, 0x00},

//	0x5040, 0xa4,	//test mode
	
	{0x5500, 0x01},
	{0x5501, 0x00},
	{0x5502, 0x07},
	{0x5503, 0xff},
	{0x5505, 0x6c},
	{0x5509, 0x02},
	{0x5780, 0xfc},
	{0x5781, 0xff},
	{0x5787, 0x40},
	{0x5788, 0x08},
	{0x578a, 0x02},
	{0x578b, 0x01},
	{0x578c, 0x01},
	{0x578e, 0x02},
	{0x578f, 0x01},
	{0x5790, 0x01},
	{0x5792, 0x00},
	{0x5980, 0x00},
	{0x5981, 0x21},
	{0x5982, 0x00},
	{0x5983, 0x00},
	{0x5984, 0x00},
	{0x5985, 0x00},
	{0x5986, 0x00},
	{0x5987, 0x00},
	{0x5988, 0x00},
	//;Because current regsiter number in group hold is more than 85 (default), change group1 and
	//group2 start address. Use group 0, 1, 2.
	{0x3201, 0x15},
	{0x3202, 0x2a},
	//;MIPI 4-Lane 4608x3456 10-bit 15fps setting
	//;don't change any PLL VCO in group hold
	{0x0305,0x01},
	{0x030e,0x01},
	{0x3018,0x7a},
	{0x3031,0x0a},// [4:0]8/10/12/16bit
	{0x3603,0x00},
	{0x3604,0x00},
	{0x360a,0x00},
	{0x360b,0x02},
	{0x360c,0x12},
	{0x360d,0x00},
	{0x3614,0x77},
	{0x3616,0x30},
	{0x3631,0x60},
	{0x3700,0x30},
	{0x3701,0x08},
	{0x3702,0x11},
	{0x3703,0x20},
	{0x3704,0x08},
	{0x3705,0x00},
	{0x3706,0x84},
	{0x3708,0x20},
	{0x3709,0x3c},
	{0x370a,0x01},
	{0x370b,0x5d},
	{0x370c,0x03},
	{0x370e,0x20},
	{0x370f,0x05},
	{0x3710,0x20},
	{0x3711,0x20},
	{0x3714,0x31},
	{0x3719,0x13},
	{0x371b,0x03},
	{0x371d,0x03},
	{0x371e,0x09},
	{0x371f,0x17},
	{0x3720,0x0b},
	{0x3721,0x18},
	{0x3722,0x0b},
	{0x3723,0x18},
	{0x3724,0x04},
	{0x3725,0x04},
	{0x3726,0x02},
	{0x3727,0x02},
	{0x3728,0x02},
	{0x3729,0x02},
	{0x372a,0x25},
	{0x372b,0x65},
	{0x372c,0x55},
	{0x372d,0x65},
	{0x372e,0x53},
	{0x372f,0x33},
	{0x3730,0x33},
	{0x3731,0x33},
	{0x3732,0x03},
	{0x3734,0x10},
	{0x3739,0x03},
	{0x373a,0x20},
	{0x373b,0x0c},
	{0x373c,0x1c},
	{0x373e,0x0b},
	{0x373f,0x80},
	{0x3800,0x00},
	{0x3801,0x20},
	{0x3802,0x00},
	{0x3803,0x0e},
	{0x3804,0x12},
	{0x3805,0x3f},
	{0x3806,0x0d},
	{0x3807,0x93},
	{0x3808,0x12},
	{0x3809,0x00},
	{0x380a,0x0d},
	{0x380b,0x80},
	{0x380c,0x05},
	{0x380d,0xf8},
	{0x380e,0x0d},
	{0x380f,0xa2},
	{0x3811,0x0f},
	{0x3813,0x02},
	{0x3814,0x01},
	{0x3815,0x01},
	{0x3820,0x00},
	{0x3821,0x06},
	{0x3829,0x00},
	{0x382a,0x01},
	{0x382b,0x01},
	{0x3830,0x08},
	{0x3f08,0x20},
	{0x4002,0x04},
	{0x4003,0x08},
	{0x4837,0x14},
	{0x3501,0xd9},
	{0x3502,0xe0},
	{0x3508,0x04},
	{0x3509,0xff},
	
	{0x3638,0x00}, //;activate 36xx
	
	//0x4800, 0x24, //bit5=1 gating clock 
	
	{0x0100,0x01},	
	{0x0100, 0x00},
	{0x0305, 0x01},
	{0x030e, 0x01},
	{0x3208, 0x00},
	{0x301a, 0xfb},
	//;don't change any PLL VCO in group hold
	
	{0x3018, 0x7a},
	{0x3031, 0x0a},// [4:0]8/10/12/16bit
	{0x3603, 0x05},
	{0x3604, 0x02},
	{0x360a, 0x00},
	{0x360b, 0x02},
	{0x360c, 0x12},
	{0x360d, 0x04},
	{0x3614, 0x77},
	{0x3616, 0x30},
	{0x3631, 0x40},
	{0x3700, 0x30},
	{0x3701, 0x08},
	{0x3702, 0x11},
	{0x3703, 0x20},
	{0x3704, 0x08},
	{0x3705, 0x00},
	{0x3706, 0x84},
	{0x3708, 0x20},
	{0x3709, 0x3c},
	{0x370a, 0x01},
	{0x370b, 0x5d},
	{0x370c, 0x03},
	{0x370e, 0x20},
	{0x370f, 0x05},
	{0x3710, 0x20},
	{0x3711, 0x20},
	{0x3714, 0x31},
	{0x3719, 0x13},
	{0x371b, 0x03},
	{0x371d, 0x03},
	{0x371e, 0x09},
	{0x371f, 0x17},
	{0x3720, 0x0b},
	{0x3721, 0x18},
	{0x3722, 0x0b},
	{0x3723, 0x18},
	{0x3724, 0x04},
	{0x3725, 0x04},
	{0x3726, 0x02},
	{0x3727, 0x02},
	{0x3728, 0x02},
	{0x3729, 0x02},
	{0x372a, 0x25},
	{0x372b, 0x65},
	{0x372c, 0x55},
	{0x372d, 0x65},
	{0x372e, 0x53},
	{0x372f, 0x33},
	{0x3730, 0x33},
	{0x3731, 0x33},
	{0x3732, 0x03},
	{0x3734, 0x10},
	{0x3739, 0x03},
	{0x373a, 0x20},
	{0x373b, 0x0c},
	{0x373c, 0x1c},
	{0x373e, 0x0b},
	{0x373f, 0x80},
	{0x3800, 0x01},
	{0x3801, 0x80},
	{0x3802, 0x02},
	{0x3803, 0x94},
	{0x3804, 0x10},
	{0x3805, 0xbf},
	{0x3806, 0x0b},
	{0x3807, 0x0f},
	{0x3808, 0x07},
	{0x3809, 0x80},
	{0x380a, 0x04},
	{0x380b, 0x38},
	{0x380c, 0x04},
	{0x380d, 0xb6},
	{0x380e, 0x04},
	{0x380f, 0x52},
	{0x3811, 0x17},
	{0x3813, 0x02},
	{0x3814, 0x03},
	{0x3815, 0x01},
	{0x3820, 0x01},
	{0x3821, 0x07},
	{0x3829, 0x02},
	{0x382a, 0x03},
	{0x382b, 0x01},
	{0x3830, 0x08},
	{0x3f08, 0x20},
	{0x4002, 0x02},
	{0x4003, 0x04},
	{0x4837, 0x14},
	{0x3501, 0x44},
	{0x3502, 0xe0},
	{0x3508, 0x08},
	{0x3509, 0xff},
	{0x3638, 0x00}, //;activate 36xx
	{0x301a, 0xf0},
	{0x3208, 0x10},
	{0x3208, 0xa0},	
	{0x0100, 0x01},
};

static struct regval_list sensor_quarter_regs[] = {
	//; XVCLK = 24Mhz, Sysclk = 80Mhz
	//;Sensor Setting
	//;Vhbinning2
	
	{0x0100, 0x00},
	//;don't change any PLL VCO in group hold
	//;group 0
	{0x0305, 0x01},
	{0x030e, 0x01},
	{0x3208, 0x00},
	{0x301a, 0xfb},
	{0x3018, 0x7a},
	{0x3031, 0x0a},
	{0x3603, 0x05},
	{0x3604, 0x02},
	{0x360a, 0x00},
	{0x360b, 0x02},
	{0x360c, 0x12},
	{0x360d, 0x04},
	{0x3614, 0x77},
	{0x3616, 0x30},
	{0x3631, 0x40},
	{0x3700, 0x30},
	{0x3701, 0x08},
	{0x3702, 0x11},
	{0x3703, 0x20},
	{0x3704, 0x08},
	{0x3705, 0x00},
	{0x3706, 0x84},
	{0x3708, 0x20},
	{0x3709, 0x3c},
	{0x370a, 0x01},
	{0x370b, 0x5d},
	{0x370c, 0x03},
	{0x370e, 0x20},
	{0x370f, 0x05},
	{0x3710, 0x20},
	{0x3711, 0x20},
	{0x3714, 0x31},
	{0x3719, 0x13},
	{0x371b, 0x03},
	{0x371d, 0x03},
	{0x371e, 0x09},
	{0x371f, 0x17},
	{0x3720, 0x0b},
	{0x3721, 0x18},
	{0x3722, 0x0b},
	{0x3723, 0x18},
	{0x3724, 0x04},
	{0x3725, 0x04},
	{0x3726, 0x02},
	{0x3727, 0x02},
	{0x3728, 0x02},
	{0x3729, 0x02},
	{0x372a, 0x25},
	{0x372b, 0x65},
	{0x372c, 0x55},
	{0x372d, 0x65},
	{0x372e, 0x53},
	{0x372f, 0x33},
	{0x3730, 0x33},
	{0x3731, 0x33},
	{0x3732, 0x03},
	{0x3734, 0x10},
	{0x3739, 0x03},
	{0x373a, 0x20},
	{0x373b, 0x0c},
	{0x373c, 0x1c},
	{0x373e, 0x0b},
	{0x373f, 0x80},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x0c},
	{0x3804, 0x12},
	{0x3805, 0x3f},
	{0x3806, 0x0d},
	{0x3807, 0x97},
	{0x3808, 0x09},
	{0x3809, 0x00},
	{0x380a, 0x06},
	{0x380b, 0xc0},
	{0x380c, 0x05},
	{0x380d, 0xf0},
	{0x380e, 0x06},
	{0x380f, 0xda},
	{0x3811, 0x17},
	{0x3813, 0x02},
	{0x3814, 0x03},
	{0x3815, 0x01},
	{0x3820, 0x01},
	{0x3821, 0x07},
	{0x3829, 0x02},
	{0x382a, 0x03},
	{0x382b, 0x01},
	{0x3830, 0x08},
	{0x3f08, 0x20},
	{0x4002, 0x02},
	{0x4003, 0x04},
	{0x4837, 0x14},
	{0x3501, 0x6d},
	{0x3502, 0x60},
	{0x3508, 0x08},
	{0x3509, 0xff},
	{0x3638, 0x00}, //;activate 36xx
	{0x301a, 0xf0},
	{0x3208, 0x10},
	{0x3208, 0xa0},	
	{0x0100, 0x01},
};


/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 * 
 */

static struct regval_list sensor_fmt_raw[] = {

};

/*
 * Low-level register I/O.
 *
 */


/*
 * On most platforms, we'd rather do straight i2c I/O.
 */
static int sensor_read(struct v4l2_subdev *sd, unsigned short reg,
    unsigned char *value) //!!!!be careful of the para type!!!
{
	int ret=0;
	int cnt=0;
	
  ret = cci_read_a16_d8(sd,reg,value);
  while(ret!=0&&cnt<2)
  {
  	ret = cci_read_a16_d8(sd,reg,value);
  	cnt++;
  }
  if(cnt>0)
  	vfe_dev_dbg("sensor read retry=%d\n",cnt);
  
  return ret;
}

static int sensor_write(struct v4l2_subdev *sd, unsigned short reg,
    unsigned char value)
{
	int ret=0;
	int cnt=0;	
  ret = cci_write_a16_d8(sd,reg,value);
  while(ret!=0&&cnt<2)
  {
  	ret = cci_write_a16_d8(sd,reg,value);
  	cnt++;
  }
  if(cnt>0)
  	vfe_dev_dbg("sensor write retry=%d\n",cnt);
  
  return ret;
}

/*
 * Write a list of register settings;
 */
static int sensor_write_array(struct v4l2_subdev *sd, struct regval_list *regs, int array_size)
{
	int i=0;
	
  if(!regs)
    return -EINVAL;
  
  while(i<array_size)
  {
    if(regs->addr == REG_DLY) {
      msleep(regs->data);
    } 
    else {
      LOG_ERR_RET(sensor_write(sd, regs->addr, regs->data))
    }
    i++;
    regs++;
  }
  return 0;
}

/* 
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function ,retrun -EINVAL
 */

/* *********************************************begin of ******************************************** */
/*
static int sensor_g_hflip(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;
    
  LOG_ERR_RET(sensor_read(sd, 0x3821, &rdval))
  
  rdval &= (1<<1);
  rdval >>= 1;
    
  *value = rdval;

  info->hflip = *value;
  return 0;
}

static int sensor_s_hflip(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;
  
  if(info->hflip == value)
    return 0;
    
  LOG_ERR_RET(sensor_read(sd, 0x3821, &rdval))
  
  switch (value) {
    case 0:
      rdval &= 0xf9;
      break;
    case 1:
      rdval |= 0x06;
      break;
    default:
      return -EINVAL;
  }
  
  LOG_ERR_RET(sensor_write(sd, 0x3821, rdval))
  
  usleep_range(10000,12000);
  info->hflip = value;
  return 0;
}

static int sensor_g_vflip(struct v4l2_subdev *sd, __s32 *value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;
  
  LOG_ERR_RET(sensor_read(sd, 0x3820, &rdval))
  
  rdval &= (1<<1);  
  *value = rdval;
  rdval >>= 1;
  
  info->vflip = *value;
  return 0;
}

static int sensor_s_vflip(struct v4l2_subdev *sd, int value)
{
  struct sensor_info *info = to_state(sd);
  unsigned char rdval;
  
  if(info->vflip == value)
    return 0;
  
  LOG_ERR_RET(sensor_read(sd, 0x3820, &rdval))

  switch (value) {
    case 0:
      rdval &= 0xf9;
      break;
    case 1:
      rdval |= 0x06;
      break;
    default:
      return -EINVAL;
  }

  LOG_ERR_RET(sensor_write(sd, 0x3820, rdval))
  
  usleep_range(10000,12000);
  info->vflip = value;
  return 0;
}
*/

static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	
	*value = info->exp;
	vfe_dev_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}

static int sensor_s_exp_gain(struct v4l2_subdev *sd, struct sensor_exp_gain *exp_gain)
{
  int exp_val, gain_val,frame_length,shutter;
  unsigned char explow=0,expmid=0,exphigh=0;
  unsigned short gainlow=0,gainhigh=0,i=0;  
  struct sensor_info *info = to_state(sd);

  exp_val = exp_gain->exp_val;
  gain_val = exp_gain->gain_val;
	if(exp_val>0xfffff)
		exp_val=0xfffff;

  //printk("exp_val = %d,gain_val = %d\n",exp_val,gain_val);

  if((info->exp == exp_val)&&(info->gain == gain_val))
  	return 0;
  
  //return -EINVAL;
  if(gain_val<1*16)
	  gain_val=16;
  if(gain_val>64*16-1)
	  gain_val=64*16-1;
  
  if(exp_val>0xfffff)
	  exp_val=0xfffff;
  

  gainlow = gain_val * 8;
  gainlow = gainlow & 0x07ff;
  for(i=1;i<=3;i++) 
  {
	  if(gainlow>=0x100) 
	  {
		  gainhigh = gainhigh + 1;
		  gainlow = gainlow / 2;
	  }
  }
   gainhigh = gainhigh <<2;
  
  exphigh	= (unsigned char) ( (0x0f0000&exp_val)>>16);
  expmid	= (unsigned char) ( (0x00ff00&exp_val)>>8);
  explow	= (unsigned char) ( (0x0000ff&exp_val)	 );
  shutter = exp_val/16;  
  if(shutter  > ov16825_sensor_vts- 4)
        frame_length = shutter + 4;
  else
        frame_length = ov16825_sensor_vts;

  //sensor_write(sd, 0x3208, 0x00);//enter group write
  sensor_write(sd, 0x3503, 0x07);
  sensor_write(sd, 0x3509, gainlow);
  sensor_write(sd, 0x3508, gainhigh);
  sensor_write(sd, 0x380f, (frame_length & 0xff));
  sensor_write(sd, 0x380e, (frame_length >> 8));
  sensor_write(sd, 0x3502, explow);
  sensor_write(sd, 0x3501, expmid);
  sensor_write(sd, 0x3500, exphigh);	
  //sensor_write(sd, 0x3208, 0x10);//end group write
  //sensor_write(sd, 0x3208, 0xa0);//init group write
  info->exp = exp_val;
  info->gain = gain_val;

  //printk("16825 sensor_set_gain = %d, %d, %d, Done!\n", gain_val, gainhigh, gainlow);
  //printk("16825 sensor_set_exp = %d, %d, %d, Done!\n", exp_val, exphigh, expmid);
  return 0;
}

static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	unsigned char explow,expmid,exphigh;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_set_exposure = %d\n", exp_val>>4);
	if(exp_val>0xfffff)
		exp_val=0xfffff;
	
	//if(info->exp == exp_val && exp_val <= (2480)*16)
	//	return 0;
  
	//sensor_write(sd, 0x3208, 0x00);//enter group write
	//sensor_write(sd, 0x3503, 0x10);
	//sensor_write(sd, 0x350d, 0x0);
	//sensor_write(sd, 0x350c, 0x0);
    exphigh = (unsigned char) ( (0x0f0000&exp_val)>>16);
    expmid  = (unsigned char) ( (0x00ff00&exp_val)>>8);
    explow  = (unsigned char) ( (0x0000ff&exp_val)   );
	
	sensor_write(sd, 0x3208, 0x00);//enter group write
	sensor_write(sd, 0x3502, explow);
	sensor_write(sd, 0x3501, expmid);
	sensor_write(sd, 0x3500, exphigh);	
	//sensor_write(sd, 0x3208, 0x10);//end group write
	//sensor_write(sd, 0x3208, 0xa0);//init group write
	printk("16825 sensor_set_exp = %d, Done!\n", exp_val);
	
	info->exp = exp_val;
	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);
	
	*value = info->gain;
	vfe_dev_dbg("sensor_get_gain = %d\n", info->gain);
	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);
	unsigned short gainlow=0;
	unsigned short gainhigh=0;
	unsigned char i=0;
	
	if(gain_val<1*16)
		gain_val=16;
	if(gain_val>64*16-1)
		gain_val=64*16-1;
	vfe_dev_dbg("sensor_set_gain = %d\n", gain_val);
    //if(info->gain == gain_val)
    //    return 0;

	gainlow = gain_val * 8;
	gainlow = gainlow & 0x07ff;
	for(i=1;i<=3;i++) 
	{
		if(gainlow>=0x100) 
		{
			gainhigh = gainhigh + 1;
			gainlow = gainlow / 2;
		}
	}
	 gainhigh = gainhigh <<2;

	sensor_write(sd, 0x3503, 0x07);
	sensor_write(sd, 0x3509, gainlow);
	sensor_write(sd, 0x3508, gainhigh);
	sensor_write(sd, 0x3208, 0x10);//end group write
	sensor_write(sd, 0x3208, 0xa0);//init group write
	
	printk("16825 sensor_set_gain = %d, %d, %d, Done!\n", gain_val, gainhigh, gainlow);
	info->gain = gain_val;
	
	return 0;
}


static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	unsigned char rdval;
	
	ret=sensor_read(sd, 0x0100, &rdval);
	if(ret!=0)
		return ret;
	
	if(on_off==CSI_STBY_ON)//sw stby on
	{
		ret=sensor_write(sd, 0x0100, rdval&0xfe);
	}
	else//sw stby off
	{
		ret=sensor_write(sd, 0x0100, rdval|0x01);
	}
	return ret;
}

/*
 * Stuff that knows about the sensor.
 */
 
static int sensor_power(struct v4l2_subdev *sd, int on)
{
  int ret;
  
  //insure that clk_disable() and clk_enable() are called in pair 
  //when calling CSI_SUBDEV_STBY_ON/OFF and CSI_SUBDEV_PWR_ON/OFF
  ret = 0;
  switch(on)
  {
    case CSI_SUBDEV_STBY_ON:
      vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
//      //disable io oe
//      vfe_dev_print("disalbe oe!\n");
//      ret = sensor_write_array(sd, sensor_oe_disable_regs, ARRAY_SIZE(sensor_oe_disable_regs));
//      if(ret < 0)
//        vfe_dev_err("disalbe oe falied!\n");           
      //software standby on
      ret = sensor_s_sw_stby(sd, CSI_STBY_ON);
      if(ret < 0)
        vfe_dev_err("soft stby falied!\n");
      usleep_range(10000,12000);
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);    
      //standby on io
      vfe_gpio_write(sd,PWDN,CSI_STBY_ON);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);    

      //inactive mclk after stadby in
      vfe_set_mclk(sd,OFF);
      break;
    case CSI_SUBDEV_STBY_OFF:
      vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);    

      //active mclk before stadby out
      vfe_set_mclk_freq(sd,MCLK);
      vfe_set_mclk(sd,ON);
      usleep_range(10000,12000);
      //standby off io
      vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
      usleep_range(10000,12000);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);    
      
//      //software standby
      ret = sensor_s_sw_stby(sd, CSI_STBY_OFF);
      if(ret < 0)
        vfe_dev_err("soft stby off falied!\n");
//      mdelay(10);
//      vfe_dev_print("enable oe!\n");
//      ret = sensor_write_array(sd, sensor_oe_enable_regs);
//      if(ret < 0)
//        vfe_dev_err("enable oe falied!\n");
      break;
    case CSI_SUBDEV_PWR_ON:
      vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);    

      //power on reset
      vfe_gpio_set_status(sd,PWDN,1);//set the gpio to output
      vfe_gpio_set_status(sd,RESET,1);//set the gpio to output
      //power down io
      vfe_gpio_write(sd,PWDN,CSI_STBY_ON);
      //reset on io
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      usleep_range(1000,1200);
      //active mclk before power on
      vfe_set_mclk_freq(sd,MCLK);
      vfe_set_mclk(sd,ON);
      usleep_range(10000,12000);
      //power supply
      vfe_gpio_write(sd,POWER_EN,CSI_PWR_ON);
      vfe_set_pmu_channel(sd,IOVDD,ON);
      vfe_set_pmu_channel(sd,AVDD,ON);
      vfe_set_pmu_channel(sd,DVDD,ON);
      vfe_set_pmu_channel(sd,AFVDD,ON);
      //standby off io
      vfe_gpio_write(sd,PWDN,CSI_STBY_OFF);
      usleep_range(10000,12000);
      //reset after power on
      vfe_gpio_write(sd,RESET,CSI_RST_OFF);
      usleep_range(30000,31000);
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);    

      break;
    case CSI_SUBDEV_PWR_OFF:
      vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");
      //make sure that no device can access i2c bus during sensor initial or power down
      //when using i2c_lock_adpater function, the following codes must not access i2c bus before calling i2c_unlock_adapter
      cci_lock(sd);   
      //inactive mclk before power off
      vfe_set_mclk(sd,OFF);
      //power supply off
      vfe_gpio_write(sd,POWER_EN,CSI_PWR_OFF);
      vfe_set_pmu_channel(sd,AFVDD,OFF);
      vfe_set_pmu_channel(sd,DVDD,OFF);
      vfe_set_pmu_channel(sd,AVDD,OFF);
      vfe_set_pmu_channel(sd,IOVDD,OFF);  
      //standby and reset io
      usleep_range(10000,12000);
      vfe_gpio_write(sd,POWER_EN,CSI_STBY_OFF);
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      //set the io to hi-z
      vfe_gpio_set_status(sd,RESET,0);//set the gpio to input
      vfe_gpio_set_status(sd,PWDN,0);//set the gpio to input
      //remember to unlock i2c adapter, so the device can access the i2c bus again
      cci_unlock(sd);    

      break;
    default:
      return -EINVAL;
  }   

  return 0;
}
 
static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
  switch(val)
  {
    case 0:
      vfe_gpio_write(sd,RESET,CSI_RST_OFF);
      usleep_range(10000,12000);
      break;
    case 1:
      vfe_gpio_write(sd,RESET,CSI_RST_ON);
      usleep_range(10000,12000);
      break;
    default:
      return -EINVAL;
  }
    
  return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
  unsigned char rdval;
  
  LOG_ERR_RET(sensor_read(sd, 0x300a, &rdval))
  if(rdval != 0x01)
    return -ENODEV;
  	
  LOG_ERR_RET(sensor_read(sd, 0x300b, &rdval))
  if(rdval != 0x68)
    return -ENODEV;
	  
  LOG_ERR_RET(sensor_read(sd, 0x300c, &rdval))
  if(rdval != 0x20)
    return -ENODEV;
  
  return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
  int ret;
  struct sensor_info *info = to_state(sd);
  
  vfe_dev_dbg("sensor_init\n");
  
  /*Make sure it is a target sensor*/
  ret = sensor_detect(sd);
  if (ret) {
    vfe_dev_err("chip found is not an target chip.\n");
    return ret;
  }
  
  vfe_get_standby_mode(sd,&info->stby_mode);
  
  if((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY) \
      && info->init_first_flag == 0) {
    vfe_dev_print("stby_mode and init_first_flag = 0\n");
    return 0;
  } 
  
  info->focus_status = 0;
  info->low_speed = 0;
  info->width = 4608;
  info->height = 3456;
  info->hflip = 0;
  info->vflip = 0;
  info->gain = 0;

  info->tpf.numerator = 1;            
  info->tpf.denominator = 15;    /* 30fps */    
  
  ret = sensor_write_array(sd, sensor_default_regs, ARRAY_SIZE(sensor_default_regs));  
  if(ret < 0) {
    vfe_dev_err("write sensor_default_regs error\n");
    return ret;
  }
  
  if(info->stby_mode == 0)
    info->init_first_flag = 0;
  
  info->preview_first_flag = 1;
  
  return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
  int ret=0;
  struct sensor_info *info = to_state(sd);
//  vfe_dev_dbg("[]cmd=%d\n",cmd);
//  vfe_dev_dbg("[]arg=%0x\n",arg);
  switch(cmd) {
    case GET_CURRENT_WIN_CFG:
      if(info->current_wins != NULL)
      {
        memcpy( arg,
                info->current_wins,
                sizeof(struct sensor_win_size) );
        ret=0;
      }
      else
      {
        vfe_dev_err("empty wins!\n");
        ret=-1;
      }
      break;
    case SET_FPS:
      ret=0;
//      if((unsigned int *)arg==1)
//        ret=sensor_write(sd, 0x3036, 0x78);
//      else
//        ret=sensor_write(sd, 0x3036, 0x32);
      break;
    case ISP_SET_EXP_GAIN:
		ret = sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
      break;
    default:
      return -EINVAL;
  }
  return ret;
}


/*
 * Store information about the video data format. 
 */
static struct sensor_format_struct {
  __u8 *desc;
  //__u32 pixelformat;
  enum v4l2_mbus_pixelcode mbus_code;
  struct regval_list *regs;
  int regs_size;
  int bpp;   /* Bytes per pixel */
}sensor_formats[] = {
	{
		.desc				= "Raw RGB Bayer",
		.mbus_code	= V4L2_MBUS_FMT_SBGGR10_10X1,//V4L2_MBUS_FMT_SGRBG10_10X1,
		.regs 			= sensor_fmt_raw,
		.regs_size 	= ARRAY_SIZE(sensor_fmt_raw),
		.bpp				= 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

  

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */


static struct sensor_win_size sensor_win_sizes[] = {
#if 1
	  /* Fullsize: 4608*3456 */
	  {
      .width      = 4608,
      .height     = 3456,
      .hoffset    = 0,
      .voffset    = 0,
      .hts        = 1528,
      .vts        = 3490,
      .pclk       = 80*1000*1000,
      .mipi_bps	  = 800*1000*1000,
      .fps_fixed  = 2,
      .bin_factor = 1,
      .intg_min   = 16,
      .intg_max   = (3490-4)<<4,
      .gain_min   = 1<<4,
      .gain_max   = 12<<4,
      .regs       = sensor_16M_regs,
      .regs_size  = ARRAY_SIZE(sensor_16M_regs),
      .set_size   = NULL,
    },
#endif
#if 1

    /* 2160P */
    {
      .width	  = 3840,
      .height 	  = 2160,
      .hoffset    = 0,
      .voffset    = 0,
	  .hts		  = 1216,
	  .vts		  = 2194,
	  .pclk 	  = 80*1000*1000,
	  .mipi_bps   = 800*1000*1000,
      .fps_fixed  = 1,
      .bin_factor = 1,
      .intg_min   = 1<<4,
      .intg_max   = (2194-4)<<4,
      .gain_min   = 1<<4,
      .gain_max   = 12<<4,
      .regs       = sensor_2160p_regs,//
      .regs_size  = ARRAY_SIZE(sensor_2160p_regs),//
      .set_size	  = NULL,
    },

    /* 1080P */
    {
      .width	  = HD1080_WIDTH,
      .height 	  = HD1080_HEIGHT,
      .hoffset	  = 0,
      .voffset	  = 0,
      .hts        = 1206,
      .vts        = 1106,
	  .pclk 	  = 80*1000*1000,
	  .mipi_bps   = 800*1000*1000,
      .fps_fixed  = 1,
      .bin_factor = 1,
      .intg_min   = 16,
      .intg_max   = (1106-4)<<4,
      .gain_min   = 1<<4,
      .gain_max   = 12<<4,
      .regs		  = sensor_1080p_regs,
      .regs_size  = ARRAY_SIZE(sensor_1080p_regs),
      .set_size	  = NULL,
    },

    /* 720p */
    {
      .width      = 2304,
      .height     = 1728,
      .hoffset    = 0,
      .voffset    = 0,
	  .hts		  = 1520,
	  .vts		  = 1754,
	  .pclk 	  = 80*1000*1000,
	  .mipi_bps   = 800*1000*1000,
      .fps_fixed  = 1,
      .bin_factor = 1,
      .intg_min   = 16,
      .intg_max   = (1754-4)<<4,
      .gain_min   = 1<<4,
      .gain_max   = 12<<4,
      .regs		  = sensor_quarter_regs,//
      .regs_size  = ARRAY_SIZE(sensor_quarter_regs),//
      .set_size	  = NULL,
    },
#endif
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_enum_fmt(struct v4l2_subdev *sd, unsigned index,
                 enum v4l2_mbus_pixelcode *code)
{
  if (index >= N_FMTS)
    return -EINVAL;

  *code = sensor_formats[index].mbus_code;
  return 0;
}

static int sensor_enum_size(struct v4l2_subdev *sd,
                            struct v4l2_frmsizeenum *fsize)
{
  if(fsize->index > N_WIN_SIZES-1)
  	return -EINVAL;
  
  fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
  fsize->discrete.width = sensor_win_sizes[fsize->index].width;
  fsize->discrete.height = sensor_win_sizes[fsize->index].height;
  
  return 0;
}


static int sensor_try_fmt_internal(struct v4l2_subdev *sd,
    struct v4l2_mbus_framefmt *fmt,
    struct sensor_format_struct **ret_fmt,
    struct sensor_win_size **ret_wsize)
{
  int index;
  struct sensor_win_size *wsize;
  struct sensor_info *info = to_state(sd);

  for (index = 0; index < N_FMTS; index++)
    if (sensor_formats[index].mbus_code == fmt->code)
      break;

  if (index >= N_FMTS) 
    return -EINVAL;
  
  if (ret_fmt != NULL)
    *ret_fmt = sensor_formats + index;
    
  /*
   * Fields: the sensor devices claim to be progressive.
   */
  
  fmt->field = V4L2_FIELD_NONE;
  
  /*
   * Round requested image size down to the nearest
   * we support, but not below the smallest.
   */
  for (wsize = sensor_win_sizes; wsize < sensor_win_sizes + N_WIN_SIZES;
       wsize++)
    if (fmt->width >= wsize->width && fmt->height >= wsize->height)
      break;
    
  if (wsize >= sensor_win_sizes + N_WIN_SIZES)
    wsize--;   /* Take the smallest one */
  if (ret_wsize != NULL)
    *ret_wsize = wsize;
  
  info->current_wins = wsize;  
    
  /*
   * Note the size we'll actually handle.
   */
  fmt->width = wsize->width;
  fmt->height = wsize->height;
  //pix->bytesperline = pix->width*sensor_formats[index].bpp;
  //pix->sizeimage = pix->height*pix->bytesperline;

  return 0;
}

static int sensor_try_fmt(struct v4l2_subdev *sd, 
             struct v4l2_mbus_framefmt *fmt)
{
  return sensor_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
           struct v4l2_mbus_config *cfg)
{
  cfg->type = V4L2_MBUS_CSI2;
  cfg->flags = 0|V4L2_MBUS_CSI2_4_LANE|V4L2_MBUS_CSI2_CHANNEL_0;
  
  return 0;
}


/*
 * Set a format.
 */
static int sensor_s_fmt(struct v4l2_subdev *sd, 
             struct v4l2_mbus_framefmt *fmt)
{
  int ret;
     
  struct sensor_format_struct *sensor_fmt;
  struct sensor_win_size *wsize;
  struct sensor_info *info = to_state(sd);
  
  vfe_dev_dbg("sensor_s_fmt\n");
  
  //sensor_write_array(sd, sensor_oe_disable_regs, ARRAY_SIZE(sensor_oe_disable_regs));
    LOG_ERR_RET(sensor_write_array(sd, sensor_default_regs, ARRAY_SIZE(sensor_default_regs)) );
  
  ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
  if (ret)
    return ret;

  if(info->capture_mode == V4L2_MODE_VIDEO)
  {
    //video
  }
  else if(info->capture_mode == V4L2_MODE_IMAGE)
  {
    //image 
    
  }

  sensor_write_array(sd, sensor_fmt->regs, sensor_fmt->regs_size);

  ret = 0;
  if (wsize->regs)
    LOG_ERR_RET(sensor_write_array(sd, wsize->regs, wsize->regs_size))
  
  if (wsize->set_size)
    LOG_ERR_RET(wsize->set_size(sd))

  info->fmt = sensor_fmt;
  info->width = wsize->width;
  info->height = wsize->height;
  ov16825_sensor_vts = wsize->vts;
	printk("ov16825 set info->exp = 0........................\n");
	info->exp = 0;
	info->gain = 0;

  vfe_dev_print("s_fmt = %x, width = %d, height = %d\n",sensor_fmt->mbus_code,wsize->width,wsize->height);

  if(info->capture_mode == V4L2_MODE_VIDEO)
  {
    //video
   
  } else {
    //capture image

  }
	
	//sensor_write_array(sd, sensor_oe_enable_regs, ARRAY_SIZE(sensor_oe_enable_regs));
	vfe_dev_print("s_fmt end\n");
	return 0;
}

/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
  struct v4l2_captureparm *cp = &parms->parm.capture;
  struct sensor_info *info = to_state(sd);

  if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
    return -EINVAL;
  
  memset(cp, 0, sizeof(struct v4l2_captureparm));
  cp->capability = V4L2_CAP_TIMEPERFRAME;
  cp->capturemode = info->capture_mode;
     
  return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
  struct v4l2_captureparm *cp = &parms->parm.capture;
  //struct v4l2_fract *tpf = &cp->timeperframe;
  struct sensor_info *info = to_state(sd);
  //unsigned char div;
  
  vfe_dev_dbg("sensor_s_parm\n");
  
  if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
    return -EINVAL;
  
  if (info->tpf.numerator == 0)
    return -EINVAL;
    
  info->capture_mode = cp->capturemode;
  
  return 0;
}


static int sensor_queryctrl(struct v4l2_subdev *sd,
    struct v4l2_queryctrl *qc)
{
  /* Fill in min, max, step and default value for these controls. */
  /* see include/linux/videodev2.h for details */
  
  switch (qc->id) {
	case V4L2_CID_GAIN:
		return v4l2_ctrl_query_fill(qc, 1*16, 64*16-1, 1, 1*16);
	case V4L2_CID_EXPOSURE:
		return v4l2_ctrl_query_fill(qc, 0, 65535*16, 1, 0);
	case V4L2_CID_FRAME_RATE:
		return v4l2_ctrl_query_fill(qc, 15, 120, 1, 120);
  }
  return -EINVAL;
}

static int sensor_g_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
  switch (ctrl->id) {
  case V4L2_CID_GAIN:
    return sensor_g_gain(sd, &ctrl->value);
  case V4L2_CID_EXPOSURE:
  	return sensor_g_exp(sd, &ctrl->value);
  }
  return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_subdev *sd, struct v4l2_control *ctrl)
{
  struct v4l2_queryctrl qc;
  int ret;
  
  qc.id = ctrl->id;
  ret = sensor_queryctrl(sd, &qc);
  if (ret < 0) {
    return ret;
  }

  if (ctrl->value < qc.minimum || ctrl->value > qc.maximum) {
    return -ERANGE;
  }
  
  switch (ctrl->id) {
    case V4L2_CID_GAIN:
      return sensor_s_gain(sd, ctrl->value);
    case V4L2_CID_EXPOSURE:
	  return sensor_s_exp(sd, ctrl->value);
  }
  return -EINVAL;
}


static int sensor_g_chip_ident(struct v4l2_subdev *sd,
    struct v4l2_dbg_chip_ident *chip)
{
  struct i2c_client *client = v4l2_get_subdevdata(sd);

  return v4l2_chip_ident_i2c_client(client, chip, V4L2_IDENT_SENSOR, 0);
}


/* ----------------------------------------------------------------------- */

static const struct v4l2_subdev_core_ops sensor_core_ops = {
  .g_chip_ident = sensor_g_chip_ident,
  .g_ctrl = sensor_g_ctrl,
  .s_ctrl = sensor_s_ctrl,
  .queryctrl = sensor_queryctrl,
  .reset = sensor_reset,
  .init = sensor_init,
  .s_power = sensor_power,
  .ioctl = sensor_ioctl,
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
  .enum_mbus_fmt = sensor_enum_fmt,
  .enum_framesizes = sensor_enum_size,
  .try_mbus_fmt = sensor_try_fmt,
  .s_mbus_fmt = sensor_s_fmt,
  .s_parm = sensor_s_parm,
  .g_parm = sensor_g_parm,
  .g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_ops sensor_ops = {
  .core = &sensor_core_ops,
  .video = &sensor_video_ops,
};

/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv = {
	.name = SENSOR_NAME,
};

static int sensor_probe(struct i2c_client *client,
      const struct i2c_device_id *id)
{
  struct v4l2_subdev *sd;
  struct sensor_info *info;
//  int ret;

  info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
  if (info == NULL)
    return -ENOMEM;
  sd = &info->sd;
  glb_sd = sd;
  cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);

  info->fmt = &sensor_formats[0];
  info->af_first_flag = 1;
  info->init_first_flag = 1;

  return 0;
}


static int sensor_remove(struct i2c_client *client)
{
  struct v4l2_subdev *sd;

  sd = cci_dev_remove_helper(client, &cci_drv);
  kfree(to_state(sd));
  return 0;
}

static const struct i2c_device_id sensor_id[] = {
  { SENSOR_NAME, 0 },
  { }
};
MODULE_DEVICE_TABLE(i2c, sensor_id);


static struct i2c_driver sensor_driver = {
  .driver = {
    .owner = THIS_MODULE,
  .name = SENSOR_NAME,
  },
  .probe = sensor_probe,
  .remove = sensor_remove,
  .id_table = sensor_id,
};
static __init int init_sensor(void)
{
	return cci_dev_init_helper(&sensor_driver);
}

static __exit void exit_sensor(void)
{
	cci_dev_exit_helper(&sensor_driver);
}

module_init(init_sensor);
module_exit(exit_sensor);

