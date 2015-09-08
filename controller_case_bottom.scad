// Creates the bottom part of the case for the lamp controller unit, which is where the Arduino Uno lives
// Note that this assumes you have downloaded the Arduino Uno dxf file at http://arduino.cc/documents/Arduino%20Uno.dxf
// and have it in the local directory.

include <controller_case_common.scad>;
guard_rail_height = floor_thickness * 8.0;

cube([arduino_width, arduino_length, floor_thickness]);

translate([-wall_thickness,0,0])
  cube([wall_thickness, arduino_length, guard_rail_height]);

translate([arduino_width,0,0])
  cube([wall_thickness, arduino_length, guard_rail_height]);

translate([-wall_thickness, arduino_length, 0])
  cube([arduino_width + (wall_thickness * 2), wall_thickness, guard_rail_height]);