/* 
 * File:   application.c
 * Author: hamada
 *
 * Created on July 8, 2023, 2:46 PM
 */

#include "application.h"

led_t led1 , led2 , led3 ,led4 ;
dc_motor_t motor_fan ;

button_t btn1 ; button_state_t btn1_state; // for led1
button_t btn2 ; button_state_t btn2_state; // for led2
button_t btn3 ; button_state_t btn3_state; // for led3
button_t btn4 ; button_state_t btn4_state; // for led4
button_t btn5 ; button_state_t btn5_state; // for motor fan

chr_lcd_4bit_t lcd1;

adc_conf_t adc1 ;  uint16 adc_value;char temp_value[];

usart_t usart_obj;

uint8 serial_value;
int main(void)
{
    application_components();
    application_intialize();
    
   
    lcd_4bit_send_string(&lcd1 ,"The system is closed");
    while(1)
    {
	 EUSART_ASYNC_ReadByteBlocking(&serial_value);
        if(serial_value == 'a') break;   // a -> system on
    }
    lcd_4bit_send_command(&lcd1,_LCD_CLEAR);
    __delay_ms(20);
    lcd_4bit_send_string(&lcd1 ,"The system is open");
    __delay_ms(1000);
    lcd_4bit_send_command(&lcd1,_LCD_CLEAR);
    __delay_ms(20);
    
    lcd_4bit_send_string_pos(&lcd1,ROW1,1,"LED 1:");
    lcd_4bit_send_string_pos(&lcd1,ROW1,11,"LED 2:");
    lcd_4bit_send_string_pos(&lcd1,ROW2,1,"LED 3:");
    lcd_4bit_send_string_pos(&lcd1,ROW2,11,"LED 4:");
    lcd_4bit_send_string_pos(&lcd1,ROW3,1,"Fan :");
    lcd_4bit_send_string_pos(&lcd1,ROW4,1,"Temp :");
    while(1)
    {
        /**Get temperature value**/
        get_temp_value();
        lcd_4bit_send_string_pos(&lcd1,ROW4,7,temp_value);
        /**Get switches values**/
        button_read_state(&btn1 , &btn1_state);
        button_read_state(&btn2 , &btn2_state);
        button_read_state(&btn3 , &btn3_state);
        button_read_state(&btn4 , &btn4_state);
        button_read_state(&btn5 , &btn5_state);
        /**Turn ON,OFF Loads**/
        if(btn1_state == BUTTON_PRESSED)
        {
            led_turn_on(&led1);
            lcd_4bit_send_string_pos(&lcd1,ROW1,7,"ON ");
        }
        else
        {
            led_turn_off(&led1);
             lcd_4bit_send_string_pos(&lcd1,ROW1,7,"OFF");
        }
        if(btn2_state == BUTTON_PRESSED)
        {
            led_turn_on(&led2);
            lcd_4bit_send_string_pos(&lcd1,ROW1,17,"ON ");
        }
        else
        {
            led_turn_off(&led2);
            lcd_4bit_send_string_pos(&lcd1,ROW1,17,"OFF");
        }
        if(btn3_state == BUTTON_PRESSED) 
        {
            led_turn_on(&led3);
            lcd_4bit_send_string_pos(&lcd1,ROW2,7,"ON ");
        }
        else
        {
            led_turn_off(&led3);
            lcd_4bit_send_string_pos(&lcd1,ROW2,7,"OFF");
        }
        if(btn4_state == BUTTON_PRESSED) 
        {
            led_turn_on(&led4);
            lcd_4bit_send_string_pos(&lcd1,ROW2,17,"ON ");
        }
        else
        {
            led_turn_off(&led4);
            lcd_4bit_send_string_pos(&lcd1,ROW2,17,"OFF");
        }
        if(btn5_state == BUTTON_PRESSED) 
        {
            dc_motor_move_right(&motor_fan);
            lcd_4bit_send_string_pos(&lcd1,ROW3,6,"ON ");
        }
        else
        {
            dc_motor_stop(&motor_fan);
            lcd_4bit_send_string_pos(&lcd1,ROW3,6,"OFF");
        }
        
       
      
    }


    
    return (EXIT_SUCCESS);
}

void application_components(void)
{
    /***********************Leds initialization*******************/
    led1.port_name=PORTC_INDEX; led1.pin =GPIO_PIN0; led1.led_status =LED_OFF;
    led2.port_name=PORTC_INDEX; led2.pin =GPIO_PIN1; led2.led_status =LED_OFF;
    led3.port_name=PORTC_INDEX; led3.pin =GPIO_PIN2; led3.led_status =LED_OFF;
    led4.port_name=PORTC_INDEX; led4.pin =GPIO_PIN3; led4.led_status =LED_OFF;
    /***********************Fan initialization*******************/
    motor_fan.dc_motor_pin[0].port=PORTC_INDEX;
    motor_fan.dc_motor_pin[0].pin= GPIO_PIN4;
    motor_fan.dc_motor_pin[0].direction =GPIO_DIRECTION_OUTPUT;
    motor_fan.dc_motor_pin[0].logic = GPIO_LOW;
    motor_fan.dc_motor_pin[1].port=PORTC_INDEX;
    motor_fan.dc_motor_pin[1].pin= GPIO_PIN5;
    motor_fan.dc_motor_pin[1].direction =GPIO_DIRECTION_OUTPUT;
    motor_fan.dc_motor_pin[1].logic = GPIO_LOW;
    /***********************buttons initialization*******************/
    btn1.button_connection = BUTTON_ACTIVE_HIGH;
    btn1.button_state = BUTTON_RELEASED;
    btn1.button_pin.port =PORTD_INDEX;
    btn1.button_pin.pin =GPIO_PIN0;
    btn1.button_pin.direction =GPIO_DIRECTION_INPUT;
    btn1.button_pin.logic=GPIO_HIGH;
    
    btn2.button_connection = BUTTON_ACTIVE_HIGH;
    btn2.button_state = BUTTON_RELEASED;
    btn2.button_pin.port =PORTD_INDEX;
    btn2.button_pin.pin =GPIO_PIN1;
    btn2.button_pin.direction =GPIO_DIRECTION_INPUT;
    btn2.button_pin.logic=GPIO_HIGH;
    
    btn3.button_connection = BUTTON_ACTIVE_HIGH;
    btn3.button_state = BUTTON_RELEASED;
    btn3.button_pin.port =PORTD_INDEX;
    btn3.button_pin.pin =GPIO_PIN2;
    btn3.button_pin.direction =GPIO_DIRECTION_INPUT;
    btn3.button_pin.logic=GPIO_HIGH;
    
    btn4.button_connection = BUTTON_ACTIVE_HIGH;
    btn4.button_state = BUTTON_RELEASED;
    btn4.button_pin.port =PORTD_INDEX;
    btn4.button_pin.pin =GPIO_PIN3;
    btn4.button_pin.direction =GPIO_DIRECTION_INPUT;
    btn4.button_pin.logic=GPIO_HIGH;
    
    btn5.button_connection = BUTTON_ACTIVE_HIGH;
    btn5.button_state = BUTTON_RELEASED;
    btn5.button_pin.port =PORTD_INDEX;
    btn5.button_pin.pin =GPIO_PIN4;
    btn5.button_pin.direction =GPIO_DIRECTION_INPUT;
    btn5.button_pin.logic=GPIO_HIGH;
    /***********************Lcd initialization*******************/
    lcd1.lcd_rs.port=PORTB_INDEX;
    lcd1.lcd_rs.pin=GPIO_PIN0;
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
    /***********************Adc initialization*******************/
    adc1.ADC_InterruptHandler = NULL;
    adc1.acquisition_time = ADC_12_TAD;
    adc1.adc_channel =ADC_CHANNEL_AN0;
    adc1.conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16;
    adc1.result_format =ADC_RESULT_RIGHT;
    adc1.voltage_reference =ADC_VOLTAGE_REFERENCE_DISABLED;
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
    ret = led_initialize(&led1);
    ret = led_initialize(&led2);
    ret = led_initialize(&led3);
    ret = led_initialize(&led4);
    
    ret = dc_motor_initialize(&motor_fan);
    
    ret = button_initialize(&btn1);
    ret = button_initialize(&btn2);
    ret = button_initialize(&btn3);
    ret = button_initialize(&btn4);
    ret = button_initialize(&btn5);
    
    ret = lcd_4bit_intialize(&lcd1);
    
    ret = ADC_Init(&adc1);
    
    ret = EUSART_ASYNC_Init(&usart_obj);
}

void get_temp_value(void)
{
    ADC_GetConversion_Blocking(&adc1,ADC_CHANNEL_AN0,&adc_value);
    adc_value = adc_value*4.88f;
    adc_value/=10;
    sprintf(temp_value,"%i",adc_value);
}