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
 * @file      raspberrypi4b_driver_ld3320_interface.c
 * @brief     raspberrypi4b driver ld3320 interface source file
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

#include "driver_ld3320_interface.h"
#include "spi.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief spi device name definition
 */
#define SPI_DEVICE_NAME "/dev/spidev0.0"    /**< spi device name */
/**
 * @brief spi device handle definition
 */
static int gs_fd;                           /**< spi handle */
static FILE *gs_fp = NULL;                  /**< fp handle */

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t ld3320_interface_spi_init(void)
{
    return spi_init(SPI_DEVICE_NAME, &gs_fd, SPI_MODE_TYPE_2, 1000 * 1000 * 2);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t ld3320_interface_spi_deinit(void)
{
    return spi_deinit(gs_fd);
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ld3320_interface_spi_read_address16(uint16_t reg, uint8_t *buf, uint16_t len)
{
    return spi_read_address16(gs_fd, reg, buf, len);
}

/**
 * @brief     interface spi bus write
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ld3320_interface_spi_write_cmd(uint8_t *buf, uint16_t len)
{
    return spi_write_cmd(gs_fd, buf, len);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void ld3320_interface_delay_ms(uint32_t ms)
{
    usleep(1000 * ms);
}

/**
 * @brief     interface delay us
 * @param[in] us
 * @note      none
 */
void ld3320_interface_delay_us(uint32_t us)
{
    usleep(us);
}

/**
 * @brief     interface mp3 init
 * @param[in] *name points to a name buffer
 * @param[in] *size points to a file size buffer
 * @return    status code
 *            - 0 success
 *            - 1 mp3 init failed
 * @note      none
 */
uint8_t ld3320_interface_mp3_init(char *name, uint32_t *size)
{
    gs_fp = fopen(name, "r");
    if (gs_fp == NULL)
    {
        return 1;
    }
    if (fseek(gs_fp, 0, SEEK_END) != 0)
    {
        return 1;
    }
    *size = ftell(gs_fp);
    
    return 0;
}

/**
 * @brief      interface mp3 read
 * @param[in]  addr is the mp3 file address
 * @param[in]  size is the read size
 * @param[out] *buffer points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 mp3 read failed
 * @note       none
 */
uint8_t ld3320_interface_mp3_read(uint32_t addr, uint16_t size, uint8_t *buffer)
{
    if (fseek(gs_fp, addr, SEEK_SET) != 0)
    {
        return 1;
    }
    if (fread((void *)buffer, 1, size, gs_fp) < 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief  interface mp3 deinit
 * @return status code
 *         - 0 success
 *         - 1 mp3 deinit failed
 * @note   none
 */
uint8_t ld3320_interface_mp3_deinit(void)
{
    if (fclose(gs_fp) != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] ind is the index
 * @param[in] *text points to a text buffer
 * @note      none
 */
void ld3320_interface_receive_callback(uint8_t type, uint8_t ind, char *text)
{
    if (type == LD3320_STATUS_ASR_FOUND_OK)
    {
        ld3320_interface_debug_print("ld3320: irq index %d %s.\n", ind, text);
    }
    else if (type == LD3320_STATUS_ASR_FOUND_ZERO)
    {
        ld3320_interface_debug_print("ld3320: irq zero.\n");
    }
    else if (type == LD3320_STATUS_MP3_LOAD)
    {
        ld3320_interface_debug_print("ld3320: irq mp3 load.\n");
    }
    else if (type == LD3320_STATUS_MP3_END)
    {
        ld3320_interface_debug_print("ld3320: irq mp3 end.\n");
    }
    else if (type == LD3320_STATUS_MP3_LOAD)
    {
        ld3320_interface_debug_print("ld3320: irq mp3 load.\n");
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknown type.\n");
    }
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ld3320_interface_reset_gpio_init(void)
{
    return wire_clock_init();
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ld3320_interface_reset_gpio_deinit(void)
{
    return wire_clock_deinit();
}

/**
 * @brief     interface reset gpio write
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ld3320_interface_reset_gpio_write(uint8_t data)
{
    return wire_clock_write(data);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void ld3320_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)printf((uint8_t *)str, len);
}
