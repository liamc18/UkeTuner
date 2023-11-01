#include <Adafruit_CircuitPlayground.h>
#include <arduinoFFT.h>

#define MIC_PIN         A4  // Microphone is attached to this analog pin
#define SAMPLES         128 // Number of samples for FFT
#define SAMPLING_FREQ   10000 // Sampling frequency for the microphone

arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQ));
}
//five to ten htz
void loop() {
  microseconds = micros();

  for(int i=0; i<SAMPLES; i++)
  {
      vReal[i] = analogRead(MIC_PIN);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
          // wait for the next sampling period
      }
      microseconds += sampling_period_us;
  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQ);

  Serial.println(peak);  // Print the frequency of the detected peak

  delay(500);
}
