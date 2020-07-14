/*!
 * @file readData.ino
 * @brief 读取温度传感器的压差和温度值
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-14
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */
#include <DFRobot_LWLP.h>

/*! 
 * @brief Construct the function
 * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
 * @param address Chip IIC address, addresses 0x0 
 */
DFRobot_LWLP lwlp;


void setup() {

  Serial.begin(9600);
  //芯片初始化
  while (lwlp.begin() != 0) {
  Serial.println("Failed to initialize the chip, please confirm the chip connection");
  delay(1000);
  }
}
void loop(void){
  DFRobot_LWLP::sLwlp_t data;
  //获取单次的测量数据
  data = lwlp.getData();
  //获取经过滤波处理后的数据
  //data = lwlp.getfilterData();
  //获取温度，单位为摄氏度
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" C");
  Serial.print("Differential Pressure: ");
  //获取压差，单位为pa
  Serial.print(data.presure);
  Serial.println(" pa");
  delay(500);

}