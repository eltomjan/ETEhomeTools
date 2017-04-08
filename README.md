# ETEhomeTools
<table><tr><th vAlign=top>cString<size></th><td>template class, it has fixed size defined by template parameter, clear structure - (max. size defined by type), has only 2 members - short size and char array with content. Has quick Utf-8 <-> Win1250 conversion feature.</td></tr></table>
<table><tr><th vAlign=top>fntXorPacker</th><td>stand-alone cmd utility, it can import only GLCD fonts (https://github.com/andygock/glcd) at the moment.<br>
Only purpose at the moment is to generate XOR packed font (aprox. 30-50% size of bitmap version) - I have working Arduino implementation
(modified Adafruit_ILI9340 Adafruit_GFX libs). There is also import of XOR fonts implemented as it has extended format and preview
features in generated code.
</td></tr></table>
