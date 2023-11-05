/* #include <NEC_ir.c>
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 03/03/2018
* Electronica y Circuitos
*
*/

int1 bits_ir[32]; 
int16 Ta,Tb,ticks;
int8 n; 
int1 bit_start_ir, nec_data_read;
int byte_ir[4];
#define NEC_HARDWARE

#ifdef NEC_HARDWARE
#INT_EXT
void interrupt_isr()
{
    Ta = get_timer1();
    ticks = Ta-Tb;
    Tb = Ta;
    
    if(bit_start_ir == 1){
      if(ticks>1350 && ticks < 1450){   // 2812.5   BIT 1
         bits_ir[n] = 1;
         n++;
      }
      if(ticks>650 && ticks < 750){     // 140       BIT 0
         bits_ir[n] = 0;
         n++;
      }
      if(n == 32){
         nec_data_read = 1;
      }
   }
   if(ticks>8200 && ticks < 8600){      // 1687    BIT START
      bit_start_ir = 1;
      n = 0; 
   }
}
#endif

void nec_ir_init(void)
{
   output_float(PIN_B0);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   ext_int_edge(0, H_TO_L);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
}

int8 bit_ir_byte(int1 D0, int1 D1, int1 D2, int1 D3, int1 D4, int1 D5, int1 D6, int1 D7)
{
   int8 dato;
   dato = D7*128 + D6*64 + D5*32 + D4*16 + D3*8 + D2*4 + D1*2 +D0*1;
   return dato; 
}

void nec_get_data(void)
{
    byte_ir[0] = bit_ir_byte(bits_ir[0],bits_ir[1],bits_ir[2],bits_ir[3],bits_ir[4],bits_ir[5],bits_ir[6],bits_ir[7]);
    byte_ir[1] = bit_ir_byte(bits_ir[8],bits_ir[9],bits_ir[10],bits_ir[11],bits_ir[12],bits_ir[13],bits_ir[14],bits_ir[15]);
    byte_ir[2] = bit_ir_byte(bits_ir[16],bits_ir[17],bits_ir[18],bits_ir[19],bits_ir[20],bits_ir[21],bits_ir[22],bits_ir[23]);
    byte_ir[3] = bit_ir_byte(bits_ir[24],bits_ir[25],bits_ir[26],bits_ir[27],bits_ir[28],bits_ir[29],bits_ir[30],bits_ir[31]);
}

unsigned long read_ir_address()
{
  unsigned long ad_ir = ((unsigned long) byte_ir[0]<<8 | byte_ir[1]);
  return ad_ir;
}

unsigned long read_ir_code()
{
  unsigned long cd_ir = ((unsigned long) byte_ir[2]<<8 | byte_ir[3]);
  return cd_ir;
}
