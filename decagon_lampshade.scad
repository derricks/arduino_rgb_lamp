// OpenSCAD file for generating a decagon lampshade for the RGB lamp

wall_thickness = 3.0;
led_square_length = 36;
wall_height = 39;
interior_channel_width = 13;

module decagon(internal_radius) {
  circle(r = internal_radius, $fn = 10);
}


bisected_internal_angle = 18;

// decagon has 10 sides, which means the circle that would circumscribe it
// is divided into angles of 36 degrees. But the radii from those points would hit
// the _corners_ of the LED square, so we want to draw a line up through that sqaure
// to create a right triangle whose right angle sits at the intersection with the 
// middle of the square. From that we can derive the hypotenuse of that triangle, which is the radius
function radius_interior(length) = ((length/2)/sin(bisected_internal_angle));

linear_extrude(height = wall_height, center = true, twist = 0) {

  // outer wall
  difference() {
    decagon(radius_interior(led_square_length) + interior_channel_width + wall_thickness);
    decagon(radius_interior(led_square_length) + interior_channel_width);
  }

  // inner wall
  difference() {
    decagon(radius_interior(led_square_length));

    decagon(radius_interior(led_square_length) - wall_thickness);
   }
}

// roof
translate([0,0, wall_height/2]) {
  linear_extrude(height= wall_thickness, center = true, twist = 0) {
    decagon(radius_interior(led_square_length) + interior_channel_width + wall_thickness);
  }
}