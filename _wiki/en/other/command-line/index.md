---
wid: "7_2"
layout: wikipage
title: Command line
subtitle: Command line mode
lang: en
---
In this mode, a document's conversion can be performed without using of GUI, but with pre-defined settings.

**Notice**

  *  Application version for Windows prints no messages to the console. If they are necessary, rebuild the application with the option CONFIG += console in the project file (lcd-image-converter.pro);
  *  Required Qt version >= 5.2.

# How to call this mode

Calling basic help:

{% highlight bash %}
```
$ ./lcd-image-converter --help
Usage: ./lcd-image-converter [options]
Tool to create image and font source files for embedded applications.

Options:
  -h, --help         Displays this help.
  -v, --version      Displays version information.
  -m, --mode <mode>  Conversion mode for application, "convert-image",
                     "convert-font" or "hex2bin".
```
{% endhighlight %}