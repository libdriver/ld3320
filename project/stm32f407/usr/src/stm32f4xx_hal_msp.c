/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      stm32f4xx_hal_msp.c
 * @brief     stm32f4xx hal msp source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-2-12
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/12  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "sdio.h"
#include "stm32f4xx_hal.h"

/** 
 * @defgroup msp HAL_MSP_Private_Functions
 * @{
 */

/**
 * @brief  Initializes the Global MSP.
 * @note   none
 */
void HAL_MspInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
            modified by the user
   */ 
}

/**
 * @brief  DeInitializes the Global MSP.
 * @note   none
 */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
            modified by the user
   */
}

/**
 * @brief     uart hal layer init
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;
  
    if (huart->Instance == USART1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
    
        /**USART1 GPIO Configuration
         *PA9  ------> USART1_TX
         *PA10 ------> USART1_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART2_CLK_ENABLE();
    
        /**USART1 GPIO Configuration
         *PA2     ------> USART2_TX
         *PA3     ------> USART2_RX 
         */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     uart hal layer deinit
 * @param[in] *huart points to a uart handle
 * @note      none
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_DISABLE();
    
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    if (huart->Instance == USART2)
    {
        __HAL_RCC_USART2_CLK_DISABLE();
    
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}

/**
 * @brief     spi hal layer init
 * @param[in] *hspi points to a spi handle
 * @note      none
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
  
    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_SPI1_CLK_ENABLE();
    
        /**SPI1 GPIO Configuration
           PA5     ------> SPI1_SCK
           PA6     ------> SPI1_MISO
           PA7     ------> SPI1_MOSI 
          */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

/**
 * @brief     spi hal layer deinit
 * @param[in] *hspi points to a spi handle
 * @note      none
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_DISABLE();
    
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    }
}

/**
 * @brief     sd hal layer init
 * @param[in] *hsd points to a sd handle
 * @note      none
 */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    DMA_HandleTypeDef Tx_DMA_Handler, Rx_DMA_Handler;
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_SDIO_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /**
     * SDIO GPIO Configuration
     * PC8     ------> SDIO D0
     * PC9     ------> SDIO D1
     * PC10    ------> SDIO D2 
     * PC11    ------> SDIO D3 
     * PC12    ------> SDIO SCK 
     */
    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
    
    /**
     * SDIO GPIO Configuration
     * PD2     ------> SDIO CMD
     */
    GPIO_Initure.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
    
    /* set sd interrupt */
    HAL_NVIC_SetPriority(SDMMC1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
    
    /* set rx dma */
    g_sd_rx_dma_handle.Instance = DMA2_Stream3;
    g_sd_rx_dma_handle.Init.Channel = DMA_CHANNEL_4;
    g_sd_rx_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_sd_rx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_sd_rx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_sd_rx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    g_sd_rx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    g_sd_rx_dma_handle.Init.Mode = DMA_PFCTRL;
    g_sd_rx_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    g_sd_rx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    g_sd_rx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    g_sd_rx_dma_handle.Init.MemBurst = DMA_MBURST_INC4;
    g_sd_rx_dma_handle.Init.PeriphBurst = DMA_PBURST_INC4;

    /* link rx dma */
    __HAL_LINKDMA(hsd, hdmarx, g_sd_rx_dma_handle);
    HAL_DMA_DeInit(&g_sd_rx_dma_handle);
    HAL_DMA_Init(&g_sd_rx_dma_handle);
    
    /* set tx dma */
    g_sd_tx_dma_handle.Instance = DMA2_Stream6;
    g_sd_tx_dma_handle.Init.Channel = DMA_CHANNEL_4;
    g_sd_tx_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_sd_tx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    g_sd_tx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    g_sd_tx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    g_sd_tx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    g_sd_tx_dma_handle.Init.Mode = DMA_PFCTRL;
    g_sd_tx_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    g_sd_tx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    g_sd_tx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    g_sd_tx_dma_handle.Init.MemBurst = DMA_MBURST_INC4;
    g_sd_tx_dma_handle.Init.PeriphBurst = DMA_PBURST_INC4;
    
    /* link tx dma */
    __HAL_LINKDMA(hsd, hdmatx, g_sd_tx_dma_handle);
    HAL_DMA_DeInit(&g_sd_tx_dma_handle);
    HAL_DMA_Init(&g_sd_tx_dma_handle);
  
    /* set sd dma interrupt */
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}

/**
 * @brief     sd hal layer deinit
 * @param[in] *hsd points to a sd handle
 * @note      none
 */
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    __HAL_RCC_SDIO_CLK_DISABLE();
    __HAL_RCC_DMA2_CLK_DISABLE();
    
    /* deinit gpio */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
    
    /* disable dma */
    HAL_DMA_DeInit(&g_sd_tx_dma_handle);
    HAL_DMA_DeInit(&g_sd_rx_dma_handle);
    
    /* disable sd and sd dma*/
    HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream6_IRQn);
}

/**
 * @}
 */
