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
 * @file      driver_ld3320_mp3.c
 * @brief     driver ld3320 mp3 source file
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

#include "driver_ld3320_mp3.h"

static ld3320_handle_t gs_handle;        /**< ld3320 handle */

/**
 * @brief  mp3 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ld3320_mp3_irq_handler(void)
{
    if (ld3320_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     mp3 example init
 * @param[in] *name points to a mp3 file name buffer
 * @param[in] *receive_callback points to a receive callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ld3320_mp3_init(char *name, void (*receive_callback)(uint8_t type, uint8_t index, char *text))
{
    uint8_t res;
    
    /* link driver */
    DRIVER_LD3320_LINK_INIT(&gs_handle, ld3320_handle_t);
    DRIVER_LD3320_LINK_SPI_INIT(&gs_handle, ld3320_interface_spi_init);
    DRIVER_LD3320_LINK_SPI_DEINIT(&gs_handle, ld3320_interface_spi_deinit);
    DRIVER_LD3320_LINK_SPI_READ_ADDRESS16(&gs_handle, ld3320_interface_spi_read_address16);
    DRIVER_LD3320_LINK_SPI_WRITE_COMMAND(&gs_handle, ld3320_interface_spi_write_cmd);
    DRIVER_LD3320_LINK_RESET_GPIO_INIT(&gs_handle, ld3320_interface_reset_gpio_init);
    DRIVER_LD3320_LINK_RESET_GPIO_DEINIT(&gs_handle, ld3320_interface_reset_gpio_deinit);
    DRIVER_LD3320_LINK_RESET_GPIO_WRITE(&gs_handle, ld3320_interface_reset_gpio_write);
    DRIVER_LD3320_LINK_DELAY_MS(&gs_handle, ld3320_interface_delay_ms);
    DRIVER_LD3320_LINK_DELAY_US(&gs_handle, ld3320_interface_delay_us);
    DRIVER_LD3320_LINK_DEBUG_PRINT(&gs_handle, ld3320_interface_debug_print);
    DRIVER_LD3320_LINK_DEBUG_RECEIVE_CALLBACK(&gs_handle, receive_callback);
    DRIVER_LD3320_LINK_MP3_READ_INT(&gs_handle, ld3320_interface_mp3_init);
    DRIVER_LD3320_LINK_MP3_READ_DEINT(&gs_handle, ld3320_interface_mp3_deinit);
    DRIVER_LD3320_LINK_MP3_READ(&gs_handle, ld3320_interface_mp3_read);
    
    /* init chip */
    res = ld3320_init(&gs_handle);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: init failed.\n");
        
        return 1;
    }
    
    /* set mp3 mode */
    res = ld3320_set_mode(&gs_handle, LD3320_MODE_MP3);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set mode failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    
    /* configure the mp3 */
    res = ld3320_configure_mp3(&gs_handle, name);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: configure mp3 failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  mp3 example start
 * @return status code
 *         - 0 success
 *         - 1 start failed
 * @note   none
 */
uint8_t ld3320_mp3_start(void)
{
    uint8_t res;
    
    /* start */
    res = ld3320_start(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* set speaker volume */
    res = ld3320_set_speaker_volume(&gs_handle, LD3320_MP3_DEFAULT_SPEAKER_VOLUME);
    if (res != 0)
    {
        return 1;
    }
    
    /* set headset volume */
    res = ld3320_set_headset_volume(&gs_handle, LD3320_MP3_DEFAULT_SPEAKER_VOLUME, LD3320_MP3_DEFAULT_SPEAKER_VOLUME);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  mp3 example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t ld3320_mp3_stop(void)
{
    uint8_t res;
    
    /* stop */
    res = ld3320_stop(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  mp3 example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ld3320_mp3_deinit(void)
{
    uint8_t res;
    
    /* mp3 deinit */
    res = ld3320_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
