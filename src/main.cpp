#include <Adafruit_CircuitPlayground.h>
#include <arduinoFFT.h>

#define MIC_PIN         A4  // Microphone is attached to this analog pin
#define SAMPLES         128 // Number of samples for FFT
#define SAMPLING_FREQ   10000 // Sampling frequency for the microphone
#define G4_FREQUENCY 392
#define C4_FREQUENCY 262
#define E4_FREQUENCY 330
#define A4_FREQUENCY 440
#define TOLERANCE 15

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

// Function to check if the frequency is within the range of a note
String getNoteName(double frequency) { //five to ten htz of tolerance
  if (abs(frequency - G4_FREQUENCY) <= TOLERANCE) return "G4";
  if (abs(frequency - C4_FREQUENCY) <= TOLERANCE) return "C4";
  if (abs(frequency - E4_FREQUENCY) <= TOLERANCE) return "E4";
  if (abs(frequency - A4_FREQUENCY) <= TOLERANCE) return "A4";
  return "Unknown"; // Return Unknown if no note matches
}

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

  String note = getNoteName(peak);
  
  Serial.println(note);

  

  delay(50);
}
