# El Tom Elzein's - Home Tools
<table><tbody><tr><th align="left" valign="top">cString&lt;size&gt;</th><td>
template class, it has fixed size defined by template parameter, clear structure - (max. size defined by type), has only 2 members - short size and char array with content. Has quick Utf-8 &lt;-&gt; Win1250 conversion feature.
</td></tr>
<tr><th align="left" valign="top">fntXorPacker</th><td>
stand-alone cmd utility, it can import only GLCD fonts (https://github.com/andygock/glcd) at the moment.<br>
Only purpose at the moment is to generate XOR packed font (aprox. 30-50% size of bitmap version) - I have working Arduino implementation
(modified Adafruit_ILI9340 Adafruit_GFX libs). There is also import of XOR fonts implemented as it has extended format and preview
features in generated code.
</td></tr>
<tr><th align="left" valign="top">cArrayPtr</th><td>
POC smarter array (smart pointer, boundaries check, parent/child chain, child window shifting, can manage external allocation; 1 child level only at the moment); created to finetune & simplify fntXorPacker logic.
</td></tr>
<tr><th align="left" valign="top">ArduinoSimulator</th><td>
Cage (port to VS 2008) using fonts created by XorPacker predecessor &amp; updated LCD libraries.
</td></tr>
</tbody></table>