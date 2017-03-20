# <img src="resources/icons/lcd-image-converter-64.png"/> LCD Image Converter
[![Travis Build Status](https://img.shields.io/travis/riuson/lcd-image-converter.svg?label=linux)](https://travis-ci.org/riuson/lcd-image-converter)
[![Coverity Scan](https://img.shields.io/coverity/scan/3997.svg?maxAge=2592000)](https://scan.coverity.com/projects/riuson-lcd-image-converter)

Tool to create bitmaps and fonts for embedded applications, v.2

Allows you to create bitmaps and fonts, and transform them to "C" source format for embedded applications.

## Features
- Supported display controllers
  - Monochrome, grayscale, color
  - With vertical and horizontal orientation of bytes
  - 8, 16, 24, 32-bit data
  - 1...32 bits per pixel
  - and other, not limited by some particular models
- Output format
  - Can be changed by templates
  - Text (source code) or binary
- Create a single image
  - With RLE compression
- Create fonts (set of images - characters)
  - Including unicode charset
  - Required characters only, not full range
- Command-line mode

## Web pages
- [Home web site](http://www.riuson.com/lcd-image-converter)
  - [Documentation](http://www.riuson.com/lcd-image-converter)
  - [Latest downloads](http://www.riuson.com/lcd-image-converter/download)
  - [Changelog](http://www.riuson.com/lcd-image-converter/changelog)
- [Source code repository on GitHub](https://github.com/riuson/lcd-image-converter)

## Contacts and support
 - Author: Vladimir
 - Mailto: riuson@gmail.com
 - [GitHub Issues](https://github.com/riuson/lcd-image-converter/issues)

## How to build on GNU/Linux
~~~
git clone https://github.com/riuson/lcd-image-converter.git ~/lcd-image-converter
cd ~/lcd-image-converter
qmake
make
~~~
Requires some additional packages.
Build logs available on [Travis-CI](https://travis-ci.org/riuson/lcd-image-converter).



## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/
