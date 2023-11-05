# PWM-control-with-infrared-signal
## Proyecto de sistema embebido para control de una salida PWM mediante el uso de una señal infrarroja
_Objetive: Embedded system to control a PWM output_

## Materials
- Microcontrolador PIC 16F877A
- pines hembra
- borneras de 2 pines
- placa fenolica
- acido cloruro ferrico
- resistencia de 10k ohm
- resistencia de 220 ohm para diodo de moc
- resistencia de 470 a 1k ohm para salida de moc con el transistor
- push boton de 4 pines
- capacitores ceramicos 22
- crystal de cuarzo de 20M
- zocalo de 40 pines
- transistores BT136 (pueden ser BT137 O BT138 dependiendo el amperaje de trabajo)
- circuito integrado MOC 4031
- sensor receptor infrarojo KY-022
- pantalla lcd 16x2 con modulo i2c

  ## Diagrama de conexiones en proteus
### _Conexiones mediante etiquetas para facilitar la lectura del esquematico_

  ![image](https://github.com/Gaddiel0710/PWM-control-with-infrared-signal/assets/135661300/f0ce5b63-3503-4882-8853-199a5e43d121)

### _Diseño de placa en vista 3D_

![image](https://github.com/Gaddiel0710/PWM-control-with-infrared-signal/assets/135661300/bc518f8e-bd09-49e9-9c41-df60d02109a2)

### _pistas de conexion en pcb_

![image](https://github.com/Gaddiel0710/PWM-control-with-infrared-signal/assets/135661300/fe57face-8b96-43d6-a062-299ca913996b)

### _vista final en fisico de la placa_


## Codigo de control
_Los codigos hexadesimales varian dependiendo el control que envia los datos_

```C
#include <16f877a.h>
#device ADC=10 
#fuses HS,NOWDT,NOPROTECT,NOPUT,NOLVP,BROWNOUT
#use delay(clock=20M)
#use i2c(Master,Fast=100000, sda=PIN_C4, scl=PIN_C3,force_sw)  // libreríá para manejar la comunicación I2C
#include "i2c_Flex_LCD.c" //driver para manejar la interface LCD I2C
#include <NEC_ir.c> // Libreria para el uso del protocolo NEC

int16 duty=0;
int Timer2,Poscaler;

void main()
{ 
   Timer2=249;
   Poscaler=1;
   setup_timer_2(t2_div_by_4,Timer2,Poscaler);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   setup_adc_ports(all_analog);        //Configurar ADC
   setup_adc(adc_clock_internal);
   
   lcd_init(0x4E,16,2); //inicializacion de la lcd
   lcd_backlight_led(ON); //encendido de la luz de fondo  
   nec_ir_init();// Inicializa el protocolo NEC
   lcd_clear();//limpia la pantalla
   //mensaje de inicio
   lcd_gotoxy(1,1);
   lcd_putc("Address: 0x0000");
   lcd_gotoxy(1,2);
   lcd_putc("Code: 0x0000");
   
   while(true)
   {
      duty=0;//variable pwm
      
      if(nec_data_read == 1)// Verifica si ha recibido datos
      {
         nec_data_read = 0;
         nec_get_data();// Obtiene los 32 bits
         unsigned long address = read_ir_address();// Lee la direccion
         unsigned long ir_code = read_ir_code();// Lee el comando         
         
         if(ir_code == 0x00FF || ir_code == 0x42BD){// Tecla ON, enciende el pwm al 100%
            duty = 1022;            
         }
         else if(ir_code == 0x02FD){// Tecla Mute, apaga el pwm al 0%
            duty = 1;            
         }         
         
         if (ir_code == 0x04FB){
            duty = 10;            
         }
         
         if (ir_code == 0x05FA){
            duty = 21;
         }
         
         if (ir_code == 0x06F9){
            duty = 31;
         }
         
         if (ir_code == 0x08F7){
            duty = 41;
         }
         
         if (ir_code == 0x09F6){
            duty = 51;
         }
         
         if (ir_code == 0x0AF5){
            duty = 61;
         }
         
         if (ir_code == 0x0CF3){
            duty = 71;
         }
         
         if (ir_code == 0x0DF2){
            duty = 82;
         }
         
         if (ir_code == 0x0EF1){
            duty = 92;
         }
         
         //incremento y decremento con varios botones         
         if(ir_code == 0x010EF){                          // Tecla 1, apaga el pwm al 10%
            duty = 102;
         }
         if(ir_code == 0x11EE){                          // Tecla 2, apaga el pwm al 20%
            duty = 205;
         }
         if(ir_code == 0x12ED){                          // Tecla 3, apaga el pwm al 30%
            duty = 307;
         }
         if(ir_code == 0x14EB){                          // Tecla 4, apaga el pwm al 40%
            duty = 409;
         }
         if(ir_code == 0x15EA){                          // Tecla 5, apaga el pwm al 50%
            duty = 511;
         }
         if(ir_code == 0x16E9){                          // Tecla 6, apaga el pwm al 60%
            duty = 614;
         }
         if(ir_code == 0x18E7){                          // Tecla sel = 7, apaga el pwm al 70%
            duty = 716;
         }
         if(ir_code == 0x19E6){                          // Tecla - = 8, apaga el pwm al 80%
            duty = 818;           
         }
         if(ir_code == 0x1AE5){                          // Tecla + = 9, apaga el pwm al 90%
            duty = 921;            
         }

        set_pwm1_duty(duty);
         
         
         lcd_clear();         
         lcd_gotoxy(1,1);
         //printf(lcd_putc,"Address: 0x%LX",address);
         printf(lcd_putc,"duty: %lu",duty);//valor pwm         
         lcd_gotoxy(1,2);
         //printf(lcd_putc,"Code: 0x%LX", ir_code);
         printf(lcd_putc,"C: 0x%LX", ir_code);//direccion del control
         printf(lcd_putc,"A: 0x%LX",address);//comando hexadecimal

      }
   }
}

```

## Video de trabajo en fisico

