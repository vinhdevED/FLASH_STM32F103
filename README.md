<h1 align="center">⚡ Interact with Flash Memory in STM32F103 ⚡</h1>
Flash memory read, write and erase operations on MCU STM32F103C8T6. In this repository, i will give some information detail and perform how to do with that. These knowledge in small project will support for Next Topic: "Firmware Over The Air"

## Memory Distribution ##
As we can see in the picture below, Flash memory is organized as a main block of up to 64 Kb (or 128 Kb) divided into **128 pages**, **each page 1 Kbytes** (medium-density devices) and 1 information block (information block). Starting from address **0x0800 0000**, program memory will be stored here

![Image](https://i.postimg.cc/KzWRfM2g/848a8400-98f6-4e9d-a3b7-579615b00681.jpg)

## 📊 Analysis and Explain  ##
### 🖋️ Write Data in Flash ###
Before writing data to flash memory, you need to initialize the flash interface and unlock with `HAL_FLASH_Unlock()` function. After that, you must erase the flash page where your data is stored. Erasing is typically done at the beginning and then you can proceed to write the new data. Next, modifying the erase init structure is very important because of suitable project. Some specific structure following below:
* Type Erase
   ```
   EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES
   ```
   > The flash memory is divided into pages, and erasing is often done at the page level.
* Page Address
   ```
   EraseInitStruct.PageAddress = StartPage
   ```
   > The variable StartPage likely holds the address of the first page you want to erase.
* NbPage( Number of Page )
   ```
   EraseInitStruct.NbPages = ((EndPage - StartPage)/FLASH_PAGE_SIZE) + 1
   ```
   > Here, you define the number of pages to be erased.  The calculation determines how many pages are between _StartPage_ and _EndPage_. `FLASH_PAGE_SIZE` represents the size of one page in bytes.
   
   > For example, if StartPage is 0x08010000 and EndPage is 0x0801FFFF, and FLASH_PAGE_SIZE is 1024 bytes, the number of pages to be erased would be `((0x0801FFFF - 0x08010000)/1024) + 1 = 64`

Some specific of this structure is then likely passed to the `HAL_FLASHEx_Erase(EraseInitStruct,.....)` function to perform the actual ensure. Now, you can write your data which can use for/while loop thourgh each words by numberofword. As line code below, it uses type of word and add for 4 after looping because 1 word = 4 bytes (32-bits). So, it need move 4 address to the next suitable place to write data:
```
HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]);
StartPageAddress += 4;  
sofar++;
```
After successfully writing the data, we need to lock the flash memory to avoid unusual accesses.
### 📰 Read Data in Flash ###
To read data from flash memory, you simply need to access the memory location where your data is stored. RxBuf is the bucket which is the address 32 bit address and  stores the data. You need to know the number of word to read from flash. 
```
*RxBuf = *(__IO uint32_t *)StartPageAddress;
```
## Result ##
In this project, we use STM32CubeIDE to interact with Flash Memory Information. 
+ In picture 1, string array is the result for converting Flash Read Data saved to buffer.

![Image](https://i.postimg.cc/QMVBzP25/7e85f2ed-afa1-4a11-b058-c3e5f3d72324.jpg)

+ In picture 2, using feature Memory to observe any flash address and tradition style of hex.

![Image](https://i.postimg.cc/fbV0gLpD/0ac1576f-b75f-49aa-80bf-45315985c842.jpg)
