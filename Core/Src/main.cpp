/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32746g_discovery_qspi.h"
#include <stdio.h>
#include "QSPI_DISCO_F746NG.h"
#include "map.h"
#include "nand.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USE_WRITE_TEST 0
#define USE_READ_TEST 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN PV */
extern "C"
{
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
/* USER CODE BEGIN PFP */


int __io_putchar(int ch)
{
	ITM_SendChar(ch);
	return ch;
}

}

char buffer[128] = "QSPI test with FAT_FS";
char ReadBuffer[128];



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
QSPI_DISCO_F746NG qspi;
FRESULT Status;
UINT control;

#define GC_RATIO		4


extern struct dhara_nand sim_nand;


char write_buf[512] = "Hello world!!!";
char read_buf[512];

size_t page_size = 1 << sim_nand.log2_page_size;
uint8_t page_buf[512];
struct dhara_map map;
dhara_error_t err;

uint8_t test[4096];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	memset(test, 0x55, sizeof(test));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  MX_USB_DEVICE_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */



  uint8_t init_return = qspi.Init();

#ifdef DEBUG
  printf("Init return value: %d\n\r", init_return);
#endif // DEBUG



  if (init_return != QSPI_OK)
  {
#ifdef DEBUG
	  printf("QSPI Initialization FAILED\n\r");
#endif // DEBUG

  }
  else
  {
#ifdef DEBUG
	  printf("QSPI Initialization PASSED\n\r");
#endif // DEBUG

  }

//  for (uint16_t i = 0; i < 4096; i++)
//  {
//	  qspi.WriteBlocks(test, i, 1);
//  }

  printf("Map init\n");
	dhara_map_init(&map, &sim_nand, page_buf, GC_RATIO);
	dhara_map_resume(&map, NULL);
	printf("  capacity: %lu\n", dhara_map_capacity(&map));


    for (int i = 0; i < 10; i++)
    {
        if (dhara_map_write(&map, 0, (const uint8_t *) write_buf, &err) < 0)
            printf("Write aborted!\r\n");
//
//        HAL_Delay(1);

        if (dhara_map_read(&map, 0, (uint8_t *) read_buf, &err) < 0)
            printf("Read aborted!\r\n");

        printf("Result: %s\r\n", read_buf);
    }

//  	for (uint8_t i = 0; i < 3; i++)
//  	{
//		if ((Status = f_mount(&USERFatFS, "0", 1)) == FR_OK)
//		{
//		  printf("Mounted!\r\n");
//
//#if USE_WRITE_TEST != 0
//		  if (f_open(&USERFile, "Test.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
//		  {
//			  printf("File opened!\r\n");
//			  if (f_write(&USERFile, buffer, sizeof(buffer), &control) == FR_OK)
//			  {
//				  printf("Write success!\r\n");
//			  }
//			  else
//			  {
//				  printf("Write failed!\r\n");
//			  }
//
//			  if (f_close(&USERFile) == FR_OK)
//			  {
//				  printf("File was closed!\r\n");
//			  }
//			  else
//			  {
//				  printf("File was not closed!\r\n");
//			  }
//		  }
//		  else
//		  {
//			  printf("File was not opened!\r\n");
//		  }
//#endif
//
//
//#if USE_READ_TEST != 0
//		  if (f_open(&USERFile, "Test.txt", FA_READ) == FR_OK)
//		  {
//			  printf("File opened!\r\n");
//
//
//			  if (f_read(&USERFile, ReadBuffer, sizeof(buffer), &control) == FR_OK)
//			  {
//				  printf("Read success!\r\n");
//			  }
//			  else
//			  {
//				  printf("Read failed!\r\n");
//			  }
//
//			  if (f_close(&USERFile) == FR_OK)
//			  {
//				  printf("File was closed!\r\n");
//			  }
//			  else
//			  {
//				  printf("File was not closed!\r\n");
//			  }
//		  }
//		  else
//		  {
//			  printf("File was not opened!\r\n");
//		  }
//#endif
//
//		  break;
//		}
//		else
//		{
//		  uint8_t buf[4096];
//		  Status = f_mkfs("0", 1, 0, buf, sizeof(buf));
//		  printf("Not mounted!\r\n");
//		}
//  	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 3;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 23;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

