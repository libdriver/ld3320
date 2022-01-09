[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver LD3320

[![API](https://img.shields.io/badge/api-reference-blue)](https://www.libdriver.com/docs/ld3320/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

LD3320 芯片是一款“语音识别”专用芯片。该芯片集成了语音识别处理器和一些外部电路，包括AD、DA 转换器、麦克风接口、声音输出接口等。本芯片不需要外接任何的辅助芯片如Flash、RAM 等，直接集成在现有的产品中即可以实现语音识别/声控/人机对话功能。并且识别的关键词语列表是可以任意动态编辑的。该芯片被应用于电磁炉、微波炉、智能家电操作、导航仪、 MP3、MP4 、自动售货机、公共照明系统、卫生系统和智能家居声控等。

LibDriver LD3320 是LibDriver推出的LD3320的全功能驱动，该驱动提供语音识别、MP3播放等功能。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example asr](#example-asr)
    - [example mp3](#example-mp3)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver LD3320的源文件。

/interface目录包含了LibDriver LD3320与平台无关的SPI总线模板。

/test目录包含了LibDriver LD3320驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver LD3320编程范例。

/doc目录包含了LibDriver LD3320离线文档。

/datasheet目录包含了LD3320数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的SPI总线模板，完成指定平台的SPI总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

#### example asr

```C
volatile uint8_t res;
volatile uint32_t timeout;
volatile uint8_t g_flag;
volatile char text[50];

static uint8_t _asr_callback(uint8_t type, uint8_t index, char *text)
{
    volatile uint8_t res;
    
    if (type == LD3320_STATUS_ASR_FOUND_OK)
    {
        ld3320_interface_debug_print("ld3320: detect index %d %s.\n", index, text);
        gs_flag = 1;
    }
    else if (type == LD3320_STATUS_ASR_FOUND_ZERO)
    {
        ld3320_interface_debug_print("ld3320: irq zero.\n");
        ld3320_asr_start();
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknow type.\n");
    }
    
    return 0;
}

res = gpio_interrupt_init();
if (res)
{
    return 1;
}
res = ld3320_asr_init(_asr_callback);
if (res)
{
    gpio_interrupt_deinit();

    return 1;
}
memset(text, 0, sizeof(char) * 50);
memcpy(text, "ni hao", strlen("ni hao"));
res = ld3320_asr_set_keys(text, 1);
if (res)
{
    ld3320_asr_deinit();
    gpio_interrupt_deinit();

    return 1;
}
gs_flag = 0;
res = ld3320_asr_start();
if (res)
{
    ld3320_asr_deinit();
    gpio_interrupt_deinit();

    return 1;
}

...

timeout = 1000 * 10;
while (timeout)
{
    if (gs_flag)
    {
        break;
    }
    timeout--;
    ld3320_interface_delay_ms(1);
}
if (timeout == 0)
{
    ld3320_interface_debug_print("ld3320: wait timeout.\n");
    ld3320_asr_deinit();
    gpio_interrupt_deinit();

    return 1;
}

...

ld3320_interface_debug_print("ld3320: found key word.\n");
ld3320_asr_deinit();
gpio_interrupt_deinit();

...

return 0;
```

#### example mp3

```C
volatile uint8_t res;
volatile uint32_t timeout;
volatile uint8_t g_flag;

static uint8_t _mp3_callback(uint8_t type, uint8_t index, char *text)
{
    if (type == LD3320_STATUS_MP3_LOAD)
    {
        ld3320_interface_debug_print("ld3320: irq mp3 load.\n");
    }
    else if (type == LD3320_STATUS_MP3_END)
    {
        gs_flag = 1;
        ld3320_interface_debug_print("ld3320: irq mp3 end.\n");
    }
    else if (type == LD3320_STATUS_MP3_LOAD)
    {
        ld3320_interface_debug_print("ld3320: irq mp3 load.\n");
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknow type.\n");
    }
    
    return 0;
}

res = gpio_interrupt_init();
if (res)
{
    return 1;
}
res = ld3320_mp3_init("xxx.mp3", _mp3_callback);
if (res)
{
    gpio_interrupt_deinit();

    return 1;
}
gs_flag = 0;
res = ld3320_mp3_start();
if (res)
{
    ld3320_mp3_deinit();
    gpio_interrupt_deinit();

    return 1;
}

...
    
timeout = 1000 * 60 * 10;
while (timeout)
{
    if (gs_flag)
    {
        break;
    }
    timeout--;
    ld3320_interface_delay_ms(1);
}
if (timeout == 0)
{
    ld3320_interface_debug_print("ld3320: wait timeout.\n");
    ld3320_mp3_deinit();
    gpio_interrupt_deinit();

    return 1;
}

...

ld3320_interface_debug_print("ld3320: play end.\n");
ld3320_mp3_deinit();
gpio_interrupt_deinit();

...

return 0;
```

### 文档

在线文档: https://www.libdriver.com/docs/ld3320/index.html

离线文档: /doc/html/index.html

### 贡献

请联系lishifenging@outlook.com

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com