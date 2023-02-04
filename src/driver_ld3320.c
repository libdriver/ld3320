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
 * @file      driver_ld3320.c
 * @brief     driver ld3320 source file
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

#include "driver_ld3320.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "IC Route LD3320"        /**< chip name */
#define MANUFACTURER_NAME         "IC Route"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                     /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.3f                     /**< chip max supply voltage */
#define MAX_CURRENT               166.7f                   /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                   /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                    /**< chip max operating temperature */
#define DRIVER_VERSION            1000                     /**< driver version */

/**
 * @brief chip register definition
 */
#define LD3320_REG_FIFO_DATA                        0x01        /**< fifo data register */
#define LD3320_REG_FIFO_INT_CONF                    0x02        /**< fifo interrupt configure register */
#define LD3320_REG_FIFO_EXT                         0x05        /**< fifo extern register */
#define LD3320_REG_FIFO_STATUS                      0x06        /**< fifo status register */
#define LD3320_REG_FIFO_CLEAR                       0x08        /**< fifo clear register */
#define LD3320_REG_CLK_CONF1                        0x11        /**< clock frequency configure 1 register */
#define LD3320_REG_CMD                              0x17        /**< command register */
#define LD3320_REG_CLK_CONF2                        0x19        /**< clock frequency configure 2 register */
#define LD3320_REG_CLK_CONF3                        0x1B        /**< clock frequency configure 3 register */
#define LD3320_REG_ADC_CONF                         0x1C        /**< adc configure register */
#define LD3320_REG_CLK_CONF4                        0x1D        /**< clock frequency configure 4 register */
#define LD3320_REG_ADC_CONTROL                      0x1E        /**< adc control register */
#define LD3320_REG_FIFO_DATA_UPPER_LOW              0x20        /**< fifo upper boundary low register */
#define LD3320_REG_FIFO_DATA_UPPER_HIGH             0x21        /**< fifo upper boundary high register */
#define LD3320_REG_FIFO_DATA_LOWER_LOW              0x22        /**< fifo lower boundary low register */
#define LD3320_REG_FIFO_DATA_LOWER_HIGH             0x23        /**< fifo lower boundary high register */
#define LD3320_REG_FIFO_MCU_WATER_MARK_L            0x24        /**< fifo mcu water mark low register */
#define LD3320_REG_FIFO_MCU_WATER_MARK_H            0x25        /**< fifo mcu water mark high register */
#define LD3320_REG_FIFO_DSP_WATER_MARK_L            0x26        /**< fifo dsp water mark low register */
#define LD3320_REG_FIFO_DSP_WATER_MARK_H            0x27        /**< fifo dsp water mark high register */
#define LD3320_REG_INT_CONF                         0x29        /**< interrupt configure register */
#define LD3320_REG_INT_FLAG                         0x2B        /**< interrupt flag register */
#define LD3320_REG_MP3_CONF                         0x33        /**< mp3 configure register */
#define LD3320_REG_ADC_GAIN                         0x35        /**< adc gain register */
#define LD3320_REG_DSP_CMD                          0x37        /**< dsp command register */
#define LD3320_REG_FIFO_EXT_UPPER_LOW               0x38        /**< dsp upper boundary low register */
#define LD3320_REG_FIFO_EXT_UPPER_HIGH              0x3A        /**< dsp upper boundary high register */
#define LD3320_REG_FIFO_EXT_LOWER_LOW               0x3C        /**< dsp lower boundary low register */
#define LD3320_REG_FIFO_EXT_LOWER_HIGH              0x3E        /**< dsp lower boundary high register */
#define LD3320_REG_FIFO_EXT_MCU_WATER_MARK_L        0x40        /**< fifo extern mcu water mark low register */
#define LD3320_REG_FIFO_EXT_MCU_WATER_MARK_H        0x42        /**< fifo extern mcu water mark high register */
#define LD3320_REG_FIFO_EXT_DSP_WATER_MARK_L        0x44        /**< fifo extern dsp water mark low register */
#define LD3320_REG_FIFO_EXT_DSP_WATER_MARK_H        0x46        /**< fifo extern dsp water mark high register */
#define LD3320_REG_INIT                             0x6F        /**< init register */
#define LD3320_REG_CLK_CONF5                        0x79        /**< clock frequency configure 5 register */
#define LD3320_REG_HEADSET_LEFT                     0x81        /**< headset left register */
#define LD3320_REG_HEADSET_RIGHT                    0x83        /**< headset right register */
#define LD3320_REG_FEEDBACK                         0x85        /**< internal feedback register */
#define LD3320_REG_ANALOG_CONTROL1                  0x87        /**< analog control 1 register */
#define LD3320_REG_ANALOG_CONTROL2                  0x89        /**< analog control 2 register */
#define LD3320_REG_GAIN_CONTROL                     0x8D        /**< internal gain control register */
#define LD3320_REG_SPEAKER                          0x8E        /**< speaker register */
#define LD3320_REG_LINEOUT                          0x8F        /**< line out register */
#define LD3320_REG_ASR_STATUS                       0xB2        /**< asr status register */
#define LD3320_REG_ASR_VAD_PARAM                    0xB3        /**< asr vad param register */
#define LD3320_REG_ASR_VAD_START                    0xB4        /**< asr vad start register */
#define LD3320_REG_ASR_VAD_SILENCE_END              0xB5        /**< asr vad silence end register */
#define LD3320_REG_ASR_VAD_VOICE_MAX_LEN            0xB6        /**< asr vad voice max length register */
#define LD3320_REG_ASR_PASS_FRAME                   0xB7        /**< asr pass frame register */
#define LD3320_REG_ASR_TIME                         0xB8        /**< asr time register */
#define LD3320_REG_ASR_STR_LEN                      0xB9        /**< asr string length register */
#define LD3320_REG_INT_AUX                          0xBA        /**< interrupt auxiliary register */
#define LD3320_REG_ASR_FORCE_STOP                   0xBC        /**< force stop asr register */
#define LD3320_REG_INIT_CONTROL                     0xBD        /**< init control register */
#define LD3320_REG_ASR_STATUS2                      0xBF        /**< asr status 2 register */
#define LD3320_REG_ASR_INDEX                        0xC1        /**< asr index register */
#define LD3320_REG_ASR_DATA                         0xC3        /**< asr data register */
#define LD3320_REG_ASR_RES_1                        0xC5        /**< result 1 register */
#define LD3320_REG_ASR_RES_2                        0xC7        /**< result 2 register */
#define LD3320_REG_ASR_RES_3                        0xC9        /**< result 3 register */
#define LD3320_REG_ASR_RES_4                        0xCB        /**< result 4 register */
#define LD3320_REG_ASR_DSP_SLEEP                    0xCD        /**< dsp sleep register */
#define LD3320_REG_LOW_POWER                        0xCF        /**< low power register */

/**
 * @brief chip register param definition
 */
#define LD3320_PLL_MP3_19        0x0F
#define LD3320_PLL_MP3_1B        0x18
#define LD3320_PLL_MP3_1D        (uint8_t)(((90.0f * ((LD3320_PLL_11) + 1.0f)) / (LD3320_CYSTAL_MHZ)) - 1.0f)
#define LD3320_PLL_11            (uint8_t)((LD3320_CYSTAL_MHZ / 2.0f) - 1.0f)
#define LD3320_PLL_ASR_19        (uint8_t)(LD3320_CYSTAL_MHZ * 32.0f / (LD3320_PLL_11 + 1.0f) - 0.51f)
#define LD3320_PLL_ASR_1B        0x48
#define LD3320_PLL_ASR_1D        0x1F

/**
 * @brief     write the data
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] address is the register address
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ld3320_write_byte(ld3320_handle_t *handle, uint8_t address, uint8_t data)
{
    uint8_t res;
    uint8_t buf[3];
    
    buf[0] = 0x04;                                         /* set write command */
    buf[1] = address;                                      /* set register address */
    buf[2] = data;                                         /* set written data */
    res = handle->spi_write_cmd((uint8_t *)buf, 3);        /* write data command */
    if (res != 0)                                          /* check result */
    {
        return 1;                                          /* return error */
    }
    else
    {
        return 0;                                          /* success return 0 */
    }
}

/**
 * @brief      read the data
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[in]  address is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ld3320_read_byte(ld3320_handle_t *handle, uint8_t address, uint8_t *data)
{
    uint8_t res;
    uint16_t reg;
    
    reg = ((uint16_t)(0x05) << 8) | address;               /* set register */
    res = handle->spi_read_address16(reg, data, 1);        /* read data */
    if (res != 0)                                          /* check result */
    {
        return 1;                                          /* return error */
    }
    else
    {
        return 0;                                          /* success return 0 */
    }
}

/**
 * @brief     common init
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
static uint8_t a_ld3320_common_init(ld3320_handle_t *handle)
{
    uint8_t res;
    uint8_t reg;
    
    res = a_ld3320_read_byte(handle, LD3320_REG_FIFO_STATUS, (uint8_t *)&reg);            /* read fifo status */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: read fifo status failed.\n");                        /* read fifo status failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_CMD, 0x35);                              /* send soft reset command */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: send command failed.\n");                            /* send command failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(10);                                                                 /* delay 10 ms */
    res = a_ld3320_read_byte(handle, LD3320_REG_FIFO_STATUS, (uint8_t *)&reg);            /* read fifo status */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: read fifo status failed.\n");                        /* read fifo status failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL2, 0x03);                  /* set analog control2 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: write byte failed.\n");                              /* write byte failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(5);                                                                  /* delay 5 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_LOW_POWER, 0x43);                        /* set low power */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set low power failed.\n");                           /* set low power failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(5);                                                                  /* delay 5 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_RES_4, 0x02);                        /* set asr result 4 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: write byte failed.\n");                              /* write byte failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF1, LD3320_PLL_11);               /* set clock configure 1 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set clock configure 1 failed.\n");                   /* set clock configure 1 failed */
        
        return 1;                                                                         /* return error */
    }
    if (handle->mode == LD3320_MODE_ASR)                                                  /* asr mode */
    {
        res = a_ld3320_write_byte(handle, LD3320_REG_ADC_CONTROL, 0x00);                  /* set the adc control */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set adc control failed.\n");                     /* set adc control failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF2, LD3320_PLL_ASR_19);       /* set clock conf 2 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 3 failed.\n");                    /* set clock conf 2 failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF3, LD3320_PLL_ASR_1B);       /* set clock conf 3 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 3 failed.\n");                    /* set clock conf 3 failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF4, LD3320_PLL_ASR_1D);       /* set clock conf 4 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 4 failed.\n");                    /* set clock conf 4 failed */
            
            return 1;                                                                     /* return error */
        }
    }
    else if (handle->mode == LD3320_MODE_MP3)                                             /* mp3 mode */
    {
        res = a_ld3320_write_byte(handle, LD3320_REG_ADC_CONTROL, 0x00);                  /* set the adc control */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set adc control failed.\n");                     /* set adc control failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF2, LD3320_PLL_MP3_19);       /* set clock conf 2 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 3 failed.\n");                    /* set clock conf 2 failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF3, LD3320_PLL_MP3_1B);       /* set clock conf 3 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 3 failed.\n");                    /* set clock conf 3 failed */
            
            return 1;                                                                     /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CLK_CONF4, LD3320_PLL_MP3_1D);       /* set clock conf 4 */
        if (res != 0)                                                                     /* check result */
        {
            handle->debug_print("ld3320: set clock conf 4 failed.\n");                    /* set clock conf 4 failed */
            
            return 1;                                                                     /* return error */
        }
    }
    else
    {
        handle->debug_print("ld3320: mode is invalid.\n");                                /* mode is invalid */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(10);                                                                 /* delay 10 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_DSP_SLEEP, 0x04);                    /* enable dsp sleep */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set dsp sleep failed.\n");                           /* set dsp sleep failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_CMD, 0x4C);                              /* send dsp sleep enable command */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: send command failed.\n");                            /* send command failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(5);                                                                  /* delay 5 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_STR_LEN, 0x00);                      /* set string length 0 */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set string length failed.\n");                       /* set string length failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_LOW_POWER, 0x4F);                        /* set low power */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set low power failed.\n");                           /* set low power failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INIT, 0xFF);                             /* set init */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set init failed.\n");                                /* set init failed */
        
        return 1;                                                                         /* return error */
    }
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     mp3 mode init
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 mp3 init failed
 * @note      none
 */
static uint8_t a_ld3320_mp3_init(ld3320_handle_t *handle)
{
    uint8_t res;
    
    res = a_ld3320_common_init(handle);                                               /* common init */
    if (res != 0)                                                                     /* check result */
    {
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INIT_CONTROL, 0x02);                 /* set mp3 mode */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set int control failed.\n");                     /* set int control failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_CMD, 0x48);                          /* active dsp */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set command failed.\n");                         /* set command failed */
        
        return 1;                                                                     /* return error */
    }
    handle->delay_ms(10);                                                             /* delay 10 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_FEEDBACK, 0x52);                     /* set feedback */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set feedback failed.\n");                        /* set feedback failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_LINEOUT, 0x00);                      /* set line out */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set line out failed.\n");                        /* set line out failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_HEADSET_LEFT, 0x00);                 /* set headset left */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set headset left failed.\n");                    /* set headset left failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_HEADSET_RIGHT, 0x00);                /* set headset right */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set headset right failed.\n");                   /* set headset right failed */
        
        return 1;                                                                     /* return error */
    }
    handle->delay_ms(2);                                                              /* delay 2 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_SPEAKER, 0xFF);                      /* set speaker */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set headset right failed.\n");                   /* set speaker failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_GAIN_CONTROL, 0xFF);                 /* set gain control */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set gain control failed.\n");                    /* set gain control failed */
        
        return 1;                                                                     /* return error */
    }
    handle->delay_ms(2);                                                              /* delay 2 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL1, 0xFF);              /* set analog control 1 */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set analog control 1 failed.\n");                /* set analog control 1 failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL2, 0xFF);              /* set analog control 2 */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set analog control 2 failed.\n");                /* set analog control 2 failed */
        
        return 1;                                                                     /* return error */
    }
    handle->delay_ms(2);                                                              /* delay 2 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DATA_LOWER_LOW, 0x00);          /* set fifo data lower low */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo data lower low failed.\n");             /* set fifo data lower low failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DATA_LOWER_HIGH, 0x00);         /* set fifo data high low */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo data lower high failed.\n");            /* set fifo data lower high failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DATA_UPPER_LOW, 0xEF);          /* set fifo data upper low */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo data upper low failed.\n");             /* set fifo data upper low failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DATA_UPPER_HIGH, 0x07);         /* set fifo data upper high */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo data upper high failed.\n");            /* set fifo data upper high failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_MCU_WATER_MARK_L, 0x77);        /* set fifo mcu watermark low */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo mcu water mark low failed.\n");         /* set fifo mcu watermark low failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_MCU_WATER_MARK_H, 0x03);        /* set fifo mcu watermark high */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo mcu water mark high failed.\n");        /* set fifo mcu watermark high failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DSP_WATER_MARK_L, 0xBB);        /* set fifo dsp watermark low */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo mcu water mark low failed.\n");         /* set fifo mcu watermark low failed */
        
        return 1;                                                                     /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DSP_WATER_MARK_H, 0x01);        /* set fifo dsp watermark high */
    if (res != 0)                                                                     /* check result */
    {
        handle->debug_print("ld3320: set fifo mcu water mark high failed.\n");        /* set fifo mcu watermark high failed */
        
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

/**
 * @brief     mp3 load data
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 mp3 load failed
 * @note      none
 */
static uint8_t a_ld3320_mp3_load(ld3320_handle_t *handle)
{
    uint8_t res;
    uint8_t data;
    uint16_t size, i;
    
    while (1)                                                                                      /* loop */
    {
        res = a_ld3320_read_byte(handle, LD3320_REG_FIFO_STATUS, (uint8_t *)&data);                /* read data */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ld3320: fifo status read failed.\n");                             /* fifo status read failed */
            
            return 1;                                                                              /* return error */
        }
        size = (uint16_t)(512 < (handle->size - handle->point) ? 
                          512 : (handle->size - handle->point));                                   /* get size */
        res = handle->mp3_read(handle->point, size, handle->buf);                                  /* read data */
        if (res != 0)                                                                              /* check result */
        {
            handle->debug_print("ld3320: mp3 read failed.\n");                                     /* mp3 read failed */
            
            return 1;                                                                              /* return error */
        }
        i = 0;                                                                                     /* set index 0 */
        while (!(data & 0x08) && (i < size))
        {
            res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_DATA, handle->buf[i]);               /* write data */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("ld3320: write fifo failed.\n");                               /* write failed */
                
                return 1;                                                                          /* return error */
            }
            handle->delay_us(60);                                                                  /* delay 60 us */
            i++;                                                                                   /* i++ */
            handle->point++;                                                                       /* point++ */
            res = a_ld3320_read_byte(handle, LD3320_REG_FIFO_STATUS, (uint8_t *)&data);            /* read data */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("ld3320: fifo status read failed.\n");                         /* fifo status read failed */
                
                return 1;                                                                          /* return error */
            }
        }
        if (!(data & 0x08))                                                                        /* check fifo full */
        {
            continue;                                                                              /* continue */
        }
        else
        {
            break;                                                                                 /* break */
        }
    }
    if (handle->point >= handle->size)                                                             /* check the size */
    {
        handle->running_status = LD3320_STATUS_NONE;                                               /* clear status */
        handle->receive_callback(LD3320_STATUS_MP3_END, 0, NULL);                                  /* run the callback */
    }
    
    return 0;                                                                                      /* success return 0 */
}

/**
 * @brief     asr mode init
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 asr init failed
 * @note      none
 */
static uint8_t a_ld3320_asr_init(ld3320_handle_t *handle)
{
    uint8_t res;

    res = a_ld3320_common_init(handle);                                                   /* common init */
    if (res != 0)                                                                         /* check result */
    {
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INIT_CONTROL, 0x00);                     /* set asr mode */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set int control failed.\n");                         /* set int control failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_CMD, 0x48);                              /* active dsp */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set command failed.\n");                             /* set command failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(10);                                                                 /* delay 10 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_LOWER_LOW, 0x80);               /* fifo ext lower low */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext lower low failed.\n");                  /* set fifo ext lower low failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_LOWER_HIGH, 0x07);              /* fifo ext lower high */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext lower high failed.\n");                 /* set fifo ext lower high failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_UPPER_LOW, 0xFF);               /* fifo ext upper low */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext upper low failed.\n");                  /* set fifo ext upper low failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_UPPER_HIGH, 0x07);              /* fifo ext upper high */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext upper high failed.\n");                 /* set fifo ext upper high failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_MCU_WATER_MARK_L, 0x00);        /* fifo ext mcu watermark low */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext mcu water mark low failed.\n");         /* set fifo ext mcu watermark low failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_MCU_WATER_MARK_H, 0x08);        /* fifo ext mcu watermark high */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext mcu water mark high failed.\n");        /* set fifo ext mcu watermark high failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_DSP_WATER_MARK_L, 0x00);        /* fifo ext mcu watermark low */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext mcu water mark low failed.\n");         /* set fifo ext mcu watermark low failed */
        
        return 1;                                                                         /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT_DSP_WATER_MARK_H, 0x08);        /* fifo ext mcu watermark high */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ld3320: set fifo ext mcu water mark high failed.\n");        /* set fifo ext mcu watermark high failed */
        
        return 1;                                                                         /* return error */
    }
    handle->delay_ms(2);                                                                  /* delay 2 ms */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      check asr busy
 * @param[in]  *handle points to an ld3320 handle structure
 * @param[out] *s points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 check asr busy failed
 * @note       none
 */
static uint8_t a_ld3320_check_asr_busy(ld3320_handle_t *handle, uint8_t *s)
{
    uint8_t res, i;
    uint8_t status;
    
    *s = 0;                                                                                /* clear status */
    for (i = 0; i < 10; i++)                                                               /* wait 100 ms */
    {
        res = a_ld3320_read_byte(handle, LD3320_REG_ASR_STATUS, (uint8_t *)&status);       /* read asr status */
        if (res != 0)                                                                      /* check result */
        {
            handle->debug_print("ld3320: read asr status failed.\n");                      /* read asr status failed */
            
            return 1;                                                                      /* return error */
        }
        if (status == 0x21)                                                                /* check status */
        {
            *s = 1;                                                                        /* flag */
            
            break;                                                                         /* break */
        }
        handle->delay_ms(10);                                                              /* delay 10 ms */
    }
    
    return 0;                                                                              /* success return 0 */
}

/**
 * @brief     asr start
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] mic_gain is the microphone gain
 * @param[in] vad is the vad param
 * @return    status code
 *            - 0 success
 *            - 1 asr start failed
 * @note      none
 */
static uint8_t a_ld3320_asr_start(ld3320_handle_t *handle, uint8_t mic_gain, uint8_t vad)
{
    uint8_t res;
    uint8_t s;
    
    res = a_ld3320_write_byte(handle, LD3320_REG_ADC_GAIN, mic_gain);           /* set ad gain */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set adc gain failed.\n");                  /* set adc gain failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_VAD_PARAM, vad);           /* set asr vad param gain */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set asr vad param failed.\n");             /* set asr vad param failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ADC_CONF, 0x09);               /* set adc conf */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set adc conf failed.\n");                  /* set adc conf failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INIT_CONTROL, 0x20);           /* set init control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set init control failed.\n");              /* set init control failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x01);             /* set init control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set fifo clear failed.\n");                /* set fifo clear failed */
        
        return 1;                                                               /* return error */
    }
    handle->delay_ms(2);                                                        /* delay 2 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x00);             /* set init control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set fifo clear failed.\n");                /* set fifo clear failed */
        
        return 1;                                                               /* return error */
    }
    handle->delay_ms(2);                                                        /* delay 2 ms */
    res = a_ld3320_check_asr_busy(handle, (uint8_t *)&s);                       /* check asr busy */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: check asr busy failed.\n");                /* check asr busy failed */
        
        return 1;                                                               /* return error */
    }
    if (s == 0)                                                                 /* check result */
    {
        handle->debug_print("ld3320: asr busy .\n");                            /* asr busy */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_STATUS, 0xFF);             /* clear asr status */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: clear asr status failed.\n");              /* clear asr status failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_DSP_CMD, 0x06);                /* start dsp asr */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: start dsp asr failed.\n");                 /* start dsp asr failed */
        
        return 1;                                                               /* return error */
    }
    handle->delay_ms(5);                                                        /* delay 5 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_ADC_CONF, 0x0B);               /* enable adc mic */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: enable adc mic failed.\n");                /* enable adc mic failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, 0x10);               /* enable sync */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: enable sync failed.\n");                   /* enable sync failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_INIT_CONTROL, 0x00);           /* set the init control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set the init control failed.\n");          /* set the init control failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     asr add key word
 * @param[in] *handle points to an ld3320 handle structure
 * @param[in] ind is the key word index
 * @param[in] *word points to a key word buffer
 * @param[in] len is the key word length
 * @return    status code
 *            - 0 success
 *            - 1 asr add key word failed
 * @note      none
 */
static uint8_t a_ld3320_asr_add_key_word(ld3320_handle_t *handle, uint8_t ind, char *word, uint16_t len)
{
    uint8_t res;
    uint16_t i, k;
    
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_INDEX, ind);                /* write asr index */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: set asr index failed.\n");                  /* set asr index failed */
        
        return 1;                                                                /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_DATA, 0x00);                /* write asr data */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: set asr data failed.\n");                   /* set asr data failed */
        
        return 1;                                                                /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x04);              /* fifo clear */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: fifo clear failed.\n");                     /* fifo clear failed */
        
        return 1;                                                                /* return error */
    }
    handle->delay_ms(2);                                                         /* delay 2 ms */
    res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x00);              /* fifo clear */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: fifo clear failed.\n");                     /* fifo clear failed */
        
        return 1;                                                                /* return error */
    }
    handle->delay_ms(2);                                                         /* delay 2 ms */
    k = (len < 50) ? len : 50;                                                   /* get the mini length */
    for (i = 0; i < k; i++)                                                      /* write data */
    {
        res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_EXT , word[i]);        /* fifo ext write */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: fifo ext write failed.\n");             /* fifo ext write failed */
            
            return 1;                                                            /* return error */
        }
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_STR_LEN, (uint8_t)k);       /* asr str length */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: asr string length failed.\n");              /* asr string length failed */
        
        return 1;                                                                /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ASR_STATUS, 0xFF);              /* clear asr status */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: asr status failed.\n");                     /* asr status failed */
        
        return 1;                                                                /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_DSP_CMD, 0x04);                 /* add dsp words status */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ld3320: dsp command failed.\n");                    /* dsp command failed */
        
        return 1;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */
}

/**
 * @brief     asr add fixed
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 asr add fixed failed
 * @note      none
 */
static uint8_t a_ld3320_asr_add_fixed(ld3320_handle_t *handle)
{
    uint8_t res, i, s;
    
    for (i = 0; i < handle->len; i++)                                                               /* add fixed length */
    {
        res = a_ld3320_check_asr_busy(handle, (uint8_t *)&s);                                       /* check asr busy */
        if (res != 0)                                                                               /* check result */
        {
            handle->debug_print("ld3320: check asr busy failed.\n");                                /* check asr busy failed */
           
            return 1;                                                                               /* return error */
        }
        if (s == 0)                                                                                 /* check status */
        {
            return 1;                                                                               /* return error */
        }
        res = a_ld3320_asr_add_key_word(handle, i, handle->text[i], 
                                       (uint16_t)strlen(handle->text[i]));                          /* asr add key word */
        if (res != 0)                                                                               /* check result */
        {
            handle->debug_print("ld3320: asr add key word failed.\n");                              /* asr add key word failed */
           
            return 1;                                                                               /* return error */
        }
    }
    
    return 0;                                                                                       /* success return 0 */
}

/**
 * @brief     reset the chip
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset failed
 * @note      none
 */
static uint8_t a_ld3320_reset(ld3320_handle_t *handle)
{
    if (handle->reset_gpio_write(1) != 0)                                    /* reset gpio write 1 */
    {
        handle->debug_print("ld3320: reset gpio write failed.\n");           /* reset gpio write failed */
       
        return 1;                                                            /* return error */
    }
    handle->delay_ms(2);                                                     /* delay 2 ms */
    if (handle->reset_gpio_write(0) != 0)                                    /* reset gpio write 0 */
    {
        handle->debug_print("ld3320: reset gpio write failed.\n");           /* reset gpio write failed */
       
        return 1;                                                            /* return error */
    }
    handle->delay_ms(2);                                                     /* delay 2 ms */
    if (handle->reset_gpio_write(1) != 0)                                    /* reset gpio write 1 */
    {
        handle->debug_print("ld3320: reset gpio write failed.\n");           /* reset gpio write failed */
       
        return 1;                                                            /* return error */
    }
    if (handle->spi_write_cmd(NULL, 0) != 0)                                 /* spi write command */
    {
        handle->debug_print("ld3320: spi write command failed.\n");          /* spi write command failed */
       
        return 1;                                                            /* return error */
    }
    handle->delay_ms(2);                                                     /* delay 2 ms */
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief     run the asr
 * @param[in] *handle points to an ld3320 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run asr failed
 * @note      none
 */
static uint8_t a_ld3320_run_asr(ld3320_handle_t *handle)
{
    uint8_t res, i, flag;
    
    flag = 2;
    for (i = 0; i < 5; i++)
    {
        res = a_ld3320_asr_init(handle);                                           /* asr init */
        if (res != 0)                                                              /* check result */
        {
            handle->debug_print("ld3320: asr init failed.\n");                     /* asr init failed */
            (void)a_ld3320_reset(handle);                                          /* reset the ld3320 */
            handle->delay_ms(100);                                                 /* delay 100 ms */
            
            return 1;                                                              /* return error */
        }
        handle->delay_ms(100);                                                     /* delay 100 ms */
        res = a_ld3320_asr_add_fixed(handle);                                      /* asr add fixed */
        if (res != 0)                                                              /* check result */
        {
            (void)a_ld3320_reset(handle);                                          /* reset the ld3320 */
            handle->delay_ms(100);                                                 /* delay 100 ms */
            
            continue;                                                              /* continue */
        }
        handle->delay_ms(10);                                                      /* delay 10 ms */
        res = a_ld3320_asr_start(handle, handle->mic_gain, handle->vad);           /* asr start */
        if (res != 0)                                                              /* check result */
        {
            (void)a_ld3320_reset(handle);                                          /* reset the ld3320 */
            handle->delay_ms(100);                                                 /* delay 100 ms */
            
            continue;                                                              /* continue */
        }
        flag = 0;                                                                  /* flag ok */
        
        break;                                                                     /* break the loop */
    }
    
    return flag;                                                                   /* return flag */
}

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
uint8_t ld3320_set_mode(ld3320_handle_t *handle, ld3320_mode_t mode)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->mode = (uint8_t)mode;   /* set mode */
    
    return 0;                       /* success return 0 */
}

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
uint8_t ld3320_get_mode(ld3320_handle_t *handle, ld3320_mode_t *mode)
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }
    
    *mode = (ld3320_mode_t)(handle->mode);        /* get mode */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t ld3320_set_key_words(ld3320_handle_t *handle, char text[50][50], uint8_t len)
{
    uint8_t i;
    
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->inited != 1)                     /* check handle initialization */
    {
        return 3;                                /* return error */
    }
    
    for (i = 0; i < len; i++)                    /* copy the text */
    {
        strcpy(handle->text[i], text[i]);        /* copy the text */
    }
    handle->len = len;                           /* set length */
    
    return 0;                                    /* success return 0 */
}

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
uint8_t ld3320_get_key_words(ld3320_handle_t *handle, char text[50][50], uint8_t *len)
{
    uint8_t i;
    
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->inited != 1)                     /* check handle initialization */
    {
        return 3;                                /* return error */
    }
    
    for (i = 0; i < handle->len; i++)            /* copy the text */
    {
        strcpy(text[i], handle->text[i]);        /* copy the text */
    }
    *len = handle->len;                          /* get length */
    
    return 0;                                    /* success return 0 */
}

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
uint8_t ld3320_set_mic_gain(ld3320_handle_t *handle, ld3320_mic_gain_t gain)
{
    if (handle == NULL)                      /* check handle */
    {
        return 2;                            /* return error */
    }
    if (handle->inited != 1)                 /* check handle initialization */
    {
        return 3;                            /* return error */
    }
    
    handle->mic_gain = (uint8_t)gain;        /* set mode */
    
    return 0;                                /* success return 0 */
}

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
uint8_t ld3320_get_mic_gain(ld3320_handle_t *handle, ld3320_mic_gain_t *gain)
{
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }
    
    *gain = (ld3320_mic_gain_t)(handle->mic_gain);        /* get mode */
    
    return 0;                                             /* success return 0 */
}

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
uint8_t ld3320_set_vad(ld3320_handle_t *handle, ld3320_vad_t vad)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    handle->vad = (uint8_t)vad;     /* set vad */
    
    return 0;                       /* success return 0 */
}

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
uint8_t ld3320_get_vad(ld3320_handle_t *handle, ld3320_vad_t *vad)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    if (handle->inited != 1)                   /* check handle initialization */
    {
        return 3;                              /* return error */
    }
    
    *vad = (ld3320_vad_t)(handle->vad);        /* get vad */
    
    return 0;                                  /* success return 0 */
}

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
uint8_t ld3320_get_status(ld3320_handle_t *handle, ld3320_status_t *status)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    *status = (ld3320_status_t)(handle->running_status);        /* get running status */
    
    return 0;                                                   /* success return 0 */
}

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
uint8_t ld3320_init(ld3320_handle_t *handle)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->spi_init == NULL)                                            /* check spi_init */
    {
        handle->debug_print("ld3320: spi_init is null.\n");                  /* spi_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_deinit == NULL)                                          /* check spi_deinit */
    {
        handle->debug_print("ld3320: spi_deinit is null.\n");                /* spi_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_read_address16 == NULL)                                  /* check spi_read */
    {
        handle->debug_print("ld3320: spi_read_address16 is null.\n");        /* spi_read_address16 is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_write_cmd == NULL)                                       /* check spi_write_cmd */
    {
        handle->debug_print("ld3320: spi_write_cmd is null.\n");             /* spi_write_cmd is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->reset_gpio_init == NULL)                                     /* check reset_gpio_init */
    {
        handle->debug_print("ld3320: reset_gpio_init is null.\n");           /* reset_gpio_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->reset_gpio_deinit == NULL)                                   /* check reset_gpio_deinit */
    {
        handle->debug_print("ld3320: reset_gpio_deinit is null.\n");         /* reset_gpio_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->reset_gpio_write == NULL)                                    /* check reset_gpio_write */
    {
        handle->debug_print("ld3320: reset_gpio_write is null.\n");          /* reset_gpio_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("ld3320: delay_ms is null.\n");                  /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_us == NULL)                                            /* check delay_us */
    {
        handle->debug_print("ld3320: delay_us is null.\n");                  /* delay_us is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                    /* check receive_callback */
    {
        handle->debug_print("ld3320: receive_callback is null.\n");          /* receive_callback is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->mp3_read_init == NULL)                                       /* check mp3_read_init */
    {
        handle->debug_print("ld3320: mp3_read_init is null.\n");             /* mp3_read_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->mp3_read_deinit == NULL)                                     /* check mp3_read_deinit */
    {
        handle->debug_print("ld3320: mp3_read_deinit is null.\n");           /* mp3_read_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->mp3_read == NULL)                                            /* check mp3_read */
    {
        handle->debug_print("ld3320: mp3_read is null.\n");                  /* mp3_read is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->spi_init() != 0)                                             /* spi init */
    {
        handle->debug_print("ld3320: spi init failed.\n");                   /* spi init failed */
       
        return 1;                                                            /* return error */
    }
    if (handle->reset_gpio_init() != 0)                                      /* reset gpio init */
    {
        handle->debug_print("ld3320: reset gpio init failed.\n");            /* reset gpio init failed */
        (void)handle->spi_deinit();                                          /* spi deinit */
        
        return 4;                                                            /* return error */
    }
    if (a_ld3320_reset(handle) != 0)                                         /* result */
    {
        handle->debug_print("ld3320: reset failed.\n");                      /* reset failed */
        (void)handle->spi_deinit();                                          /* spi deinit */
        (void)handle->reset_gpio_deinit();                                   /* gpio deinit */
        
        return 4;                                                            /* return error */
    }
    handle->mic_gain = LD3320_MIC_GAIN_COMMON;                               /* set mic gain common */
    handle->vad = LD3320_VAD_COMMON;                                         /* set vad common */
    handle->running_status = LD3320_STATUS_NONE;                             /* set status none */
    handle->inited = 1;                                                      /* flag finished */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t ld3320_irq_handler(ld3320_handle_t *handle)
{
    uint8_t res;
    uint8_t flag;
    uint8_t asr_status;
    uint8_t asr_status2;
    uint8_t count;
    uint8_t i;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    if (handle->mode == LD3320_MODE_ASR)                                                          /* asr mode */
    {
        res = a_ld3320_read_byte(handle, LD3320_REG_INT_FLAG, (uint8_t *)&flag);                  /* read int flag */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: read int flag failed.\n");                               /* read int flag failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, 0x00);                             /* write int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: write int conf failed.\n");                              /* write int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_INT_CONF, 0x00);                        /* write fifo int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: write fifo int conf failed.\n");                         /* write fifo int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_read_byte(handle, LD3320_REG_ASR_STATUS, (uint8_t *)&asr_status);          /* read asr status flag */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: read asr status failed.\n");                             /* read asr status failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_read_byte(handle, LD3320_REG_ASR_STATUS2 , (uint8_t *)&asr_status2);       /* read asr status2 flag */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: read asr status2 failed.\n");                            /* read asr status2 failed */
            
            return 1;                                                                             /* return error */
        }
        if (((flag & 0x10) != 0) && (asr_status == 0x21) && (asr_status2 == 0x35))                /* check flags */
        {
            res = a_ld3320_read_byte(handle, LD3320_REG_INT_AUX , (uint8_t *)&count);             /* read count */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_ASR_ERROR;                                 /* set error */
                handle->debug_print("ld3320: read count failed.\n");                              /* read count failed */
                
                return 1;                                                                         /* return error */
            }
            if ((count > 0) && (count <= 4))                                                      /* check count */
            {
                handle->running_status = LD3320_STATUS_ASR_FOUND_OK;                              /* set ok */
            }
            else
            {
                handle->running_status = LD3320_STATUS_ASR_FOUND_ZERO;                            /* set zero */
            }
        }
        else
        {
            handle->running_status = LD3320_STATUS_ASR_FOUND_ZERO;                                /* set zero */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_INT_FLAG, 0x00);                             /* write int flag */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: write int flag failed.\n");                              /* write int flag failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_ADC_CONF, 0x00);                             /* write adc conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: write adc conf failed.\n");                              /* write adc conf failed */
            
            return 1;                                                                             /* return error */
        }
    }
    else if (handle->mode == LD3320_MODE_MP3)                                                     /* if mp3 mode */
    {
        uint8_t reg1;
        uint8_t reg2;
        uint8_t status;
        
        res = a_ld3320_read_byte(handle, LD3320_REG_INT_FLAG, (uint8_t *)&flag);                  /* read int flag */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: read int flag failed.\n");                               /* read int flag failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_read_byte(handle, LD3320_REG_INT_CONF, (uint8_t *)&reg1);                  /* read int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_MP3_ERROR;                                     /* set mp3 error */
            handle->debug_print("ld3320: read int conf failed.\n");                               /* read int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_read_byte(handle, LD3320_REG_FIFO_INT_CONF, (uint8_t *)&reg2);             /* read fifo int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_MP3_ERROR;                                     /* set mp3 error */
            handle->debug_print("ld3320: read fifo int conf failed.\n");                          /* read fifo int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, 0x00);                             /* write int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_MP3_ERROR;                                     /* set mp3 error */
            handle->debug_print("ld3320: write int conf failed.\n");                              /* write int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_INT_CONF, 0x00);                        /* write fifo int conf */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_MP3_ERROR;                                     /* set mp3 error */
            handle->debug_print("ld3320: write fifo int conf failed.\n");                         /* write fifo int conf failed */
            
            return 1;                                                                             /* return error */
        }
        res = a_ld3320_read_byte(handle, LD3320_REG_INT_AUX, (uint8_t *)&status);                 /* read status */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_MP3_ERROR;                                     /* set mp3 error */
            handle->debug_print("ld3320: read status failed.\n");                                 /* read status failed */
            
            return 1;                                                                             /* return error */
        }
        if ((status & 0x20) != 0)                                                                 /* check the status */
        {
            res = a_ld3320_write_byte(handle, LD3320_REG_INT_FLAG , 0x00);                        /* write int conf */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: write int conf failed.\n");                          /* write int conf failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_INT_AUX, 0x00);                          /* write int aux */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: write int aux failed.\n");                           /* write int aux failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_ASR_FORCE_STOP, 0x00);                   /* asr force stop */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: asr force stop failed.\n");                          /* asr force stop failed */
                
                return 1;                                                                         /* return error */
            }
            handle->point = 0;                                                                    /* set zero */
            res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x01);                       /* clear fifo */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: clear fifo failed.\n");                              /* clear fifo failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_CLEAR, 0x00);                       /* clear fifo */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: clear fifo failed.\n");                              /* clear fifo failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_MP3_CONF, 0x00);                         /* set mp3 conf */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: mp3 conf failed.\n");                                /* mp3 conf failed */
                
                return 1;                                                                         /* return error */
            }
            handle->running_status = LD3320_STATUS_MP3_END;                                       /* set end */
        }
        else if (handle->point >= handle->size &&
                 handle->running_status == LD3320_STATUS_ASR_RUNNING)                             /* check size */
        {
            res = a_ld3320_write_byte(handle, LD3320_REG_ASR_FORCE_STOP, 0x01);                   /* set asr force stop */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: asr force stop failed.\n");                          /* asr force stop failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, 0x10);                         /* set int conf */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: set int conf failed.\n");                            /* set int conf failed */
                
                return 1;                                                                         /* return error */
            }
            handle->running_status = LD3320_STATUS_MP3_END;                                       /* set end */
        }
        else
        {
            res = a_ld3320_mp3_load(handle);                                                      /* mp3 load */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: mp3 load failed.\n");                                /* mp3 load failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, reg1);                         /* write int conf */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: write int conf failed.\n");                          /* write int conf failed */
                
                return 1;                                                                         /* return error */
            }
            res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_INT_CONF, reg2);                    /* write fifo int conf */
            if (res != 0)                                                                         /* check result */
            {
                handle->running_status = LD3320_STATUS_MP3_ERROR;                                 /* set mp3 error */
                handle->debug_print("ld3320: write fifo int conf failed.\n");                     /* write fifo int conf failed */
                
                return 1;                                                                         /* return error */
            }
            handle->running_status = LD3320_STATUS_MP3_LOAD;                                      /* set load */
        }
    }
    else                                                                                          /* unknown mode */
    {
        handle->debug_print("ld3320: mode is invalid.\n");                                        /* mode is invalid failed */
       
        return 4;                                                                                 /* return error */
    }
    if (handle->running_status == LD3320_STATUS_ASR_FOUND_OK)                                     /* asr ok */
    {
        handle->running_status = LD3320_STATUS_NONE;                                              /* clear status */
        res = a_ld3320_read_byte(handle, LD3320_REG_ASR_RES_1 , (uint8_t *)&i);                   /* read asr index */
        if (res != 0)                                                                             /* check result */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: read asr index failed.\n");                              /* read asr index failed */
            
            return 1;                                                                             /* return error */
        }
        if (i >= handle->len)                                                                     /* check index */
        {
            handle->running_status = LD3320_STATUS_ASR_ERROR;                                     /* set error */
            handle->debug_print("ld3320: index is invalid.\n");                                   /* index is invalid */
            
            return 1;                                                                             /* return error */
        }
        handle->receive_callback(LD3320_STATUS_ASR_FOUND_OK, i, handle->text[i]);                 /* run callback */
    }
    else if (handle->running_status == LD3320_STATUS_ASR_FOUND_ZERO)                              /* asr zero */
    {
        handle->running_status = LD3320_STATUS_NONE;                                              /* clear status */
        handle->receive_callback(LD3320_STATUS_ASR_FOUND_ZERO, 0, NULL);                          /* run the callback */
    }
    else if (handle->running_status == LD3320_STATUS_MP3_END)                                     /* mp3 end */
    {
        handle->running_status = LD3320_STATUS_NONE;                                              /* clear status */
        handle->receive_callback(LD3320_STATUS_MP3_END, 0, NULL);                                 /* run the callback */
    }
    else if (handle->running_status == LD3320_STATUS_MP3_LOAD)                                    /* mp3 load */
    {
        handle->running_status = LD3320_STATUS_ASR_RUNNING;                                       /* clear status */
        handle->receive_callback(LD3320_STATUS_MP3_LOAD, 0, NULL);                                /* run the callback */
    }
    else
    {
        handle->debug_print("ld3320: unknown status.\n");                                         /* unknown status failed */
            
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t ld3320_start(ld3320_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    if (handle->mode == LD3320_MODE_ASR)                                         /* asr mode */
    {
        res = a_ld3320_run_asr(handle);                                          /* run asr */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: run adr failed.\n");                    /* run receive callback failed */
            
            return 1;                                                            /* return error */
        }
        handle->running_status = LD3320_STATUS_ASR_RUNNING;                      /* set running */
    }
    else if(handle->mode == LD3320_MODE_MP3)                                     /* mp3 mode */
    {
        uint8_t volume;
        
        if (handle->point >= handle->size)                                       /* check point */
        {
            handle->debug_print("ld3320: point is the end.\n");                  /* point is the end */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_mp3_init(handle);                                         /* mp3 init */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: mp3 init failed.\n");                   /* mp3 init failed */
            
            return 1;                                                            /* return error */
        }
        volume = 5;
        volume =((15 - volume) & 0x0F) << 2;                                     /* get register data */
        res = a_ld3320_write_byte(handle, LD3320_REG_SPEAKER, volume | 0xC3);    /* set speaker volume */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set speaker volume failed.\n");         /* set speaker volume failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL1, 0x78);     /* enable control1 */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set analog control1 failed.\n");        /* set analog control1 failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_mp3_load(handle);                                         /* load data */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: load data failed.\n");                  /* load data failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_INT_AUX, 0x00);             /* write int aux */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set int aux failed.\n");                /* set int aux failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_CMD, 0x48);                 /* write command */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set command failed.\n");                /* set command failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_MP3_CONF, 0x01);            /* set mp3 conf */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set mp3 conf failed.\n");               /* set mp3 conf failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_INT_CONF, 0x04);            /* set int conf */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set int conf failed.\n");               /* set int conf failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_FIFO_INT_CONF, 0x01);       /* set fifo int conf */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set fifo int conf failed.\n");          /* set fifo int conf failed */
            
            return 1;                                                            /* return error */
        }
        res = a_ld3320_write_byte(handle, LD3320_REG_FEEDBACK, 0x5A);            /* set feedback */
        if (res != 0)                                                            /* check result */
        {
            handle->debug_print("ld3320: set feedback failed.\n");               /* set feedback failed */
            
            return 1;                                                            /* return error */
        }
        handle->running_status = LD3320_STATUS_MP3_RUNNING;                      /* set status */
    }
    else
    {
        handle->debug_print("ld3320: mode is invalid.\n");                       /* mode is invalid */
        
        return 1;
    }
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t ld3320_deinit(ld3320_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    if (handle->mode == LD3320_MODE_ASR)                                 /* asr mode */
    {
        res = a_ld3320_reset(handle);                                    /* reset handle */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: reset failed.\n");              /* reset failed */
            
            return 1;                                                    /* return error */
        }
        res = handle->reset_gpio_deinit();                               /* deinit */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: gpio deinit failed.\n");        /* gpio deinit failed */
            
            return 4;                                                    /* return error */
        }
        res = handle->spi_deinit();                                      /* spi deinit */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: spi deinit failed.\n");         /* spi deinit failed */
            
            return 5;                                                    /* return error */
        }
    }
    else if (handle->mode == LD3320_MODE_MP3)                            /* mp3 mode */
    {
        res = a_ld3320_reset(handle);                                    /* reset handle */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: reset failed.\n");              /* reset failed */
            
            return 1;                                                    /* return error */
        }
        res = handle->reset_gpio_deinit();                               /* deinit */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: gpio deinit failed.\n");        /* gpio deinit failed */
            
            return 4;                                                    /* return error */
        }
        res = handle->spi_deinit();                                      /* spi deinit */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: spi deinit failed.\n");         /* spi deinit failed */
            
            return 5;                                                    /* return error */
        }
        res = handle->mp3_read_deinit();                                 /* close */
        if (res != 0)                                                    /* check result */
        {
            handle->debug_print("ld3320: mp3 deinit failed.\n");         /* deinit failed */
            
            return 6;                                                    /* return error */
        }
    }
    else                                                                 /* invalid mode */
    {
        handle->debug_print("ld3320: mode is invalid.\n");               /* mode is invalid */
        
        return 1;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t ld3320_stop(ld3320_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->mode == LD3320_MODE_ASR)                            /* asr mode */
    {
        res = a_ld3320_reset(handle);                               /* reset handle */
        if (res != 0)                                               /* check result */
        {
            handle->debug_print("ld3320: reset failed.\n");         /* reset failed */
            
            return 1;                                               /* return error */
        }
    }
    else if (handle->mode == LD3320_MODE_MP3)                       /* mp3 mode */
    {
        res = a_ld3320_reset(handle);                               /* reset handle */
        if (res != 0)                                               /* check result */
        {
            handle->debug_print("ld3320: reset failed.\n");         /* reset failed */
            
            return 1;                                               /* return error */
        }
    }
    else                                                            /* invalid mode */
    {
        handle->debug_print("ld3320: mode is invalid.\n");          /* mode is invalid */
        
        return 1;                                                   /* return error */
    }
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t ld3320_set_speaker_volume(ld3320_handle_t *handle, uint8_t volume)
{
    uint8_t res;
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    if (volume > 15)                                                            /* check volume */
    {
        handle->debug_print("ld3320: volume is invalid.\n");                    /* volume is invalid */
        
        return 4;                                                               /* return error */
    }
    
    volume =((15 - volume) & 0x0F) << 2;                                        /* get register data */
    res = a_ld3320_write_byte(handle, LD3320_REG_SPEAKER, volume | 0xC3);       /* set speaker volume */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set speaker volume failed.\n");            /* set speaker volume failed */
        
        return 1;                                                               /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL1, 0x78);        /* enable control1 */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("ld3320: set analog control1 failed.\n");           /* set analog control1 failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ld3320_get_speaker_volume(ld3320_handle_t *handle, uint8_t *volume)
{
    uint8_t res;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }

    res = a_ld3320_read_byte(handle, LD3320_REG_SPEAKER, volume);           /* get speaker volume */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("ld3320: get speaker volume failed.\n");        /* get speaker volume failed */
        
        return 1;                                                           /* return error */
    }
    *volume = (*volume) >> 2;                                               /* set volume */
    *volume = (*volume) & 0xF;                                              /* set volume */
    *volume = 15 - (*volume);                                               /* set volume */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t ld3320_set_headset_volume(ld3320_handle_t *handle, uint8_t volume_left, uint8_t volume_right)
{
    uint8_t res;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    if (volume_left > 15)                                                                   /* check volume_left */
    {
        handle->debug_print("ld3320: volume left is invalid.\n");                           /* volume left is invalid */
        
        return 4;                                                                           /* return error */
    }
    if (volume_right > 15)                                                                  /* check volume_right */
    {
        handle->debug_print("ld3320: volume right is invalid.\n");                          /* volume right is invalid */
        
        return 4;                                                                           /* return error */
    }
    
    volume_left =((15 - volume_left) & 0x0F) << 2;                                          /* get register data */
    res = a_ld3320_write_byte(handle, LD3320_REG_HEADSET_LEFT, volume_left | 0xC3);         /* set headset left volume */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("ld3320: set headset left failed.\n");                          /* set headset left failed */
        
        return 1;                                                                           /* return error */
    }
    volume_right =((15 - volume_right) & 0x0F) << 2;                                        /* get register data */
    res = a_ld3320_write_byte(handle, LD3320_REG_HEADSET_RIGHT, volume_right | 0xC3);       /* set headset right volume */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("ld3320: set headset right failed.\n");                         /* set headset right failed */
        
        return 1;                                                                           /* return error */
    }
    res = a_ld3320_write_byte(handle, LD3320_REG_ANALOG_CONTROL1, 0x78);                    /* enable control1 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("ld3320: set analog control1 failed.\n");                       /* set analog control1 failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t ld3320_get_headset_volume(ld3320_handle_t *handle, uint8_t *volume_left, uint8_t *volume_right)
{
    uint8_t res;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    
    res = a_ld3320_read_byte(handle, LD3320_REG_HEADSET_LEFT, volume_left);         /* get headset left volume */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("ld3320: get headset left failed.\n");                  /* get headset left failed */
        
        return 1;                                                                   /* return error */
    }

    res = a_ld3320_read_byte(handle, LD3320_REG_HEADSET_RIGHT, volume_right);       /* get headset right volume */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("ld3320: get headset right failed.\n");                 /* get headset right failed */
        
        return 1;                                                                   /* return error */
    }
    *volume_left = (*volume_left) >> 2;                                             /* set left volume */
    *volume_left = (*volume_left) & 0xF;                                            /* set left volume */
    *volume_left = 15 - (*volume_left);                                             /* set left volume */
    *volume_right = (*volume_right) >> 2;                                           /* set right volume */
    *volume_right = (*volume_right) & 0xF;                                          /* set right volume */
    *volume_right = 15 - (*volume_right);                                           /* set right volume */
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t ld3320_configure_mp3(ld3320_handle_t *handle, char *name)
{
    uint8_t res;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = handle->mp3_read_init(name, &handle->size);                  /* mp3 read init */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("ld3320: mp3 read init failed.\n");        /* mp3 read init failed */
        
        return 1;                                                      /* return error */
    }
    handle->point = 0;                                                 /* set point 0 */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t ld3320_set_reg(ld3320_handle_t *handle, uint8_t reg, uint8_t data)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }
    
    return a_ld3320_write_byte(handle, reg, data);       /* write data */
}

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
uint8_t ld3320_get_reg(ld3320_handle_t *handle, uint8_t reg, uint8_t *data)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_ld3320_read_byte(handle, reg, data);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an ld3320 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ld3320_info(ld3320_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ld3320_info_t));                         /* initialize ld3320 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
