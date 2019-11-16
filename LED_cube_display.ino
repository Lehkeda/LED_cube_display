#include <FastLED.h>
#include <SD.h>

#define CHIP_SELECT 53
#define DATA_OUT 51
#define DATA_IN 50
#define CLOCK 52
#define CUBE_PIN 48
#define NUM_LEDS 150

CRGB leds[NUM_LEDS];
File myFile;
//String frames_files=
//void 

void read_files(){
  int current_led_in_the_face=0;
  myFile = SD.open("colors");
  if (myFile) {
    while(true){
      FastLED.clear();
      File frame = myFile.openNextFile();
      if(! frame) return;
      int current_led=0;
      int current_led_in_the_row=0;
      
      char current_color[10] = "";
      char i=0;
      while (frame.available()) {
        char chr = frame.read();
        current_color[i] = chr;
        if(chr == '\n'){
          if(current_led_in_the_face == 30){
            current_led_in_the_face=99;
            frame.close();
            break;
            }
          if(current_led_in_the_row==3){
            current_led_in_the_row= 0;
            current_led += 9;
            }

          leds[current_led] = atoi(current_color);
          current_led++;
          current_led_in_the_row++;
          current_led_in_the_face++;
          i=0;
          for(int x=0; x<10; x++){
            current_color[i]=0;
          }
          continue;
          }
          i++;
        }
        frame.close();

        if(current_led_in_the_face==99){
          current_led_in_the_face=0;
          continue;
          }
        FastLED.show();

      }
      myFile.close();
    }
    
}

void setup() {
  FastLED.addLeds<WS2812B, CUBE_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  SD.begin();
  Serial.begin(115200);

  
  
}

void loop() {
    read_files();
}
