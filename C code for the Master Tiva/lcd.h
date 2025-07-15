#ifndef _LCD_H
#define _LCD_H


#define LF      0x0A
#define FF      0x0C
#define CR      0x0D

#define ESC     0x1B


INT8U wr_ch_LCD( INT8U Ch );
void wr_str_LCD( INT8U *pStr );
void clr_LCD();
void home_LCD();
void Set_cursor( INT8U Ch );
void lcd_task(void *pvParameters);

#endif
