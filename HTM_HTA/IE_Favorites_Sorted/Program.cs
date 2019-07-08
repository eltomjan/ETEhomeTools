using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Xsl;

//https://www.codeproject.com/Articles/22267/Internet-Explorer-Favorites-deconstructed

namespace CodeProject.IEFavDecon
{
    class Program
    {
        static void Main(string[] args)
        {
            FavoritesNode node = FavoritesNode.LoadRoot();
            XBELConverter.ConvertFavoritesToXBEL(node, "bookmarks.xml");

#if TRANSFORMTOWEBPAGE
            // in this case we have to make a XmlReader with special XmlReaderSettings and set ProhibitDtd to false
            // otherwise the XmlTransform will refuse to process the XBEL related DTD.
            XmlReaderSettings readerSettings = new XmlReaderSettings();
            readerSettings.ProhibitDtd = false;
            XmlReader xmlReader = XmlReader.Create("bookmarks.xml", readerSettings);
            XslCompiledTransform compiledTransform = new XslCompiledTransform();
            compiledTransform.Load("xbel2xhtml.xsl");
            compiledTransform.Transform(xmlReader, new XsltArgumentList(), new StreamWriter("bookmarks.html"));
#endif
        }
    }
}
