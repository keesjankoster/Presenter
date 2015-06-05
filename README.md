# Presenter
Presentation Application for LUKE Theatre Group (http://luketheatre.co.uk) based on openFrameworks (http://openframeworks.cc). Presenter runs on Desktops (tested on Windows) and during shows on a Raspberry Pi.

## Presentations

Presentations are stored in /bin/data/Presentations/[PRESENTATION]
The PRESENTATION directory contains a presentation.xml file for configuration and all other assets like pictures and videos.

An example:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<slides>
  <slide backgroundcolor="#000000" backgroundimage="slide01.jpg" transition="none"/>
  <slide backgroundcolor="#000000" backgroundimage="slide02.jpg" transition="fade"/>
  <slide backgroundcolor="#000000" backgroundimage="slide03.jpg" transition="scrolling"/>
  <slide backgroundcolor="#000000" backgroundimage="slide04.jpg" transition="fadetoblack"/>
  <slide backgroundcolor="#000000" backgroundvideo="slide2.mp4" transition="fade"/>
</slides>
```
