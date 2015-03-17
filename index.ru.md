---
wid: "root_about"
layout: default
title: О проекте
weight: 0
lang: ru
---
Русский
This application allows you to create bitmaps and fonts, and transform them to "C" source format for embedded applications.
The transformation of the images to the source code is made by using templates. Therefore, by modifying the templates, you can change the format of the output within certain limits.

As a result, the output of the program by default may be not suitable for your needs. It can and will need to be adjusted.

## Features

  * Create a single image;
  * Create fonts (set of images - characters):
      * Including unicode charset.
  Create the data structures for displays:
      * Monochrome, grayscale, color;
      * With vertical and horizontal orientation of bytes;
      * 8, 16, 24, 32-bit data;
      * <li>1...32 bits per pixel;
      * RLE compression;
      * With different display controllers, not limited by one particular model.

![Main window]({{ relative-baseurl }}images/main.png "Main window")
