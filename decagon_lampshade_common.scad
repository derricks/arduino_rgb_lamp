// common code for the lampshade

wall_thickness = 3.0;
bisected_internal_angle = 18;
led_square_length = 36;
interior_channel_width = 13;


module decagon(internal_radius) {
  circle(r = internal_radius, $fn = 10);
}

// decagon has 10 sides, which means the circle that would circumscribe it
// is divided into angles of 36 degrees. But the radii from those points would hit
// the _corners_ of the LED square, so we want to draw a line up through that sqaure
// to create a right triangle whose right angle sits at the intersection with the 
// middle of the square. From that we can derive the hypotenuse of that triangle, which is the radius
function radius_interior(length) = ((length/2)/sin(bisected_internal_angle));

// the standard case of returning the radius interior based on the led_square_length
function standard_radius_interior() = radius_interior(led_square_length);


