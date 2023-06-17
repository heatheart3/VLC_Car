  目前问题：
* 周期不稳定问题
<br/>

明日计划：   
*  思考如何让滑动窗口动态适应周期的微动
*  测量驱动小灯的周期是否合适；测量S3的adc是否修复
*  数据可视化分析周期不稳定带来的窗口对不准问题的严重性
  
可优化：
* I2S引起的错位，进行数据清洗的方式非常粗糙


已解决问题:
* 缓冲区溢出 (已解决，处理速度是够的，不调用串口即可)
* I2S错位问题  （已解决，数据清洗一次即可）
* 为什么会出现在示波器里没有出现的数据？(ESP32ADC对噪声敏感，容易出现误差)

未能解决的问题：
*  据说5.1框架修复了dma adc 的bug，但是尝试过了，I2S依然没有解决次序错乱问题。采样率能不能达到2M没有测试，因为当时尚且没有这么高速的需求。

想不通的疑惑：
* 周期为什么会不稳定
* 取出的数字不能是4000~6000中间的值，比如5000。如果使用5000，会导致read一直报超时error，不知道是bug还是因为conv长度的原因    

PS:莫学长是真的该死   