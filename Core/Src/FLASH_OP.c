/*
 * FLASH_OP.c
 *
 *  Created on: Feb 24, 2024
 *      Author: trand
 */


#include "FLASH_OP.h"
#include "string.h"
#include "stdio.h"

/* STM32F103 have 128 PAGES (Page 0 to Page 127) of 1 KB each.
 * This makes up 128 KB Flash Memory
 * FLASH_PAGE_SIZE should be able to get the size of the Page according to the controller
 * Address Memory usually use unsigned 32 bit integer
 */

static uint32_t GetPage(uint32_t address){
	for(int idx = 0;idx <128;idx ++){
		/* Page Boundary Check
		 * Starting Address of FLASH MEMORY - 0x0800 0000
		 */
		if( (address <(0x08000000 + FLASH_PAGE_SIZE*(idx+1)) ) && (address >= (0x08000000 + FLASH_PAGE_SIZE*idx))){
			return (0x0800000 + FLASH_PAGE_SIZE*idx);
		}
	}
	return 0;
}

uint8_t bytes_temp[4];


/* Convert float into an array of four bytes (uint8_t)
 * Union allows you to store different data types in the same memory location
 * A float typically occupies << 4 bytes >>
 * */
void float2Bytes(uint8_t * ftoa_bytes_temp, float float_variable){
	union {
		float a;
		uint8_t bytes[4];
	} thing;

	thing.a = float_variable;
	for(uint8_t i=0; i <4;i++){
		//  Copying the individual bytes of the float into the output array.
		ftoa_bytes_temp[i] = thing.bytes[i];
	}
}

float Bytes2float(uint8_t * ftoa_bytes_temp){
	union{
		float a;
		uint8_t bytes[4];
	} thing;

	for(uint8_t i=0;i<4;i++){
		thing.bytes[i] = ftoa_bytes_temp[i];
	}

	float float_variable = thing.a;
	return float_variable;
}

uint32_t Flash_Write_Data(uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords){

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar = 0;


	/* Unlock the flash to enable the flash control register access */
	HAL_FLASH_Unlock();

	/* Erase the user Flash Area */

	uint32_t StartPage = GetPage(StartPageAddress);
	uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	uint32_t EndPage = GetPage(EndPageAdress);

	/* Fill Erase Structure */
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = StartPage;
	/* Calculate the number of pages to be erased and stores in NbPages */
	EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK){

		/*Error occurred while page erase.*/
		 return HAL_FLASH_GetError();
	}

	/* Program the user Flash area word by word*/
	while(sofar <numberofwords){
		/* Specifies word-level programming mode
		 * Increase by 4 to point to the next word address << assuming that word size if 4 bytes >>
		 */
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]) == HAL_OK){
			StartPageAddress += 4;
			sofar++;
		}else{
			/*  Error occurred while writing data in Flash memory */
			return HAL_FLASH_GetError();
		}
	}

	/* Lock the Flash to disable the flash control register access
	 * Recommended to protect the FLASH MEMORY against possible unwanted operation
	 */
	HAL_FLASH_Lock();

	return 0;
}

void Flash_Read_Data(uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords){
	while(1){
		/* Start Address of the page , from where you want to start reading the data
		 * RxBuf - 32 bit array, where you want to store
		 */
		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
		if(!(numberofwords--)) break;
	}
}

void Convert_To_Str(uint32_t *Data, char *Buf){
	int numberofbytes = ((strlen((char*)Data)/4) + ((strlen((char *)Data) %4) != 0 ))*4;

	for(int i=0; i<numberofbytes;i++){
		Buf[i] = Data[i/4]>>(8*(i%4));
	}
}

void Flash_Write_NUM(uint32_t StartSectorAddress, float Num){
	float2Bytes(bytes_temp, Num);

	Flash_Write_Data(StartSectorAddress, (uint32_t *)bytes_temp, 1);
}

float Flash_Read_NUM(uint32_t StartSectorAddress){
	uint8_t buffer[4];
	float value;

	Flash_Read_Data(StartSectorAddress, (uint32_t *) buffer, 1);

	value = Bytes2float(buffer);
	return value;
}

