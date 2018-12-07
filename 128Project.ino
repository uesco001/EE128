// This is our BST
unsigned char alphabet[] = {'0','T','E','M','N','A','I','O','G','K','D','W',
'R','U','S','1','2','Q','Z','Y','C','X','B','J','P','3','L','4', 'F','V','H'};
// This is the BST
unsigned char Long = 2; // Long pulse
unsigned char Short = 1; // Short pulse
unsigned char pulse [5] = {0,0,0,0,0};
unsigned char UARTflag = 0;


ISR(TIMER1_COMPA_vect)
{
  UARTflag = 1;
  Serial.print('p');
 

}
//****************************** This funtion is BST *****************************  
unsigned char search(unsigned char cnt, unsigned char i, unsigned char value[])
{
  if(value[cnt] == Long)
  {
    i = i*2 + 1; // increment index for long value 
    cnt++;       // increment cnt
    search(cnt,i,value); // call funtion again
  }
  else if( value[cnt] == Short)
  {
    i = i*2 + 2;    // increments index for short value
    cnt++;          // incremetn cnt
    search(cnt,i,value); // call funtion again
  }
  else
  {
    return alphabet[i];  // base case 
  }
}
//****************************** This funtion is BST *****************************


unsigned char getSignal(){
  
  unsigned char value,x,i,b;
  long timedEvent[8] = {0};
  unsigned char  pulse[5] = {0};
  long pulse_width = 0;
  long upper_long = 20625;
  long lower_long = 4550;
  long upper_short = 4500;
  long lower_short = 0;
  unsigned int eventCount = 0;
  TCCR1B = 0x00;                    // so timer doesn't go off
  TIMSK1 = 0x00;            //Timer/Counter1 Interrupt Mask Register for OCR1A 
  TCCR1A = 0;
  eventCount = 0;
  UARTflag = 0;
  TCNT1 = 0;
  TIFR1 = (1<<ICF1);                             // Clear ICF (Input Capture flag) flag
  TCCR1B = 0x00;                               // Falling Edge, no prescaler
Serial.print("wait\n");
  while ( (TIFR1&(1<<ICF1)) == 0);              // Wait for timer to detect Falling Edge
  TIFR1 = (1<<ICF1); 
  TCNT1 = 0;
  TCCR1B = 0x05;                            // prescale to 64 
  OCR1A = 65535;   
  TIMSK1 = 0x02;                    
  eventCount = 1;
  x = 1;

  while((eventCount < 8) && (UARTflag == 0)){
      TCCR1B ^= 1<<6 ;
      Serial.print('r');
      while ( !( (TIFR1&(1<<ICF1)))){if(UARTflag){break;} } // Wait for timer to detect Falling Edge

      TIFR1 = (1<<ICF1); 
      Serial.print('t');
      if( (eventCount < 8 ) && !UARTflag){
        timedEvent[eventCount] = ICR1;
        TIFR1 = (1<<ICF1);                         // Clear ICF flag
        eventCount++;
       
        }
      else                                         //Can I optimize this
      {
        eventCount = 8;
      }
      
      
      
  }

   
       TIMSK1 = 0;
     
       TCCR1B = 0x00;   /// stop the timer
   
       /////////////////////////////// FINNA TRYNNA CONVERT TIME TO 1 AND 0
       
       for(i = 0; i < 4; i++){
          pulse_width = timedEvent[i*2 + 1] - timedEvent[i*2];
          if(/*pulse_width < upper_long && */pulse_width > lower_long )
          {
            value = 2;
          }
          else if( pulse_width < upper_short && pulse_width > lower_short)
          {
            value = 1;
          }
          else{
            value = 0;
          }
          Serial.print(timedEvent[i*2 + 1],DEC);
          Serial.print(" - ");
          Serial.print(timedEvent[i*2],DEC);
          Serial.print(" = ");
          Serial.print(pulse_width),DEC;
          Serial.print('\n');
          pulse[i] = value; 
       }
       b = search(0,0,pulse);
       return b;
     
      
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

Serial.write(getSignal());
Serial.print('\n');


}
