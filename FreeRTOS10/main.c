/*
 * FreeRTOS10.c
 *
 * Created: 9/14/2018 8:16:18 PM
 * Author : robert
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "usart.c"
#include "I2C.c"
#include "hcsr04.h"


/*
	FreeRTOS.org V5.1.1 - Copyright (C) 2003-2008 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section 
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

    ***************************************************************************
    ***************************************************************************
    *                                                                         *
    * SAVE TIME AND MONEY!  We can port FreeRTOS.org to your own hardware,    *
    * and even write all or part of your application on your behalf.          *
    * See http://www.OpenRTOS.com for details of the services we provide to   *
    * expedite your project.                                                  *
    *                                                                         *
    ***************************************************************************
    ***************************************************************************

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and 
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety 
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting, 
	licensing and training services.
*/

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 */

/*
Changes from V1.2.0
	
	+ Changed the baud rate for the serial test from 19200 to 57600.

Changes from V1.2.3

	+ The integer and comtest tasks are now used when the cooperative scheduler 
	  is being used.  Previously they were only used with the preemptive
	  scheduler.

Changes from V1.2.5

	+ Set the baud rate to 38400.  This has a smaller error percentage with an
	  8MHz clock (according to the manual).

Changes from V2.0.0

	+ Delay periods are now specified using variables and constants of
	  portTickType rather than unsigned portLONG.

Changes from V2.6.1

	+ The IAR and WinAVR AVR ports are now maintained separately.

Changes from V4.0.5

	+ Modified to demonstrate the use of co-routines.

*/



/* Scheduler include files. */


/* Priority definitions for most of the tasks in the demo application.  Some
tasks just use the idle priority. */
#define mainMY_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

/* The period between executions of the check task. */
#define blinkTime1			( ( TickType_t ) 1 / portTICK_PERIOD_MS )
#define blinkTime2			( ( TickType_t ) 2 / portTICK_PERIOD_MS )
#define blinkTime3			( ( TickType_t ) 3 / portTICK_PERIOD_MS )
#define blinkTime4			( ( TickType_t ) 4 / portTICK_PERIOD_MS )
#define blinkTime5			( ( TickType_t ) 5 / portTICK_PERIOD_MS )
#define blinkTime20			( ( TickType_t ) 20 / portTICK_PERIOD_MS )
#define blinkTime100			( ( TickType_t ) 100 / portTICK_PERIOD_MS )
#define blinkTime200			( ( TickType_t ) 200 / portTICK_PERIOD_MS )
#define blinkTime300			( ( TickType_t ) 300 / portTICK_PERIOD_MS )
#define blinkTime1000			( ( TickType_t ) 1000 / portTICK_PERIOD_MS )
#define blinkTime7000			( ( TickType_t ) 7000 / portTICK_PERIOD_MS )


static void mytask1( void *pvParameters );
static void mytask2( void *pvParameters );
//static void mytask3( void *pvParameters );
static void mytask4( void *pvParameters );

//static xMutex = xSemaphoreCreateMutex();void comando(unsigned char comando);


//IPC



//#if (configUSE_IDLE_HOOK == 1) 
/*
 * The idle hook is used to scheduler co-routines.
 */
//void vApplicationIdleHook( void );
//#endif

/*-----------------------------------------------------------*/
//xMutex = xSemaphoreCreateRecursiveMutex();

//SemaphoreHandle_t xSerialSemaphore;

TaskHandle_t task2_handle;
TaskHandle_t task4_handle;

static long int gz = 0;
unsigned char sm;
unsigned char dato;

char automatico = 0;
char aspiradora = 0;


portSHORT main( void )
{
// 	if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
// 	{
// 		xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
// 		if ( ( xSerialSemaphore ) != NULL )
// 		xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
// 	}
	DDRC = (1 << PORTC2);
	
	
	//DDRB = (1<<PORTB5) | (1<<PORTB6);   //PINB5 y 6 salidas
	Port_Init();
	USART_init();
	startI2c();
	
	PORTB = 0b00000000;
	PORTD = 0b00000000;
	
	writeI2c (0x68, 0x19, 0b00000111);//prescaler
	writeI2c (0x68, 0x1b, 0b00011000);//2000 degrés/s gyro
	//writeI2c (0x68, 0x23, 0b00000000);//fifo???
	writeI2c (0x68, 0x6b, 0b00000001);//x axis reference
		
	/* Create the tasks defined within this file. */
	xTaskCreate( mytask1, ( const portCHAR * ) "Task1", configMINIMAL_STACK_SIZE, NULL, mainMY_TASK_PRIORITY, NULL );
	xTaskCreate( mytask2, ( const portCHAR * ) "Task2", configMINIMAL_STACK_SIZE, NULL, mainMY_TASK_PRIORITY, &task2_handle );
	//xTaskCreate( mytask3, ( const portCHAR * ) "Task3", configMINIMAL_STACK_SIZE, NULL, mainMY_TASK_PRIORITY, NULL );
	xTaskCreate( mytask4, ( const portCHAR * ) "Task4", configMINIMAL_STACK_SIZE, NULL, mainMY_TASK_PRIORITY + 1 , NULL );
	vTaskSuspend(task2_handle);
	vTaskSuspend(task4_handle);
	sei();
	/* In this port, to use preemptive scheduler define configUSE_PREEMPTION 
	as 1 in portmacro.h.  To use the cooperative scheduler define 
	configUSE_PREEMPTION as 0. */
	vTaskStartScheduler();

	while(1)
	 ;

	return 0;
}
/*-----------------------------------------------------------*/

void derecha(){
	taskENTER_CRITICAL();
	PORTB = 0b00011000; //atras derecha
	PORTD = 0b01100000; //adelante izquierda
	//vTaskDelay(blinkTime1);
	_delay_us(400);
	PORTB = 0b00010000;
	PORTD = 0b00100000;
	//_delay_us(6);
	taskEXIT_CRITICAL();
	vTaskDelay(blinkTime1);
}


void izquierda(){
	taskENTER_CRITICAL();
	PORTB = 0b00010100;
	PORTD = 0b10100000;
	//vTaskDelay(blinkTime1);
	_delay_us(400);
	PORTB = 0b00010000;
	PORTD = 0b00100000;
	//_delay_us(6);
	taskEXIT_CRITICAL();
	vTaskDelay(blinkTime1);
}

void adelante(){
	if(gz >= -1 && gz <= 1){
		PORTB = 0b00010000;
		PORTD = 0b00100000;
		_delay_us(5);
		//vTaskDelay(blinkTime1);
		PORTB = 0b00010100; //adelante derecha
		//PORTB = 0b00011000; //atras derecha
		//PORTD = 0b10100000;  //atras izquierda
		PORTD = 0b01100000;   //adelante izquierda
		//vTaskDelay(blinkTime1);
		_delay_us(5);
	}else 
		if ( gz < -1){
			derecha();
		}else if(gz > 1)
			izquierda();
}


long int timeDato = 0;
static void mytask1( void *pvParameters ) 
{
	for( ;; )
	{
		int x;
 		char num[6];
// 		int x = readI2c (104, 0x3b);//READ X accelerometer (in16_t)
// 		itoa(x, num, 10);
 		char str[10] ="AcX = ";
// 		USART_putstring(str);
// 		USART_putstring(num);
// 		x = readI2c (104, 0x3d);//READ Y accelerometer (in16_t)
// 		itoa(x, num, 10);
// 		strcpy(str, " | AcY = ");
// 		USART_putstring(str);
// 		USART_putstring(num);
// 		x = readI2c (104, 0x3f);//READ Z accelerometer (in16_t)
// 		itoa(x, num, 10);
// 		strcpy(str, " | AcZ = ");
// 		USART_putstring(str);
// 		USART_putstring(num);
// // 		x = readI2c (104, 0x41);//READ temperature (in16_t)
// // 		itoa(x, num, 10);
// // 		strcpy(str, " | Tmp = ");
// // 		USART_putstring(str);
// // 		USART_putstring(num);
// 		x = readI2c (104, 0x43);//READ X gyroscope (in16_t)43
// 		itoa(x, num, 10);
// 		strcpy(str, " | GyX = ");
// 		USART_putstring(str);
// 		USART_putstring(num);
// 		x = readI2c (104, 0x45);//READ Y gyroscope (in16_t)45
// 		itoa(x, num, 10);
// 		strcpy(str, " | GyY = ");
// 		USART_putstring(str);
// 		USART_putstring(num);
		
		x = readI2c (104, 0x47);//READ Z gyroscope (in16_t)47
		gz += trunc(x/160);
		itoa(gz, num, 10);
		strcpy(str, " | GyZ = ");
		if(timeDato >= 20){
			USART_putstring(str);
			USART_putstring(num);	
		}		
		
// 		distancia hc-sr04
		sm = hc_sr04_meas();
		itoa(sm, num, 10);
		strcpy(str, "&");
		strcat(str, num);
		strcat(str, "(");
		if(timeDato >= 20){
			USART_putstring(str);
			timeDato = 0;
		}
		dato = recibe_caracter_usart();
		comando(dato);
		vTaskDelay(blinkTime1);
		timeDato++;
		//vTaskSuspend(xHandle1);
		//vTaskDelay(blinkTime200);
	}
}

static void mytask2( void *pvParameters ) 
{
	for( ;; )
	{
		adelante();
	}
}

/*static void mytask3( void *pvParameters ) 
{
	for( ;; )
	{
	   sm = hc_sr04_meas();
	   char str1[10];
	   char str2[10] = "&";
	   itoa(sm, str1, 10);
	   strcat(str2, str1);
	   strcat(str2, "&");
	   USART_putstring(str2);
	   vTaskDelay(blinkTime300);		
	}
}*/


static void mytask4( void *pvParameters ) 
{
	for( ;; )
	{
 		vTaskDelay(blinkTime300);
	    if(sm < 20 ){
			taskENTER_CRITICAL();
			while(gz < 57000){
				PORTB = 0b00011000; //atras derecha
				PORTD = 0b01100000; //adelante izquierda
				int x = readI2c (104, 0x47);//READ Z gyroscope (in16_t)47
				gz += round(x/140);
			}
			gz = 0;
			taskEXIT_CRITICAL();
		}
	}
}


/*-----------------------------------------------------------*/
/*
#if (configUSE_IDLE_HOOK == 1) 
void vApplicationIdleHook( void )
{
	vCoRoutineSchedule();
}
#endif
*/

void comando(unsigned char comando){
	switch(comando){
		case '1':
			PORTB = 0b00010100;
			PORTD |= 0b01100000;
			_delay_ms(50);
			PORTB = 0b00010000;
			PORTD &= 0b00110000;
			break;
		case '2':
			PORTB = 0b00011000; 
			PORTD |= 0b10100000;
			_delay_ms(50);
			PORTB = 0b00010000;
			PORTD &= 0b00110000;
			break;
		case '3':
			PORTB = 0b00010100;
			PORTD |= 0b10100000;
			_delay_ms(50);
			PORTB = 0b00010000;
			PORTD &= 0b00110000;
			break;
		case '4':
			PORTB = 0b00011000; //atras derecha
			PORTD |= 0b01100000; //adelante izquierda
			_delay_ms(50);
			PORTB = 0b00010000;
			PORTD &= 0b00110000;
			break;
		case '8'://auto
			if (!automatico){
				vTaskResume(task2_handle);
				vTaskResume(task4_handle);
				automatico = 1;
			}
			else{
				vTaskSuspend(task2_handle);
				vTaskResume(task4_handle);
				automatico = 0;
			}
			break;
		case '9'://aspiradora
			if(!aspiradora){
				PORTD |= 0b00010000;
				aspiradora = 1;
			}else{
				PORTD &= ~(0b00010000);
				aspiradora = 0;
			}
			break;
	}
}