#include <FastLED.h>
#include <SD.h>

#define CHIP_SELECT 53
#define DATA_OUT 51
#define DATA_IN 50
#define CLOCK 52
#define CUBE_PIN 48
#define NUM_LEDS 150

CRGB leds[NUM_LEDS];
File frame;

void read_files(){
  frame = SD.open("colors/all.txt");
  if (frame) {
    int color_value_position=0;
    String current_color = "";
    bool skip_next=false;
    int current_led=0;
    int current_led_in_the_row=0;
    
    while (frame.available()){
      char color_value=frame.read();
     
      if(color_value == '\n'){
        if(skip_next){skip_next= false; continue;}
        
        if(current_led_in_the_row==3){
          current_led_in_the_row= 0;
          current_led += 9;
        }

        leds[current_led] = current_color.toInt();
        current_led++;
        current_led_in_the_row++;
        current_color="";
        }else if(color_value == '_'){
          color_value_position=0;
          current_color="";
          current_led_in_the_row= 0;
          current_led = 0;
          skip_next=true;
          FastLED.show();
        }else{
          current_color += color_value;
        }
    } //frame.available()
  } // frame
  frame.close();
}

void setup() {
  FastLED.addLeds<WS2812B, CUBE_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  SD.begin();
  Serial.begin(115200);

//  read_files();
  
}

void loop() {
    read_files();
}
