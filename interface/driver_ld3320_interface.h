/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
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
 * @file      driver_ld3320_interface.h
 * @brief     driver ld3320 interface header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-05-12
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/05/12  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_LD3320_INTERFACE_H
#define DRIVER_LD3320_INTERFACE_H

#include "driver_ld3320.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ld3320_interface_driver ld3320 interface driver function
 * @brief    ld3320 interface driver modules
 * @ingroup  ld3320_driver
 * @{
 */

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t ld3320_interface_spi_init(void);

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t ld3320_interface_spi_deinit(void);

/**
 * @brief      interface spi bus read
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ld3320_interface_spi_read_address16(uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief     interface spi bus write
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ld3320_interface_spi_write_cmd(uint8_t *buf, uint16_t len);

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ld3320_interface_reset_gpio_init(void);

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ld3320_interface_reset_gpio_deinit(void);

/**
 * @brief     interface reset gpio write
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ld3320_interface_reset_gpio_write(uint8_t data);

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void ld3320_interface_delay_ms(uint32_t ms);

/**
 * @brief     interface delay us
 * @param[in] us time
 * @note      none
 */
void ld3320_interface_delay_us(uint32_t us);

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void ld3320_interface_debug_print(const char *const fmt, ...);

/**
 * @brief     interface mp3 init
 * @param[in] *name pointer to a name buffer
 * @param[in] *size pointer to a file size buffer
 * @return    status code
 *            - 0 success
 *            - 1 mp3 init failed
 * @note      none
 */
uint8_t ld3320_interface_mp3_init(char *name, uint32_t *size);

/**
 * @brief  interface mp3 deinit
 * @return status code
 *         - 0 success
 *         - 1 mp3 deinit failed
 * @note   none
 */
uint8_t ld3320_interface_mp3_deinit(void);

/**
 * @brief      interface mp3 read
 * @param[in]  addr mp3 file address
 * @param[in]  size read size
 * @param[out] *buffer pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 mp3 read failed
 * @note       none
 */
uint8_t ld3320_interface_mp3_read(uint32_t addr, uint16_t size, uint8_t *buffer);

/**
 * @brief     interface receive callback
 * @param[in] type receive callback type
 * @param[in] ind index
 * @param[in] *text pointer to a text buffer
 * @note      none
 */
void ld3320_interface_receive_callback(uint8_t type, uint8_t ind, char *text);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
