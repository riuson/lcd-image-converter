---
wid: "5"
layout: wikipage
title: О шаблонах
subtitle: Описание шаблонов
lang: ru
---
Преобразование из массива байтов в исходник "C" осуществляется с помощью подстановки данных в файл шаблона. Этот файл является простым текстовым файлом с выделенными ключевыми словами, окружёнными *$(тег)* или *@тег@*.

Собственный файл шаблона можно выбрать в меню Настройки -> Преобразование -> Шаблоны, либо можно использовать встроенные шаблоны по умолчанию, указав пути к файлам *:/templates/image_convert* and *:/templates/font_convert* .

Имя тега | Прежнее имя | Краткое описание
:--------|:------------|:----------------
 doc_data_type | dataType | image или font |
 doc_filename | fileName | путь к исходному xml файлу |
 doc_name | documentName | имя документа, вводится при создании
 doc_name_ws | documentName_ws | имя документа без пробелов
 fnt_encoding | encoding | кодировка, испольуемая для преобразования символов в шестнадцатеричное представление
 fnt_use_bom | bom | добавляется ли (yes) Byte Order Mark к коду символа или нет (no)
 fnt_family | fontFamily | семейство шрифта
 fnt_size | fontSize | размер
 fnt_style | fontStyle | стиль
 fnt_string | string | строка символов, реализованных в исходнике шрифта
 fnt_width_type | fontWidthType | proportional или monospaced
 fnt_antialiasing | fontAntialiasing | есть сглаживание (yes) или нет (no)
 img_byte_order | bytesOrder | порядок байтов
 img_data_block_size | dataBlockSize | число бит в блоке данных
 img_rle | rle | используется ли RLE сжатие (yes) или нет (no)
 img_split_to_rows | splitToRows | разбивается ли строковое представление данных по строкам изображения
 out_blocks_count | blocksCount | число блоков данных в массиве
 out_bpp | bpp | число бит на точку
 out_char_code | charCode | символ в шестнадцатеричном виде
 out_char_text | charText | символ в текстовом виде
 out_comma | comma | подставляет запятую ',', если не является последним вхождением в списке, тогда подставляет пустую строку
 out_data_indent | imageDataIndent | отступы строк данных изображения (табуляция или пробел, из файла шаблона)
 out_image_data | imageData | массив блоков данных изображения в текстовом виде
 out_image_height | height | высота изображения
 out_image_width | width | ширина изображения
 out_images_count | imagesCount | число изображений (символов) в файле шрифта
 out_images_max_height | | максимальная высота изображения
 out_images_max_width | | максимальная ширина изображения
 out_preset_name | preset | название предустановки преобразования
 pre_conv_type | convType | тип преобразования (monochrome, grayscale, color)
 pre_inverse | inverse | инверсия изображения перед обработкой
 pre_mono_edge | edge | значение порога для monochrome-edge
 pre_mono_type | monoType | тип монохромного преобразования (edge, digguse dither, ordered dither, threshold dither)
 pre_scan_main | scanMain | основное направление сканирования
 pre_scan_sub | scanSub | направление сканирования линий
 pre_use_bands | bands | используются полосы (yes) или нет (no)
 pre_band_width | bandWidth | ширина полос
 tmpl_filename | templateFile | имя файла шаблона

Участок шаблона между **$(start_block_images_table)** и **$(end_block_images_table)** повторяется для каждого изображения (символа) в файле.

Список используемых кодеков:

  *  UTF-8
  *  UTF-16
  *  UTF-16BE
  *  UTF-16LE
  *  UTF-32
  *  UTF-32BE
  *  UTF-32LE
  *  Windows-1250 ... 1258
  *  ...

[Шаблон изображения](https://github.com/riuson/lcd-image-converter/blob/master/resources/image.tmpl)

{% highlight text %}
```$(start_block_header)
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
{% endhighlight %}

[Шаблон шрифта](https://github.com/riuson/lcd-image-converter/blob/master/resources/font.tmpl)

{% highlight text %}
```$(start_block_header)
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
{% endhighlight %}
