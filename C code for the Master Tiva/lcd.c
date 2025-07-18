#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "lcd.h"
#include "glob_def.h"
#include "tmodel.h"
#include "semphr.h"
#include "portmacro.h"

extern QueueHandle_t xQueue_lcd;
extern SemaphoreHandle_t xSemaphore_lcd;
/*****************************    Defines    *******************************/

#define QUEUE_LEN 128

enum LCD_states
{
    LCD_POWER_UP,
    LCD_INIT,
    LCD_READY,
    LCD_ESC_RECEIVED,
};

/*****************************   Constants   *******************************/
const INT8U LCD_init_sequense[] =
    {
        0x30, // Reset
        0x30, // Reset
        0x30, // Reset
        0x20, // Set 4bit interface
        0x28, // 2 lines Display
        0x0C, // Display ON, Cursor OFF, Blink OFF
        0x06, // Cursor Increment
        0x01, // Clear Display
        0x02, // Home
        0xFF  // stop
};


enum LCD_states my_state = LCD_POWER_UP;
INT8U LCD_init;

/*****************************   Functions   *******************************/
INT8U wr_ch_LCD(INT8U Ch)
/*****************************************************************************
 *   OBSERVE  : LCD_PROC NEEDS 20 mS TO PRINT OUT ONE CHARACTER
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
    if (uxQueueSpacesAvailable(xQueue_lcd))
    {
        if (xSemaphoreTake(xSemaphore_lcd, portMAX_DELAY))
        {
            if (xQueueSend(xQueue_lcd, &Ch, portMAX_DELAY))
            {
                xSemaphoreGive(xSemaphore_lcd);
                return 1;
            }
        }
    }
    return 0;
}

void wr_str_LCD(INT8U *pStr)
{
    while (*pStr)
    {
        wr_ch_LCD(*pStr);
        pStr++;
    }
}

void move_LCD(INT8U x, INT8U y)
{

    INT8U Pos;

    Pos = y * 0x40 + x;
    Pos |= 0x80;
    wr_ch_LCD(ESC);
    wr_ch_LCD(Pos);
}


void wr_ctrl_LCD_low(INT8U Ch)
{
    INT8U temp;
    volatile int i;

    temp = GPIO_PORTC_DATA_R & 0x0F;
    temp = temp | ((Ch & 0x0F) << 4);
    GPIO_PORTC_DATA_R = temp;
    for (i = 0; i < 1000; i)
        i++;
    GPIO_PORTD_DATA_R &= 0xFB; // Select Control mode, write
    for (i = 0; i < 1000; i)
        i++;
    GPIO_PORTD_DATA_R |= 0x08; // Set E High

    for (i = 0; i < 1000; i)
        i++;

    GPIO_PORTD_DATA_R &= 0xF7; // Set E Low

    for (i = 0; i < 1000; i)
        i++;
}

void wr_ctrl_LCD_high(INT8U Ch)

{
    wr_ctrl_LCD_low((Ch & 0xF0) >> 4);
}

void out_LCD_low(INT8U Ch)

{
    INT8U temp;

    temp = GPIO_PORTC_DATA_R & 0x0F;
    GPIO_PORTC_DATA_R = temp | ((Ch & 0x0F) << 4);
    // GPIO_PORTD_DATA_R &= 0x7F;        // Select write
    GPIO_PORTD_DATA_R |= 0x04; // Select data mode
    GPIO_PORTD_DATA_R |= 0x08; // Set E High
    GPIO_PORTD_DATA_R &= 0xF7; // Set E Low
}

void out_LCD_high(INT8U Ch)

{
    out_LCD_low((Ch & 0xF0) >> 4);
}

void wr_ctrl_LCD(INT8U Ch)

{
    static INT8U Mode4bit = FALSE;
    INT16U i;

    wr_ctrl_LCD_high(Ch);
    if (Mode4bit)
    {
        for (i = 0; i < 1000; i++)
            ;
        wr_ctrl_LCD_low(Ch);
    }
    else
    {
        if ((Ch & 0x30) == 0x20)
            Mode4bit = TRUE;
    }
}

void clr_LCD()

{
    if (xSemaphoreTake(xSemaphore_lcd, portMAX_DELAY))
    {
        wr_ctrl_LCD(0x01);
        xSemaphoreGive(xSemaphore_lcd);
    }
}

void home_LCD()

{
    if (xSemaphoreTake(xSemaphore_lcd, portMAX_DELAY))
    {
        wr_ctrl_LCD(0x02);
        xSemaphoreGive(xSemaphore_lcd);
    }
}

void Set_cursor(INT8U Ch)

{

    wr_ctrl_LCD(Ch);
}

void out_LCD(INT8U Ch)

{
    INT16U i;

    out_LCD_high(Ch);
    for (i = 0; i < 1000; i++)
        ;
    out_LCD_low(Ch);
}

void set_state(enum LCD_states new_state)
{
    my_state = new_state;
}

void lcd_task(void *pvParameters)

{
    INT8U ch;
    while (1)
    {
        switch (my_state)
        {
        case LCD_POWER_UP:
            LCD_init = 0;
            set_state(LCD_INIT);
            vTaskDelay(100 / portTICK_RATE_MS);
            break;

        case LCD_INIT:
            if (LCD_init_sequense[LCD_init] != 0xFF)
                wr_ctrl_LCD(LCD_init_sequense[LCD_init++]);
            else
            {
                set_state(LCD_READY);
                // open_queue( Q_LCD );
            }
            vTaskDelay(100 / portTICK_RATE_MS);
            break;

        case LCD_READY:
            if (uxQueueMessagesWaiting(xQueue_lcd))
            {
                if (xSemaphoreTake(xSemaphore_lcd, portMAX_DELAY))
                {
                    if (xQueueReceive(xQueue_lcd, &ch, portMAX_DELAY))
                    {
                        switch (ch)
                        {
                        case 0xFF:
                            clr_LCD();
                            break;
                        case ESC:
                            set_state(LCD_ESC_RECEIVED);
                            break;
                        default:
                            out_LCD(ch);
                            xSemaphoreGive(xSemaphore_lcd);
                        }
                    }
                }
            }
            break;

        case LCD_ESC_RECEIVED:
            if (uxQueueMessagesWaiting(xQueue_lcd))
            {
                if (xSemaphoreTake(xSemaphore_lcd, portMAX_DELAY))
                {
                    if (xQueueReceive(xQueue_lcd, &ch, portMAX_DELAY))
                    {
                        if (ch & 0x80)
                        {
                            Set_cursor(ch);
                        }
                        else
                        {
                            switch (ch)
                            {
                            case '@':
                                home_LCD();
                                break;
                            }
                        }
                        set_state(LCD_READY);
                        vTaskDelay(100 / portTICK_RATE_MS);
                        xSemaphoreGive(xSemaphore_lcd);
                    }
                }
            }
            break;
        }
    }
}



