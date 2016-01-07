#ifndef _KD_IMGSENSOR_H
#define _KD_IMGSENSOR_H

#include <linux/ioctl.h>

#ifndef ASSERT
    #define ASSERT(expr)        BUG_ON(!(expr))
#endif

#define IMGSENSORMAGIC 'i'
//IOCTRL(inode * ,file * ,cmd ,arg )
//S means "set through a ptr"
//T means "tell by a arg value"
//G means "get by a ptr"
//Q means "get by return a value"
//X means "switch G and S atomically"
//H means "switch T and Q atomically"

/*******************************************************************************
*
********************************************************************************/
#define YUV_INFO(_id, name, getCalData)\
    { \
    _id, name, \
    NSFeature::YUVSensorInfo<_id>::createInstance(name, #name), \
    (NSFeature::SensorInfoBase*(*)()) \
    NSFeature::YUVSensorInfo<_id>::getInstance, \
    NSFeature::YUVSensorInfo<_id>::getDefaultData, \
    getCalData, \
	NSFeature::YUVSensorInfo<_id>::getNullFlickerPara \
    }
#define RAW_INFO(_id, name, getCalData)\
    { \
    _id, name, \
    NSFeature::RAWSensorInfo<_id>::createInstance(name, #name), \
    (NSFeature::SensorInfoBase*(*)()) \
    NSFeature::RAWSensorInfo<_id>::getInstance, \
    NSFeature::RAWSensorInfo<_id>::getDefaultData, \
    getCalData, \
	NSFeature::RAWSensorInfo<_id>::getFlickerPara \
    }
/*******************************************************************************
*
********************************************************************************/

//sensorOpen
//This command will TBD
#define KDIMGSENSORIOC_T_OPEN            _IO(IMGSENSORMAGIC,0)
//sensorGetInfo
//This command will TBD
#define KDIMGSENSORIOC_X_GETINFO            _IOWR(IMGSENSORMAGIC,5,ACDK_SENSOR_GETINFO_STRUCT)
//sensorGetResolution
//This command will TBD
#define KDIMGSENSORIOC_X_GETRESOLUTION      _IOWR(IMGSENSORMAGIC,10,ACDK_SENSOR_RESOLUTION_INFO_STRUCT)
//sensorFeatureControl
//This command will TBD
#define KDIMGSENSORIOC_X_FEATURECONCTROL    _IOWR(IMGSENSORMAGIC,15,ACDK_SENSOR_FEATURECONTROL_STRUCT)
//sensorControl
//This command will TBD
#define KDIMGSENSORIOC_X_CONTROL            _IOWR(IMGSENSORMAGIC,20,ACDK_SENSOR_CONTROL_STRUCT)
//sensorClose
//This command will TBD
#define KDIMGSENSORIOC_T_CLOSE            _IO(IMGSENSORMAGIC,25)
//sensorSearch 
#define KDIMGSENSORIOC_T_CHECK_IS_ALIVE     _IO(IMGSENSORMAGIC, 30) 
//set sensor driver
#define KDIMGSENSORIOC_X_SET_DRIVER         _IOWR(IMGSENSORMAGIC,35,SENSOR_DRIVER_INDEX_STRUCT)
//get socket postion
#define KDIMGSENSORIOC_X_GET_SOCKET_POS     _IOWR(IMGSENSORMAGIC,40,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_SET_I2CBUS     _IOWR(IMGSENSORMAGIC,45,u32)
//set I2C bus 
#define KDIMGSENSORIOC_X_RELEASE_I2C_TRIGGER_LOCK     _IO(IMGSENSORMAGIC,50)
//Set Shutter Gain Wait Done
#define KDIMGSENSORIOC_X_SET_SHUTTER_GAIN_WAIT_DONE   _IOWR(IMGSENSORMAGIC,55,u32)//HDR
//set mclk
#define KDIMGSENSORIOC_X_SET_MCLK_PLL         _IOWR(IMGSENSORMAGIC,60,ACDK_SENSOR_MCLK_STRUCT)
#define KDIMGSENSORIOC_X_GETINFO2            _IOWR(IMGSENSORMAGIC,65,IMAGESENSOR_GETINFO_STRUCT)
//set open/close sensor index
#define KDIMGSENSORIOC_X_SET_CURRENT_SENSOR   _IOWR(IMGSENSORMAGIC,70,u32)
/*******************************************************************************
*
********************************************************************************/
/* SENSOR CHIP VERSION */
#define OV5646MIPI_SENSOR_ID                    0x5646
#define OV5645MIPI_SENSOR_ID                    0x5645
#define OV5646AFMIPI_SENSOR_ID                  0x5644
#define OV5645AFMIPI_SENSOR_ID                  0x5643
#define GC0329_SENSOR_ID			            0xC0
/* CAMERA DRIVER NAME */
#define CAMERA_HW_DEVNAME            "kd_camera_hw"

/* SENSOR DEVICE DRIVER NAME */
#define SENSOR_DRVNAME_OV5646_MIPI_YUV "ov5645_mipi_qiutai"
#define SENSOR_DRVNAME_OV5645_MIPI_YUV "ov5645_mipi_yuv_truly"
#define SENSOR_DRVNAME_OV5646AF_MIPI_YUV "ov5645_mipi_qiutai_af"
#define SENSOR_DRVNAME_OV5645AF_MIPI_YUV "ov5645_mipi_yuv_truly_af"
#define SENSOR_DRVNAME_GC0329_YUV   "gc0329_yuv"


/*******************************************************************************
*
********************************************************************************/

void KD_IMGSENSOR_PROFILE_INIT(void); 
void KD_IMGSENSOR_PROFILE(char *tag); 

#define mDELAY(ms)     mdelay(ms) 
#define uDELAY(us)       udelay(us) 
#endif //_KD_IMGSENSOR_H


