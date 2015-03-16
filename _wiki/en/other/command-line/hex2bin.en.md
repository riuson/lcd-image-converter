---
wid: "7_2_3"
layout: wikipage
title: HEX 2 BIN 
lang: en
---
# How to call this mode

Help for hex to bin conversion mode:

{% highlight bash %}
```$ ./lcd-image-converter --mode=hex2bin --help
Usage: ./lcd-image-converter [options]
Tool to create image and font source files for embedded applications.

Options:
  -h, --help           Displays this help.
  -v, --version        Displays version information.
  -m, --mode <mode>    Conversion mode for application, "convert-image",
                       "convert-font" or "hex2bin".
  -i, --input <path>   Full <path> to image in *.C format.
  -o, --output <path>  Full <path> to output binary result.
```
{% endhighlight %}

# Preset

Settings:

  *  Image -> Prefix = "0x"
  *  Image -> Suffix = ""
  *  Image -> Delimiter = " "

Template file for image:

{% highlight bash %}
```$(start_block_images_table)
le
uint16 $(out_image_width)
uint16 $(out_image_height)
uint16 $(out_blocks_count)
uint$(img_data_block_size) $(out_image_data)
$(end_block_images_table)
```
{% endhighlight %}

uint8, uint16, uint24, uint32 - data size.

le, be - little endian and big endian.

# Example

{% highlight bash %}
```$ ./lcd-image-converter --mode=hex2bin \
    --input=/temp/colors.c \
    --output=/temp/colors.bin
```
{% endhighlight %}

"C" file after conversion to sources:

{% highlight bash %}
```static const uint8_t image_data_colors[10] = {
    0xd0, 
    0xf0, 
    0x90, 
    0xf0, 
    0xd0, 
    0xb0, 
    0xd0, 
    0xf0, 
    0x90, 
    0xf0
};
const tImage colors = { image_data_colors, 10, 5};
```
{% endhighlight %}

With modified template:

{% highlight bash %}
```le
uint16 10
uint16 5
uint16 10
uint8 0xd0 
    0xf0 
    0x90 
    0xf0 
    0xd0 
    0xb0 
    0xd0 
    0xf0 
    0x90 
    0xf0
```
{% endhighlight %}

Binary file after conversion from hex to bin::

{% highlight bash %}
```00000000: 0A 00 05 00 0A 00 D0 F0    90 F0 D0 B0 D0 F0 90 F0
```
{% endhighlight %}
