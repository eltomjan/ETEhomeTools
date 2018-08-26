# El Tom Elzein's - Home Tools
<table><tbody><tr><th align="left" valign="top">cString&lt;size&gt;</th><td>
Template class, it has fixed size defined by template parameter, clear structure - (max. size defined by type), has only 2 members - short size and char array with content. Has quick Utf-8 &lt;-&gt; Win1250 conversion feature.
</td></tr>
<tr><th align="left" valign="top">fntXorPacker</th><td>
Cmd utility - can import GLCD fonts (https://github.com/andygock/glcd), ASCII or its own XORed fonts.<br>
It can generate XOR packed font (aprox. 30-50% size of bitmap version) - I have working Arduino implementation
(modified Adafruit_ILI9340 Adafruit_GFX libs).<br>
Import of XOR fonts was implemented as it has extended format and preview features in generated code now and comment may be used as ASCII source for reimport (as it is simple to make small adjustments in any text editor).
</td></tr>
<tr><th align="left" valign="top">cArrayPtr</th><td>
POC smarter array (smart pointer, boundaries check, data encapsulation/protection, parent/child window chain, child window shifting, can manage external allocation; 1 child level only at the moment); created to finetune & simplify terrible fntXorPacker still a bit "R&D" logic.
</td></tr>
<tr><th align="left" valign="top">ArduinoSimulator</th><td>
Cage (port to VS 2008) using fonts created by XorPacker predecessor &amp; updated LCD libraries.
</td></tr>
<tr><th align="left" valign="top">gfxXorPacker.cpp</th><td>
fntXorPacker row XOR applied to GFX fonts bitmap + test processing on all current fonts / Picopixel skipped (2B increase).
</td></tr>
<tr><th align="left" valign="top">KecalPort.cpp</th><td>
PC port of tiny & simple ZX Spectrum Kecal/Hlas TTS, export 2 WAV, added analog wave modulation to simulate ULA output RC integration element.
</td></tr>
<tr><th align="left" valign="top">scrollTable.htm</th><td>
Extracted from much bigger HTML5 IE/Chome/FF project.
</td></tr>
</tbody></table>
