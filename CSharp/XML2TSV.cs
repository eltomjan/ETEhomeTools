using System;
using System.Collections.Generic;
using System.Xml;
using System.Text;

public class Program
{
    public static void Main(string[] args)
    {

        XmlDocument doc = new XmlDocument();
        bool /**/sortLast = false,/*/sortLast = true,//*/
             /**/horizontal = false;/*/horizontal = true;//*/
        if(args.Length == 3)
        {
            sortLast = args[0] == "1";
            horizontal = args[1] == "1";
            doc.Load(args[2]);
        } else doc.Load(args[0]);
        XmlNode node = doc.DocumentElement.FirstChild;
        Stack<XmlNode> s = new Stack<XmlNode>();
        if (node == null)
            return;
        s.Push(node);
        List<String> path = new List<String>();
        String text = "", id = "DeviceId";
        while (s.Count > 0)
        {
            node = s.Pop();
            if (null != node.NextSibling) {
                s.Push(node.NextSibling);
            } else if (null != node.FirstChild) {
                if (!node.Name.StartsWith("No")) path.Add(node.Name); // <No#> from JSON conversion
                s.Push(node.FirstChild);
            }
            if (null != node.FirstChild // child which
             && null != node.FirstChild.FirstChild // does not have children
             && null == node.FirstChild.FirstChild.FirstChild) {
                XmlNode back = node;
                node = node.FirstChild;
                String key = "";
                if (sortLast) {
                    SortedList<String, String> lastChildrens = new SortedList<String, String>();
                    while (null != node) {
                        if (node.Name == id) key = node.InnerText;
                        else
                        {
                            lastChildrens[node.Name] = node.InnerText;
                        }
                        node = node.NextSibling;
                    }
                    if (0 == key.Length) key = back.Name;
                    if (horizontal) {
                        if (path.Count == 0) text += $"{key}\\";
                        else text += $"{String.Join("\\", path.ToArray())}\\{key}\\";
                    }
                    foreach (KeyValuePair<String, String> item in lastChildrens) {
                        if (horizontal) text += $"\t{item.Key}\t{item.Value}";
                        else {
                            if (path.Count == 0) text += $"{key}\\{item.Key}\t{item.Value}\n";
                            else text += $"{String.Join("\\", path.ToArray())}\\{key}\\{item.Key}\t{item.Value}\n";
                        }
                    }
                } else {
                    XmlNode keyNode = node;
                    while (null != keyNode) {
                        if (keyNode.Name == id)
                        {
                            key = keyNode.InnerText;
                            break;
                        } else keyNode = keyNode.NextSibling;
                    }
                    if (0 == key.Length) key = back.Name;
                    if (horizontal) {
                        if (path.Count == 0) text += $"{key}\\";
                        else text += $"{String.Join("\\", path.ToArray())}\\{key}\\";
                    }
                    while (null != node) {
                        if (node != keyNode)
                        {
                            if (horizontal) text += $"\t{node.Name}\t{node.InnerText}";
                            else {
                                if (path.Count == 0) text += $"{key}\\{node.Name}\t{node.InnerText}\n";
                                else text += $"{String.Join("\\", path.ToArray())}\\{key}\\{node.Name}\t{node.InnerText}\n";
                            }
                        }
                        node = node.NextSibling;
                    }
                }
                if (horizontal) text += '\n';
            }
        }
        Console.WriteLine(text);
    }
}