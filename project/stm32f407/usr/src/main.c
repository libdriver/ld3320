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
 * @file      main.c
 * @brief     main source file
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
#include "driver_ld3320_asr_test.h"
#include "driver_ld3320_mp3_test.h"
#include "driver_ld3320_asr.h"
#include "driver_ld3320_mp3.h"
#include "shell.h"
#include "clock.h"
#include "gpio.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t volatile g_len;                   /**< uart buffer length */
static volatile uint8_t gs_flag;           /**< global flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     asr callback
 * @param[in] type is the irq type
 * @param[in] i is the asr index
 * @param[in] *text points to a asr result buffer
 * @note      none
 */
static void a_asr_callback(uint8_t type, uint8_t i, char *text)
{
    uint8_t res;
    
    if (type == LD3320_STATUS_ASR_FOUND_OK)
    {
        ld3320_interface_debug_print("ld3320: detect index %d %s.\n", i, text);
        
        /* flag found */
        gs_flag = 1;
    }
    else if (type == LD3320_STATUS_ASR_FOUND_ZERO)
    {
        ld3320_interface_debug_print("ld3320: irq zero.\n");
        
        /* start */
        res = ld3320_asr_start();
        if (res != 0)
        {
            ld3320_interface_debug_print("ld3320: start failed.\n");
        }
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknown type.\n");
    }
}

/**
 * @brief     mp3 callback
 * @param[in] type is the irq type
 * @param[in] i is the asr index
 * @param[in] *text points to a asr result buffer
 * @note      none
 */
static void a_mp3_callback(uint8_t type, uint8_t i, char *text)
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
 * @brief     ld3320 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ld3320(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"file", required_argument, NULL, 1},
        {"keyword", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char path[73]= "test.mp3";
    char key[33]= "ha-lou";
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* file path */
            case 1 :
            {
                /* set the file path */
                memset(path, 0, sizeof(char) * 73);
                strncpy(path, optarg, 72);
                
                break;
            }
            
            /* keyword */
            case 2 :
            {
                /* set the keyword */
                memset(key, 0, sizeof(char) * 33);
                strncpy(key, optarg, 32);
                
                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run the register test */
        res = ld3320_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_asr", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = ld3320_asr_test_irq_handler;
        
        /* set the asr test */
        res = ld3320_asr_test();
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* gpio deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("t_mp3", type) == 0)
    {
        uint8_t res;
        uint16_t i, len;
        
        /* replace the - */
        len = (uint16_t)strlen(path);
        for (i = 0; i < len; i++)
        {
            if (path[i] == '-')
            {
                path[i] = ' ';
            }
        }
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = ld3320_mp3_test_irq_handler;
        
        /* run the mp3 test */
        res = ld3320_mp3_test(path);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* gpio deinit */
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("e_asr", type) == 0)
    {
        uint8_t res;
        char text[1][50];
        uint16_t i, len;
        uint32_t timeout;
        
        /* replace the - */
        len = (uint16_t)strlen(key);
        for (i = 0; i < len; i++)
        {
            if (key[i] == '-')
            {
                key[i] = ' ';
            }
        }
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = ld3320_asr_irq_handler; 
        
        /* output */
        ld3320_interface_debug_print("ld3320: key word is %s.\n", key);
        
        /* set the asr callback */
        res = ld3320_asr_init(a_asr_callback);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* set the keywords */
        memset(text[0], 0, sizeof(char) * 50);
        memcpy(text[0], key, strlen(key));
        res = ld3320_asr_set_keys(text, 1);
        if (res != 0)
        {
            (void)ld3320_asr_deinit();
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* start asr */
        gs_flag = 0;
        res = ld3320_asr_start();
        if (res != 0)
        {
            (void)ld3320_asr_deinit();
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* waiting */
        timeout = 1000 * 10;
        while (timeout != 0)
        {
            if (gs_flag != 0)
            {
                break;
            }
            timeout--;
            ld3320_interface_delay_ms(1);
        }
        
        /* check the result */
        if (timeout == 0)
        {
            ld3320_interface_debug_print("ld3320: wait timeout.\n");
            (void)ld3320_asr_deinit();
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();

            return 1;
        }
        ld3320_interface_debug_print("ld3320: found key word.\n");
        
        /* deinit */
        (void)ld3320_asr_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("e_mp3", type) == 0)
    {
        uint8_t res;
        uint16_t i, len;
        uint32_t timeout;
        
        /* replace the - */
        len = (uint16_t)strlen(path);
        for (i = 0; i < len; i++)
        {
            if (path[i] == '-')
            {
                path[i] = ' ';
            }
        }
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = ld3320_mp3_irq_handler;
        ld3320_interface_debug_print("ld3320: play %s.\n", path);
        
        /* mp3 init */
        res = ld3320_mp3_init(path, a_mp3_callback);
        if (res != 0)
        {
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* start mp3 */
        gs_flag = 0;
        res = ld3320_mp3_start();
        if (res != 0)
        {
            (void)ld3320_mp3_deinit();
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        
        /* waiting */
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
        
        /* check the result */
        if (timeout == 0)
        {
            ld3320_interface_debug_print("ld3320: wait timeout.\n");
            (void)ld3320_mp3_deinit();
            g_gpio_irq = NULL;
            (void)gpio_interrupt_deinit();
            
            return 1;
        }
        ld3320_interface_debug_print("ld3320: play end.\n");
        
        /* deinit */
        (void)ld3320_mp3_deinit();
        g_gpio_irq = NULL;
        (void)gpio_interrupt_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ld3320_interface_debug_print("Usage:\n");
        ld3320_interface_debug_print("  ld3320 (-i | --information)\n");
        ld3320_interface_debug_print("  ld3320 (-h | --help)\n");
        ld3320_interface_debug_print("  ld3320 (-p | --port)\n");
        ld3320_interface_debug_print("  ld3320 (-t reg | --test=reg)\n");
        ld3320_interface_debug_print("  ld3320 (-t asr | --test=asr)\n");
        ld3320_interface_debug_print("  ld3320 (-t mp3 | --test=mp3) [--file=<path>]\n");
        ld3320_interface_debug_print("  ld3320 (-e asr | --example=asr) [--keyword=<word>]\n");
        ld3320_interface_debug_print("  ld3320 (-e mp3 | --example=mp3) [--file=<path>]\n");
        ld3320_interface_debug_print("\n");
        ld3320_interface_debug_print("Options:\n");
        ld3320_interface_debug_print("  -e <asr | mp3>, --example=<asr | mp3>\n");
        ld3320_interface_debug_print("                          Run the driver example.\n");
        ld3320_interface_debug_print("      --file=<path>       Set the mp3 file path.([default: test.mp3])\n");
        ld3320_interface_debug_print("  -h, --help              Show the help.\n");
        ld3320_interface_debug_print("  -i, --information       Show the chip information.\n");
        ld3320_interface_debug_print("      --keyword=<word>    Set the asr keyword.([default: ha-lou])\n");
        ld3320_interface_debug_print("  -p, --port              Display the pin connections of the current board.\n");
        ld3320_interface_debug_print("  -t <reg | asr | mp3>, --test=<reg | asr | mp3>\n");
        ld3320_interface_debug_print("                          Run the driver test.\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ld3320_info_t info;
        
        /* print ld3320 info */
        ld3320_info(&info);
        ld3320_interface_debug_print("ld3320: chip is %s.\n", info.chip_name);
        ld3320_interface_debug_print("ld3320: manufacturer is %s.\n", info.manufacturer_name);
        ld3320_interface_debug_print("ld3320: interface is %s.\n", info.interface);
        ld3320_interface_debug_print("ld3320: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ld3320_interface_debug_print("ld3320: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ld3320_interface_debug_print("ld3320: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ld3320_interface_debug_print("ld3320: max current is %0.2fmA.\n", info.max_current_ma);
        ld3320_interface_debug_print("ld3320: max temperature is %0.1fC.\n", info.temperature_max);
        ld3320_interface_debug_print("ld3320: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ld3320_interface_debug_print("ld3320: SCK connected to GPIOA PIN5.\n");
        ld3320_interface_debug_print("ld3320: MISO connected to GPIOA PIN6.\n");
        ld3320_interface_debug_print("ld3320: MOSI connected to GPIOA PIN7.\n");
        ld3320_interface_debug_print("ld3320: CS connected to GPIOA PIN4.\n");
        ld3320_interface_debug_print("ld3320: SCK connected to P2.\n");
        ld3320_interface_debug_print("ld3320: MISO connected to P1.\n");
        ld3320_interface_debug_print("ld3320: MOSI connected to P0.\n");
        ld3320_interface_debug_print("ld3320: CS connected to CSB.\n");
        ld3320_interface_debug_print("ld3320: MD connected to VCC.\n");
        ld3320_interface_debug_print("ld3320: WRB connected to GND.\n");
        ld3320_interface_debug_print("ld3320: RSTB connected to GPIOA PIN8.\n");
        ld3320_interface_debug_print("ld3320: INTB connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register ld3320 function */
    shell_init();
    shell_register("ld3320", ld3320);
    uart_print("ld3320: welcome to libdriver ld3320.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ld3320: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ld3320: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ld3320: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ld3320: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ld3320: param is invalid.\n");
            }
            else
            {
                uart_print("ld3320: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
