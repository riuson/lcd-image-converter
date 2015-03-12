---
wid: "5"
layout: wikipage
title: Templates
subtitle:  About templates
lang: en
---
Conversion from bytes array to "C" source file made with help of template file.
This file is simple text file with key tags, surrounded with *$(tag)* or *@tag@* .

You can select own files in **Options -> Conversion -> Templates**, or use defaults templates *:/templates/image_convert* and *:/templates/font_convert* .

 Tag name | Deprecated name | Short description
:--------|:------------|:----------------
 doc_data_type | dataType | image or font
 doc_filename | fileName | xml source data file
 doc_name | documentName | name of document, entered on creation
 doc_name_ws | documentName_ws | documentName without spaces
 fnt_encoding | encoding | encoding, used to convert character to hexadecimal representation
 fnt_use_bom | bom | append (yes) on not append (no) Byte Order Mark to character's code
 fnt_family | fontFamily | font family
 fnt_size | fontSize | font size
 fnt_style | fontStyle | font style
 fnt_string | string | string of characters, what contained in file
 fnt_width_type | fontWidthType | font width type (proportional or monospaced)
 fnt_antialiasing | fontAntialiasing | font antialiasing
 img_byte_order | bytesOrder | bytes order
 img_data_block_size | dataBlockSize | data blocks size in bits
 img_rle | rle | RLE compression used (yes) or not used (no)
 img_split_to_rows | splitToRows | split string representation by rows
 out_blocks_count | blocksCount | number of blocks in data array
 out_bpp | bpp | bits per pixel
 out_char_code | charCode | character's hexadecimal representation
 out_char_text | charText | character in text form
 out_comma | comma | places ',', but nothing ('') at the end of list
 out_data_indent | imageDataIndent | image data indent (tab or spaces from template file)
 out_image_data | imageData | image's data array as text
 out_image_height | height | image height
 out_image_width | width | image width
 out_images_count | imagesCount | number of images (characters) in data file
 out_images_max_height | | images max height
 out_images_max_width | | images max width
 out_preset_name | preset | conversion preset name
 pre_conv_type | convType | conversion type (monochrome, grayscale, color)
 pre_inverse | inverse | invert image before processing
 pre_mono_edge | edge | edge value for monochrome-edge
 pre_mono_type | monoType | monochrome conversion type (edge, digguse dither, ordered dither, threshold dither)
 pre_scan_main | scanMain | main scanning direction
 pre_scan_sub | scanSub | line scanning direction
 pre_use_bands | bands | used bands (yes or no)
 pre_band_width | bandWidth | width of band
 tmpl_filename | templateFile | template file name

Template block between **$(start_block_images_table)** and **$(end_block_images_table)** repeats for each image in data file.

List of used codecs:

  *  UTF-8
  *  UTF-16
  *  UTF-16BE
  *  UTF-16LE
  *  UTF-32
  *  UTF-32BE
  *  UTF-32LE
  *  Windows-1250 ... 1258
  *  ...

[Image template](https://github.com/riuson/lcd-image-converter/blob/master/resources/image.tmpl)
```
$(start_block_header)
/*******************************************************************************
* $(doc_data_type)
* filename: $(doc_filename)
* name: $(doc_name)
*
* preset name: $(out_preset_name)
* data block size: $(img_data_block_size) bit(s), uint$(img_data_block_size)_t
* RLE compression enabled: $(img_rle)
* conversion type: $(pre_conv_type), $(pre_mono_type) $(pre_mono_edge)
* bits per pixel: $(out_bpp)
*
* preprocess:
*  main scan direction: $(pre_scan_main)
*  line scan direction: $(pre_scan_sub)
*  inverse: $(pre_inverse)
*******************************************************************************/

/*
 typedef struct {
     const uint$(img_data_block_size)_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>
$(end_block_header)

$(start_block_images_table)
static const uint$(img_data_block_size)_t image_data_$(doc_name_ws)[$(out_blocks_count)] = {
    $(out_image_data)
};
const tImage $(doc_name_ws) = { image_data_$(doc_name_ws), $(out_image_width), $(out_image_height),
    $(img_data_block_size) };
$(end_block_images_table)
```

[Font template](https://github.com/riuson/lcd-image-converter/blob/master/resources/font.tmpl)
```
$(start_block_header)
/*******************************************************************************
* $(doc_data_type)
* filename: $(doc_filename)
* name: $(doc_name)
* family: $(fnt_family)
* size: $(fnt_size)
* style: $(fnt_style)
* included characters: $(fnt_string)
* antialiasing: $(fnt_antialiasing)
* type: $(fnt_width_type)
* encoding: $(fnt_encoding)
* unicode bom: $(fnt_use_bom)
*
* preset name: $(out_preset_name)
* data block size: $(img_data_block_size) bit(s), uint$(img_data_block_size)_t
* RLE compression enabled: $(img_rle)
* conversion type: $(pre_conv_type), $(pre_mono_type) $(pre_mono_edge)
* bits per pixel: $(out_bpp)
*
* preprocess:
*  main scan direction: $(pre_scan_main)
*  line scan direction: $(pre_scan_sub)
*  inverse: $(pre_inverse)
*******************************************************************************/

/*
 typedef struct {
     long int code;
     const tImage *image;
     } tChar;
 typedef struct {
     int length;
     const tChar *chars;
     } tFont;
*/

#include <stdint.h>
$(end_block_header)

$(start_block_images_table)
static const uint$(img_data_block_size)_t image_data_$(doc_name_ws)_0x$(out_char_code)[$(out_blocks_count)] = {
    $(out_image_data)
};
static const tImage $(doc_name_ws)_0x$(out_char_code) = { image_data_$(doc_name_ws)_0x$(out_char_code),
    $(out_image_width), $(out_image_height), $(img_data_block_size)};
$(end_block_images_table)

static const tChar $(doc_name_ws)_array[] = {
$(start_block_images_table)
  // character: '$(out_char_text)'
  {0x$(out_char_code), &$(doc_name_ws)_0x$(out_char_code)}$(out_comma)
$(end_block_images_table)
};

$(start_block_font_def)
const tFont $(doc_name_ws) = { $(out_images_count), $(doc_name_ws)_array };
$(end_block_font_def)
```
