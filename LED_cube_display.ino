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
File temp;
long int face_color_values[10][3]={
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
};


long int temp_face_color_values[10][3]{
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
};

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
    String current_color = "";
    bool skip_next=false;
    int current_row=0;
    int current_led_in_the_row=0;
    int start_position=0;
    
    while (frame.available()){
      char color_value=frame.read();

      if(color_value == '\n'){
        if(skip_next){skip_next= false; continue;}
        
        if(current_led_in_the_row==3){
          current_led_in_the_row= 0;
          current_row++;
        }
        
        set_faces_colors(current_color.toInt(), current_row, current_led_in_the_row);
        current_led_in_the_row++;
        current_color="";
      }else if(color_value == '_'){
        //reset everything
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

void read_into_array(){
  temp = SD.open("display/n9.txt");
  if (temp) {
    String current_color = "";
    int current_row=0;
    int current_led_in_the_row=0;
    while (temp.available()){
      char color_value=temp.read();
      if(color_value == '\n'){
        if(current_led_in_the_row==3){
          current_led_in_the_row= 0;
          current_row++;
        }

        face_color_values[(9-current_row)][(2-current_led_in_the_row)]=current_color.toInt();
        current_led_in_the_row++;
        current_color="";
      }else{
        current_color += color_value;
      }
    } //frame.available()
  } // frame
  temp.close();
}

void show_on_cube(){
  for(int x=0; x<3; x++){
    for(int y=9; y>=0; y--){
      set_faces_colors(temp_face_color_values[y][x], y, x);
    }
  }

  FastLED.show();
  delay(250);
  FastLED.clear();
  delay(250);
}

void shift_and_dispay(){
  //copy to temporary array
  for(int x=0; x<3; x++){
      for(int y=9; y>=0; y--){
        temp_face_color_values[y][x]=face_color_values[y][x];
      }
  }

  show_on_cube();
  
  //shift 3rd col to 2nd col
  for(int x=0; x <3; x++){
    for(int y=9; y>=0; y--){
      if(x<2){
        temp_face_color_values[y][x]=temp_face_color_values[y][x+1];
      }else if(x==2){
        temp_face_color_values[y][x]=0;
      }
    }
  }

  show_on_cube();

  // shift 2nd col to 1st col
  for(int x=0; x <2; x++){
    for(int y=9; y>=0; y--){
      if(x<2){
        temp_face_color_values[y][x]=temp_face_color_values[y][x+1];
      }else if(x==1){
        temp_face_color_values[y][x]=0;
      }
    }
  }

  show_on_cube();

  //set 0 col to 0
  for(int y=9; y>=0; y--){
    temp_face_color_values[y][0]=0;
  }

  show_on_cube();
}

void setup() {
  FastLED.addLeds<WS2812B, CUBE_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  SD.begin();
  Serial.begin(115200);

//  read_files();
}


void loop() {
  read_into_array();
  shift_and_dispay();

//    read_files();
}
