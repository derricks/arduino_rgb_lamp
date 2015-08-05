
#include "rgb_led_lamp.h"

#define RED_OUT 3
#define BLUE_OUT 6
#define GREEN_OUT 9

#define PROG_RGB_CYCLE 1

#define PROGRAM_FPS 15
#define MAX_PROGRAM_SIZE 50

color_step program[MAX_PROGRAM_SIZE];
int programCounter = 0;

void setup() {
  startProgram(PROG_RGB_CYCLE);
  Serial.begin(9600);
}

void loop() {
  
  if (programAtEnd()) {
    startProgram(PROG_RGB_CYCLE);
  }

  if (millis() > program[programCounter].fire_time) {
    setColor(program[programCounter].red, program[programCounter].green, program[programCounter].blue);
    programCounter++;
  } else {
    setTweenValues();
  }
}

void setTweenValues() {
  if (programCounter == 0) {
    // program hasn't started yet
    return;
  }

  // start point is the previous item!
  color_step start_point = program[programCounter - 1];

  // end point is either the next item in the program
  // or the beginning of the program if this is the last step in the program 
  //   (at which point programCounter will have been pushed past the last step)
  color_step end_point = program[programCounter].fire_time == 0 ? program[0] : program[programCounter];

  unsigned long current_millis = millis();
  
  // what percentage of the way through the cycle are we?
  float percentage = (float)((current_millis - start_point.fire_time))/(float)((end_point.fire_time - start_point.fire_time));
  
  // use that percentage against the difference for the different colors
  setColor(color_offset(start_point.red, end_point.red, percentage),
           color_offset(start_point.green, end_point.green, percentage),
           color_offset(start_point.blue, end_point.blue, percentage));
}

// given a starting color, an ending color, and a "how far" percentage,
// return what the color should be for tweening
byte color_offset(byte start_color, byte end_color, float percentage) {
  return byte(start_color + ((end_color - start_color) * percentage));
}

boolean programAtEnd() {
  return programCounter >= MAX_PROGRAM_SIZE || program[programCounter].fire_time == 0;
}

void clearProgram() {
  for (int programStep = 0; programStep < MAX_PROGRAM_SIZE; programStep++) {
    program[programStep].fire_time = 0;
  }
  programCounter = 0;
}

// todo: enums
void startProgram(int programCode) {
  clearProgram();
  unsigned long curMillis = millis();

  if (programCode == PROG_RGB_CYCLE) {
    // red
    color_step red = {curMillis + 1000, 255, 0, 0};
    color_step orange = {curMillis + 3000, 255, 128, 0};
    color_step yellow = {curMillis + 5000, 255, 255, 0};
    color_step green = {curMillis + 7000, 0, 255, 0};
    color_step blue = {curMillis + 9000, 0, 0, 255};
    color_step purple = {curMillis + 11000, 128, 0, 255};

    //red
    pushColorOnQueue(red);

    // orange
    pushColorOnQueue(orange);

    // yellow
    pushColorOnQueue(yellow);

    // green
    pushColorOnQueue(green);

    // blue
    pushColorOnQueue(blue);

    // purple
    pushColorOnQueue(purple);

    // red
    pushColorOnQueue((color_step){curMillis + 13000, 255, 0, 0});
  }

  // reset counter after pushing items onto it, to make sure that loop starts at the beginning
  programCounter = 0;
}

// todo: allow for brightness control
void setColor(byte red, byte green, byte blue) {
  analogWrite(RED_OUT, normalizeColor(red));
  analogWrite(GREEN_OUT, normalizeColor(green));
  analogWrite(BLUE_OUT, normalizeColor(blue));
}

// ensures that a color value is between 0 and 255, inclusively.
int normalizeColor(byte in_color) {
  if (in_color < 0) {
    return 0;
  }

  if (in_color > 255) {
    return 255;
  }

  return in_color;
}

void pushColorOnQueue(color_step new_step) {
  // don't use programAtEnd, because the step in the program when this is called is fire_time == 0
  // but we do want to make sure we don't write into areas we shouldn't
  if (programCounter >= MAX_PROGRAM_SIZE) {
    return;
  }

  program[programCounter] = new_step;
  programCounter++;
}
