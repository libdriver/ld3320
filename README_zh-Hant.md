[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LD3320

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ld3320/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

LD3320 芯片是一款“語音識別”專用芯片。該芯片集成了語音識別處理器和一些外部電路，包括AD、DA 轉換器、麥克風接口、聲音輸出接口等。本芯片不需要外接任何的輔助芯片如Flash、RAM 等，直接集成在現有的產品中即可以實現語音識別/聲控/人機對話功能。並且識別的關鍵詞語列表是可以任意動態編輯的。該芯片被應用於電磁爐、微波爐、智能家電操作、導航儀、 MP3、MP4 、自動售貨機、公共照明系統、衛生系統和智能家居聲控等。

LibDriver LD3320 是LibDriver推出的LD3320的全功能驅動，該驅動提供語音識別、MP3播放等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example asr](#example-asr)
    - [example mp3](#example-mp3)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver LD3320的源文件。

/interface目錄包含了LibDriver LD3320與平台無關的SPI總線模板。

/test目錄包含了LibDriver LD3320驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver LD3320編程範例。

/doc目錄包含了LibDriver LD3320離線文檔。

/datasheet目錄包含了LD3320數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example asr

```C
#include "driver_ld3320_asr.h"

uint8_t res;
uint32_t timeout;
uint8_t g_flag;
char text[50];

static void a_asr_callback(uint8_t type, uint8_t index, char *text)
{
    uint8_t res;
    
    if (type == LD3320_STATUS_ASR_FOUND_OK)
    {
        ld3320_interface_debug_print("ld3320: detect index %d %s.\n", index, text);
        gs_flag = 1;
    }
    else if (type == LD3320_STATUS_ASR_FOUND_ZERO)
    {
        ld3320_interface_debug_print("ld3320: irq zero.\n");
        (void)ld3320_asr_start();
    }
    else
    {
        ld3320_interface_debug_print("ld3320: irq unknow type.\n");
    }
}

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
res = ld3320_asr_init(a_asr_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}
memset(text, 0, sizeof(char) * 50);
strcpy(text, "ni hao";
res = ld3320_asr_set_keys(text, 1);
if (res != 0)
{
    (void)ld3320_asr_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}
gs_flag = 0;
res = ld3320_asr_start();
if (res != 0)
{
    (void)ld3320_asr_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}

...

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
if (timeout == 0)
{
    ld3320_interface_debug_print("ld3320: wait timeout.\n");
    (void)ld3320_asr_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}

...

ld3320_interface_debug_print("ld3320: found key word.\n");
(void)ld3320_asr_deinit();
(void)gpio_interrupt_deinit();

...

return 0;
```

#### example mp3

```C
#include "driver_ld3320_mp3.h"

uint8_t res;
uint32_t timeout;
uint8_t g_flag;

static uint8_t a_mp3_callback(uint8_t type, uint8_t index, char *text)
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
}

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
res = ld3320_mp3_init("xxx.mp3", a_mp3_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();

    return 1;
}
gs_flag = 0;
res = ld3320_mp3_start();
if (res != 0)
{
    (void)ld3320_mp3_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}

...
    
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
    (void)ld3320_mp3_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}

...

ld3320_interface_debug_print("ld3320: play end.\n");
(void)ld3320_mp3_deinit();
(void)gpio_interrupt_deinit();

...

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/ld3320/index.html](https://www.libdriver.com/docs/ld3320/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。