#include <utils/Log.h>
#include <utils/Errors.h>
#include <fcntl.h>
#include <math.h>


//#include "msdk_sensor_exp.h"
#include "camera_custom_sensor.h"
#if 1// seanlin 120925 for default #if (defined(IMX073_MIPI_RAW)||defined(OV5642_RAW))
//#include "camera_calibration_eeprom.h"
#include "camera_calibration_cam_cal.h"
#endif
#include "kd_imgsensor.h"
//#include "image_sensor.h"

//TODO:remove once build system ready
//#include "camera_custom_cfg.h"

//#include "src/lib/inc/MediaLog.h"

//#define LOG_TAG "SENSORLIST"
//MUINT32 MT9P012_getCalData(PGET_SENSOR_CALIBRATION_DATA_STRUCT pGetCalData);


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


//MSDK_SENSOR_INIT_FUNCTION_STRUCT SensorList[MAX_NUM_OF_SUPPORT_SENSOR] =
MSDK_SENSOR_INIT_FUNCTION_STRUCT SensorList[] =
{
#if defined(OV5646_MIPI_YUV)
        YUV_INFO(OV5646MIPI_SENSOR_ID, SENSOR_DRVNAME_OV5646_MIPI_YUV,NULL), 
#endif    
#if defined(OV5645_MIPI_YUV)
		YUV_INFO(OV5645MIPI_SENSOR_ID, SENSOR_DRVNAME_OV5645_MIPI_YUV,NULL), 
#endif
#if defined(GC0329_YUV)
    YUV_INFO(GC0329_SENSOR_ID, SENSOR_DRVNAME_GC0329_YUV, NULL), 
#endif


/*  ADD sensor driver before this line */
    {0,{0},NULL, NULL, NULL}//end of list
};

UINT32 GetSensorInitFuncList(MSDK_SENSOR_INIT_FUNCTION_STRUCT **ppSensorList)
{
    if (NULL == ppSensorList) {
        ALOGE("ERROR: NULL pSensorList\n");
        return MHAL_UNKNOWN_ERROR;
    }
    *ppSensorList = &SensorList[0];
	return MHAL_NO_ERROR;
} // GetSensorInitFuncList()






