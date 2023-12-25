/* 
 * File:   application.c
 * Author: hamada
 *
 * Created on July 8, 2023, 2:46 PM
 */

#include "application.h"


chr_lcd_4bit_t lcd1;
keypad_t keypad1;
uint8 value;
uint8 pass_input[4];
uint8 store_password[4] ="1234";
uint8 pass_counter =0;

usart_t usart_obj;

int main(void)
{
    application_components();
    application_intialize();
    /*Gets password and check 
     *If password is incorrect the program repeat ask the user to enter the password
     */
    get_password();
    /**Continue if password is correct**/
    lcd_4bit_send_command(&lcd1,_LCD_CLEAR); 
    __delay_ms(20);
    lcd_4bit_send_string_pos(&lcd1,ROW1,7,"Welcome");
    lcd_4bit_send_string_pos(&lcd1,ROW2,6,"Smart Home");
    EUSART_ASYNC_WriteByteBlocking('a');
    
    
    while(1)
    {
        
        
        
    }


    
    return (EXIT_SUCCESS);
}

void application_components(void)
{
   /***********************LCD initialization*******************/
    lcd1.lcd_rs.port=PORTB_INDEX;
    lcd1.lcd_rs.pin=GPIO_PIN6;
    lcd1.lcd_rs.direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_rs.logic=GPIO_LOW;
    lcd1.lcd_en.port=PORTB_INDEX;
    lcd1.lcd_en.pin=GPIO_PIN1;
    lcd1.lcd_en.direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_en.logic=GPIO_LOW;
    lcd1.lcd_data[0].port=PORTB_INDEX;
    lcd1.lcd_data[0].pin=GPIO_PIN2;
    lcd1.lcd_data[0].direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_data[0].logic=GPIO_HIGH;
    lcd1.lcd_data[1].port=PORTB_INDEX;
    lcd1.lcd_data[1].pin=GPIO_PIN3;
    lcd1.lcd_data[1].direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_data[1].logic=GPIO_HIGH;
    lcd1.lcd_data[2].port=PORTB_INDEX;
    lcd1.lcd_data[2].pin=GPIO_PIN4;
    lcd1.lcd_data[2].direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_data[2].logic=GPIO_HIGH;
    lcd1.lcd_data[3].port=PORTB_INDEX;
    lcd1.lcd_data[3].pin=GPIO_PIN5;
    lcd1.lcd_data[3].direction=GPIO_DIRECTION_OUTPUT;
    lcd1.lcd_data[3].logic=GPIO_HIGH;
    /***********************keypad initialization*******************/
    keypad1.keypad_row_pins[0].direction =GPIO_DIRECTION_OUTPUT;
    keypad1.keypad_row_pins[0].logic = GPIO_LOW;
    keypad1.keypad_row_pins[0].port = PORTD_INDEX;
    keypad1.keypad_row_pins[0].pin = GPIO_PIN0;
    
    keypad1.keypad_row_pins[1].direction =GPIO_DIRECTION_OUTPUT;
    keypad1.keypad_row_pins[1].logic = GPIO_LOW;
    keypad1.keypad_row_pins[1].port = PORTD_INDEX;
    keypad1.keypad_row_pins[1].pin = GPIO_PIN1;
    
    keypad1.keypad_row_pins[2].direction =GPIO_DIRECTION_OUTPUT;
    keypad1.keypad_row_pins[2].logic = GPIO_LOW;
    keypad1.keypad_row_pins[2].port = PORTD_INDEX;
    keypad1.keypad_row_pins[2].pin = GPIO_PIN2;
    
    keypad1.keypad_row_pins[3].direction =GPIO_DIRECTION_OUTPUT;
    keypad1.keypad_row_pins[3].logic = GPIO_LOW;
    keypad1.keypad_row_pins[3].port = PORTD_INDEX;
    keypad1.keypad_row_pins[3].pin = GPIO_PIN3;
    
    keypad1.keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT;
    keypad1.keypad_columns_pins[0].logic = GPIO_LOW;
    keypad1.keypad_columns_pins[0].port = PORTD_INDEX;
    keypad1.keypad_columns_pins[0].pin = GPIO_PIN4;
    
    keypad1.keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT;
    keypad1.keypad_columns_pins[1].logic = GPIO_LOW;
    keypad1.keypad_columns_pins[1].port = PORTD_INDEX;
    keypad1.keypad_columns_pins[1].pin = GPIO_PIN5;
    
    keypad1.keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT;
    keypad1.keypad_columns_pins[2].logic = GPIO_LOW;
    keypad1.keypad_columns_pins[2].port = PORTD_INDEX;
    keypad1.keypad_columns_pins[2].pin = GPIO_PIN6;
    
    keypad1.keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT;
    keypad1.keypad_columns_pins[3].logic = GPIO_LOW;
    keypad1.keypad_columns_pins[3].port = PORTD_INDEX;
    keypad1.keypad_columns_pins[3].pin = GPIO_PIN7;
     /***********************usart initialization*******************/
 
    usart_obj.baudrate = 9600 ;
    usart_obj.baudrate_gen_gonfig=BAUDRATE_ASYN_8BIT_lOW_SPEED;
    
    usart_obj.usart_tx_cfg.usart_tx_enable=EUSART_ASYNCHRONOUS_TX_ENABLE;
    usart_obj.usart_tx_cfg.usart_tx_interrupt_enable=EUSART_ASYNCHRONOUS_INTERRUPT_TX_DISABLE;
    usart_obj.usart_tx_cfg.usart_tx_9bit_enable=EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE;
   
    usart_obj.usart_rx_cfg.usart_rx_enable=EUSART_ASYNCHRONOUS_RX_ENABLE;
    usart_obj.usart_rx_cfg.usart_rx_interrupt_enable=EUSART_ASYNCHRONOUS_INTERRUPT_RX_DISABLE;
    usart_obj.usart_rx_cfg.usart_rx_9bit_enable=EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE;
   
    usart_obj.EUSART_RxDefaultInterruptHandler=NULL;
    usart_obj.EUSART_RxDefaultInterruptHandler=NULL;
    usart_obj.EUSART_FramingErrorHandler=NULL;
    usart_obj.EUSART_OverrunErrorHandler=NULL;
    
}


void application_intialize(void)
{
    Std_ReturnType ret ;

    ret = lcd_4bit_intialize(&lcd1);
    ret = keypad_initialize(&keypad1);
    
    ret = EUSART_ASYNC_Init(&usart_obj);
}

void get_password(void)
{
    lcd_4bit_send_string_pos(&lcd1,ROW1,1,"Enter The Password :");
    while(1)
    {
        keypad_get_value(&keypad1,&value);
        __delay_ms(150);
        if(value>0 && value!='#')
        {
            pass_input[pass_counter]=value;
            lcd_4bit_send_char_data_pos(&lcd1,ROW2,pass_counter+1,'*');
            value=0;
            pass_counter++;
        }
        else if(value == '#')
        {
            int i = 0;
            for(i = 0 ; i<4 ;i++)
            {
                if(pass_input[i] != store_password[i]) break;
            }
            if(i==4)
            {
                value=0;
                pass_counter=0;
                break;
            }
            else
            {
                lcd_4bit_send_command(&lcd1,_LCD_CLEAR);
                __delay_ms(20);
                lcd_4bit_send_string_pos(&lcd1,ROW1,1,"Wrong Password");
                lcd_4bit_send_string_pos(&lcd1,ROW2,1,"Try Again");
                __delay_ms(1000);
                lcd_4bit_send_command(&lcd1,_LCD_CLEAR);
                __delay_ms(20);
                lcd_4bit_send_string_pos(&lcd1,ROW1,1,"Enter The Password :");
                value=0;
            }
            pass_counter=0;
        }
    }
}

