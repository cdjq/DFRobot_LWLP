# DFRobot_LWLP
这是一款高精度差压传感器（测量两个气管之间的差值），将高性能 MEMS 压力敏感芯片和专用调理芯片封装在双气嘴的管壳内，两个气路结构中压力互为参考，降低环境对输出的影响。采用独有算法实现对传感器进行多阶温度补偿，并以数字 I2C 的形式输出。
注意：为了保准测量精度和传感器的寿命，请尽量在洁净气体中使用。传感器气嘴中不能进水，否则会造成传感器损坏

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

   1.读取温度传感器的压差和温度值<br>
   
## Installation
To use this library, please download the library file first, and paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /*!
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   */
  DFRobot_LWLP(TwoWire *pWire = &Wire);

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

```

## Compatibility

MCU           | Work Well    | Work Wrong   | Untested    | Remarks
--------------| :----------: | :----------: | :---------: | -----
Arduino Uno   |      √       |              |             | 
Mega2560      |      √       |              |             | 
Leonardo      |      √       |              |             | 
ESP32         |      √       |              |             | 
micro:bit     |      √       |              |             | 


## History

- data 2019-8-25
- version V0.1


## Credits

Written by fengli(li.feng@dfrobot.com), 2019.8.25 (Welcome to our [website](https://www.dfrobot.com/))





