//MSP430-G2553
//Alex Steel, Alex Jackson, Mike Johns, Ben Zalewski, Ryan Boylan
//Final Project
#include <msp430.h>
void PIN_SETUP(void);                   //Declare LED function
void UART(void);                        //Declare UART function
int counter = 0;                        //Initialize counter variable
int combo [4] = {1,2,3,4};              //Initialize combo array
int input [9] = {0,0,0,0,0,0,0,0,0};    //Initialize input array
const int reset = 255;                  //Initialize reset constant
int main(void)                          //Main function
{
    WDTCTL = WDTPW | WDTHOLD;           //stop watchdog timer
    PIN_SETUP();                        //Call PIN_SETUP function
    UART();                             //Call UART function
    __enable_interrupt();               //Enable interrupts.
    while(1)                            //Infinite while loop
    {
        __bis_SR_register(CPUOFF + GIE);
        if(counter==5)                  //Check state of counter
            {
                if(input[0]==combo[0] && input[1]==combo[1] && input[2]==combo[2] && input[3]==combo[3] && input[4] != reset)
                {
                    P2OUT &= ~BIT3;   //turn off pin(disable alarm)
                    P2OUT &= ~BIT5;   //turn off pin(unlock safe)
                    counter=0;      //reset counter
                    input[0] = 0;   //reset input array
                    input[1] = 0;   //reset input array
                    input[2] = 0;   //reset input array
                    input[3] = 0;   //reset input array
                    input[4] = 0;   //reset input array
                }
                else if(input[0]!=combo[0] || input[1]!=combo[1] || input[2]!=combo[2] || input[3]!=combo[3])
                {
                    P2OUT |= BIT3;  //turn on pin(enable alarm)
                    counter = 0;    //reset counter
                    input[0] = 0;   //reset input array
                    input[1] = 0;   //reset input array
                    input[2] = 0;   //reset input array
                    input[3] = 0;   //reset input array
                    input[4] = 0;   //reset input array
                }
            }
        if(counter==9)                  //check state of counter
            {
                if(input[0]==combo[0] && input[1]==combo[1] && input[2]==combo[2] && input[3]==combo[3] && input[4]==reset)
                    {
                    P2OUT &= ~BIT5;     //turn off pin(unlock safe)
                    combo[0] = input[5];  //set new combo to input
                    combo[1] = input[6];  //set new combo to input
                    combo[2] = input[7];  //set new combo to input
                    combo[3] = input[8];  //set new combo to input
                    counter = 0;        //reset counter
                    }
                    input[0] = 0;       //reset input array
                    input[1] = 0;       //reset input array
                    input[2] = 0;       //reset input array
                    input[3] = 0;       //reset input array
                    input[4] = 0;       //reset input array
                    input[5] = 0;       //reset input array
                    input[6] = 0;       //reset input array
                    input[7] = 0;       //reset input array
                    input[8] = 0;       //reset input array
            }
    }
}
void PIN_SETUP(void)        //PIN_SETUP function
{
    P2DIR |= BIT5;          //Set pin direction to output
    P2OUT |= BIT5;          //turn on pin(lock safe)
    P2DIR |= BIT3;          //Set pin direction to output
    P2OUT &= ~BIT3;         //turn off pin(alarm off)
    P1IE  |= BIT3;          //enable interrupt
    P1IES |= BIT3;          //Hi/lo edge trigger
    P1REN |= BIT3;          //Enable pull up resistor
    P1IFG &= ~BIT3;         //Clear interrupt flag
}
void UART(void)             //UART function
{
    P1SEL |= BIT1 + BIT2;   //P1.1 = RXD P1.2 = TXD
    P1SEL2 |= BIT1 + BIT2;  //P1.1 = RXD P1.2 = TXD
    UCA0CTL1 |= UCSSEL_2;   //smclk
    UCA0BR0 = 104;          //1MHz 9600 baud rate
    UCA0BR1 = 0;            //1MHz 9600 baud rate
    UCA0MCTL = UCBRS0;      //modulation UBRSx = 1
    UCA0CTL1 &= ~UCSWRST;   //initialize usci state machine
    IE2 |= UCA0RXIE;        //enable RX interrupt
}
#pragma vector=USCIAB0RX_VECTOR     //UART Interrupt routine
__interrupt void TESTTHING(void)    //interrupt name
{
  while (!(IFG2&UCA0TXIFG));        //Wait until a byte is ready, is USCI_A0 TX buffer ready?
  {
    input[counter] = UCA0RXBUF;   //input array is filled by user input(contents of UCA0RXBUF)
    counter=counter+1;          //increment counter
    if(counter==5 || counter==9)  //if counter = 5 or counter = 9
    {
        __bic_SR_register_on_exit(CPUOFF);
    }
    UCA0TXBUF=UCA0RXBUF;        //transmit back what is recieved
  }
}
#pragma vector=PORT1_VECTOR         //interrupt protocol and memory allocation(when button is pressed)
__interrupt void Port_1(void)       //interrupt name
{
  P2OUT |= BIT5;                    //turn on pin(lock safe)
  P1IFG &= ~BIT3;                   //clear interrupt flag
}
