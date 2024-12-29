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
 * @file      driver_ld3320_mp3_test.c
 * @brief     driver ld3320 mp3 test source file
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

#include "driver_ld3320_mp3_test.h"

static ld3320_handle_t gs_handle;        /**< ld3320 handle */
static volatile uint8_t gs_flag;         /**< global flag */

/**
 * @brief  mp3 test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ld3320_mp3_test_irq_handler(void)
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
 * @brief     mp3 callback
 * @param[in] type irq type
 * @param[in] i asr index
 * @param[in] *text pointer to a asr result buffer
 * @note      none
 */
static void a_callback(uint8_t type, uint8_t i, char *text)
{
    if (type == LD3320_STATUS_MP3_LOAD)
    {
        /* do nothing */
    }
    else if (type == LD3320_STATUS_MP3_END)
    {
        /* flag end */
        gs_flag = 1;
        ld3320_interface_debug_print("ld3320: irq mp3 end.\n");
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknown type.\n");
    }
}

/**
 * @brief     mp3 test
 * @param[in] *name pointer to a name buffer
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ld3320_mp3_test(char *name)
{
    uint8_t res;
    uint32_t timeout;
    ld3320_info_t info;
    
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
    DRIVER_LD3320_LINK_DEBUG_RECEIVE_CALLBACK(&gs_handle, a_callback);
    DRIVER_LD3320_LINK_MP3_READ_INT(&gs_handle, ld3320_interface_mp3_init);
    DRIVER_LD3320_LINK_MP3_READ_DEINT(&gs_handle, ld3320_interface_mp3_deinit);
    DRIVER_LD3320_LINK_MP3_READ(&gs_handle, ld3320_interface_mp3_read);
    
    /* get information */
    res = ld3320_info(&info);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        ld3320_interface_debug_print("ld3320: chip is %s.\n", info.chip_name);
        ld3320_interface_debug_print("ld3320: manufacturer is %s.\n", info.manufacturer_name);
        ld3320_interface_debug_print("ld3320: interface is %s.\n", info.interface);
        ld3320_interface_debug_print("ld3320: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ld3320_interface_debug_print("ld3320: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ld3320_interface_debug_print("ld3320: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ld3320_interface_debug_print("ld3320: max current is %0.2fmA.\n", info.max_current_ma);
        ld3320_interface_debug_print("ld3320: max temperature is %0.1fC.\n", info.temperature_max);
        ld3320_interface_debug_print("ld3320: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ld3320_interface_debug_print("ld3320: start mp3 test.\n");
    
    /* init */
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
    ld3320_interface_debug_print("ld3320: play %s.\n", name);
    res = ld3320_configure_mp3(&gs_handle, name);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: configure mp3 failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start */
    res = ld3320_start(&gs_handle);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: start failed.\n");
        (void)ld3320_deinit(&gs_handle);
    }
    
    /* set speaker volume */
    res = ld3320_set_speaker_volume(&gs_handle, 1);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set speaker volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
    }
    
    /* set headset volume */
    res = ld3320_set_headset_volume(&gs_handle, 1, 1);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set headset volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
    }
    
    timeout = 1000 * 60 * 10;
    while (timeout != 0)
    {
        if (gs_flag != 0)
        {
            break;
        }
        timeout--;
        ld3320_interface_delay_ms(1);
    }
    if (timeout == 0)
    {
        ld3320_interface_debug_print("ld3320: wait timeout.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish mp3 test */
    ld3320_interface_debug_print("ld3320: finish mp3 test.\n");
    (void)ld3320_deinit(&gs_handle);
    
    return 0;
}
