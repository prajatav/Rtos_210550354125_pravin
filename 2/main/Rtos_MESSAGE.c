/* 

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
 
#include "FreeRtOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"
#include "esp32.h"

#define MaxQueueSize 5
#define MaxElementsPerQueue 30

/* Local Task Declaration */
static void MyTask1(void* pvParameters);
static void MyTask2(void* pvParameters);

xTaskHandle TaskHandle_1;
xTaskHandle TaskHandle_2;
xTaskHandle TaskHandle_3;
xTaskHandle TaskHandle_4;
xTaskHandle TaskHandle_5;


xQueueHandle MyQueueHandleId;

#define LED_Task1    0x02u
#define LED_Task2   0x04u

#define LED_PORT LPC_GPIO2->FIOPIN

int main(void)
{
	SystemInit();			/* Initialize the controller */
	UART_Init(38400);		 /* Initialize the Uart module */	
	LPC_GPIO2->FIODIR = 0xffffffffu;

	MyQueueHandleId = xQueueCreate(MaxQueueSize,MaxElementsPerQueue);	/* Cretae a queue */

	if(MyQueueHandleId != 0)
	{
		UART_Printf("\n\rQueue Created");

		xTaskCreate( MyTask1, ( signed char * )"Task1", configMINIMAL_STACK_SIZE, NULL, 3, &TaskHandle_1 );
		xTaskCreate( MyTask2, ( signed char * )"Task2", configMINIMAL_STACK_SIZE, NULL, 4, &TaskHandle_2 );

		vTaskStartScheduler();	/* start the scheduler */
	}
	else
		UART_Printf("\n\rQueue not Created");

	while(1);

	return 0;
}

char TxBuffer[5][MaxElementsPerQueue]={"Hello world", "Good Morning", "welcome", "Free Rtos", "Good Bye"};
char RxBuffer[5][MaxElementsPerQueue];

static void MyTask1(void* pvParameters){

	unsigned char i;

	LED_PORT = LED_Task1;	  /* Led to indicate the execution of Task1*/
	UART_Printf("\n\rTask1, Reading the data from queue");

	for(i=0;i<5;i++)
		if(pdTRUE == xQueueReceive(MyQueueHandleId,RxBuffer[i],100))
		{
			LED_PORT = LED_Task1;	  /* Led to indicate the execution of Task1*/
			UART_Printf("\n\rBack in task1, Received data is:%s",RxBuffer[i]);
		}
		else
		{
			LED_PORT = LED_Task1;	  /* Led to indicate the execution of Task1*/
			UART_Printf("\n\rBack in task1, No Data received");
		}

	vTaskDelete(TaskHandle_1);

}


static void MyTask2(void* pvParameters)
{
	unsigned char i;

	LED_PORT = LED_Task2;	  /* Led to indicate the execution of Task2*/

	UART_Printf("\n\rTask2, Filling the data onto queue");

	for(i=0;i<5;i++)
		if(pdTRUE == xQueueSend(MyQueueHandleId,TxBuffer[i],100))
		{
			 LED_PORT = LED_Task2;	  /* Led to indicate the execution of Task2*/
			 UART_Printf("\n\rTask2: Successfully sent the data");
		}
		else
		{
			LED_PORT = LED_Task2;	  /* Led to indicate the execution of Task2*/
			UART_Printf("\n\rSending Failed");
		}

	UART_Printf("\n\rExiting task2");
	vTaskDelete(TaskHandle_2);
}

