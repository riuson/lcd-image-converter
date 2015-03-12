---
wid: "7_2_2"
layout: wikipage
title: Font
lang: en
---
# How to call this mode

Help for image conversion mode:

```
$ ./lcd-image-converter --mode=convert-font --help
Usage: ./lcd-image-converter [options]
Tool to create image and font source files for embedded applications.

Options:
  -h, --help                   Displays this help.
  -v, --version                Displays version information.
  -m, --mode <mode>            Conversion mode for application,
                               "convert-image", "convert-font" or "hex2bin".
  --family <family>            <Family> of new font.
  --size <size>                Font <size>.
  --mono, --monospaced         Make monospaced font.
  --style                       Font <style>, Regular, Bold, Italic or Bold Italic.
  --antialiasing               Use antialiasing.
  --chars-list <characters>    Characters, what included to the font.
  --chars-range <range>        Characters range, for example "32-255".
  --chars-encoding <encoding>  Characters encoding, for example "UTF-8".
  --big-endian                 Use big-endian instead of little-endian.
  -o, --output <path>          Full <path> to output result.
  --template <path>            Full <path> to template file, used in
                               conversion. [Optional]
  --doc-name <name>            Document name.
  --preset-name <name>         Output preset <name> from predefined presets in
                               application settings.
```

Preset. required in - preset-name, must be created in advance.

If option --template was omitted, application will use the template file specified in the preset itself.

# Example

```
$ ./lcd-image-converter --mode=convert-font \
    --family=Ubuntu \
    --size=14 \
    --mono \
    --style=Bold \
    --chars-list="abcабв" \
    --output=/temp/test.c \
    --doc-name=test \
    --preset-name=Monochrome
```

```
$ ./lcd-image-converter --mode=convert-font \
    --family=Ubuntu \
    --size=14 \
    --mono \
    --style=Bold \
    --chars-range="1040...1050" \
    --chars-encoding=UTF-16 \
    --output=/temp/test.c \
    --doc-name=test \
    --preset-name=Monochrome
```