/* 

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
 

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t intmq;
void tone_task(void *data)
{
    while(1)
    {
        printf(" task1 started \n");
        //...............//
        printf(" task1 ended \n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);

    }
}

void ttwo_task(void *data)
{
    while(1)
    {
        printf(" task2 started \n");
        //...............//
        printf(" task2 ended \n");
        vTaskDelay(2000/ portTICK_PERIOD_MS);

    }
}

void tthree_task(void *data)
{
    while(1)
    {
        printf(" task3 started \n");
        //...............//
        printf(" task3 ended \n");
        vTaskDelay(5000/ portTICK_PERIOD_MS);

    }
}




void tfour_task(void *data)
{
    int integer = 50;
    while(1)
    {
        printf(" task4 started \n");
        
        
        printf(" task4 ended \n");
        xQueueSend(intmq,&integer, 0);
        vTaskDelay(3000/ portTICK_PERIOD_MS);

    }
}


void tfive_task(void *data)
{
    int buff;
    
    while(1)
    {
        printf("task5 started \n");
        xQueueReceive(intmq,&buff, 0);
        printf("integer from task4 is: %d\n",buff);
    
        printf(" task5 ended \n");
        vTaskDelay(3000/ portTICK_PERIOD_MS);
      
      
    }
   
}



void app_main(void)
{
    xTaskCreate(tone_task, "tone_tsk", 2048, NULL, 10, NULL);
    xTaskCreate(ttwo_task, "ttwo_tsk", 2048, NULL, 8, NULL);
    xTaskCreate(tthree_task, "tthree_tsk", 2048, NULL, 6, NULL);
    xTaskCreate(tfour_task, "tfour_tsk", 2048, NULL, 5, NULL);
    xTaskCreate(tfive_task, "tfive_tsk", 2048, NULL, 3, NULL);
    intmq = xQueueCreate(3, sizeof(int));
   
}
