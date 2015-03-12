---
wid: "7_1"
layout: wikipage
title: Сборка
subtitle: Сборка из исходников
lang: ru
---
Клонировать репозиторий:

{% highlight bash %}
```
git clone https://riuson@code.google.com/p/lcd-image-converter/ 
```
{% endhighlight %}

Qt Creator:

  *  Установить Qt SDK;
  *  Открыть проект "lcd-image-converter.pro" в Qt Creator;
  *  Пересобрать проект.

Консоль:

{% highlight bash %}
```
cd lcd-image-converter
qmake
make
# запустить приложение
./_linux/release/lcd-image-converter
```
{% endhighlight %}

Пример вывода:

{% highlight bash %}
```
user@desktop:~/workspace/lcd-image-converter (git: develop)
➜ qmake
RCC: Error in 'resources/resources.qrc': Cannot find file 'lcd-image-converter-ru.qm'
user@desktop:~/workspace/lcd-image-converter (git: develop)
➜ make
Updating './resources/lcd-image-converter-ru.qm'...
    Generated 168 translation(s) (168 finished and 0 unfinished)
    Ignored 5 untranslated source text(s)
Updating './resources/lcd-image-converter-ru.qm'...
    Generated 168 translation(s) (168 finished and 0 unfinished)
/usr/bin/uic-qt4 controls/mainwindow.ui -o .uic/ui_mainwindow.h

...

g++ -m64 -Wl,-O1 -o _linux/release/lcd-image-converter .obj/main.o .obj/bitmapcontainer.o .obj/mainwindow.o 
.obj/widgetbitmapeditor.o .obj/editortabimage.o .obj/editortabfont.o .obj/dialogsavechanges.o .obj/bitmaphelper.o 
.obj/dialogresize.o .obj/fontcontainer.o .obj/charactersmodel.o .obj/dialogfontselect.o .obj/dialogabout.o 
.obj/recentlist.o .obj/starttab.o .obj/actionimagehandlers.o .obj/actionhandlersbase.o .obj/actionfonthandlers.o 
.obj/actionhelphandlers.o .obj/actionsetuphandlers.o .obj/actionfilehandlers.o .obj/dialogfontpreview.o 
.obj/revisionlabel.o .obj/revisioninfo.o .obj/converterhelper.o .obj/conversionmatrixoptions.o .obj/bitstream.o 
.obj/dialogpreview.o .obj/matrixpreviewmodel.o .obj/conversionmatrix.o .obj/dialogconvert.o .obj/dialogsetup.o 
.obj/parser.o .obj/matrixitemdelegate.o .obj/moc_bitmapcontainer.o .obj/moc_mainwindow.o 
.obj/moc_widgetbitmapeditor.o .obj/moc_editortabimage.o .obj/moc_editortabfont.o .obj/moc_dialogsavechanges.o 
.obj/moc_dialogresize.o .obj/moc_fontcontainer.o .obj/moc_charactersmodel.o .obj/moc_dialogfontselect.o 
.obj/moc_dialogabout.o .obj/moc_recentlist.o .obj/moc_starttab.o .obj/moc_actionimagehandlers.o 
.obj/moc_actionhandlersbase.o .obj/moc_actionfonthandlers.o .obj/moc_actionhelphandlers.o 
.obj/moc_actionsetuphandlers.o .obj/moc_actionfilehandlers.o .obj/moc_dialogfontpreview.o .obj/moc_revisionlabel.o 
.obj/moc_conversionmatrixoptions.o .obj/moc_dialogpreview.o .obj/moc_matrixpreviewmodel.o .obj/moc_conversionmatrix.o 
.obj/moc_dialogconvert.o .obj/moc_dialogsetup.o .obj/moc_parser.o .obj/qrc_resources.o    
-L./_linux/release -L/usr/lib/x86_64-linux-gnu -lQtXmlPatterns -lQtXml -lQtGui -lQtCore -lpthread
```
{% endhighlight %}

[Полный лог сборки программы для ревизии be13e6f34980](build.log_.4f8e4f99.zip).
