---
wid: "7_2"
layout: wikipage
title: Командная строка
subtitle: Режим командной строки
lang: ru
---
В данном режиме преобразование нового документа может быть выполнено без использования GUI, используя заранее подготовленные настройки.

Внимание

  *  В версии для Windows не выводятся сообщения в консоль. Если они необходимы, надо пересобрать приложение с включённой опцией CONFIG += console в файле проекта (lcd-image-converter.pro);
  *  Требуется версия Qt >= 5.2.

Вызов общей справки:

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