//s_add new sensor driver here
//export funtions
UINT32 OV5646_MIPI_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 OV5645_MIPI_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
UINT32 GC0329_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);


//! Add Sensor Init function here
//! Note:
//! 1. Add by the resolution from ""large to small"", due to large sensor
//!    will be possible to be main sensor.
//!    This can avoid I2C error during searching sensor.
//! 2. This file should be the same as mediatek\custom\common\hal\imgsensor\src\sensorlist.cpp
ACDK_KD_SENSOR_INIT_FUNCTION_STRUCT kdSensorList[MAX_NUM_OF_SUPPORT_SENSOR+1] =
{
#if defined(OV5646_MIPI_YUV)
    {OV5646MIPI_SENSOR_ID, SENSOR_DRVNAME_OV5646_MIPI_YUV, OV5646_MIPI_YUV_SensorInit}, 
#endif
#if defined(OV5645_MIPI_YUV)
    {OV5645MIPI_SENSOR_ID, SENSOR_DRVNAME_OV5645_MIPI_YUV, OV5645_MIPI_YUV_SensorInit}, 
#endif
#if defined(GC0329_YUV)
    {GC0329_SENSOR_ID, SENSOR_DRVNAME_GC0329_YUV, GC0329_YUV_SensorInit}, 
#endif

/*  ADD sensor driver before this line */
    {0,{0},NULL}, //end of list
};
//e_add new sensor driver here

