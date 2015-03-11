---
wid: "6_3"
layout: wikipage
title: Исходный код
lang: ru
---
Определения структур

```
typedef struct
{
    const unsigned char *data;
    uint16_t width;
    uint16_t height;
    uint8_t  dataSize;
} tImage;

typedef struct
{
    long int code;
    const tImage *image;
} tChar;

typedef struct
{
    int length;
    const tChar *chars;
} tFont;
```

Установка точки на дисплее

```
void set_pixel(int x, int y, int color)
{
    if (color != 0)
    {
        // set pixel
        ...
    }
    else
    {
        // reset pixel
        ...
    }
}
```

Вывод изображения

```
void draw_bitmap_mono(int x, int y, const tImage *image)
{
    uint8_t value = 0;
    int x0, y0;
    int counter = 0;
    const uint8_t *pdata = (const uint8_t *) image->data;
    // rows
    for (y0 = 0; y0 < image->height; y0++)
    {
        // columns
        for (x0 = 0; x0 < image->width; x0++)
        {
            // load new data
            if (counter == 0)
            {
                value = *pdata++;
                counter = image->dataSize;
            }
            counter--;

            // set pixel
            if ((value & 0x80) != 0)
                set_pixel(x + x0, y + y0, 1);
            else
                set_pixel(x + x0, y + y0, 0);

            value = value << 1;
        }
    }
}
```

Вывод изображения (RLE)

```
void draw_bitmap_mono_rle(int x, int y, const tImage *image)
{
    uint8_t value = 0;
    int x0, y0;
    int counter = 0;
    int8_t sequence = 0;
    int8_t nonsequence = 0;
    const uint8_t *pdata = (const uint8_t *) image->data;
    // rows
    for (y0 = 0; y0 < image->height && (y0 + y) < 320; y0++)
    {
        // columns
        for (x0 = 0; x0 < image->width; x0++)
        {
            // load new data
            if (counter == 0)
            {
                if ((sequence == 0) && (nonsequence == 0))
                {
                    sequence = *pdata++;
                    if (sequence < 0)
                    {
                        nonsequence = -sequence;
                        sequence = 0;
                    }
                }
                if (sequence > 0)
                {
                    value = *pdata;

                    sequence--;

                    if (sequence == 0)
                        pdata++;
                }
                if (nonsequence > 0)
                {
                    value = *pdata++;

                    nonsequence--;
                }
                counter = image->dataSize;
            }
            counter--;

            // set pixel
            if ((value & 0x80) != 0)
                set_pixel(x + x0, y + y0, 1);
            else
                set_pixel(x + x0, y + y0, 0);

            value = value << 1;
        }
    }
}
```

Поиск символа в шрифте по коду

```
const tChar *find_char_by_code(int code, const tFont *font)
{
    int count = font->length;
    int first = 0;
    int last = count - 1;
    int mid = 0;

    if (count > 0)
    {
        if ((code >= font->chars[0].code) && (code <= font->chars[count - 1].code))
        {
            while (last >= first)
            {
                mid = first + ((last - first) / 2);

                if (font->chars[mid].code < code)
                    first = mid + 1;
                else
                    if (font->chars[mid].code > code)
                        last = mid - 1;
                    else
                        break;
            }

            if (font->chars[mid].code == code)
                return (&font->chars[mid]);
        }
    }

    return (0);
}
```

Получение следующего кода символа в строке UTF-8

```
int utf8_next_char(const char *str, int start, int *resultCode, int *nextIndex)
{
    int len = 0;
    int index = 0;
    *resultCode = 0;

    while (*(str + index) != 0)
    {
        len++;
        index++;
    }

    unsigned char c;
    unsigned int code = 0;
    int result = 0;
    int skip = 0;

    *resultCode = 0;
    *nextIndex = -1;

    if (start >= 0 && start < len)
    {
        index = start;

        while (index < len)
        {
            c = *(str + index);
            index++;

            // msb
            if (skip == 0)
            {
                // if range 0x00010000-0x001fffff
                if ((c & 0xf8) == 0xf0)
                {
                    skip = 3;
                    code = c;
                }
                // if range 0x00000800-0x0000ffff
                else if ((c & 0xf0) == 0xe0)
                {
                    skip = 2;
                    code = c;
                }
                // if range 0x00000080-0x000007ff
                else if ((c & 0xe0) == 0xc0)
                {
                    skip = 1;
                    code = c;
                }
                // if range 0x00-0x7f
                else //if ((c & 0x80) == 0x00)
                {
                    skip = 0;
                    code = c;
                }
            }
            else // not msb
            {
                code = code << 8;
                code |= c;
                skip--;
            }
            if (skip == 0)
            {
                // completed
                *resultCode = code;
                *nextIndex = index;
                result = 1;
                break;
            }
        }
    }
    return (result);
}
```

Вывод строки

```
void draw_string(const char *str, int x, int y, const tFont *font)
{
    int len = ustrlen(str);
    int index = 0;
    int code = 0;
    int x1 = x;
    int nextIndex;
    while (index < len)
    {
        if (utf8_next_char(str, index, &code, &nextIndex) != 0)
        {
            const tChar *ch = find_char_by_code(code, font);
            if (ch != 0)
            {
                draw_bitmap_mono(x1, y, ch->image);
                x1 += ch->image->width;
            }
        }
        index = nextIndex;
        if (nextIndex < 0)
            break;
    }
}
```