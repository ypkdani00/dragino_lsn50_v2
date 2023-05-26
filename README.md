=============================================================
# Dragino LSN50 & LSN50-v2 LoRa STM32 Source Code
  Repackaged to build with STM32CubeIDE
=============================================================

This is a copy of the repository [LoRa_STM32](https://github.com/dragino/LoRa_STM32) from Dragino. 

The repository contains the sources to build a sample LoRa application for their [LSN50](http://www.dragino.com/products/lora/item/128-lsn50.html) 
and [LSN50-v2](https://www.dragino.com/products/lora-lorawan-end-node/item/155-lsn50-v2.html).

The core of the code is unmodified, but in this version is added the WNK8010-TT sensor I2C.
See the code:
- Driver/BSP/Components/wnk8010
- Src/bsp.c
- Src/main.c

Tha packeg peylaod is:
- 2byte Battery Voltage
- 3byte RAW Pressure value WNK8010
- 2byte Temperature value WNK8010

or search the #define USE_WNK8010

Tested this with the LSN50-v2 and STM32CubeIDE 1.12.1.

[Contact me](https://www.danielecortellazzi.it/contact/) if you need embedded customizations.
