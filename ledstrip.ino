#include <RunningMedian.h>
#include <Adafruit_NeoPixel.h>

#define PIN 3   // input pin Neopixel is attached to
#define STROBE 4
#define RESET 5
#define DC_One A0
#define DC_Two A1

#define NUMPIXELS      23 // number of neopixels in Ring
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // timing delay

int redColor = 255;
int greenColor = 255;
int blueColor = 255;

int freq_amp;
int Frequencies_One[7];
int Frequencies_Two[7];
int j;
int prevFreq = 0;

//int smooth[50];
RunningMedian smooth = RunningMedian(5);

int t = 0;

void setup() {
  pixels.begin(); // Initializes the NeoPixel library.
  Serial.begin(9600);

  // Set spectrum shield pin configurations
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(DC_One, INPUT);
  pinMode(DC_Two, INPUT);
  digitalWrite(STROBE, HIGH);
  digitalWrite(RESET, HIGH);

  // Initialize Spectrum Analyzers
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(STROBE, HIGH);
  delay(1);
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, LOW);
}

void loop() {
  Read_Frequencies();
  Graph_Frequencies();
//  fadeOut();
  delay(5);
}

void fadeOut() {
  int reduceRate = 4;
  redColor = max(redColor - reduceRate, 0);
  greenColor = max(greenColor - reduceRate, 0);
  blueColor = max(blueColor - reduceRate, 0);

  if (redColor == 0 && greenColor == 0 && blueColor == 0) {
    setColor();
    t = 0;
  }

  for (int i=0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));
    pixels.show();
  }
}

int maxVal(int a, int b) 
{
  if (a > b) {return a;}
  else {return b;}
}

void setColor(){
  Serial.println("SETTING COLOR.");
  redColor = random(0, 255);
  greenColor = random(0,255);
  blueColor = random(0, 255);  
}

void Read_Frequencies() {
  for (freq_amp = 0; freq_amp < 7; freq_amp++) {
//    digitalWrite(STROBE, HIGH);
//    delayMicroseconds(50);
//    digitalWrite(STROBE, LOW);
//    delayMicroseconds(50);

    Frequencies_One[freq_amp] = analogRead(DC_One);
    Frequencies_Two[freq_amp] = analogRead(DC_Two);
    digitalWrite(STROBE, HIGH);
    digitalWrite(STROBE, LOW);
  }
}

void setLights(int index, int r, int g, int b) {
  pixels.setPixelColor(index, pixels.Color(r, g, b));
  int initialLed = NUMPIXELS / 2;

//  pixels.setPixelColor(initialLed + (r / 2 - 7) + 1, pixels.Color(0, 0, 0));
//  pixels.setPixelColor(initialLed - (r / 2 - 7) - 1, pixels.Color(0, 0, 0));
  for (int i = 0; i < r / 2 - 7; i++) {
//    pixels.setPixelColor(initialLed + i + 1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(initialLed + i, pixels.Color(255, 0, 0));
//    pixels.setPixelColor(initialLed + i - 1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(initialLed - i, pixels.Color(255, 0, 0));
  }
  
  pixels.show();
}

void setAllLights(int r, int g, int b) {
  for (int l = 0; l < NUMPIXELS; l++) {
    setLights(l, r, g, b);
  }
}

void Graph_Frequencies() {
  int freqVal = maxVal(Frequencies_One[1], Frequencies_Two[1])/4;
  int freqVal2 = maxVal(Frequencies_One[0], Frequencies_Two[0])/4;

//  Serial.println(freqVal * freqVal2/20);
  int curVal = freqVal * freqVal2 / 4;
//  Serial.println(abs(curVal - prevFreq));
  prevFreq = curVal;

  smooth.add(freqVal ^ 3);
//  smooth.add(abs(curVal - prevFreq));
//  Serial.println(freqVal^2);
//  for (j = 0; j < 7; j++) {
//    for (j = 0; j < 2; j++) {
//    if ( Frequencies_Two[j] > Frequencies_One[j]) {
        
//      pixels.setPixelColor(j, pixels.Color(Frequencies_Two[j]/4, Frequencies_Two[j]/4, Frequencies_Two[j]/4));
//      pixels.show();
//      Serial.println(Frequencies_Two[j]);
//    } else {
//      pixels.setPixelColor(j, pixels.Color(Frequencies_One[j]/4, Frequencies_One[j]/4, Frequencies_One[j]/4));
//      pixels.show();
//      Serial.println(Frequencies_One[j]);
//    }
//  }
  int median = maxVal(smooth.getAverage(), 15) / 2;
  Serial.println(median);

  setAllLights(median, median, median);
}

//void updateSmooth(int val) {
////  Serial.print("Size of smooth: ");
////  Serial.println(sizeof(smooth) / sizeof(smooth[0]));
//  int len = sizeof(smooth) / sizeof(smooth[0]);
//  for (int i = 0; i < len - 1; i++) {
//    smooth[i] = smooth[i + 1];
//  }
//  smooth[len - 1] = val;
////  displaySmooth();
//}
//
//void displaySmooth() {
//  int len = sizeof(smooth) / sizeof(smooth[0]);
//  for (int i = 0; i < len; i++) {
//    Serial.print(smooth[i]);
//    Serial.print(" ");
//  }
//  Serial.println("");
//}
//
////void getSmoothMedian() {
////  int len = sizeof(smooth) / sizeof(smooth[0]);
////  
////}
