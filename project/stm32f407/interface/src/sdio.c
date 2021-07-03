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
 * @file      sdio.c
 * @brief     sdio source file
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

#include "delay.h" 
#include "sdio.h"

/**
 * @brief sdio var definition
 */
uint8_t                 g_sd_tx_done;        /**< sd tx done */
uint8_t                 g_sd_rx_done;        /**< sd rx done */
SD_HandleTypeDef        g_sd_handle;         /**< sd handle */
HAL_SD_CardInfoTypeDef  g_card_info;         /**< card information */
DMA_HandleTypeDef g_sd_tx_dma_handle;        /**< sd tx dma handle */
DMA_HandleTypeDef g_sd_rx_dma_handle;        /**< sd rx dma handle */

/**
 * @brief  sdio bus init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 *         - 2 get card information failed
 *         - 3 set 4 wire bus failed
 * @note   none
 */
uint8_t sdio_init(void)
{
    HAL_StatusTypeDef res;
    
    g_sd_handle.Instance = SDIO;
    g_sd_handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    g_sd_handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    g_sd_handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    g_sd_handle.Init.BusWide = SDIO_BUS_WIDE_1B;
    g_sd_handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    g_sd_handle.Init.ClockDiv = SDIO_TRANSFER_CLK_DIV;
    
    res = HAL_SD_Init(&g_sd_handle);
    if(res != HAL_OK)
    {
        return 1;
    }
    
    res = HAL_SD_GetCardInfo(&g_sd_handle, &g_card_info);
    if(res != HAL_OK)
    {
        return 2;
    }
    
    res = HAL_SD_ConfigWideBusOperation(&g_sd_handle, SDIO_BUS_WIDE_4B);
    if(res != HAL_OK)
    {
        return 3;
    }
    
    return 0;
}

/**
 * @brief  sdio bus deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sdio_deinit(void)
{
    if (HAL_SD_DeInit(&g_sd_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      sdio read
 * @param[in]  sector is the read first sector
 * @param[out] *buf points to a data buffer
 * @param[in]  cnt is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 read timeout
 * @note       none
 */
uint8_t sdio_read(uint32_t sector, uint8_t *buf, uint32_t cnt)
{
    HAL_StatusTypeDef res;
    uint32_t timeout = 1000;
    
    g_sd_rx_done = 0;
    res = HAL_SD_ReadBlocks_DMA(&g_sd_handle, (uint8_t*)buf, sector, cnt);
    if (res == HAL_OK)
    {
        while (1)
        {
            if (g_sd_rx_done)
            {
                break;
            }
            else
            {
                timeout--;
                if (timeout == 0)
                {
                    return 2;
                }
            }
            delay_ms(1);
        }
        while (HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER)
        {
            
        }
        
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief     sdio write
 * @param[in] sector is the written first sector
 * @param[in] *buf points to a data buffer
 * @param[in] cnt is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 write timeout
 * @note      none
 */
uint8_t sdio_write(uint32_t sector, uint8_t *buf, uint32_t cnt)
{
    HAL_StatusTypeDef res;
    uint32_t timeout = 1000;
    
    g_sd_tx_done = 0;
    res = HAL_SD_WriteBlocks_DMA(&g_sd_handle, (uint8_t*)buf, sector, cnt);
    if (res == HAL_OK)
    {
        while (1)
        {
            if (g_sd_tx_done)
            {
                break;
            }
            else
            {
                timeout--;
                if (timeout == 0)
                {
                    return 2;
                }
            }
            delay_ms(1); 
        }
        while (HAL_SD_GetCardState(&g_sd_handle) != HAL_SD_CARD_TRANSFER)
        {
            
        }
        return 0;
    }
    else
    {
        return 1;
    }
}
