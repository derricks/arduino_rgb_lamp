
#include "rgb_led_lamp.h"

#define RED_OUT 3
#define BLUE_OUT 6
#define GREEN_OUT 9

#define PROG_RGB_CYCLE 1

#define PROGRAM_FPS 15
#define MAX_PROGRAM_SIZE 190

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
  }
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
    addTweeningSteps(red, orange, PROGRAM_FPS * 2);

    // orange
    pushColorOnQueue(orange);
    addTweeningSteps(orange, yellow, PROGRAM_FPS * 2);

    // yellow
    pushColorOnQueue(yellow);
    addTweeningSteps(yellow, green, PROGRAM_FPS * 2);

    // green
    pushColorOnQueue(green);
    addTweeningSteps(green, blue, PROGRAM_FPS* 2);

    // blue
    pushColorOnQueue(blue);
    addTweeningSteps(blue, purple, PROGRAM_FPS * 2);

    // purple
    pushColorOnQueue(purple);
    // don't use red because we need a farther along fire_time
    addTweeningSteps(purple, (color_step) {
      purple.fire_time + 1000, red.red, red.green, red.blue
    }, PROGRAM_FPS * 2);
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

// figures out weening steps from from_step to to_step and adds them to the queue
// though it requires to_step, it is up to the caller to explicitly add to_step (and from_step)
// to the program.
// The reason for this is twofold:
// 1) We don't want to solely rely on the math to work out for getting to to_step
//    if you're going from green = 3 to green = 4 over 40 steps, each individual step
//    rounds down to 0. But you still want to ensure that you get to the final state
// 2) If this method put to_step on the queue, the caller would need to keep track of which from_step and to_steps
//    had been put on lest it duplicate. i.e., going from red -> orange poses no problems, but going from red -> orange -> yellow
//    requires the caller to know that orange got plopped onto the stack and so shouldn't be re-added for the orange -> yellow bit
void addTweeningSteps(color_step from_step, color_step to_step, int num_steps) {
  // figure out the step differential for each step between from_step and to_step
  int time_step_amount = int((to_step.fire_time - from_step.fire_time) / num_steps);
  byte red_step_amount = byte((to_step.red - from_step.red) / num_steps);
  byte green_step_amount = byte((to_step.green - from_step.green) / num_steps);
  byte blue_step_amount = byte((to_step.blue - from_step.blue) / num_steps);

  for (int step_counter = 0; step_counter < num_steps; step_counter++) {
    // always starting with from as a base, add scaled versions of the steps
    // on the first pass, we want the tween step to be original_fire_time + 1 fire_time_interval
    // but on the second, we want the tween step to be original_fire_time + 2 fire_time_interval
    int multiplier = step_counter + 1;
    pushColorOnQueue((color_step) {
      from_step.fire_time + (time_step_amount * multiplier),
                          from_step.red + (red_step_amount * multiplier),
                          from_step.green + (green_step_amount * multiplier),
                          from_step.blue + (blue_step_amount * multiplier)
    });
  }
}

