---
wid: "7_2_1"
layout: wikipage
title: Image
lang: en
---
# How to call this mode

Help for image conversion mode:

{% highlight bash %}
```
$ ./lcd-image-converter --mode=convert-image --help
Usage: ./lcd-image-converter [options]
Tool to create image and font source files for embedded applications.

Options:
  -h, --help            Displays this help.
  -v, --version         Displays version information.
  -m, --mode <mode>     Conversion mode for application, "convert-image",
                        "convert-font" or "hex2bin".
  -i, --input <path>    Full <path> to source image in binary format.
  -o, --output <path>   Full <path> to output result.
  --template <path>     Full <path> to template file, used in conversion.
                        [Optional]
  --doc-name <name>     Document name.
  --preset-name <name>  Output preset <name> from predefined presets in
                        application settings.
```
{% endhighlight %}

Preset. required in - preset-name, must be created in advance.

If option --template was omitted, application will use the template file specified in the preset itself.

# Example

{% highlight bash %}
```
$ ./lcd-image-converter --mode=convert-image \
    --doc-name=test \
    --preset-name=Monochrome \
    --input=/temp/box.png \
    --output=/temp/box.c \
    --template=/temp/image.tmpl
```
{% endhighlight %}