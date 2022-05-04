### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8

gpio pin: RSTB/INTB GPIO27/GPIO17

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```

### 3. ld3320

#### 3.1 command Instruction

​           ld3320 is a basic command which can test all ld3320 driver function:

#### 3.1 command Instruction

​           ld3320 is a basic command which can test all ld3320 driver function:

​           -i        show ld3320 chip and driver information.

​           -h       show ld3320 help.

​           -p       show ld3320 pin connections of the current board.

​           -t (reg | asr | mp3 -f <filepath>)

​           -t  reg        run ld3320 register test.

​           -t asr        run ld3320 asr test.

​           -t mp3 -f <filepath>        run ld3320 mp3 test.filepath is the mp3 music file path.

​           -c (asr -k <keywords> | mp3 -f <filepath>)

​           -c asr -k <keywords>        run ld3320 asr function.keywords is the asr key words. 

​           -c mp3 -f <filepath>        run ld3320 mp3 function.filepath is mp3 music file path. 

#### 3.2 command example

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
ld3320: set volume 0x00.
ld3320: check volume ok.
ld3320: ld3320_set_headset_volume/ld3320_get_headset_volume test.
ld3320: set left volume 0x0B.
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
./ld3320 -t mp3 -f ./music/we-are-the-world.mp3

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
ld3320: play 0:we are the world.mp3.
ld3320: irq mp3 load.
.
.
.
ld3320: irq mp3 end.
ld3320: irq mp3 load.
ld3320: finish mp3 test.
```

```shell
./ld3320 -c asr -k ni-hao

ld3320: key word is ni hao.
ld3320: irq zero.
ld3320: detect index 0 ni hao.
ld3320: found key word.
```

```shell
./ld3320 -c mp3 -f ./music/we-are-the-world.mp3

ld3320: play 0:we are the world.mp3.
ld3320: irq mp3 load.
.
.
.
ld3320: play end.
```

```shell
./ld3320 -h

ld3320 -i
	show ld3320 chip and driver information.
ld3320 -h
	show ld3320 help.
ld3320 -p
	show ld3320 pin connections of the current board.
ld3320 -t reg
	run ld3320 register test.
ld3320 -t asr
	run ld3320 asr test.
ld3320 -t mp3 -f <filepath>
	run ld3320 mp3 test.filepath is the mp3 music file path.
ld3320 -c asr -k <keywords>
	run ld3320 asr function.keywords is the asr key words.
ld3320 -c mp3 -f <filepath>
	run ld3320 mp3 function.filepath is mp3 music file path.
```

