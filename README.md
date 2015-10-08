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
  <slide backgroundcolor="#000000" backgroundvideo="video.mp4" transition="fade"/>
    <item type="title" font="MyriadPro-Bold.otf">Father God, you're by my side</item>
    <item type="lyric" font="MyriadPro-Bold.otf"><![CDATA[Father God,
you're by my side
And you will never,
ever leave me
Father God,
you're by my side
And you will never,
ever leave me]]></item>
	  <item type="lyric" font="Amble-Bold.ttf"><![CDATA[When the road is really tough
And it's very hard to walk
When I'm feeling really rough
And it's even hard to talk to you]]></item>
  </slide>
</slides>
```
## Screenshots

![alt tag](https://github.com/keesjankoster/Presenter/raw/master/doc/screen1.png)

![alt tag](https://github.com/keesjankoster/Presenter/raw/master/doc/screen2.png)

![alt tag](https://github.com/keesjankoster/Presenter/raw/master/doc/screen3.png)

![alt tag](https://github.com/keesjankoster/Presenter/raw/master/doc/screen4.png)

## Codecs

It seems that on Windows Codecs need to be installed: http://www.codecguide.com/download_kl.htm


