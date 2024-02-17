[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LD3320

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ld3320/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

Ld3320은 음성 인식을 위한 특수 칩입니다. 이 칩은 음성 인식 프로세서와 AD, DA 변환기, 마이크 인터페이스, 음성 출력 인터페이스 등을 포함한 일부 외부 회로를 통합합니다. 이 칩은 FLASH, RAM 등과 같은 보조 칩이 필요하지 않습니다. 기존 제품에 직접 통합하여 음성 인식/음성 제어/인간-컴퓨터 대화 기능을 실현할 수 있습니다. 키워드 목록은 동적으로 편집할 수 있습니다. 이 칩은 인덕션 쿠커, 전자레인지, 스마트 가전 조작, 네비게이터, MP3, MP4, 자판기, 공공 조명 시스템, 건강 시스템 및 스마트 홈 음성 제어에 사용됩니다.

LibDriver LD3320은 LibDriver에서 출시한 LD3320의 전체 기능 드라이버입니다. 음성 인식, MP3 재생 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example asr](#example-asr)
    - [example mp3](#example-mp3)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver LD3320의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver LD3320용 플랫폼 독립적인 SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver LD3320드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver LD3320프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver LD3320오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 LD3320데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인SPI버스 템플릿을 참조하여 지정된 플랫폼에 대한 SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/ld3320/index.html](https://www.libdriver.com/docs/ld3320/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.