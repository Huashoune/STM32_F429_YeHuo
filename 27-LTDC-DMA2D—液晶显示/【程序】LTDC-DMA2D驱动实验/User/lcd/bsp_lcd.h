#ifndef __LCD_H
#define	__LCD_H

#include "stm32f4xx.h"
#include <stdio.h>



/**
  * @}
  */  
  
  

#define LCD_WIDTH   800
#define LCD_HEIGHT  480  
  
//第一层显存首地址，SDRAM的首地址 
#define LCD_LAYER1_START_ADDR   0xD0000000
#define LCD_LAYER1_BUFFER_SIZE  LCD_WIDTH * LCD_HEIGHT * 2

//第二层显存首地址
#define LCD_LAYER2_START_ADDR   (LCD_LAYER1_START_ADDR + LCD_LAYER1_BUFFER_SIZE)  
#define LCD_LAYER2_BUFFER_SIZE  LCD_WIDTH * LCD_HEIGHT * 4

  

/*部分液晶信号线的引脚复用编号是AF9*/
#define GPIO_AF_LTDC_AF9          ((uint8_t)0x09)  


/**
  * @}
  */ 
//红色数据线
#define LTDC_R0_GPIO_PORT        	GPIOH
#define LTDC_R0_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define LTDC_R0_GPIO_PIN         	GPIO_Pin_2
#define LTDC_R0_PINSOURCE        	GPIO_PinSource2
#define LTDC_R0_AF			          GPIO_AF_LTDC

#define LTDC_R1_GPIO_PORT        	GPIOH
#define LTDC_R1_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define LTDC_R1_GPIO_PIN         	GPIO_Pin_3
#define LTDC_R1_PINSOURCE        	GPIO_PinSource3
#define LTDC_R1_AF			          GPIO_AF_LTDC

#define LTDC_R2_GPIO_PORT        	GPIOH
#define LTDC_R2_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define LTDC_R2_GPIO_PIN         	GPIO_Pin_8
#define LTDC_R2_PINSOURCE        	GPIO_PinSource8
#define LTDC_R2_AF			          GPIO_AF_LTDC

#define LTDC_R3_GPIO_PORT        	GPIOB
#define LTDC_R3_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define LTDC_R3_GPIO_PIN         	GPIO_Pin_0
#define LTDC_R3_PINSOURCE        	GPIO_PinSource0
#define LTDC_R3_AF			          GPIO_AF_LTDC_AF9

#define LTDC_R4_GPIO_PORT        	GPIOA
#define LTDC_R4_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define LTDC_R4_GPIO_PIN         	GPIO_Pin_11
#define LTDC_R4_PINSOURCE        	GPIO_PinSource11
#define LTDC_R4_AF			          GPIO_AF_LTDC

#define LTDC_R5_GPIO_PORT        	GPIOA
#define LTDC_R5_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define LTDC_R5_GPIO_PIN         	GPIO_Pin_12
#define LTDC_R5_PINSOURCE        	GPIO_PinSource12
#define LTDC_R5_AF			          GPIO_AF_LTDC

#define LTDC_R6_GPIO_PORT        	GPIOB
#define LTDC_R6_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define LTDC_R6_GPIO_PIN         	GPIO_Pin_1
#define LTDC_R6_PINSOURCE        	GPIO_PinSource1
#define LTDC_R6_AF			          GPIO_AF_LTDC_AF9

#define LTDC_R7_GPIO_PORT        	GPIOG
#define LTDC_R7_GPIO_CLK         	RCC_AHB1Periph_GPIOG
#define LTDC_R7_GPIO_PIN         	GPIO_Pin_6
#define LTDC_R7_PINSOURCE        	GPIO_PinSource6
#define LTDC_R7_AF			          GPIO_AF_LTDC
//绿色数据线
#define LTDC_G0_GPIO_PORT        	GPIOE
#define LTDC_G0_GPIO_CLK         	RCC_AHB1Periph_GPIOE
#define LTDC_G0_GPIO_PIN         	GPIO_Pin_5
#define LTDC_G0_PINSOURCE        	GPIO_PinSource5
#define LTDC_G0_AF			          GPIO_AF_LTDC

#define LTDC_G1_GPIO_PORT        	GPIOE
#define LTDC_G1_GPIO_CLK         	RCC_AHB1Periph_GPIOE
#define LTDC_G1_GPIO_PIN         	GPIO_Pin_6
#define LTDC_G1_PINSOURCE        	GPIO_PinSource6
#define LTDC_G1_AF			          GPIO_AF_LTDC

#define LTDC_G2_GPIO_PORT        	GPIOH
#define LTDC_G2_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define LTDC_G2_GPIO_PIN         	GPIO_Pin_13
#define LTDC_G2_PINSOURCE        	GPIO_PinSource13
#define LTDC_G2_AF			          GPIO_AF_LTDC

#define LTDC_G3_GPIO_PORT        	GPIOG
#define LTDC_G3_GPIO_CLK         	RCC_AHB1Periph_GPIOG
#define LTDC_G3_GPIO_PIN         	GPIO_Pin_10
#define LTDC_G3_PINSOURCE        	GPIO_PinSource10
#define LTDC_G3_AF			          GPIO_AF_LTDC_AF9

#define LTDC_G4_GPIO_PORT        	GPIOH
#define LTDC_G4_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define LTDC_G4_GPIO_PIN         	GPIO_Pin_15
#define LTDC_G4_PINSOURCE        	GPIO_PinSource15
#define LTDC_G4_AF			          GPIO_AF_LTDC

#define LTDC_G5_GPIO_PORT        	GPIOI
#define LTDC_G5_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define LTDC_G5_GPIO_PIN         	GPIO_Pin_0
#define LTDC_G5_PINSOURCE        	GPIO_PinSource0
#define LTDC_G5_AF			          GPIO_AF_LTDC

#define LTDC_G6_GPIO_PORT        	GPIOC
#define LTDC_G6_GPIO_CLK         	RCC_AHB1Periph_GPIOC
#define LTDC_G6_GPIO_PIN         	GPIO_Pin_7
#define LTDC_G6_PINSOURCE        	GPIO_PinSource7
#define LTDC_G6_AF			          GPIO_AF_LTDC

#define LTDC_G7_GPIO_PORT        	GPIOI
#define LTDC_G7_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define LTDC_G7_GPIO_PIN         	GPIO_Pin_2
#define LTDC_G7_PINSOURCE        	GPIO_PinSource2
#define LTDC_G7_AF			          GPIO_AF_LTDC

//蓝色数据线
#define LTDC_B0_GPIO_PORT        	GPIOE
#define LTDC_B0_GPIO_CLK         	RCC_AHB1Periph_GPIOE
#define LTDC_B0_GPIO_PIN         	GPIO_Pin_4
#define LTDC_B0_PINSOURCE        	GPIO_PinSource4
#define LTDC_B0_AF			          GPIO_AF_LTDC

#define LTDC_B1_GPIO_PORT        	GPIOG
#define LTDC_B1_GPIO_CLK         	RCC_AHB1Periph_GPIOG
#define LTDC_B1_GPIO_PIN         	GPIO_Pin_12
#define LTDC_B1_PINSOURCE        	GPIO_PinSource12
#define LTDC_B1_AF			          GPIO_AF_LTDC

#define LTDC_B2_GPIO_PORT        	GPIOD
#define LTDC_B2_GPIO_CLK         	RCC_AHB1Periph_GPIOD
#define LTDC_B2_GPIO_PIN         	GPIO_Pin_6
#define LTDC_B2_PINSOURCE        	GPIO_PinSource6
#define LTDC_B2_AF			          GPIO_AF_LTDC

#define LTDC_B3_GPIO_PORT        	GPIOG
#define LTDC_B3_GPIO_CLK         	RCC_AHB1Periph_GPIOG
#define LTDC_B3_GPIO_PIN         	GPIO_Pin_11
#define LTDC_B3_PINSOURCE        	GPIO_PinSource11
#define LTDC_B3_AF			          GPIO_AF_LTDC

#define LTDC_B4_GPIO_PORT        	GPIOI
#define LTDC_B4_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define LTDC_B4_GPIO_PIN         	GPIO_Pin_4
#define LTDC_B4_PINSOURCE        	GPIO_PinSource4
#define LTDC_B4_AF			          GPIO_AF_LTDC

#define LTDC_B5_GPIO_PORT        	GPIOA
#define LTDC_B5_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define LTDC_B5_GPIO_PIN         	GPIO_Pin_3
#define LTDC_B5_PINSOURCE        	GPIO_PinSource3
#define LTDC_B5_AF			          GPIO_AF_LTDC

#define LTDC_B6_GPIO_PORT        	GPIOB
#define LTDC_B6_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define LTDC_B6_GPIO_PIN         	GPIO_Pin_8
#define LTDC_B6_PINSOURCE        	GPIO_PinSource8
#define LTDC_B6_AF			          GPIO_AF_LTDC

#define LTDC_B7_GPIO_PORT        	GPIOB
#define LTDC_B7_GPIO_CLK         	RCC_AHB1Periph_GPIOB
#define LTDC_B7_GPIO_PIN         	GPIO_Pin_9
#define LTDC_B7_PINSOURCE        	GPIO_PinSource9
#define LTDC_B7_AF			          GPIO_AF_LTDC

//控制信号线
/*像素时钟CLK*/
#define LTDC_CLK_GPIO_PORT        GPIOG
#define LTDC_CLK_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define LTDC_CLK_GPIO_PIN         GPIO_Pin_7
#define LTDC_CLK_PINSOURCE        GPIO_PinSource7
#define LTDC_CLK_AF			          GPIO_AF_LTDC
/*水平同步信号HSYNC*/
#define LTDC_HSYNC_GPIO_PORT      GPIOI
#define LTDC_HSYNC_GPIO_CLK       RCC_AHB1Periph_GPIOI
#define LTDC_HSYNC_GPIO_PIN       GPIO_Pin_10
#define LTDC_HSYNC_PINSOURCE      GPIO_PinSource10
#define LTDC_HSYNC_AF			        GPIO_AF_LTDC
/*垂直同步信号VSYNC*/
#define LTDC_VSYNC_GPIO_PORT      GPIOI
#define LTDC_VSYNC_GPIO_CLK       RCC_AHB1Periph_GPIOI
#define LTDC_VSYNC_GPIO_PIN       GPIO_Pin_9
#define LTDC_VSYNC_PINSOURCE      GPIO_PinSource9
#define LTDC_VSYNC_AF			        GPIO_AF_LTDC

/*数据使能信号DE*/
#define LTDC_DE_GPIO_PORT         GPIOF
#define LTDC_DE_GPIO_CLK          RCC_AHB1Periph_GPIOF
#define LTDC_DE_GPIO_PIN          GPIO_Pin_10
#define LTDC_DE_PINSOURCE         GPIO_PinSource10
#define LTDC_DE_AF			          GPIO_AF_LTDC
/*液晶屏使能信号DISP，高电平使能*/
#define LTDC_DISP_GPIO_PORT        GPIOD
#define LTDC_DISP_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define LTDC_DISP_GPIO_PIN         GPIO_Pin_4
/*液晶屏背光信号，高电平使能*/
#define LTDC_BL_GPIO_PORT         GPIOD
#define LTDC_BL_GPIO_CLK          RCC_AHB1Periph_GPIOD
#define LTDC_BL_GPIO_PIN          GPIO_Pin_7


void LCD_Init(void);
void DIS_Line(void);

void DIS_RECT(void);
void DIS_Line_DMA2D(void);


void DIS_RECT_LAYER1(void);

void DIS_Line_Layer1(void);
#endif /* __LCD_H */
