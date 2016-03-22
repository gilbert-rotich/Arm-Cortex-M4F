#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"


#define PWM_FREQUENCY 25


int main(void) {
	volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint32_t ui8Adjust;
	ui8Adjust =83;

	//set clock
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	//configure PWM clock divide system clock by 64.
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	//enable peripherals
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); //module 1 controls the lights
	//ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);


	//configure pin as PWM
	ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);

	//Port F pin o and pin 4 are connected to s2 and s1 switches on the launchpad
	//unlocks the GPIOLOCK register for PF0 using direct register programming
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0X01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4| GPIO_PIN_0, GPIO_DIR_MODE_IN);
	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// count that will be loaded into the Load register
	ui32PWMClock = SysCtlClockGet()/64;

	//subract 1 since the counter will go down to zero
	ui32Load = (ui32PWMClock/PWM_FREQUENCY)-1;

	//config PWM options
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);


	//set period in clock ticks
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 5000);  // 20ms
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 5000);

	//set the pulse width
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,250);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,250);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,250);

	//enable PWM generator
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	//turn on output pins
	ROM_PWMOutputState(PWM1_BASE,PWM_OUT_5_BIT|PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);

	//PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2,2000);

	while(1)
		// to see if SW2 is pressed
	{
		SysCtlDelay((SysCtlClockGet()/(3*1000))*500);
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,250+(250/2));
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,250+(250/2));
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,250+(250/2));

		SysCtlDelay((SysCtlClockGet()/(3*1000))*500);
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,250);
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,250);
		ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,250);

/*		if(ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0X00)
		{
			ui8Adjust--;
			if(ui8Adjust<56)
			{
				ui8Adjust = 56;
			}
			ROM_PWMPulseWidthSet(PWM1_BASE,PWM_OUT_0, ui8Adjust*ui32Load/1000);
		}
		//see if Sw1 is pressed to increment the pulse width
		if(ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) ==0x00)
		{
			ui8Adjust++;
			if(ui8Adjust>111)
			{
				ui8Adjust = 111;
			}
			ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust*ui32Load/1000);
		}

		ROM_SysCtlDelay(4000000);
*/
	}
}
