#define Smoke PORTB.RB0
#define Fire  PORTB.RB1

#define buzzer PORTB.RB2
long temp_res;

void main() 
{
  ADCON1 = 0x0f;  // Configure analog inputs and Vref
  TRISA  = 0xFF;  // PORTA is input
  Usart_Init(9600);
  TRISB.RB0 = 1;
  TRISB.RB1 = 1;        
  TRISB.RB2 = 0;
  buzzer = 1;  
  
  while(1)
  {
    temp_res = Adc_Read(2); // Get results of AD conversion
    temp_res = (temp_res*50000)/1023;
                                     
    Usart_Write('D');                   
    Usart_Write('A');                   
    Usart_Write('T');                   
    Usart_Write('A');
    Usart_Write((temp_res/1000)+48);
    Usart_Write(((temp_res%1000)/100)+48);                                
    Usart_Write(((temp_res%100)/10)+48);
    Usart_Write((temp_res%10)+48);

    if(Smoke == 1)
    {
      Usart_Write('1');                   
    }
      
    if(Smoke == 0)
    {
      Usart_Write('0');                   
    }
    
    if(Fire == 1)
    {
      Usart_Write('1');                   
    }
    
    if(Fire == 0)
    {
      Usart_Write('0');                   
    }
    
    if(Smoke == 0 || Fire == 0)
    {
     buzzer = 0;
     Usart_Write('1');                   
    }
    
    else
    {
     buzzer = 1;                          
     Usart_Write('0');                   
    }
    
    
    Delay_ms(2000);
    
   }
}


