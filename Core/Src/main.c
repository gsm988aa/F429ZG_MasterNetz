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
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "loopback.h"
//
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart4 , (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */








#define SOCK_TCPS        0
#define SOCK_UDPS        1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05},
                            .ip = {192, 168, 1, 88},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 1, 1},
                            .dns = {8, 8, 8, 8},
                            .dhcp = NETINFO_STATIC };

uint8_t gDATABUF[2048];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void W5500_Select(void);
//void W5500_Unselect(void);
//void W5500_ReadBuff(uint8_t* buff, uint16_t len);
//void W5500_WriteBuff(uint8_t* buff, uint16_t len);
//uint8_t W5500_ReadByte(void);
//void W5500_WriteByte(uint8_t byte);
//void network_init(void);
//
//
//
//
//void W5500_WriteByte(uint8_t byte);
//void network_init(void);

uint8_t W5500_ReadByte(void) {
  uint8_t byte;
  W5500_ReadBuff(&byte, sizeof(byte));
  return byte;
}

void W5500_WriteByte(uint8_t byte) {
  W5500_WriteBuff(&byte, sizeof(byte));
}

void W5500_Select(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //CS LOW
}

void W5500_Unselect(void) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
  HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
  HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}
//uint8_t spi_rb(void) {
//	uint8_t rbuf;
//	HAL_SPI_Receive(&hspi1, &rbuf, 1, 0xFFFFFFFF);
//	return rbuf;
//}
//
//void spi_wb(uint8_t b) {
//	HAL_SPI_Transmit(&hspi1, &b, 1, 0xFFFFFFFF);
//}
//uint8_t W5500_ReadByte(void) {
//  uint8_t byte;
//  W5500_ReadBuff(&byte, sizeof(byte));
//  return byte;
//}
//
//void W5500_WriteByte(uint8_t byte) {
//  W5500_WriteBuff(&byte, sizeof(byte));
//}

void network_init(void)
{
  uint8_t tmpstr[6];
  ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
  ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

  // Display Network Information
  ctlwizchip(CW_GET_ID,(void*)tmpstr);
  printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
  printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
  printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
  printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
  printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
  printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
  printf("======================\r\n");
}




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t tmp;
  int32_t retr = 0;
  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};




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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  MX_SPI6_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);



/* WIZCHIP SOCKET Buffer initialize */
if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
{
   printf("WIZCHIP Initialized fail.\r\n");
   while(1);
}

/* PHY link status check */
do
{
   if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
      printf("Unknown PHY Link stauts.\r\n");
}while(tmp == PHY_LINK_OFF);
	printf("SOCKET ERROR =1");
network_init();

  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//      printf("enter while\r\n");


//      HAL_Delay(500);
     
//    DHT_data d = DHT_getData(DHT22);
//     printf("Temp: %2.1f \r\n", d.temp );
 
		if( (retr = loopback_tcps(SOCK_TCPS, gDATABUF, 5000)) < 0) {
    printf("SOCKET ERROR : %ld\r\n", retr);
  }

		if( (retr = loopback_udps(SOCK_UDPS, gDATABUF, 3000)) < 0) {
			printf("SOCKET ERROR : %ld\r\n", retr);
		}

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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
