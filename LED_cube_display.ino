#include <FastLED.h>
#include <SD.h>
#include "letters.h"

#define CHIP_SELECT 53
#define DATA_OUT 51
#define DATA_IN 50
#define CLOCK 52
#define CUBE_PIN 48
#define NUM_LEDS 150

CRGB leds[NUM_LEDS];
File frame;
File temp;
char *(letters)[256];

long int current_letter[10][3]={
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

long int next_letter[10][3]={
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

void set_letters_array(){
  letters['1']=number_1;
  letters['2']=number_2;
  letters['3']=number_3;
  letters['4']=number_4;
  letters['5']=number_5;
  letters['6']=number_6;
  letters['7']=number_7;
  letters['8']=number_8;
  letters['9']=number_9;
  letters['0']=letter_o;
  letters['a']=letter_a;
  letters['b']=letter_b;
  letters['c']=letter_c;
  letters['d']=letter_d;
  letters['e']=letter_e;
  letters['f']=letter_f;
  letters['g']=letter_g;
  letters['h']=letter_h;
  letters['i']=letter_i;
  letters['j']=letter_j;
  letters['k']=letter_k;
  letters['l']=letter_l;
  letters['m']=letter_m;
  letters['n']=letter_n;
  letters['o']=letter_o;
  letters['p']=letter_p;
  letters['q']=letter_q;
  letters['r']=letter_r;
  letters['s']=letter_s;
  letters['t']=letter_t;
  letters['u']=letter_u;
  letters['v']=letter_v;
  letters['w']=letter_w;
  letters['x']=letter_x;
  letters['y']=letter_y;
  letters['z']=letter_z;
  letters[' ']=space;
  letters['-']=dash;
  
}

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

void set_current_letter_color(char letter){
  for(char x=0; x<3; x++){
    for(char y=9; y>=0; y--){
      //if the current bit in the letter array is set to on, set it's color to gold 
      //otherwise, set it to black
      current_letter[y][x]=(bitRead(letters[letter][y], 2-x)? CRGB::Gold : 0);
    }
  }  
}

void set_next_letter_color(char letter){
  for(char x=0; x<3; x++){
    for(char y=9; y>=0; y--){
      //if the current bit in the letter array is set to on, set it's color to gold 
      //otherwise, set it to black
      next_letter[y][x]=(bitRead(letters[letter][y], 2-x)? CRGB::Gold : 0);
    }
  }  
}


void show_on_cube(){
  for(int x=0; x<3; x++){
    for(int y=9; y>=0; y--){
      set_faces_colors(current_letter[y][x], 9-y, x);
    }
  }

  FastLED.show();
  delay(70);
  FastLED.clear();
  delay(70);
}

void shift_and_dispay(){

  show_on_cube();
  
  //shift 3rd col to 2nd col
  for(int x=0; x <3; x++){
    for(int y=9; y>=0; y--){
      if(x<2){
        current_letter[y][x]=current_letter[y][x+1];
      }else if(x==2){
        current_letter[y][x]=0;
      }
    }
  }

  show_on_cube();

  // shift 2nd col to 1st col
  for(int x=0; x <3; x++){
    for(int y=9; y>=0; y--){
      if(x<2){
        current_letter[y][x]=current_letter[y][x+1];
      }else if(x==1){
        current_letter[y][x]=0;
      }else if(x==2){
        //copy next letter col 0 to current letter col 2
         current_letter[y][x]=next_letter[y][0];
      }
    }
  }

  show_on_cube();

  //set 0 col to 0
  for(int x=0; x <3; x++){
    for(int y=9; y>=0; y--){
      if(x==0){
        current_letter[y][x]=0;
      }else if(x>=1){
        // copy next letter col 0 to current letter col 1 
        // and next letter col 1 current letter col 2 
        current_letter[y][x]=next_letter[y][x-1];
      }
    }
  }
  show_on_cube();

  //copy next letter to current letter
  for(int x=0; x <3; x++){
    for(int y=9; y>=0; y--){
        current_letter[y][x]=next_letter[y][x];
      }
    }

  show_on_cube();
  
}

void parse_message(char msg[200]){
  for(int i=0; msg[i]; i++){
    set_current_letter_color(msg[i]);
    if(!msg[i+1]){
      set_next_letter_color(' '); 
    }else{
      set_next_letter_color(msg[i+1]);   
    }
    shift_and_dispay();
  }
//  current_letter_done
  
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

void setup() {
  FastLED.addLeds<WS2812B, CUBE_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  SD.begin();
  Serial.begin(115200);
  set_letters_array();

//  read_files();
}


void loop() {
  read_files();

  delay(1500);
  
  parse_message("hello world");
  delay(500);
  
  parse_message("how are you");

  delay(1500);

  parse_message("bye world");
  delay(200);

//    read_files();
}
