CatmullRom
==========

Created by: Griffin Ledingham
Student #: V00712188
E-Mail: gcledingham@gmail.com

If an issue occurs running the program, please email me arrange for me to bring in my laptop to demo.

CatmullRom is a math based curve drawing program, which interpolates a curve based on user defined points. This spline may then be used to trace an object along, or to have camera follow a specified path.

There are 5 different camera modes, which may be used to view, and modify the drawn spline. ArcBall view gives the user full control over the camera, to rotate around and view all aspects of the spline. Playback view snaps a camera to the spline's patch, and follow the track, looking at the centre object the entire time. Front, Side and Top views are used to modify the spine by placing, deleting, and moving points present on the screen.

When a point is placed, deleted or moved, the program automatically interpolates a segment of the curve using the 4 nearest points in the spline.

The arc length of the curve is estimated using the distance between points, and a segmented arc length calculation is also present.