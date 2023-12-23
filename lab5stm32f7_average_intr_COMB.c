// stm32f7_average_intr_COMB.c

#include "stm32f7_wm8994_init.h"
#include "stm32f7_display.h"

#define SOURCE_FILE_NAME "stm32f7_average_intr_COMB.c"
#define N 6
#define ALFA -0.95
#define BETA -0.6

extern int16_t rx_sample_L;
extern int16_t rx_sample_R;
extern int16_t tx_sample_L;
extern int16_t tx_sample_R;

float32_t w[N+1] = {0, 0, 0, 0, 0, 0, 0};

enum filtertype{FIR,IIR};

void BSP_AUDIO_SAI_Interrupt_CallBack()
{
// when we arrive at this interrupt service routine (callback)
// the most recent input sample values are (already) in global variables
// rx_sample_L and rx_sample_R
// this routine should write new output sample values in
// global variables tx_sample_L and tx_sample_R
  int16_t i;
  float32_t w0, yn; 
  
  // uncomment just one of the following two lines
  enum filtertype myfilter=FIR;
  // enum filtertype myfilter=IIR;
	

  w0  = (float32_t)(rx_sample_L);
  
  switch (myfilter)
  {
	case FIR:
	   yn = w0 + (float32_t)(ALFA) * w[N];
	   break;
  	case IIR:
	   w0 = w0 + (float32_t)(BETA) * w[N];
		 yn = w0;
		 // yn += (float32_t)(ALFA) * w[N]; // don't uncomment this line
	   break;
	default:
	   yn = w0;
  }
  
  tx_sample_L = (int16_t)(yn);
  tx_sample_R = (int16_t)(yn);

  w[0] = w0;
  for (i=N ; i>0 ; i--) w[i] = w[i-1];

  BSP_LED_Toggle(LED1);

  return;
}

int main(void)
{

  stm32f7_wm8994_init(AUDIO_FREQUENCY_8K,
                      IO_METHOD_INTR,
                      INPUT_DEVICE_INPUT_LINE_1,
                      OUTPUT_DEVICE_HEADPHONE,
                      WM8994_HP_OUT_ANALOG_GAIN_0DB,
                      WM8994_LINE_IN_GAIN_0DB,
                      WM8994_DMIC_GAIN_9DB,
                      SOURCE_FILE_NAME,
	              NOGRAPH);

  while(1){}
}
