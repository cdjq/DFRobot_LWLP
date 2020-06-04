/*!
 * @file DFRobot_LWPS.cpp
 * @brief Define the infrastructure of the DFRobot_LWPS class
 * @n This is a library of ...
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-26
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_LWPS
 */

#include "DFRobot_LWPS.h"

DFRobot_LWPS::DFRobot_LWPS(TwoWire *pWire, uint8_t address)
{
  _address = address;
  _pWire = pWire;
  
}
int DFRobot_LWPS::begin(){

  _pWire->begin();
  _pWire->beginTransmission(_address);
  if(_pWire->endTransmission() != 0){
    DBG("bus data access error");
    return ERR_DATA_BUS;
   }
   getfilterData();
   lwps.presureDrift = lwps.presure;
  return ERR_OK;


}
void DFRobot_LWPS::configChip(void){
  uint8_t config[3]={0xaa,0x00,0x80}; //配置列表
  write(config,3);//发送配置数据
  delay(30);//延时30毫秒等待芯片采集转换数据
}
DFRobot_LWPS::sLwps_t DFRobot_LWPS::getData(void){//接受芯片寄存器数据
  uint8_t data[7];
  configChip();
  uint8_t error = readData(data,7);
  if(error != 7){
    DBG("data request error");
  }

  lwps.status = data[0];//读芯片输出数据状态
  lwps.presureDataH = data[1];//压力最高字节
  lwps.presureDataM = data[2];//压力中间字节
  lwps.presureDataL = data[3];//压力最低字节
  lwps.tempDataH = data[4];//温度最高字节 
  lwps.tempDataM = data[5];//温度中间字节  
  lwps.tempDataL = data[6];//温度最低字节 
  
  /*****************将采集的温度压力值分别合并成三个字节并获取有效数据**************/
  lwps.presureData = (lwps.presureDataH<<8)+(lwps.presureDataM);//合并压力原始值  
  lwps.presureData=lwps.presureData>>2;//移位获得有效位

  lwps.presure=(lwps.presureData/16384.0)*(MAX_PRESURE+MIN_PRESURE)-MIN_PRESURE - lwps.presureDrift;//将ADC值换算成最终压力值;
  lwps.tempData = 0 ;
  lwps.tempData = (lwps.tempDataH<<8)+(lwps.tempDataM);//合并温度原始值
  lwps.temperature=(lwps.tempData/65536.0)*(MAX_TEMP+MIN_TEMP)-MIN_TEMP;//将ADC值换算成最终温度值；
  
  return lwps;
  }
DFRobot_LWPS::sLwps_t DFRobot_LWPS::getfilterData(void){//滤波处理函数
  uint8_t readTime;//读取传感器值计数
  uint8_t i,j,k;
  /**************采集5次传感器数值**********************/
  for(readTime=0;readTime<5;readTime++){;
            getData();
            lwps.presureFilter[readTime]=lwps.presure;
            lwps.temperatureFilter[readTime]=lwps.temperature;

    }   
  /*************************************************************压力滤波***********************************************************/
    /*******************排序******************/
  for(i=0;i<5-1;++i)
  {
      for(j=0;j<5-i-1;++j)
      {
          if(lwps.presureFilter[j] > lwps.presureFilter[j+1]) 
          {
              lwps.presureFilterMem = lwps.presureFilter[j] ; 
              lwps.presureFilter[j] = lwps.presureFilter[j+1] ;  
              lwps.presureFilter[j+1] = lwps.presureFilterMem;  
          }
      }
  }   
       /***************去掉最大值最小值求平均*****************/
  lwps.presureFilterSum=0; 
  for(k=1;k<4;k++)    
  {
      lwps.presureFilterSum=lwps.presureFilterSum+ lwps.presureFilter[k];
  }   
  lwps.presure=lwps.presureFilterSum/3;
      /***********************************************************温度滤波***************************************************************/
      /*******************排序******************/
  for(i=0;i<5-1;++i)
  {
      for(j=0;j<5-i-1;++j)
      {
          if(lwps.temperatureFilter[j] > lwps.temperatureFilter[j+1]) 
          {
              lwps.temperatureFilterMem = lwps.temperatureFilter[j] ; 
              lwps.temperatureFilter[j] =  lwps.temperatureFilter[j+1] ;  
              lwps.temperatureFilter[j+1] =  lwps.temperatureFilterMem;  
          }
      }
  }   
  /***************去掉最大值最小值求平均*****************/
  lwps.temperatureFilterSum=0; 
  for(k=1;k<4;k++)    
  {
      lwps.temperatureFilterSum=lwps.temperatureFilterSum + lwps.temperatureFilter[k];
  }   
    
  lwps.temperature=lwps.temperatureFilterSum/3;

  return lwps;
}


void DFRobot_LWPS::write(const void* pBuf,size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_address);
  for (uint8_t i = 0; i < size; i++) {
    _pWire->write(_pBuf[i]);
    delay(10);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_LWPS::readData(void *pBuf, size_t size) {
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  //读取芯片返回的数据
  _pWire->requestFrom(_address,size);
  uint8_t len = 0;
  for (uint8_t i = 0 ; i < size; i++) {
    _pBuf[i] = _pWire->read();
    len++;
    //Serial.println(_pBuf[i]);
  }
  _pWire->endTransmission();
  return len;
}