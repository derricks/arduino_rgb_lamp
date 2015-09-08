// forms the top of the controller case, which simply slides over the bottom part and has ridges into which
// one can slide the transistor panel
// note that the model is made as if it's lying down, with the assumption that it will be rotated in one's slicer program

include <controller_case_common.scad>;

perfboard_thickness = 5.0; // provide room for solder
height = 33.0;
groove_depth = 3.0;
perfboard_width = 43.0;
interior_width = arduino_width + (wall_thickness * 2);
groove_shelf_width = ((interior_width - perfboard_width)/2) + groove_depth;

module groove_base() {
  cube([groove_shelf_width, arduino_length + wall_thickness, floor_thickness]);
}


// the roof of the box
translate([(-wall_thickness), 0, 0])
  cube([arduino_width + (wall_thickness * 4), arduino_length + wall_thickness, floor_thickness]);

// the walls
translate([(-wall_thickness * 2), 0, 0,]) 
  cube([wall_thickness, arduino_length + wall_thickness, height]);

translate([arduino_width + (wall_thickness * 2), 0, 0])
  cube([wall_thickness, arduino_length + wall_thickness, height]);


// the back
translate([-wall_thickness * 2, arduino_length + wall_thickness, 0])
  cube([arduino_width + (wall_thickness * 5), wall_thickness, height]);

// grooves for perfboard
translate([-wall_thickness, 0, groove_depth]) groove_base();

translate([interior_width - groove_shelf_width, 0, groove_depth]) groove_base();

