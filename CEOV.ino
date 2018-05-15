//A brain enhancer to conquer random
//special thanks to 

#include <Arduboy2.h>
#include <Sprites.h>
#include "autopowerdown.h"
Arduboy2 arduboy;

#define OFF HIGH

//computechallenge variables
int LNpair;
int LNpairmin = 0;
int LNpairmax = 8;
int Vnu, Cnu;
bool LNPvowel, LNPeven, LNPe1, LNPy;
const char    vowel[]      = "AEIOUAEIOUAEIOUAEIOUYaeiouaeiouaeiouaeiouy";
const char    consonant[]  = "BCDFGHJKLMNPQRSTVWXZYbCdfghjklmnpqrstvwxzy";
      int     VCL          = 42;
const char    even[]       = "02468";
const char    odd[]        = "13579";
const int     EOL          = 5;

//computekm
const int Sarr[] = {0,1,2,3,4,5,6,7};
const int Ssize = sizeof(Sarr)/ sizeof(Sarr[0]);
int Darr[] = {0,1,2,3}; // {0,1,2,3,4,5,6,7} for 8-choice game (order critical)
int Dsize = sizeof(Darr)/ sizeof(Darr[0]);
int gametype = 0; //0 == 4-choice game, 1 == 8-choice game

//swaptool
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void RDarr (int Sarr[], int n){
  for (int k = n-1; k > 0; k--) {
    swap(&Darr[k], &Sarr[k]);
  }
}

void randomizearr ( int Darr[], int Dsize )
{
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = Dsize-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = random() % (i+1);
        // Swap arr[i] with the element at random index
        swap(&Darr[i], &Darr[j]);
    }
}

//user variables
boolean led[2] = { OFF, OFF };
int Qright = 0;
int Qwrong = 0;
int Tanswered = 0;
int ratio = Qright/ Qwrong;
int Qlooked = 0;

//other variables
bool firstpress = false;
int state = 0;
int choice = 0;
int maxchoices = 0;
boolean buttonfree = true;
boolean showbutton = false;
char challenge[1];
bool gameinprogress = false;

int leftcounter, rightcounter, horizontalcounter, upcounter, downcounter, verticalcounter;
bool uppress, leftpress, rightpress, downpress, consent = false;
int keylinger = 4;
int sx, sy;
int sw = 27; int sh = 22; int sc = 6;

void computechallenge() {
  LNpair = random(LNpairmin, LNpairmax);
  switch(LNpair) {
    case 0:
      LNPeven = true; LNPvowel = true; LNPe1 = true;
      challenge[0] = even[random(EOL)];
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[1] = vowel[Vnu];break;
    case 1:
      LNPeven = true; LNPvowel = false; LNPe1 = true;
      challenge[0] = even[random(EOL)];
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[1] = consonant[Cnu];break;
    case 2:
      LNPeven = false; LNPvowel = true; LNPe1 = true;
      challenge[0] = odd[random(EOL)];
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[1] = vowel[Vnu];break;
    case 3:
      LNPeven = false; LNPvowel = false; LNPe1 = true;
      challenge[0] = odd[random(EOL)];
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[1] = consonant[Cnu];break;   
    case 4:
      LNPvowel = true; LNPeven = true; if(gametype == 0){LNPe1 = true;} else {LNPe1 = false;}
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[0] = vowel[Vnu];
      challenge[1] = even[random(EOL)];break;
    case 5:
      Vnu = random(VCL);
      LNPvowel = true; LNPeven = false; if(gametype == 0){LNPe1 = true;} else {LNPe1 = false;}
      if(Vnu == 20 || Vnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[0] = vowel[Vnu];
      challenge[1] = odd[random(EOL)];break;
    case 6:
      LNPvowel = false; LNPeven = true; if(gametype == 0){LNPe1 = true;} else {LNPe1 = false;}
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[0] = consonant[Cnu];
      challenge[1] = even[random(EOL)];break;
    case 7:
      LNPvowel = false; LNPeven = false; if(gametype == 0){LNPe1 = true;} else {LNPe1 = false;}
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){LNPy = true;} else{LNPy = false;}
      challenge[0] = consonant[Cnu];
      challenge[1] = odd[random(EOL)];break;
  }
}

  int greenlight =0;
  int redlight =0;
  
void correct(){
  //greenlight = 100;
  Qright++;
  Tanswered++;
  computechallenge();
  randomizearr (Darr, Dsize);
}

void incorrect(){
  //redlight = 100;
  Qwrong++;
  Tanswered++;
  computechallenge();
  randomizearr (Darr, Dsize);
}

void akm(int c) {
  switch(c){
  case 0: //EV
  if (LNPeven == true && (LNPvowel == true || LNPy == true) && LNPe1 == true) {correct();}
  else {incorrect();}break;
  case 1: //EC
  if (LNPeven == true && (LNPvowel == false || LNPy == true) && LNPe1 == true) {correct();}
  else {incorrect();}break;
  case 2: //OV
  if (LNPeven == false && (LNPvowel == true || LNPy == true) && LNPe1 == true) {correct();}
  else {incorrect();}break;
  case 3: //OC
  if (LNPeven == false && (LNPvowel == false || LNPy == true) && LNPe1 == true) {correct();}
  else {incorrect();}break;
  case 4: //VE
  if (LNPeven == true && (LNPvowel == true || LNPy == true) && LNPe1 == false) {correct();}
  else {incorrect();}break;
  case 5: //VO
  if (LNPeven == false && (LNPvowel == true || LNPy == true) && LNPe1 == false) {correct();}
  else {incorrect();}break;
  case 6: //CE
  if (LNPeven == true && (LNPvowel == false || LNPy == true) && LNPe1 == false) {correct();}
  else {incorrect();}break;
  case 7: //CO
  if (LNPeven == false && (LNPvowel == false || LNPy == true) && LNPe1 == false) {correct();}
  else {incorrect();}break;
  }
}

//spritesheet
const unsigned char PROGMEM eng_plus_mask[] =
{
// width, height,
25, 24,
// FRAME 00
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf9, 0xff, 0x29, 0xff, 0x29, 0xff, 0x09, 0xff, 0x01, 0xff, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x29, 0xff, 0x29, 0xff, 0x09, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x00, 0xff, 0xf0, 0xff, 0x08, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x81, 0xff, 0x81, 0xff, 0xf9, 0xff, 0x00, 0xff, 0xf9, 0xff, 0x00, 0xff, 0x00, 0xff, 0x01, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 01
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf9, 0xff, 0x29, 0xff, 0x29, 0xff, 0x09, 0xff, 0x01, 0xff, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x29, 0xff, 0x29, 0xff, 0x09, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0x91, 0xff, 0x00, 0xff, 0x10, 0xff, 0x28, 0xff, 0x29, 0xff, 0xc8, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x00, 0xff, 0x09, 0xff, 0xf8, 0xff, 0x08, 0xff, 0x09, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 02
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0x78, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf8, 0xff, 0x00, 0xff, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x81, 0xff, 0x81, 0xff, 0xf8, 0xff, 0x00, 0xff, 0xf8, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 03
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf0, 0xff, 0x09, 0xff, 0x09, 0xff, 0x90, 0xff, 0x00, 0xff, 0x11, 0xff, 0x29, 0xff, 0x29, 0xff, 0xc8, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf8, 0xff, 0x00, 0xff, 0x08, 0xff, 0xf8, 0xff, 0x08, 0xff, 0x08, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 04
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 0xff, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x81, 0xff, 0x81, 0xff, 0xf9, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf8, 0xff, 0x28, 0xff, 0x29, 0xff, 0x08, 0xff, 0x00, 0xff, 0x78, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf8, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x28, 0xff, 0x28, 0xff, 0x09, 0xff, 0x00, 0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 05
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 0xff, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf1, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x81, 0xff, 0x81, 0xff, 0xf9, 0xff, 0x01, 0xff, 0xf9, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf0, 0xff, 0x08, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 0xff, 0xf8, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x08, 0xff, 0x08, 0xff, 0xf1, 0xff, 0x00, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 06
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0x91, 0xff, 0x01, 0xff, 0x11, 0xff, 0x29, 0xff, 0x29, 0xff, 0xc9, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x01, 0xff, 0x09, 0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf8, 0xff, 0x29, 0xff, 0x29, 0xff, 0x08, 0xff, 0x00, 0xff, 0x79, 0xff, 0x81, 0xff, 0x01, 0xff, 0xf8, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x28, 0xff, 0x28, 0xff, 0x09, 0xff, 0x00, 0xff, 0xf8, 0xff, 0x11, 0xff, 0x20, 0xff, 0xf8, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 07
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0xf1, 0xff, 0x09, 0xff, 0x09, 0xff, 0x91, 0xff, 0x01, 0xff, 0x11, 0xff, 0x29, 0xff, 0x29, 0xff, 0xc9, 0xff, 0x01, 
0xff, 0xf9, 0xff, 0x11, 0xff, 0x21, 0xff, 0xf9, 0xff, 0x01, 0xff, 0x09, 0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0xf0, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 0xff, 0xf9, 0xff, 0x09, 0xff, 0x09, 0xff, 0xf0, 0xff, 0x00, 
0xff, 0xf9, 0xff, 0x08, 0xff, 0x08, 0xff, 0xf1, 0xff, 0x00, 0xff, 0x00, 0xff, 0x01, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01, 

// FRAME 08
0xf8, 0xf8, 0x04, 0xfc, 0x02, 0xfe, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 
0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x02, 0xfe, 0x04, 0xfc, 0xf8, 0xf8, 
0xff, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 
0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 
0x01, 0x01, 0x02, 0x03, 0x04, 0x07, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 
0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x08, 0x0f, 0x04, 0x07, 0x02, 0x03, 0x01, 0x01,
};

int ha = 24;

void akmd(int& d) {
  switch(d){
  case 0: //ev
    Sprites::drawPlusMask(27+ha, 0, eng_plus_mask, 0);
    break;
  case 1: //ec
    Sprites::drawPlusMask(0+ha, 22, eng_plus_mask, 1);
    break;
  case 2: //ov
    Sprites::drawPlusMask(54+ha, 22, eng_plus_mask, 2);
    break;
  case 3: //oc
    Sprites::drawPlusMask(27+ha, 44, eng_plus_mask, 3);
    break;
  case 4: //ve
    Sprites::drawPlusMask(3+ha, 3, eng_plus_mask, 4);
    break;
  case 5: //ce
    Sprites::drawPlusMask(51+ha, 3, eng_plus_mask, 5);
    break;
  case 6: //vo
    Sprites::drawPlusMask(3+ha, 41, eng_plus_mask, 6);
    break;
  case 7: //co
    Sprites::drawPlusMask(51+ha, 41, eng_plus_mask, 7);
    break;
  }
}

// Main Menu
const char option0[] PROGMEM = "Start/Continue";
const char option1[] PROGMEM = "Setup";
const char option2[] PROGMEM = "Reset";
const char * const options[] PROGMEM =
{
  option0,
  option1,
  option2,
};
const uint8_t optionCount = sizeof(options) / sizeof(options[0]);
const uint8_t maxIndex = optionCount - 1;
const __FlashStringHelper * FlashString(const char * string)
{
  return reinterpret_cast<const __FlashStringHelper *>(string);
}
uint8_t selectedIndex;

unsigned long test, test2;

void setup() {arduboy.boot();arduboy.flashlight();arduboy.setFrameRate(15);arduboy.display();}

//#########################################################################################################################
//############# MAIN LOOP #################################################################################################
//#########################################################################################################################
void loop() {
autoPowerDown(6);
  //Prevent the Arduboy from running too fast
  if(!arduboy.nextFrame()) return;
  switch(state) {
    case 0: //main menu
      arduboy.clear();
      arduboy.setCursor(52,0);arduboy.print("CEOV");
      
  arduboy.pollButtons();
  if(arduboy.justPressed(UP_BUTTON)){if(selectedIndex > 0) --selectedIndex;}
  if(arduboy.justPressed(DOWN_BUTTON)){if(selectedIndex < maxIndex) ++selectedIndex;}

arduboy.setCursor(0, 32);
  
  // Loop through all options
  for(uint8_t i = 0; i < optionCount; ++i)
  {
    // If the current option is the selected option;
    if(i == selectedIndex)
    {
      // Draw an arrow
      arduboy.print(F("> "));
    }
    else
    {
      // Otherwise just indent
      arduboy.print(F("  "));
    }
  
    // Print the string in the array, This is quite complicated if you don't know about, pointers and casting.
    arduboy.println(FlashString(pgm_read_word(&options[i])));
  }
      if(arduboy.pressed(UP_BUTTON) || arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON) && firstpress == false) {
        arduboy.initRandomSeed();
        firstpress = true;
      }
      if(arduboy.pressed(A_BUTTON) && buttonfree) {
        buttonfree = false;
        if(selectedIndex == 0){
          if(gameinprogress==false){computechallenge(); randomizearr (Darr, Dsize);
          gameinprogress=true;}
          state = 1;
        }
        if(selectedIndex == 1){
          state = 2;
        }
        if(selectedIndex == 2){
          Qright = Qwrong = Tanswered = Qlooked = 0; gameinprogress=false;
        }
      }
      break;
    case 1: //game
      //if (greenlight >0 || redlight >0){greenlight -=4; redlight -=4;}
      //arduboy.setRGBled(redlight,greenlight,0);
      arduboy.clear();
    Sprites::drawPlusMask(27+ha, 0, eng_plus_mask, Darr[0]);
    Sprites::drawPlusMask(0+ha, 22, eng_plus_mask, Darr[1]);
    Sprites::drawPlusMask(54+ha, 22, eng_plus_mask, Darr[2]);
    Sprites::drawPlusMask(27+ha, 44, eng_plus_mask, Darr[3]);
    if (gametype == 1){
    Sprites::drawPlusMask(3+ha, 3, eng_plus_mask, Darr[4]);
    Sprites::drawPlusMask(51+ha, 3, eng_plus_mask, Darr[5]);
    Sprites::drawPlusMask(3+ha, 41, eng_plus_mask, Darr[6]);
    Sprites::drawPlusMask(51+ha, 41, eng_plus_mask, Darr[7]);
    }
    //arduboy.drawRoundRect(sx+ha, sy, sw, sh, sc);
      arduboy.setCursor(58,28);arduboy.print(challenge);
      if (showbutton == true){;
      arduboy.setCursor(0,0);arduboy.print(Qright);
      arduboy.setCursor(0,28);arduboy.print(Qright/Qwrong);
      arduboy.setCursor(0,57);arduboy.print(Qwrong);
      //arduboy.setCursor(100,57);arduboy.print(Tanswered);
      }
//debug keycounters
//arduboy.setCursor(72,0);arduboy.print(upcounter);
//arduboy.setCursor(20,35);arduboy.print(leftcounter);
//arduboy.setCursor(99,26);arduboy.print(rightcounter);
//arduboy.setCursor(70,46);arduboy.print(downcounter);

 arduboy.pollButtons();
  if(arduboy.pressed(UP_BUTTON)) {upcounter = keylinger; uppress = true; consent = true;}
  if(arduboy.pressed(LEFT_BUTTON)) {leftcounter = -keylinger; leftpress = true; consent = true;} //
  if(arduboy.pressed(RIGHT_BUTTON)) {rightcounter = keylinger; rightpress = true; consent = true;} //
  if(arduboy.pressed(DOWN_BUTTON)) {downcounter = -keylinger; downpress = true; consent = true;}
  //generate visual
  if((upcounter != 0 || leftcounter != 0 || rightcounter != 0 || downcounter != 0) && consent == true){
    int horizontalcounter=leftcounter+rightcounter; int verticalcounter=upcounter+downcounter;
      switch (gametype){
        case 0:{
          if (abs(horizontalcounter) > abs(verticalcounter)){ //axis choose
            if (horizontalcounter > 0){arduboy.drawRoundRect(-1+ha,21,sw,sh,sc);}
            else {arduboy.drawRoundRect(53+ha,21,sw,sh,sc);}
          }
          else {
            if (verticalcounter > 0){arduboy.drawRoundRect(26+ha,-1,sw,sh,sc);}
            else {arduboy.drawRoundRect(26+ha,-1,sw,sh,sc);}
          }
        }break;
        case 1:{
          if (horizontalcounter == 0 && verticalcounter > 0){arduboy.drawRoundRect(26+ha,-1,sw,sh,sc);}//up
          if (horizontalcounter < 0 && verticalcounter == 0){arduboy.drawRoundRect(-1+ha,21,sw,sh,sc);}//left
          if (horizontalcounter > 0 && verticalcounter == 0){arduboy.drawRoundRect(53+ha,21,sw,sh,sc);}//right
          if (horizontalcounter == 0 && verticalcounter < 0){arduboy.drawRoundRect(26+ha,43,sw,sh,sc);}//down
          if (horizontalcounter < 0 && verticalcounter > 0){arduboy.drawRoundRect(2+ha,2,sw,sh,sc);}//northwest
          if (horizontalcounter > 0 && verticalcounter > 0){arduboy.drawRoundRect(50+ha,2,sw,sh,sc);}//northeast
          if (horizontalcounter < 0 && verticalcounter < 0){arduboy.drawRoundRect(2+ha,40,sw,sh,sc);}//southwest
          if (horizontalcounter > 0 && verticalcounter < 0){arduboy.drawRoundRect(50+ha,40,sw,sh,sc);}//southeast
        }break;
      }
  }
  if(arduboy.justReleased(UP_BUTTON)) {uppress = false;}
  if(arduboy.justReleased(LEFT_BUTTON)) {leftpress = false;} //
  if(arduboy.justReleased(RIGHT_BUTTON)) {rightpress = false;} //
  if(arduboy.justReleased(DOWN_BUTTON)) {downpress = false;}
  if (upcounter != 0 && uppress == false){upcounter -=1;}
  if (leftcounter != 0 && leftpress == false){leftcounter +=1;} //
  if (rightcounter != 0 && rightpress == false){rightcounter -=1;} //
  if (downcounter != 0 && downpress == false){downcounter +=1;}
  if (uppress == false && leftpress == false && rightpress == false && downpress == false && consent == true){
    int horizontalcounter=leftcounter+rightcounter; int verticalcounter=upcounter+downcounter;
    upcounter = leftcounter = rightcounter = downcounter = 0; consent = false;
//void generatekeypress()
      switch (gametype){
        case 0:{
          if (abs(horizontalcounter) > abs(verticalcounter)){ //axis choose
            if (horizontalcounter > 0){akm(Darr[2]);}
            else {akm(Darr[1]);}
          }
          else {
            if (verticalcounter > 0){akm(Darr[0]);}
            else {akm(Darr[3]);}
          }
        }break;
        case 1:{
          if (horizontalcounter == 0 && verticalcounter > 0){akm(Darr[0]);}//up
          if (horizontalcounter < 0 && verticalcounter == 0){akm(Darr[1]);}//left
          if (horizontalcounter > 0 && verticalcounter == 0){akm(Darr[2]);}//right
          if (horizontalcounter == 0 && verticalcounter < 0){akm(Darr[3]);}//down
          if (horizontalcounter < 0 && verticalcounter > 0){akm(Darr[4]);}//northwest
          if (horizontalcounter > 0 && verticalcounter > 0){akm(Darr[5]);}//northeast
          if (horizontalcounter < 0 && verticalcounter < 0){akm(Darr[6]);}//southwest
          if (horizontalcounter > 0 && verticalcounter < 0){akm(Darr[7]);}//southeast
        }break;
      }
  }
      if(arduboy.pressed(A_BUTTON)) {
        showbutton = true;
        if(arduboy.pressed(B_BUTTON) && buttonfree) {
          buttonfree = false;
          state = 0;
        }
      }
      if(arduboy.notPressed(A_BUTTON)){
        showbutton = false;
      }
      break;
    case 2:{ //2 choice mode UNFINISHED
      arduboy.clear();
      maxchoices = 0;
    arduboy.setCursor(0,0);test=micros()/(10*exp(6));arduboy.print(test);
      if(arduboy.pressed(A_BUTTON)) {
        showbutton = true;
        if(arduboy.pressed(B_BUTTON) && buttonfree) {
          buttonfree = false;
          state = 0;
        }
      }
      if(arduboy.notPressed(A_BUTTON)){
        showbutton = false;
      }
      break;
    case 3: //setup UNFINISHED
       if(arduboy.pressed(A_BUTTON) && buttonfree) {
          buttonfree = false;
          state = 0;
       }
      break;
  }

  if (buttonfree == false) {
    if (!arduboy.pressed(UP_BUTTON) && !arduboy.pressed(DOWN_BUTTON) && !arduboy.pressed(LEFT_BUTTON) && !arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(A_BUTTON) && !arduboy.pressed(B_BUTTON)) {
      buttonfree = true;
    }
  }
  arduboy.display();
}
