[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LD3320

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ld3320/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

Ld3320は、音声認識用の特別なチップです。 このチップは、音声認識プロセッサと、AD、DAコンバータ、マイクインターフェイス、音声出力インターフェイスなどのいくつかの外部回路を統合しています。 このチップは、フラッシュ、RAMなどの補助チップを必要としません。既存の製品に直接統合して、音声認識/音声制御/人間とコンピュータの対話機能を実現できます。 キーワードリストは動的に編集できます。 このチップは、電磁調理器、電子レンジ、スマート家電操作、ナビゲーター、MP3、MP4、自動販売機、公共照明システム、健康システム、スマートホーム音声制御で使用されます。

LibDriver LD3320は、LibDriverによって起動されたLD3320の全機能ドライバーであり、音声認識、MP3再生、およびその他の機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example asr](#example-asr)
    - [example mp3](#example-mp3)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver LD3320のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver LD3320用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver LD3320ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver LD3320プログラミング例が含まれています。

/ docディレクトリには、LibDriver LD3320オフラインドキュメントが含まれています。

/ datasheetディレクトリには、LD3320データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/ld3320/index.html](https://www.libdriver.com/docs/ld3320/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。