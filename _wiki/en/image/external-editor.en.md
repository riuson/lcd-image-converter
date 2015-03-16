---
wid: "2_3"
layout: wikipage
title: External editor
lang: en
---
This feature allows you to edit the image in an external editor.

Algorithm:

  *  The image is saved to a temporary file PNG;
  *  Graphical editor called with the first argument - the name of the file;
  *  While an external editor is running, this application is blocked;
  *  After closing of the editor, this application checks for changes in a temporary file;
  *  If there is a changes, then original image will be replaced by temporary file.

The path to the external editor is specified in the **Options -> External Editor**:

![Dialog](resize-1.png "Dialog")