#ifdef BUILD_LK
#else
#include <linux/string.h>
#if defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <mach/mt_gpio.h>
#endif
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
#define LCM_PRINT printf
#else
#if defined(BUILD_UBOOT)
	#define LCM_PRINT printf
#else
	#define LCM_PRINT printk
#endif
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0xFE
#define REGFLAG_END_OF_TABLE      							0xFFF   // END OF REGISTERS MARKER

#ifndef TRUE
    #define   TRUE     1
#endif
 
#ifndef FALSE
    #define   FALSE    0
#endif

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define read_reg_V2(cmd, buffer, buffer_size)				lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)

struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static struct LCM_setting_table lcm_initialization_setting[] = {
	/* Structure Format :

	{DCS command, count of parameters, {parameter list}}
	{REGFLAG_DELAY, milliseconds of time, {}},
	...
	Setting ending by predefined flag
	{REGFLAG_END_OF_TABLE, 0x00, {}} */

	{0x00,	1,	{0x00}},
	{0xff,	3,	{0x80, 0x09, 0x01}},
	{0x00,	1,	{0x80}},
	{0xff,	2,	{0x80, 0x09}},
	{0x00,	1,	{0xB1}},
	{0xB0,	1,	{0x80}},
	{0x00,	1,	{0xB4}},
	{0xC0,	1,	{0x10}},
	{0x00,	1,	{0x81}},
	{0xC5,	2,	{0x66, 0xA3}},
	{0x00,	1,	{0x90}},
	{0xC5,	2,	{0xD6, 0x87}},
	{0x00,	1,	{0x00}},
	{0xD8,	2,	{0x55, 0x55}},
	{0x00,	1,	{0x00}},
	{0xD9,	1,	{0x32}},
	{0x00,	1,	{0x81}},
	{0xC1,	1,	{0x77}},
	{0x00,	1,	{0x89}},
	{0xC4,	1,	{0x00}},
	{0x00,	1,	{0xA2}},
	{0xC0,	3,	{0x02, 0x1B, 0x02}},
	{0x00,	1,	{0x81}},
	{0xC4,	1,	{0x83}},
	{0x00,	1,	{0x92}},
	{0xC5,	1,	{0x01}},
	{0x00,	1,	{0xB1}},
	{0xC5,	1,	{0xA9}},
	{0x00,	1,	{0xC0}},
	{0xC5,	1,	{0x00}},
	{0x00,	1,	{0x8B}},
	{0xB0,	1,	{0x40}},
	{0x00,	1,	{0xC6}},
	{0xB0,	1,	{0x03}},
	{0x00,	1,	{0xB2}},
	{0xF5,	5,	{0x15, 0x00, 0x15, 0x00, 0x06}},
	{0x00,	1,	{0x93}},
	{0xC5,	1,	{0x03}},
	{0x00,	1,	{0x92}},
	{0xB3,	1,	{0x45}},
	{0x00,	1,	{0x90}},
	{0xB3,	1,	{0x02}},
	{0x00,	1,	{0x80}},
	{0xC0,	5,	{0x00, 0x58, 0x00, 0x14, 0x16}},
	{0x00,	1,	{0x80}},
	{0xC4,	1,	{0x30}},
	{0x00,	1,	{0x8A}},
	{0xC4,	1,	{0x40}},
	{0x00,	1,	{0x90}},
	{0xC0,	6,	{0x00, 0x56, 0x00, 0x00, 0x00, 0x03}},
	{0x00,	1,	{0xA6}},
	{0xC1,	3,	{0x01, 0x00, 0x00}},
	{0x00,	1,	{0x80}},
	{0xCE,	12,	{0x8B, 0x03, 0x0A, 0x8A, 0x03, 0x0A, 0x89, 0x03, 0x0A, 0x88, 0x03, 0x0A}},
	{0x00,	1,	{0xA0}},
	{0xCE,	14,	{0x38, 0x07, 0x83, 0x54, 0x8C, 0x0A, 0x00, 0x38, 0x06, 0x83, 0x55, 0x8C, 0x0A, 0x00}},
	{0x00,	1,	{0xB0}},
	{0xCE,	14,	{0x38, 0x05, 0x83, 0x56, 0x8C, 0x0A, 0x00, 0x38, 0x04, 0x83, 0x57, 0x8C, 0x0A, 0x00}},
	{0x00,	1,	{0xC0}},
	{0xCE,	14,	{0x38, 0x03, 0x83, 0x58, 0x8C, 0x0A, 0x00, 0x38, 0x02, 0x83, 0x59, 0x8C, 0x0A, 0x00}},
	{0x00,	1,	{0xD0}},
	{0xCE,	14,	{0x38, 0x01, 0x83, 0x5A, 0x8C, 0x0A, 0x00, 0x38, 0x00, 0x83, 0x5B, 0x8C, 0x0A, 0x00}},
	{0x00,	1,	{0xC0}},
	{0xCF,	10,	{0x3D, 0x3D, 0x20, 0x20, 0x00, 0x00, 0x01, 0x80, 0x00, 0x0A}},
	{0x00,	1,	{0xC0}},
	{0xCB,	15,	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0x00,	1,	{0xD0}},
	{0xCB,	15,	{0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14}},
	{0x00,	1,	{0xE0}},
	{0xCB,	10,	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0x00,	1,	{0x80}},
	{0xCC,	10,	{0x26, 0x25, 0x21, 0x22, 0x0C, 0x0A, 0x10, 0x0E, 0x02, 0x04}},
	{0x00,	1,	{0x90}},
	{0xCC,	15,	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x25, 0x21, 0x22, 0x0B}},
	{0x00,	1,	{0xA0}},
	{0xCC,	15,	{0x09, 0x0F, 0x0D, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0x00,	1,	{0xB0}},
	{0xCC,	10,	{0x25, 0x26, 0x21, 0x22, 0x0B, 0x0D, 0x0F, 0x09, 0x03, 0x01}},
	{0x00,	1,	{0xC0}},
	{0xCC,	15,	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x26, 0x21, 0x22, 0x0C}},
	{0x00,	1,	{0xD0}},
	{0xCC,	15,	{0x0E, 0x10, 0x0A, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0x00,	1,	{0x00}},
	{0xE1,	16,	{0x00, 0x02, 0x08, 0x12, 0x0A, 0x1B, 0x0F, 0x0F, 0x00, 0x03, 0x03, 0x05, 0x0D, 0x29, 0x26, 0x1F}},
	{0x00,	1,	{0x00}},
	{0xE2,	16,	{0x00, 0x02, 0x08, 0x12, 0x0A, 0x1A, 0x0F, 0x0F, 0x00, 0x03, 0x02, 0x05, 0x0D, 0x28, 0x25, 0x1F}},
	{0x00,	1,	{0xA0}},
	{0xC1,	1,	{0xEA}},
	{0x00,	1,	{0xA6}},
	{0xC1,	3,	{0x01, 0x00, 0x00}},
	{0x00,	1,	{0xC6}},
	{0xB0,	1,	{0x03}},
	{0x00,	1,	{0x81}},
	{0xC5,	1,	{0x66}},
	{0x00,	1,	{0xB6}},
	{0xF5,	1,	{0x06}},
	{0x00,	1,	{0x8B}},
	{0xB0,	1,	{0x40}},
	{0x00,	1,	{0xB1}},
	{0xB0,	1,	{0x80}},
	{0x00,	1,	{0x92}},
	{0xF5,	7,	{0x0E, 0x14, 0x0E, 0x14, 0x00, 0x14, 0x00}},
	{0x00,	1,	{0xB2}},
	{0xF5,	4,	{0x15, 0x00, 0x15, 0x00}},
	{0x00,	1,	{0x93}},
	{0xC5,	1,	{0x03}},
	{0x00,	1,	{0x80}},
	{0xC5,	1,	{0x0B}},
	{0x00,	1,	{0x00}},
	{0xFF,	3,	{0xFF, 0xFF, 0xFF}},
	{0x11,	1,	{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{0x29,	1,	{0x00}},
	{REGFLAG_DELAY, 10, {}/*},
	{0x28,	1,	{0x00}},
	{REGFLAG_DELAY, 50, {}},
	{0x10,	1,	{0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}*/}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
	// Display off sequence
	{0x28, 1, {0x00}},
	{REGFLAG_DELAY, 50, {}},

    // Sleep Mode On
	{0x10, 1, {0x00}},
	{REGFLAG_DELAY, 120, {}},
	{REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update){
	unsigned int i;
    for(i = 0; i < count; i++) {
		unsigned cmd;
        cmd = table[i].cmd;
        switch (cmd) {
            case REGFLAG_DELAY :
                MDELAY(table[i].count);
                break;
            case REGFLAG_END_OF_TABLE :
                break;
            default:
				dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
       	}
    }
}

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util){
    memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params){
		memset(params, 0, sizeof(LCM_PARAMS));
	
		params->type   = LCM_TYPE_DSI;

		params->width  = FRAME_WIDTH;
		params->height = FRAME_HEIGHT;

		// enable tearing-free
		params->dbi.te_mode 				= LCM_DBI_TE_MODE_VSYNC_ONLY;
		params->dbi.te_edge_polarity		= LCM_POLARITY_RISING;

		params->dsi.mode = BURST_VDO_MODE;
	
		// DSI
		/* Command mode setting */
		params->dsi.LANE_NUM				= LCM_TWO_LANE;
		//The following defined the fomat for data coming from LCD engine.
		params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
		params->dsi.data_format.trans_seq   = LCM_DSI_TRANS_SEQ_MSB_FIRST;
		params->dsi.data_format.padding     = LCM_DSI_PADDING_ON_LSB;
		params->dsi.data_format.format      = LCM_DSI_FORMAT_RGB888;

		// Video mode setting		
		params->dsi.intermediat_buffer_num = 2;

		params->dsi.PS=LCM_PACKED_PS_24BIT_RGB888;
		params->dsi.word_count=480*3;
		
		params->dsi.vertical_sync_active				= 4;
		params->dsi.vertical_backporch					= 16;
		params->dsi.vertical_frontporch					= 20;
		params->dsi.vertical_active_line				= FRAME_HEIGHT; 

		params->dsi.horizontal_sync_active				= 10;
		params->dsi.horizontal_backporch				= 50;
		params->dsi.horizontal_frontporch				= 60;
		params->dsi.horizontal_blanking_pixel 			= 60;
		params->dsi.horizontal_active_pixel				= FRAME_WIDTH;

		params->dsi.pll_div1 = 0;		// div1=0,1,2,3;div1_real=1,2,4,4 ----0: 546Mbps  1:273Mbps
		params->dsi.pll_div2 = 1;		// div2=0,1,2,3;div1_real=1,2,4,4	
		params->dsi.fbk_div = 15;    // fref=26MHz, fvco=fref*(fbk_div+1)*2/(div1_real*div2_real)	
		params->dsi.fbk_sel = 1;
	/*	params->dsi.HS_TRAIL=4;
		params->dsi.HS_PRPR=3;
		params->dsi.HS_ZERO=10;
		params->dsi.LPX=3;
		params->dsi.TA_GET=15;
		params->dsi.TA_SURE=4;
		params->dsi.TA_GO=12;
		params->dsi.CLK_TRAIL=4;
		params->dsi.CLK_ZERO=19;
		params->dsi.CLK_HS_PRPR=3;*/
		params->dsi.noncont_clock = TRUE; 
		params->dsi.noncont_clock_period = 1;
}

static void lcm_init(void){
	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(120);
	push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

static void lcm_resume(void){
	lcm_init();
}

static void lcm_suspend(void){
	push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
}

static unsigned int lcm_compare_id(void){
	int id=0;
	char buffer[5];
	int array[4];

	SET_RESET_PIN(1);
	MDELAY(10);
	SET_RESET_PIN(0);
	MDELAY(10);
	SET_RESET_PIN(1);
	MDELAY(50);
  
	array[0] = 0x00053700;
	dsi_set_cmdq(array, 1, 1);
  
	read_reg_V2(0xA1, buffer, 5);
	id = buffer[2]<<8 | buffer[3];

	printk("[darren] OTM8018B kernel %s \n", __func__);
	printk("[darren] %s id = 0x%08x \n", __func__, id);
	return (id == 0x8009)?1:0;
}

static unsigned int lcm_esd_check(void){
	static int err_count = 0;
	int i;
	int result;
	unsigned char buffer[12];
	unsigned int array[16];

	for (i = 0; i < 12; i++)
		buffer[i] = 0x00;

	array[0] = 0x00063700;
	dsi_set_cmdq(array, 1, 1);

	read_reg_V2(0x0A, buffer, 6);
	if (buffer[0] != 0x9C || (buffer[3] == 0x02 && buffer[4] & 0x02))
	{
		err_count ++;
		if (err_count > 1)
		{
			printk("lcm_esd_true recover register [0x0A] value = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
			result = 1;
			err_count = 0;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 0;
		err_count = 0;
	}
	return result;
}

static unsigned int lcm_esd_recover(void){
    lcm_init();
    return TRUE;
}

LCM_DRIVER otm8018b_dsi_vdo_dijing_lcm_drv = 
{
    .name			= "otm8018b_dsi_vdo_dijing",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.compare_id     = lcm_compare_id,	
	.esd_check 		= lcm_esd_check,
	.esd_recover 	= lcm_esd_recover,
};