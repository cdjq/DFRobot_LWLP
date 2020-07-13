/*!
 * @file DFRobot_LWLP.h
 * @brief Define the infrastructure of the DFRobot_LWLP class
 * @n This is a library of ...
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-26
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */
 
#ifndef DFROBOT_LWLP_H
#define DFROBOT_LWLP_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>

//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


#define SENSOR_TYPE    LWPX  //传感器型号
#define MAX_PRESURE   600 //量程的最大压力(Pa)
#define MIN_PRESURE    600 //量程的最小压力(Pa)

#define MAX_TEMP    85.0 //量程的最大温度(℃)
#define MIN_TEMP     40.0 //量程的最小温度(℃)



class DFRobot_LWLP
{
public:
  #define ERR_OK             0      //No error
  #define ERR_DATA_BUS      -1      //Data bus error
  #define ERR_IC_VERSION    -2      //Chip version does not match
  typedef struct
  {
    uint8_t  status;/**<状态位>*/
    uint8_t  presureDataH;/**<传感器原始值高字节>*/
    uint8_t  presureDataM;/**<传感器原始值中间字节>*/
    uint8_t  presureDataL;/**<传感器原始值低字节>*/
    uint8_t  tempDataH;/**<温度原始值高字节>*/
    uint8_t  tempDataM;/**<温度原始值中间字节>*/
    uint8_t  tempDataL;/**<温度原始值低字节>*/
    uint16_t  presureData;/**<合并后的原始压力值>*/
    uint16_t  tempData;/**<合并后的温度原始值>*/
    float  presure;/**<换算压力变量>*/
    float  temperature;/**<换算温度变量>*/
    float  presureFilter[5];    /**<滤波存储的压力原始值>*/
    float  presureFilterMem;/**<滤波压力排序中间存储值>*/
    double  presureFilterSum;/**<排序后累加>*/
    float  presureFilterResult;/**<滤波后最终值>*/
    float  temperatureFilter[5];    /**<滤波存储的压力原始值>*/
    float  temperatureFilterMem;/**<滤波温度排序中间存储值>*/
    double  temperatureFilterSum;/**<排序后累加>*/
    float  temperatureFilterResult;/**<滤波后最终值>*/
    float  presureDrift;
  }sLwlp_t ;
  
  /*!
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   * @param address Chip IIC address, two optional addresses 0x01 and 0x00(0x00 in default).
   */
  DFRobot_LWLP(TwoWire *pWire = &Wire, uint8_t address = 0x00);

  /**
   * @brief Initialize the function
   * @return Return 0 indicates a successful initialization, while other values indicates failure and return to error code.
   */
  int begin();
  /**
   * @brief 获取单次测量的传感器数据
   * @return 结构体类型的数据
   */
  sLwlp_t getData(void);
  /**
   * @brief 滤波处理函数,获取滤波处理的传感器数据
   * @return 结构体类型的数据
   */
  sLwlp_t getfilterData(void);
  
private:
  sLwlp_t  lwlp;
  TwoWire *_pWire;
  uint8_t _address;
  void configChip(void);//配置芯片
  /**
   * @brief Transport data to chip
   * @param Data address
   * @param Data length
   */
  void write(const void* pBuf,size_t size);
  /**
   * @brief Write command to sensor chip.
   * @param pBuf  The data contained in the command.
   * @param size  Number of command data
   * @return Return 0 indicates the successful read, other return values suggest unsuccessful read.
   */
  uint8_t readData(void *pBuf,size_t size);
};   
#endif
