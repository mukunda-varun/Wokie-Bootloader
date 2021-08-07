/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - disabling the write protection of the Flash sectors where the 
  *               user loads his binary file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/** @addtogroup STM32F4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"
#include <stdlib.h>
extern status otaStatus;
extern uint8_t *ptr;
 extern uint8_t Received_Data_Flag;
 extern uint8_t* data_ptr;
 extern uint32_t Data_len;
 extern uint8_t Received_Data;
 extern char versionNumber[15];
 extern uint8_t versionStatus;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t key = 0;
pFunction Jump_To_Application;
uint32_t JumpAddress;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };
uint8_t FileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Download a file via serial port
 * @param  None
 * @retval None
 */

void jump_to_User_application(void)
{
	HAL_DeInit();
#if defined(USE_HAL_DRIVER)
    HAL_RCC_DeInit();
#endif /* defined(USE_HAL_DRIVER) */
#if defined(USE_STDPERIPH_DRIVER)
    RCC_DeInit();
#endif /* defined(USE_STDPERIPH_DRIVER) */

    /**
     * Step: Disable systick timer and reset it to default values
     */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

	JumpAddress =*(__IO uint32_t*) (APPLICATION_ADDRESS+4);
	/* Jump to user application */
	Jump_To_Application = (pFunction) JumpAddress;
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
	Jump_To_Application();
	while(1){
	}
}
void SerialDownload(void)
{
	uint8_t Number[10] = "          ";
	int32_t Size = 0;

	//SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");

	Size = Ymodem_Receive(&tab_1024[0]);
	if (Size > 0)
	{
		//SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
		//SerialPutString(FileName);
		//Int2Str(Number, Size);
		//SerialPutString("\n\r Size: ");
		//SerialPutString(Number);
		//SerialPutString(" Bytes\r\n");
		//SerialPutString("-------------------\n");

	}
	else if (Size == -1)
	{
		//SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
	}
	else if (Size == -2)
	{
	//	SerialPutString("\n\n\rVerification failed!\n\r");
	}
	else if (Size == -3)
	{
		//SerialPutString("\r\n\nAborted by user.\n\r");
	}
	else
	{
		//SerialPutString("\n\rFailed to receive the file!\n\r");
	}
}

/**
 * @brief  Upload a file via serial port.
 * @param  None
 * @retval None
 */


/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
	while (1)
	{
		/* Receive key */
		if (otaStatus == pass)  //execute the new program
		{
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
//			HAL_Delay(1000);
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
			jump_to_User_application();
		}
		else if(otaStatus == fail)
		{
			key = GetKey();
			if (key == 0x31)
			{
				/* Download user application in the Flash */
//				ptr = NULL;
				SerialDownload();
				jump_to_User_application();
			}
			else if (key == 0x23) /* execute the new program */
			{
				jump_to_User_application();
			}
		}
	}
}

/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
