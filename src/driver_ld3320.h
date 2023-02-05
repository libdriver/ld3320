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
 * @file      driver_ld3320.h
 * @brief     driver ld3320 header file
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

#ifndef DRIVER_LD3320_H
#define DRIVER_LD3320_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ld3320_driver ld3320 driver function
 * @brief    ld3320 driver modules
 * @{
 */

/**
 * @addtogroup ld3320_basic_driver
 * @{
 */

/**
 * @brief ld3320 crystal frequency definition
 */
#ifndef LD3320_CYSTAL_MHZ 
    #define LD3320_CYSTAL_MHZ        22.1184f        /* 22.1184 MHz */
#endif

/**
 * @brief ld3320 mode enumeration definition
 */
typedef enum
{
    LD3320_MODE_ASR = 0x01,        /**< asr mode */
    LD3320_MODE_MP3 = 0x02,        /**< mp3 mode */
} ld3320_mode_t;

/**
 * @brief ld3320 mic gain enumeration definition
 */
typedef enum
{
    LD3320_MIC_GAIN_COMMON = 0x43,        /**< common */
    LD3320_MIC_GAIN_NOISE  = 0x2F,        /**< noise */
} ld3320_mic_gain_t;

/**
 * @brief ld3320 vad enumeration definition
 */
typedef enum
{
    LD3320_VAD_COMMON = 0x12,        /**< common */
    LD3320_VAD_FAR    = 0x0A,        /**< far */
} ld3320_vad_t;

/**
 * @brief ld3320 asr status enumeration definition
 */
typedef enum
{
    LD3320_STATUS_NONE           = 0x00,        /**< none */
    LD3320_STATUS_ASR_RUNNING    = 0x01,        /**< asr running */
    LD3320_STATUS_ASR_FOUND_OK   = 0x02,        /**< asr found ok */
    LD3320_STATUS_ASR_FOUND_ZERO = 0x03,        /**< asr found zero */
    LD3320_STATUS_ASR_ERROR      = 0x04,        /**< asr error */
    LD3320_STATUS_MP3_RUNNING    = 0x05,        /**< mp3 running */
    LD3320_STATUS_MP3_LOAD       = 0x06,        /**< mp3 load */
    LD3320_STATUS_MP3_END        = 0x07,        /**< mp3 end */
    LD3320_STATUS_MP3_ERROR      = 0x08,        /**< mp3 error */
} ld3320_status_t;

/**
 * @brief ld3320 handle structure definition
 */
typedef struct ld3320_handle_s
{
    uint8_t (*spi_init)(void);                                                       /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                     /**< point to a spi_deinit function address */
    uint8_t (*spi_read_address16)(uint16_t addr, uint8_t *buf, uint16_t len);        /**< point to a spi_read_address16 function address */
    uint8_t (*spi_write_cmd)(uint8_t *buf, uint16_t len);                            /**< point to a spi_write_cmd function address */
    uint8_t (*reset_gpio_init)(void);                                                /**< point to a reset_gpio_init function address */
    uint8_t (*reset_gpio_deinit)(void);                                              /**< point to a reset_gpio_deinit function address */
    uint8_t (*reset_gpio_write)(uint8_t data);                                       /**< point to a reset_gpio_write function address */
    void (*delay_ms)(uint32_t ms);                                                   /**< point to a delay_ms function address */
    void (*delay_us)(uint32_t us);                                                   /**< point to a delay_us function address */
    void (*debug_print)(const char *const fmt, ...);                                 /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type, uint8_t index, char *text);               /**< point to a receive_callback function address */
    uint8_t (*mp3_read_init)(char *name, uint32_t *size);                            /**< point to an mp3_read_init function address */
    uint8_t (*mp3_read)(uint32_t addr, uint16_t size, uint8_t *buffer);              /**< point to an mp3_read function address */
    uint8_t (*mp3_read_deinit)(void);                                                /**< point to an mp3_read_deinit function address */
    uint8_t inited;                                                                  /**< inited flag */
    uint8_t mode;                                                                    /**< running mode */
    uint8_t running_status;                                                          /**< running status */
    uint8_t mic_gain;                                                                /**< mic gain */
    uint8_t vad;                                                                     /**< vad */
    char text[50][50];                                                               /**< inner text */
    uint8_t len;                                                                     /**< inner text length */
    uint32_t point;                                                                  /**< mp3 play point */
    uint32_t size;                                                                   /**< mp3 size */
    uint8_t buf[512];                                                                /**< buffer */
} ld3320_handle_t;

/**
 * @brief ld3320 information structure definition
 */
typedef struct ld3320_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ld3320_info_t;

/**
 * @}
 */

/**
 * @defgroup ld3320_link_driver ld3320 link driver function
 * @brief    ld3320 link driver modules
 * @ingroup  ld3320_driver
 * @{
 */

/**
 * @brief     initialize ld3320_handle_t structure
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] STRUCTURE is ld3320_handle_t
 * @note      none
 */
#define DRIVER_LD3320_LINK_INIT(HANDLE, STRUCTURE)                    memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_SPI_INIT(HANDLE, FUC)                      (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_SPI_DEINIT(HANDLE, FUC)                    (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read_address16 function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a spi_read_address16 function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_SPI_READ_ADDRESS16(HANDLE, FUC)            (HANDLE)->spi_read_address16 = FUC

/**
 * @brief     link spi_write_cmd function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a spi_write_cmd function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_SPI_WRITE_COMMAND(HANDLE, FUC)             (HANDLE)->spi_write_cmd = FUC

/**
 * @brief     link reset_gpio_init function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a reset_gpio_init function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_RESET_GPIO_INIT(HANDLE, FUC)               (HANDLE)->reset_gpio_init = FUC

/**
 * @brief     link reset_gpio_deinit function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a reset_gpio_deinit function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_RESET_GPIO_DEINIT(HANDLE, FUC)             (HANDLE)->reset_gpio_deinit = FUC

/**
 * @brief     link reset_gpio_write function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a reset_gpio_write function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_RESET_GPIO_WRITE(HANDLE, FUC)              (HANDLE)->reset_gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_DELAY_MS(HANDLE, FUC)                      (HANDLE)->delay_ms = FUC

/**
 * @brief     link delay_us function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a delay_us function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_DELAY_US(HANDLE, FUC)                      (HANDLE)->delay_us = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_DEBUG_PRINT(HANDLE, FUC)                   (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_DEBUG_RECEIVE_CALLBACK(HANDLE, FUC)        (HANDLE)->receive_callback = FUC

/**
 * @brief     link mp3_read_init function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to an mp3_read_init function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_MP3_READ_INT(HANDLE, FUC)                  (HANDLE)->mp3_read_init = FUC

/**
 * @brief     link mp3_read_deinit function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to an mp3_read_deinit function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_MP3_READ_DEINT(HANDLE, FUC)                (HANDLE)->mp3_read_deinit = FUC

/**
 * @brief     link mp3_read function
 * @param[in] HANDLE points to an ld3320 handle structure
 * @param[in] FUC points to an mp3_read function address
 * @note      none
 */
#define DRIVER_LD3320_LINK_MP3_READ(HANDLE, FUC)                      (HANDLE)->mp3_read = FUC

/**
 * @}
 */

/**
 * @defgroup ld3320_basic_driver ld3320 basic driver function
 * @brief    ld3320 basic driver modules
 * @ingroup  ld3320_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an ld3320 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ld3320_info(ld3320_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 mode is invalid
 * @note      none
 */
uint8_t ld3320_irq_handler(ld3320_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t ld3320_init(ld3320_handle_t *handle);

/**
 * @brief     deinit the chip
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 gpio deinit failed
 *            - 5 spi deinit failed
 *            - 6 mp3 deinit failed
 * @note      none
 */
uint8_t ld3320_deinit(ld3320_handle_t *handle);

/**
 * @brief     start the process
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_start(ld3320_handle_t *handle);

/**
 * @brief     stop the process
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_stop(ld3320_handle_t *handle);

/**
 * @brief     configure the mp3
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] *name points to a name buffer
 * @return    status code
 *            - 0 success
 *            - 1 configure mp3 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_configure_mp3(ld3320_handle_t *handle, char *name);

/**
 * @brief     set the chip mode
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_set_mode(ld3320_handle_t *handle, ld3320_mode_t mode);

/**
 * @brief      get the chip mode
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *mode points to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_mode(ld3320_handle_t *handle, ld3320_mode_t *mode);

/**
 * @brief     set the key words
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] **text points to a text buffer
 * @param[in] len is the key words length
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_set_key_words(ld3320_handle_t *handle, char text[50][50], uint8_t len);

/**
 * @brief      get the key words
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] **text points to a text buffer
 * @param[out] *len points to a key words length buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_key_words(ld3320_handle_t *handle, char text[50][50], uint8_t *len);

/**
 * @brief     set the microphone gain
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] gain is microphone gain
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_set_mic_gain(ld3320_handle_t *handle, ld3320_mic_gain_t gain);

/**
 * @brief      get the microphone gain
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *gain points to a microphone gain buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_mic_gain(ld3320_handle_t *handle, ld3320_mic_gain_t *gain);

/**
 * @brief     set the vad
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] vad is the vad param
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_set_vad(ld3320_handle_t *handle, ld3320_vad_t vad);

/**
 * @brief      get the vad
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *vad points to a vad param buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_vad(ld3320_handle_t *handle, ld3320_vad_t *vad);

/**
 * @brief      get the status
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_status(ld3320_handle_t *handle, ld3320_status_t *status);

/**
 * @brief     set the speaker volume
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] volume is the speaker volume
 * @return    status code
 *            - 0 success
 *            - 1 set speaker volume
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 volume is invalid
 * @note      volume <= 15
 */
uint8_t ld3320_set_speaker_volume(ld3320_handle_t *handle, uint8_t volume);

/**
 * @brief      get the speaker volume
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *volume points to a speaker volume buffer
 * @return     status code
 *             - 0 success
 *             - 1 get speaker volume
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_speaker_volume(ld3320_handle_t *handle, uint8_t *volume);

/**
 * @brief     set the headset volume
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] volume_left is the left volume
 * @param[in] volume_right is the right volume
 * @return    status code
 *            - 0 success
 *            - 1 set headset volume
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 volume is invalid
 * @note      none
 */
uint8_t ld3320_set_headset_volume(ld3320_handle_t *handle, uint8_t volume_left, uint8_t volume_right);

/**
 * @brief      get the headset volume
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *volume_left points to a left volume buffer
 * @param[out] *volume_right points to a right volume buffer
 * @return     status code
 *             - 0 success
 *             - 1 get headset volume
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_headset_volume(ld3320_handle_t *handle, uint8_t *volume_left, uint8_t *volume_right);

/**
 * @}
 */

/**
 * @defgroup ld3320_extern_driver ld3320 extern driver function
 * @brief    ld3320 extern driver modules
 * @ingroup  ld3320_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] reg is the register address
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ld3320_set_reg(ld3320_handle_t *handle, uint8_t reg, uint8_t data);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[in]  reg is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ld3320_get_reg(ld3320_handle_t *handle, uint8_t reg, uint8_t *data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
