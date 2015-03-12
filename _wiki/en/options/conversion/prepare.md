---
wid: "4_1_1"
layout: wikipage
title: Prepare
lang: en
---
This settings specifies the transformation prior to converting images to text.

By default, each image is treated as a color, and is stored as a color. This conversion allows you to change the type of image before processing:

  *  Monochrome;
  *  Edge;
  *  Edge value;
  *  Diffuse Dither;
  *  Ordered Dither;
  *  Threshold Dither;
  *  Grayscale (function [qGray](http://qt-project.org/doc/qt-4.8/qcolor.html#qGray-2));
  *  Color (no changes);

You can also invert the colors.

The order of scanning points of the image is controlled by script. There are 16 predefined scripts. Also, you can use your own script, based on the most suitable of the predefined.

In the right pane displays a simple animation showing the scanning order of the points in the specified script. The parameters of time are the total period of the animation and the update interval.

![Prepare dialog](prepare-1.png "Prepare dialog")

Additionally, possible to set scan bands, which can be convenient for some LCD with a specific memory allocation.

E.g. this sequence:

![Scan direction example](prepare-2.png "Scan direction example")

For display memory allocation:

![Memory allocation example](prepare-3.png "Memory allocation example")
