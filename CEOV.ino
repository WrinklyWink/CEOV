//A brain enhancer to conquer random
//special thanks to 

#include <Arduboy2.h>
//#include "autopowerdown.h"
Arduboy2 arduboy;

#define OFF HIGH

//computechallenge variables
int LNpair;
int LNpairmin = 0;
int LNpairmax = 8;
int Vnu, Cnu;
bool v, e, y;
const char    vowel[]      = "AEIOUAEIOUAEIOUAEIOUYaeiouaeiouaeiouaeiouy";
const char    consonant[]  = "BCDFGHJKLMNPQRSTVWXZYbCdfghjklmnpqrstvwxzy";
      int     VCL          = 42;
const char    even[]       = "02468";
const char    odd[]        = "13579";
const int     EOL          = 5;

//computekm
//int computeanswerkm;
const int Sarr[] = {0, 1, 2, 3};
const int Ssize = sizeof(Sarr)/ sizeof(Sarr[0]);
int Darr[] = {0, 1, 2, 3};
int Dsize = sizeof(Darr)/ sizeof(Darr[0]);

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

void computechallenge() {
  LNpair = random(LNpairmin, LNpairmax);
  switch(LNpair) {
    case 0:
      v = true; e = true;
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){y = true;}
      else {y = false;}
      challenge[0] = vowel[Vnu];
      challenge[1] = even[random(EOL)];break;
    case 1:
      Vnu = random(VCL);
      v = true; e = false;
      if(Vnu == 20 || Vnu == 41){y = true;}
      else {y = false;}
      challenge[0] = vowel[Vnu];
      challenge[1] = odd[random(EOL)];break;
    case 2:
      v = false; e = true;
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){y = true;}
      else {y = false;}
      challenge[0] = consonant[Cnu];
      challenge[1] = even[random(EOL)];break;
    case 3:
      v = false; e = false;
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){y = true;}
      else {y = false;}
      challenge[0] = consonant[Cnu];
      challenge[1] = odd[random(EOL)];break;
    case 4:
      e = true; v = true;
      challenge[0] = even[random(EOL)];
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){y = true;}
      else {y = false;}
      challenge[1] = vowel[Vnu];break;
    case 5:
      e = true; v = false;
      challenge[0] = even[random(EOL)];
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){y = true;}
      else {y = false;}
      challenge[1] = consonant[Cnu];break;
    case 6:
      e = false; v = true;
      challenge[0] = odd[random(EOL)];
      Vnu = random(VCL);
      if(Vnu == 20 || Vnu == 41){y = true;}
      else {y = false;}
      challenge[1] = vowel[Vnu];break;
    case 7:
      e = false; v = false;
      challenge[0] = odd[random(EOL)];
      Cnu = random(VCL);
      if(Cnu == 20 || Cnu == 41){y = true;}
      else {y = false;}
      challenge[1] = consonant[Cnu];break;
  }
}

void correct(){
  digitalWrite(GREEN_LED, led[1]);
  Qright++;
  Tanswered++;
  computechallenge();
  randomizearr (Darr, Dsize);
}

void incorrect(){
  digitalWrite(RED_LED, led[0]);
  Qwrong++;
  Tanswered++;
  computechallenge();
  randomizearr (Darr, Dsize);
}

void akm(int c) {
  switch(c){
  case 0: //ev
  if (e == true && (v == true || y == true)) {correct();}
  else {incorrect();}break;
  case 1: //ec
  if (e == true && (v == false || y == true)) {correct();}
  else {incorrect();}break;
  case 2: //ov
  if (e == false && (v == true || y == true)) {correct();}
  else {incorrect();}break;
  case 3: //oc
  if (e == false && (v == false || y == true)) {correct();}
  else {incorrect();}break;
  }
}
/*
void akmd(int d) {
  switch(d){
  case 0: //ev
    arduboy.setCursor(56,6);arduboy.print("EV");
    break;
  case 1: //ec
    arduboy.setCursor(34,26);arduboy.print("EC");
    break;
  case 2: //ov
    arduboy.setCursor(78,26);arduboy.print("OV");
    break;
  case 3: //oc
    arduboy.setCursor(56,46);arduboy.print("OC");
    break;
  }
}
*/

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

void setup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(20);
  arduboy.display();
}

void loop() {
  //autoPowerDown(6);
  //Prevent the Arduboy from running too fast
  if(!arduboy.nextFrame()) return;
  switch(state) {
    case 0: //main menu
      arduboy.clear();
      arduboy.setCursor(52,0);arduboy.print("CEOV");
      
  arduboy.pollButtons();
  if(arduboy.justPressed(UP_BUTTON))
  {
    if(selectedIndex > 0)
      --selectedIndex;
  }
  if(arduboy.justPressed(DOWN_BUTTON))
  {
    if(selectedIndex < maxIndex)
      ++selectedIndex;
  }

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
  
    // Print the string in the array
    // This is quite complicated if you don't know about
    // pointers and casting.
    arduboy.println(FlashString(pgm_read_word(&options[i])));
  }
      if(arduboy.pressed(UP_BUTTON) || arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON) && firstpress == false) {
        arduboy.initRandomSeed();
        firstpress = true;
      }
      if(arduboy.pressed(A_BUTTON) && buttonfree) {
        computechallenge();
        randomizearr (Darr, Dsize);
        buttonfree = false;
        if(selectedIndex == 0){
          state = 1;
        }
        if(selectedIndex == 1){
          state = 2;
        }
        if(selectedIndex == 2){
          Qright = 0; Qwrong = 0; Tanswered = 0; Qlooked = 0;
        }
      }
      break;
    case 1: //game
      arduboy.clear();
      arduboy.drawRoundRect(53, 3, 21, 18, 1); //up
    arduboy.setCursor(56,6);//arduboy.print(Darr[0]);
    switch (Darr[0]){
      case 0:arduboy.print("eV");break;
      case 1:arduboy.print("eC");break;
      case 2:arduboy.print("oV");break;
      case 3:arduboy.print("oC");break;
    }
      arduboy.drawRoundRect(31, 23, 21, 18, 1); //left
    arduboy.setCursor(34,26);//arduboy.print(Darr[1]);
    switch (Darr[1]){
      case 0:arduboy.print("eV");break;
      case 1:arduboy.print("eC");break;
      case 2:arduboy.print("oV");break;
      case 3:arduboy.print("oC");break;
    }
      arduboy.drawRoundRect(75, 23, 21, 18, 1); //right
    arduboy.setCursor(78,26);//arduboy.print(Darr[2]);
    switch (Darr[2]){
      case 0:arduboy.print("eV");break;
      case 1:arduboy.print("eC");break;
      case 2:arduboy.print("oV");break;
      case 3:arduboy.print("oC");break;
    }
      arduboy.drawRoundRect(53, 43, 21, 18, 1); //down
    arduboy.setCursor(56,46);//arduboy.print(Darr[3]);
    switch (Darr[3]){
      case 0:arduboy.print("eV");break;
      case 1:arduboy.print("eC");break;
      case 2:arduboy.print("oV");break;
      case 3:arduboy.print("oC");break;
    }
      arduboy.setCursor(58,28);arduboy.print(challenge);
      if (showbutton == true){;
      arduboy.setCursor(0,0);arduboy.print(Qright);
      arduboy.setCursor(0,28);arduboy.print(Qright/Qwrong);
      arduboy.setCursor(0,57);arduboy.print(Qwrong);
      //arduboy.setCursor(100,57);arduboy.print(Tanswered);
      }
  if(arduboy.pressed(UP_BUTTON) && buttonfree) {
    buttonfree = false;
    akm (Darr[0]);
  }
  if(arduboy.pressed(LEFT_BUTTON) && buttonfree) {
    buttonfree = false;
    akm (Darr[1]);
  }
  if(arduboy.pressed(RIGHT_BUTTON) && buttonfree) {
    buttonfree = false;
    akm (Darr[2]);
  }
  if(arduboy.pressed(DOWN_BUTTON) && buttonfree) {
    buttonfree = false;
    akm (Darr[3]);
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
    case 2: //2 choice mode UNFINISHED
      arduboy.clear();
      maxchoices = 0;
      arduboy.drawRoundRect(43, 3, 21, 18, 1); //left box
      arduboy.drawRoundRect(65, 3, 21, 18, 1); //right box, i need more boxes..
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
