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
 * @file      driver_ld3320_register_test.c
 * @brief     driver ld3320 register test source file
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

#include "driver_ld3320_register_test.h"
#include <stdlib.h>

static ld3320_handle_t gs_handle;        /**< ld3320 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ld3320_register_test(void)
{
    uint8_t res;
    uint8_t len;
    uint8_t volume_left, volume_left_check;
    uint8_t volume_right, volume_right_check;
    char text[1][50];
    char text_check[1][50];
    ld3320_info_t info;
    ld3320_mode_t mode;
    ld3320_mic_gain_t gain;
    ld3320_vad_t vad;
    ld3320_status_t status;
    
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
    DRIVER_LD3320_LINK_DEBUG_RECEIVE_CALLBACK(&gs_handle, ld3320_interface_receive_callback);
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
    ld3320_interface_debug_print("ld3320: start register test.\n");
    
    /* init */
    res = ld3320_init(&gs_handle);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: init failed.\n");
       
        return 1;
    }
    
    /* ld3320_set_mode/ld3320_get_mode test */
    ld3320_interface_debug_print("ld3320: ld3320_set_mode/ld3320_get_mode test.\n");
    
    /* mp3 mode */
    res = ld3320_set_mode(&gs_handle, LD3320_MODE_MP3);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set mode failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set mp3 mode.\n");
    res = ld3320_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get mode failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check mode %s.\n", mode == LD3320_MODE_MP3 ? "ok" : "error");
    
    /* asr mode */
    res = ld3320_set_mode(&gs_handle, LD3320_MODE_ASR);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set mode failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set asr mode.\n");
    res = ld3320_get_mode(&gs_handle, &mode);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get mode failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check mode %s.\n", mode == LD3320_MODE_ASR ? "ok" : "error");
    
    /* ld3320_set_key_words/ld3320_get_key_words test */
    ld3320_interface_debug_print("ld3320: ld3320_set_key_words/ld3320_get_key_words test.\n");
    memset(text[0], 0, sizeof(char) * 50);
    strncpy((char *)text[0], "ni hao", strlen("ni hao"));
    res = ld3320_set_key_words(&gs_handle, text, 1);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set key words failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set key words \"ni hao\".\n");
    res = ld3320_get_key_words(&gs_handle, text_check, (uint8_t *)&len);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get key words failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check key words %s.\n", 
                                 (len == 1) && (strncmp((char *)text[0], (char *)text_check[0], (uint16_t)strlen("ni hao")) == 0) ? "ok" : "error");
    
    /* ld3320_set_mic_gain/ld3320_get_mic_gain test */
    ld3320_interface_debug_print("ld3320: ld3320_set_mic_gain/ld3320_get_mic_gain test.\n");
    
    /* set gain common */
    res = ld3320_set_mic_gain(&gs_handle, LD3320_MIC_GAIN_COMMON);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set mic gain failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set mic common gain.\n");
    res = ld3320_get_mic_gain(&gs_handle, &gain);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get mic gain failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check mic gain %s.\n", gain == LD3320_MIC_GAIN_COMMON ? "ok" : "error");
    
    /* set gain noise */
    res = ld3320_set_mic_gain(&gs_handle, LD3320_MIC_GAIN_NOISE);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set mic gain failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set mic noise gain.\n");
    res = ld3320_get_mic_gain(&gs_handle, &gain);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get mic gain failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check mic gain %s.\n", gain == LD3320_MIC_GAIN_NOISE ? "ok" : "error");
    
    /* ld3320_set_vad/ld3320_get_vad test */
    ld3320_interface_debug_print("ld3320: ld3320_set_vad/ld3320_get_vad test.\n");
    
    /* set vad common */
    res = ld3320_set_vad(&gs_handle, LD3320_VAD_COMMON);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set vad failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set vad common.\n");
    res = ld3320_get_vad(&gs_handle, &vad);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get vad failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check vad %s.\n", vad == LD3320_VAD_COMMON ? "ok" : "error");
    
    /* set vad far */
    res = ld3320_set_vad(&gs_handle, LD3320_VAD_FAR);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set vad failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set vad far.\n");
    res = ld3320_get_vad(&gs_handle, &vad);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get vad failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check vad %s.\n", vad == LD3320_VAD_FAR ? "ok" : "error");
    
    /* ld3320_set_speaker_volume/ld3320_get_speaker_volume test */
    ld3320_interface_debug_print("ld3320: ld3320_set_speaker_volume/ld3320_get_speaker_volume test.\n");
    volume_left = rand() % 16;
    res = ld3320_set_speaker_volume(&gs_handle, volume_left);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set speaker volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set volume 0x%02X.\n", volume_left);
    res = ld3320_get_speaker_volume(&gs_handle, (uint8_t *)&volume_left_check);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get speaker volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check volume %s.\n", volume_left_check == volume_left ? "ok" : "error");
    
    /* ld3320_set_headset_volume/ld3320_get_headset_volume test */
    ld3320_interface_debug_print("ld3320: ld3320_set_headset_volume/ld3320_get_headset_volume test.\n");
    volume_left = rand() % 16;
    volume_right = rand() % 16;
    res = ld3320_set_headset_volume(&gs_handle, volume_left, volume_right);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: set headset volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: set left volume 0x%02X.\n", volume_left);
    ld3320_interface_debug_print("ld3320: set right volume 0x%02X.\n", volume_right);
    res = ld3320_get_headset_volume(&gs_handle, (uint8_t *)&volume_left_check, (uint8_t *)&volume_right_check);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get headset volume failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: check left volume %s.\n", volume_left_check == volume_left ? "ok" : "error");
    ld3320_interface_debug_print("ld3320: check right volume %s.\n", volume_right_check == volume_right ? "ok" : "error");
    
    /* ld3320_get_status test */
    ld3320_interface_debug_print("ld3320: ld3320_get_status test.\n");
    res = ld3320_get_status(&gs_handle, &status);
    if (res != 0)
    {
        ld3320_interface_debug_print("ld3320: get status failed.\n");
        (void)ld3320_deinit(&gs_handle);
        
        return 1;
    }
    ld3320_interface_debug_print("ld3320: status is 0x%02X.\n", status);
    
    /* finish register test */
    ld3320_interface_debug_print("ld3320: finish register test.\n");
    (void)ld3320_deinit(&gs_handle);
    
    return 0;
}
