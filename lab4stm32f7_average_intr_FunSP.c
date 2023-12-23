// stm32f7_average_intr_FunSP.c

#include "stm32f7_wm8994_init.h"
#include "stm32f7_display.h"

#define SOURCE_FILE_NAME "stm32f7_average_intr_FunSP.c"
#define N 5
#define invN 1.0/N

extern int16_t rx_sample_L;
extern int16_t rx_sample_R;
extern int16_t tx_sample_L;
extern int16_t tx_sample_R;

float32_t h[N];
float32_t x[N+1] = {0, 0 ,0, 0, 0, 0};
float32_t yn_1=0.0;

void BSP_AUDIO_SAI_Interrupt_CallBack()
{
// when we arrive at this interrupt service routine (callback)
// the most recent input sample values are (already) in global variables
// rx_sample_L and rx_sample_R
// this routine should write new output sample values in
// global variables tx_sample_L and tx_sample_R
  int16_t i;
  float32_t yn = 0.0;
	

  x[0] = (float32_t)(rx_sample_L);
  for (i=0 ; i<N ; i++) yn += h[i]*x[i];
  tx_sample_L = (int16_t)(yn);
	
  yn = (x[0]-x[N]) * (float32_t) (invN) + yn_1;
	//yn = (x[0]-x[N]);
	
  tx_sample_R = (int16_t) (yn);
	
	for (i=N ; i>0 ; i--) x[i] = x[i-1];
  yn_1 = yn;

  BSP_LED_Toggle(LED1);

  return;
}

int main(void)
{
  int i;

  for (i=0 ; i<N ; i++) h[i] = (float32_t)(invN);

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
