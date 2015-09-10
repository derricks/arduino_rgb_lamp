// code to construct the decagon's base

include <decagon_lampshade_common.scad>;

floor_thickness = 2.0;
wall_edge = standard_radius_interior() + interior_channel_width + wall_thickness;
foot_dimension = 12;
foot_fudge_factor = 7;

// create a decagon that is the full base
linear_extrude(height = floor_thickness, center = true, twist = 0) {
  difference() {
    decagon(wall_edge);
    decagon(standard_radius_interior() - (standard_radius_interior()/2));
  }
}

module foot(x,y) {
  translate([x,y,-foot_dimension]) {
     cube([foot_dimension, foot_dimension, foot_dimension]);
  }
}

// feet
foot(wall_edge - foot_dimension -foot_fudge_factor,-foot_dimension/2);
foot(-wall_edge + foot_fudge_factor,-foot_dimension/2);
foot(-foot_dimension/2, wall_edge - foot_dimension - foot_fudge_factor);
foot(-foot_dimension/2, -wall_edge + foot_fudge_factor);