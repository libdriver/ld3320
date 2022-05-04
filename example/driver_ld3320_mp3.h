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
 * @file      driver_ld3320_mp3.h
 * @brief     driver ld3320 mp3 header file
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

#ifndef DRIVER_LD3320_MP3_H
#define DRIVER_LD3320_MP3_H

#include "driver_ld3320_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ld3320_example_driver
 * @{
 */

/**
 * @brief ld3320 mp3 example default definition
 */
#define LD3320_MP3_DEFAULT_SPEAKER_VOLUME          1        /**< level 1 */
#define LD3320_MP3_DEFAULT_HEADERSET_VOLUME        1        /**< level 1 */

/**
 * @brief  mp3 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ld3320_mp3_irq_handler(void);

/**
 * @brief     mp3 example init
 * @param[in] *name points to a mp3 file name buffer
 * @param[in] *receive_callback points to a receive callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ld3320_mp3_init(char *name, void (*receive_callback)(uint8_t type, uint8_t index, char *text));

/**
 * @brief  mp3 example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ld3320_mp3_deinit(void);

/**
 * @brief  mp3 example start
 * @return status code
 *         - 0 success
 *         - 1 start failed
 * @note   none
 */
uint8_t ld3320_mp3_start(void);

/**
 * @brief  mp3 example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t ld3320_mp3_stop(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
