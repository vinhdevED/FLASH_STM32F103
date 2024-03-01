<h1 align="center">⚡ Interact with Flash Memory in STM32F103 ⚡</h1>
Flash memory read, write and erase operations on MCU STM32F103C8T6. In this repository, i will give some information detail and perform how to do with that. These knowledge in small project will support for Next Topic: "Firmware Over The Air"

## Memory Distribution ##
As we can see in the picture below, Flash memory is organized as a main block of up to 64 Kb (or 128 Kb) divided into **128 pages**, **each page 1 Kbytes** (medium-density devices) and 1 information block (information block). Starting from address **0x0800 0000**, program memory will be stored here

![Image](https://i.postimg.cc/KzWRfM2g/848a8400-98f6-4e9d-a3b7-579615b00681.jpg)

## 📊 Analysis and Explain  ##
### 🖋️ Write Data in Flash ###
Before writing data to flash memory, you need to initialize the flash interface and unlock with **HAL_FLASH_Unlock()** function. After that, you must erase the flash page where your data is stored. Erasing is typically done at the beginning and then you can proceed to write the new data. Next, modifying the erase init structure is very important because of suitable project. Some specific structure following below:
1. Type Erase
   ```
   EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES
   ```
   > The flash memory is divided into pages, and erasing is often done at the page level.
2. Page Address
   ```
   EraseInitStruct.PageAddress = StartPage
   ```
   > The variable StartPage likely holds the address of the first page you want to erase.
3. NbPage( Number of Page )
   ```
   EraseInitStruct.NbPages = ((EndPage - StartPage)/FLASH_PAGE_SIZE) + 1
   ```
   > Here, you define the number of pages to be erased.  The calculation determines how many pages are between _StartPage_ and _EndPage_. **FLASH_PAGE_SIZE** represents the size of one page in bytes.
   
   > For example, if StartPage is 0x08010000 and EndPage is 0x0801FFFF, and FLASH_PAGE_SIZE is 2048 bytes, the number of pages to be erased would be ((0x0801FFFF - 0x08010000)/2048) + 1 = 64.
### 📰 Read Data in Flash ###


## Result ##
