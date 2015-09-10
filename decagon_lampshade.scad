// OpenSCAD file for generating a decagon lampshade for the RGB lamp

include <decagon_lampshade_common.scad>;

wall_height = 39;


linear_extrude(height = wall_height, center = true, twist = 0) {

  // outer wall
  difference() {
    decagon(standard_radius_interior() + interior_channel_width + wall_thickness);
    decagon(standard_radius_interior() + interior_channel_width);
  }

  // inner wall
  difference() {
    decagon(standard_radius_interior());

    decagon(standard_radius_interior() - wall_thickness);
   }
}

// roof
translate([0,0, wall_height/2]) {
  linear_extrude(height= wall_thickness, center = true, twist = 0) {
    decagon(standard_radius_interior() + interior_channel_width + wall_thickness);
  }
}