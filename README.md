# FLASH_STM32F103
Flash memory read, write and erase operations on MCU STM32F103C8T6. In this repository, i will give some information detail and perform how to do with that.
## Memory Distribution ##
As we can see in the picture below, Flash memory is organized as a main block of up to 64 Kb (or 128 Kb) divided into **128 pages**, **each page 1 Kbytes** (medium-density devices) and 1 information block (information block). Starting from address **0x0800 0000**, program memory will be stored here
![Image](https://i.postimg.cc/KzWRfM2g/848a8400-98f6-4e9d-a3b7-579615b00681.jpg)
