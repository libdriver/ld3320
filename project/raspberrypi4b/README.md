### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

GPIO Pin: RSTB/INTB GPIO27/GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ld3320 REQUIRED)
```

### 3. LD3320

#### 3.1 Command Instruction

1. Show ld3320 chip and driver information.

   ```shell
   ld3320 (-i | --information)
   ```

2. Show ld3320 help.

   ```shell
   ld3320 (-h | --help)
   ```

3. Show ld3320 pin connections of the current board.

   ```shell
   ld3320 (-p | --port)
   ```

4. Run ld3320 register test.

   ```shell
   ld3320 (-t reg | --test=reg)
   ```

5. Run ld3320 asr test.

   ```shell
   ld3320 (-t asr | --test=asr)
   ```

6. Run ld3320 mp3 test, path is the mp3 music file path.

   ```shell
   ld3320 (-t mp3 | --test=mp3) [--file=<path>]
   ```

7. Run ld3320 asr function, word is the asr key word. 

   ```shell
   ld3320 (-e asr | --example=asr) [--keyword=<word>]
   ```

8. Run ld3320 mp3 function, path is mp3 music file path. 

   ```shell
   ld3320 (-e mp3 | --example=mp3) [--file=<path>]
   ```

#### 3.2 Command Example

```shell
./ld3320 -i

ld3320: chip is IC Route LD3320.
ld3320: manufacturer is IC Route.
ld3320: interface is SPI.
ld3320: driver version is 1.0.
ld3320: min supply voltage is 3.0V.
ld3320: max supply voltage is 3.3V.
ld3320: max current is 166.70mA.
ld3320: max temperature is 85.0C.
ld3320: min temperature is -40.0C.
```

```shell
./ld3320 -p

ld3320: SCK connected to GPIO11(BCM).
ld3320: MISO connected to GPIO9(BCM).
ld3320: MOSI connected to GPIO10(BCM).
ld3320: CS connected to GPIO8(BCM).
ld3320: SCK connected to P2.
ld3320: MISO connected to P1.
ld3320: MOSI connected to P0.
ld3320: CS connected to CSB.
ld3320: MD connected to VCC.
ld3320: WRB connected to GND.
ld3320: RSTB connected to GPIO27(BCM).
ld3320: INTB connected to GPIO17(BCM).
```

```shell
./ld3320 -t reg

ld3320: chip is IC Route LD3320.
ld3320: manufacturer is IC Route.
ld3320: interface is SPI.
ld3320: driver version is 1.0.
ld3320: min supply voltage is 3.0V.
ld3320: max supply voltage is 3.3V.
ld3320: max current is 166.70mA.
ld3320: max temperature is 85.0C.
ld3320: min temperature is -40.0C.
ld3320: start register test.
ld3320: ld3320_set_mode/ld3320_get_mode test.
ld3320: set mp3 mode.
ld3320: check mode ok.
ld3320: set asr mode.
ld3320: check mode ok.
ld3320: ld3320_set_key_words/ld3320_get_key_words test.
ld3320: set key words "ni hao".
ld3320: check key words ok.
ld3320: ld3320_set_mic_gain/ld3320_get_mic_gain test.
ld3320: set mic common gain.
ld3320: check mic gain ok.
ld3320: set mic noise gain.
ld3320: check mic gain ok.
ld3320: ld3320_set_vad/ld3320_get_vad test.
ld3320: set vad common.
ld3320: check vad ok.
ld3320: set vad far.
ld3320: check vad ok.
ld3320: ld3320_set_speaker_volume/ld3320_get_speaker_volume test.
ld3320: set volume 0x07.
ld3320: check volume ok.
ld3320: ld3320_set_headset_volume/ld3320_get_headset_volume test.
ld3320: set left volume 0x06.
ld3320: set right volume 0x09.
ld3320: check left volume ok.
ld3320: check right volume ok.
ld3320: ld3320_get_status test.
ld3320: status is 0x00.
ld3320: finish register test.
```

```shell
./ld3320 -t asr

ld3320: chip is IC Route LD3320.
ld3320: manufacturer is IC Route.
ld3320: interface is SPI.
ld3320: driver version is 1.0.
ld3320: min supply voltage is 3.0V.
ld3320: max supply voltage is 3.3V.
ld3320: max current is 166.70mA.
ld3320: max temperature is 85.0C.
ld3320: min temperature is -40.0C.
ld3320: start asr test.
ld3320: please speak hello.
ld3320: detect index 0 ha lou.
ld3320: finish asr test.
```

```shell
./ld3320 -t mp3 --file=./music/we-are-the-world.mp3

ld3320: chip is IC Route LD3320.
ld3320: manufacturer is IC Route.
ld3320: interface is SPI.
ld3320: driver version is 1.0.
ld3320: min supply voltage is 3.0V.
ld3320: max supply voltage is 3.3V.
ld3320: max current is 166.70mA.
ld3320: max temperature is 85.0C.
ld3320: min temperature is -40.0C.
ld3320: start mp3 test.
ld3320: play ./music/we are the world.mp3.
ld3320: irq mp3 end.
ld3320: finish mp3 test.
```

```shell
./ld3320 -e asr --keyword=ha-lou

ld3320: key word is ha lou.
ld3320: detect index 0 ha lou.
ld3320: found key word.
```

```shell
./ld3320 -e mp3 --file=./music/we-are-the-world.mp3

ld3320: play ./music/we are the world.mp3.
ld3320: irq mp3 end.
ld3320: play end.
```

```shell
./ld3320 -h

Usage:
  ld3320 (-i | --information)
  ld3320 (-h | --help)
  ld3320 (-p | --port)
  ld3320 (-t reg | --test=reg)
  ld3320 (-t asr | --test=asr)
  ld3320 (-t mp3 | --test=mp3) [--file=<path>]
  ld3320 (-e asr | --example=asr) [--keyword=<word>]
  ld3320 (-e mp3 | --example=mp3) [--file=<path>]

Options:
  -e <asr | mp3>, --example=<asr | mp3>
                          Run the driver example.
      --file=<path>       Set the mp3 file path.([default: test.mp3])
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --keyword=<word>    Set the asr keyword.([default: ha-lou])
  -p, --port              Display the pin connections of the current board.
  -t <reg | asr | mp3>, --test=<reg | asr | mp3>
                          Run the driver test.
```
