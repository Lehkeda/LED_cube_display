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

void set_face_one(long int color, int row, int col){
  leds[((row*12)+col)]=color;
}
void set_face_two(long int color, int row, int col){
  leds[((row*12)+3+col)]=color;
}
void set_face_three(long int color, int row, int col){
  leds[((row*12)+6+col)]=color;
}
void set_face_four(long int color, int row, int col){
  leds[((row*12)+9+col)]=color;
}

void set_faces_colors(long int color, int row, int col){
  set_face_one(color, row, col);
  set_face_two(color, row, col);
  set_face_three(color, row, col);
  set_face_four(color, row, col);
}

void read_files(){
  frame = SD.open("display/new.txt");
  if (frame) {
    int color_value_position=0;
    String current_color = "";
    bool skip_next=false;
    int current_row=0;
    int current_led_in_the_row=0;
    
    while (frame.available()){
      char color_value=frame.read();
     
      if(color_value == '\n'){
        if(skip_next){skip_next= false; continue;}
        
        if(current_led_in_the_row==3){
          current_led_in_the_row= 0;
          current_row++;
        }
        
//        leds[current_led] = current_color.toInt();
        set_faces_colors(current_color.toInt(), current_row, current_led_in_the_row);
//        current_led++;
        current_led_in_the_row++;
        current_color="";
        }else if(color_value == '_'){
          color_value_position=0;
          current_color="";
          current_led_in_the_row= 0;
          current_row = 0;
          skip_next=true;
          FastLED.show();
          delay(5);
          FastLED.clear();
          delay(5);
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
