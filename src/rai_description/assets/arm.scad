% scale(1000) import("arm.stl");

// Append pure shapes (cube, cylinder and sphere), e.g:
// cube([10, 10, 10], center=true);
// cylinder(r=10, h=10, center=true);
// sphere(10);
translate([50, 440, 560]){
    rotate([0, 0, 137]){
        cube([510, 30, 30], center=true);
    }
};

translate([266, 239, 550]){
    cylinder(60, r = 25, true);
}