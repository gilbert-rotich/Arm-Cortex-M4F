#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"



int main(void)
{
	//set up the system clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Enable the UART0 and GPIOA peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//Configure the pins for the receiver and transmitter
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//Initialize the parameter for the UART
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	// Array to store the data read from the ADC FIFO
	//In this I used the sequencer 1 which has a FIFO depth of 4
	uint32_t ui32ADC0Value[4];

	// variables that are used to store the temperature from the sensor data
	volatile uint32_t ui32TempAvg;
	volatile uint32_t ui32TempValueC;
	volatile uint32_t ui32TempValueF;

	//Enabling the ADC0 peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	//setting up the sampling rate
	SysCtlDelay(3);


	//configuring the ADC sequencer
	ADCSequenceConfigure(ADC0_BASE, 1 , ADC_TRIGGER_PROCESSOR, 0 );


	//configure step 1 on sequence 0. This will sample the temperature sensor
	//(ADC_CTL_TS)
	ADCSequenceStepConfigure(ADC0_BASE, 1 , 0, ADC_CTL_TS);
	ADCSequenceStepConfigure(ADC0_BASE, 1 , 1, ADC_CTL_TS);
	ADCSequenceStepConfigure(ADC0_BASE, 1 , 2, ADC_CTL_TS);


	//sample the temperature (ADC_CTL_TS),
	//configure the interrupt flag (ADC_CTL_IE)
	//and  sampling to be done by telling the ADC logic that this the last conversion(ADC_CTL_END)
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS |ADC_CTL_IE | ADC_CTL_END);

	//Enable the ADC sequencer 1
	ADCSequenceEnable(ADC0_BASE, 1);


	//sample the temperature sensor infinitely. Display the value on the console
	while(1){
		//clear the interrupt status flag by making sure it is cleared before sampling
/*		ADCIntClear(ADC0_BASE, 1);


		//trigger the ADC conversion
		ADCProcessorTrigger(ADC0_BASE, 1);

		//wait for the conversion to finish
		while(ADCIntStatus(ADC0_BASE, 1, false)){
		}

		//get or read the ADC value
		ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);

		//Calculating the avg temp using fixed points
		ui32TempAvg = (ui32ADC0Value[0]+ui32ADC0Value[1]+ui32ADC0Value[2]+ui32ADC0Value[3]+2)/4;


		// calculating temperature value in celsius the using an equation provided in the data sheet
		ui32TempValueC = (1475 - ((2475*ui32TempAvg))/4096)/10;


		//compute the temperature in farenheit
		ui32TempValueF = ((ui32TempValueC*9)+160)/5;
*/
		UARTprintf("Temp value");

		//UARTCharPut(UART0_BASE, 'e');

		SysCtlDelay(80000000/12);
	}


}
