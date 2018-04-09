# Examples

**00-orientation.cpp** tests the orientation of three consecutive clicks;

**01-reflex.cpp** tests whether the vertices are reflex;

**02-intersect.cpp** tests whether two segments intersect;

**03-ear.cpp** tests whether vertices are an ear;

**04-classify.cpp** classifies vertices in regular, start, end, split or merge;

**05-mouth.cpp** tests whether vertices are a mouth;

**06-triangle-spinning.cpp** draws a spinning triangle;

**07-point-in-polygon.cpp** tests whether the point is in the polygon;

**08-dcel.cpp** constructs a dcel from a polygon:

	* start drawing a polygon;

	* press enter to build a dcel from the polygon;

	* press m to alternate between add edges and vertices.

**09-visibility.cpp** draws the visibility polygon of a point in the n polygon in the screen:

	* start drawing a polygon;

	* press n to draw the next polygon;

	* press enter to see the visibility polygon.

**10-image.cpp** draws every vertices of an image and the mouse position repels the vertices;

	* open passing a image by command line argument, if the image is large, the program may freeze;

	* press enter to spread the points across the visible area;

	* click to make the points go to the mouse cursor position.

**11-convex-hull.cpp** draws a convex hull of a set of points.

	* add points by clicking;

	* press space to add a random point in the visible area.

**12-point-in-polygon-with-holes.cpp** tests whether the point is in the polygon with holes.
