/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c EEPROM(AT24C02)应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_debug_usart.h"



uint16_t EEPROM_ADDRESS;




static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);



static void  IIC_Start(void);
static void  IIC_Stop(void);
static uint8_t IIC_ReadByte(void);
static void IIC_SendByte(uint8_t data);
static uint8_t IIC_Wait_ACK(void);
static void IIC_ACK(void);
static void IIC_NACK(void);


/**
  * @brief  I2C1 I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  
#if  USE_HARD_IIC 
  
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*!< EEPROM_I2C Periph clock enable */
  EEPROM_I2C_CLK_INIT(EEPROM_I2C_CLK, ENABLE);
  
  /*!< EEPROM_I2C_SCL_GPIO_CLK and EEPROM_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< GPIO configuration */
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE, EEPROM_I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE, EEPROM_I2C_SDA_AF);  
  
  /*!< Configure EEPROM_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure EEPROM_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
 
#else

  GPIO_InitTypeDef  GPIO_InitStructure;  
    
  /*!< EEPROM_I2C_SCL_GPIO_CLK and EEPROM_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);


  /*!< Configure EEPROM_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure EEPROM_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;
  GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

#endif
 
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		                    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_OwnAddress1 =I2C_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;	                            /* 通信速率 */
  I2C_Init(EEPROM_I2C, &I2C_InitStructure);	                                      /* I2C1 初始化 */
  I2C_Cmd(EEPROM_I2C, ENABLE);  	                                                /* 使能 I2C1 */

  I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);  
}

/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)
{
  I2C_GPIO_Config(); 
 
#if  USE_HARD_IIC 
  
  I2C_Mode_Configu();
  
#endif

/* 根据头文件i2c_ee.h中的定义来选择EEPROM要写入的地址 */
#ifdef EEPROM_Block0_ADDRESS
  /* 选择 EEPROM Block0 来写入 */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, WriteAddr);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
  /* Send the byte to be written */
  I2C_SendData(EEPROM_I2C, *pBuffer); 
   
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
  
#if   USE_HARD_IIC
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))  
   {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(EEPROM_I2C, WriteAddr);  

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
  } 
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(EEPROM_I2C, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
  
  return 1;

#else


  
  /* Send START condition */
  IIC_Start(); 
 
  /* Send EEPROM address for write */
  IIC_SendByte( EEPROM_ADDRESS | IIC_WRITE_DIRECTION);
  if(IIC_Wait_ACK() != 0)
  {
    goto ack_faild;
  }


  /* Send the EEPROM's internal address to write to */    
  IIC_SendByte( WriteAddr);  

  if(IIC_Wait_ACK() != 0)
  {
    goto ack_faild;
  }
  
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    IIC_SendByte( *pBuffer); 
    
    if(IIC_Wait_ACK() != 0)
    {
      goto ack_faild;
    }

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;
  }

  /* Send STOP condition */
  IIC_Stop();
  
  return 1;
  
ack_faild:
     /* Send STOP condition */
  IIC_Stop();
  
  return 0;
  
#endif  
  
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  
#if   USE_HARD_IIC
  
    I2CTimeout = I2CT_LONG_TIMEOUT;

  //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))   
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
    }
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
 
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(EEPROM_I2C, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, ReadAddr);  

     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(EEPROM_I2C, ENABLE);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    }

		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 	
		{
		  /* Read a byte from the device */
      *pBuffer = I2C_ReceiveData(EEPROM_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;
		}			
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
  
  return 1;
  
#else
  
  

  /* Send START condition */
  IIC_Start();
  //*((u8 *)0x4001080c) &=~0x80;  

  /* Send EEPROM address for write */
  IIC_SendByte( EEPROM_ADDRESS  | IIC_WRITE_DIRECTION);
  
  if(IIC_Wait_ACK() != 0)
  {
    goto ack_faild;
  }


  /* Send the EEPROM's internal address to write to */
  IIC_SendByte(ReadAddr);  
  
  if(IIC_Wait_ACK() != 0)
  {
    goto ack_faild;
  }

  /* Send STRAT condition a second time */  
  IIC_Start();

  /* Send EEPROM address for read */
  IIC_SendByte( EEPROM_ADDRESS | IIC_READ_DIRECTION);
  
  if(IIC_Wait_ACK() != 0)
  {
    goto ack_faild;
  }
  
  

  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      IIC_NACK();
      
      /* Send STOP Condition */
      IIC_Stop();
    }
		{
		  /* Read a byte from the device */
      *pBuffer = IIC_ReadByte();

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;
      
        /* Enable Acknowledgement to be ready for another reception */
      IIC_ACK();
      
		}			
  }


  
  //释放总线
  IIC_Stop();
  
  return 1;


ack_faild:
  
  //释放总线
   IIC_Stop();
  
   return 0;
    
#endif  
  
}

/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  
#if   USE_HARD_IIC
  
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    /* Read EEPROM_I2C SR1 register */
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
  /* STOP condition */    
  I2C_GenerateSTOP(EEPROM_I2C, ENABLE); 
  
#else


  do
  {
    IIC_Stop();
    /* Send START condition */
    IIC_Start();

    /* Send EEPROM address for write */
    IIC_SendByte( EEPROM_ADDRESS  | IIC_WRITE_DIRECTION);
    /* Send EEPROM address for write */
  }while(IIC_Wait_ACK() != 0 );
  

  /* STOP condition */    
  IIC_Stop(); 
  
#endif  
  
}



/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0;
}


void IIC_Delay(void)
{
  uint8_t i ;  
  for(i=0;i<50;i++);  
}


//1.初始化IIC引脚
//2.模拟IIC的起始及停止信号
//3.使用引脚以模拟IIC的方式读取及输出数据



//起始信号
static void  IIC_Start(void)
{
    IIC_SDA_1;
    IIC_SCL_1;
  
    IIC_Delay();
  
    IIC_SDA_0;
    IIC_Delay();
    
    IIC_SCL_0;
    IIC_Delay();

}

//停止信号
static void  IIC_Stop(void)
{
    IIC_SDA_0;
    IIC_SCL_1;
  
    IIC_Delay();
  
    IIC_SDA_1;
    IIC_Delay();
    
    IIC_SCL_1;
    IIC_Delay();

}

//接收一个字节的数据
static uint8_t IIC_ReadByte(void)
{
  uint8_t i;  
  uint8_t value = 0;
  
  for(i=0;i<8;i++)
  {
    value <<= 1 ;    
    
    IIC_SCL_1;
    IIC_Delay();

    if(IIC_READ_SDA())
    {
      value++;
    }
    else
    {
    
    }        

    IIC_SCL_0;      
    IIC_Delay(); 
    

    
  }
  
  return value;
}


//发送一个字节的数据
static void IIC_SendByte(uint8_t data)
{
  uint8_t i;  
  
  for(i=0;i<8;i++)
  {   
    if(data &0x80)
    {
       IIC_SDA_1;
    }
    else
    {
      IIC_SDA_0;
    }   
    IIC_Delay();     
    
    
    IIC_SCL_1;
    IIC_Delay();     
       
    IIC_SCL_0;      
    IIC_Delay(); 
    
    data <<= 1 ;    
    
    //释放总线
    if(i==7)
    {
      IIC_SDA_1;
    }
    
  }  

}


//等待应答信号
//返回值为1时表示非应答，无响应，为0时，表示应答，有响应
static uint8_t IIC_Wait_ACK(void)
{
  uint8_t ack_value;
  
  IIC_SDA_1;
  IIC_Delay(); 
  
  IIC_SCL_1;  
  IIC_Delay(); 

  if(IIC_READ_SDA())
    ack_value = 1;
  else
    ack_value = 0;
  
  IIC_SCL_0;      
  IIC_Delay();  
  
  return ack_value;
}


//发送应答信号
static void IIC_ACK(void)
{
  IIC_SDA_0;
  IIC_Delay();     
  
  IIC_SCL_1;
  IIC_Delay();  
  IIC_SCL_0;
  
  IIC_Delay(); 
  
   //释放总线
  IIC_SDA_1;
}

//发送非应答信号
static void IIC_NACK(void)
{
  IIC_SDA_1;
  IIC_Delay();     
  
  IIC_SCL_1;
  IIC_Delay();  
  IIC_SCL_0;
  
  IIC_Delay(); 

}


/*********************************************END OF FILE**********************/
