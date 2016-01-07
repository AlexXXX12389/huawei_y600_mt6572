#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
//#include <mach/mt6577_pm_ldo.h>

static struct alsps_hw cust_alsps_hw_rpr400 = {
    .i2c_num    = 1,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .i2c_addr   = {0x72, 0x48, 0x78, 0x00},
    /*Lenovo-sw chenlj2 add 2011-06-03,modify parameter below two lines*/
    /*lenovo-sw jixj 2011.12.22 modified being*/
    /*
    .als_level  = { 4, 40,  80,   120,   160, 250,  400, 800, 1200,  1600, 2000, 3000, 5000, 10000, 65535},
    .als_value  = {10, 20,20,  120, 120, 280,  280,  280, 1600,  1600,  1600,  6000,  6000, 9000,  10240, 10240},
    */
    //.als_level  = {1,  2,   4,   8, 12,  20, 37, 48,  62, 180, 352, 400, 800, 1300, 2900},
    //.als_value  = {10, 16,25,  40, 63, 101,  160,  254, 403,  640,  1016,  1613,  2560, 4064,  6451, 10240},
    /* LENOVO.SW begin chenyb1 2013.2.27 add new value */
    .als_level  = {2, 7,  18,   597,   1899, 6024, 9638, 11553, 11553, 11553, 11553, 11553, 11553, 11553, 11553, 11553, 11553, 11553},
    .als_value  = {5, 20, 90,350,  1100, 3500, 5600, 6700, 8700, 8700, 8700, 8700, 8700, 8700, 8700, 8700, 8700, 8700},
    /* LENOVO.SW end chenyb1 2013.2.27 add new value */
    /*lenovo-sw jixj 2011.12.22 modified end*/
    /*Lenovo-sw chenlj2 modified 2011-08-09*/
    .ps_threshold_high = 90,
    .ps_threshold_low = 75,
    .ps_threshold = 900,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw_rpr400;
}
