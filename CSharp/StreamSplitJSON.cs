    using Newtonsoft.Json;
    using System.IO;
    
    namespace ConvertBigJSON
    {
        class Program
        {
            static void Main(string[] args)
            {
                long start = 0, end = 0, pos = 0;
                //long oldPos = 0, oldLine = 0;
                long size = 500000;
                bool neverEnd = true;
                while (neverEnd)
                {
                    end = start + size - 1;
                    FileStream fs = File.Open(@"bigOne.json", FileMode.Open, FileAccess.Read, FileShare.None);
                    StreamReader sr = new StreamReader(fs);
                    FileStream ofs = new FileStream(@"PartNo"+start+".json", FileMode.Create);
                    StreamWriter sw = new StreamWriter(ofs);
                    using (JsonWriter writer = new JsonTextWriter(sw))
                    using (var jsonReader = new JsonTextReader(sr))
                    {
                        while (neverEnd)
                        {
                            neverEnd &= jsonReader.Read();
                            if (jsonReader.TokenType == JsonToken.StartObject && jsonReader.Path.IndexOf("BigArrayPathStart") == 0)
                            {
                                if (pos > end)
                                {
                                    break;
                                }
                                if (pos++ < start)
                                {
                                    do
                                    {
                                        jsonReader.Read();
                                    } while (jsonReader.TokenType != JsonToken.EndObject);
                                    continue;
                                }
                            }
                            //if (jsonReader.TokenType == JsonToken.Float)
                            //{
                            //    long shift = (oldLine - jsonReader.LineNumber) + (oldPos - jsonReader.LinePosition);
                            //    string x = jsonReader.ReadAsString();
                            //}
                            if (jsonReader.TokenType >= JsonToken.PropertyName)
                            {
                                writer.WriteToken(jsonReader);
                            }
                            else if (jsonReader.TokenType == JsonToken.StartObject)
                            {
                                writer.WriteStartObject();
                            }
                            else if (jsonReader.TokenType == JsonToken.StartArray)
                            {
                                writer.WriteStartArray();
                            }
                            else if (jsonReader.TokenType == JsonToken.StartConstructor)
                            {
                                writer.WriteStartConstructor(jsonReader.Value.ToString());
                            }
                            //oldPos = jsonReader.LinePosition;
                            //oldLine = jsonReader.LineNumber;
                        }
                        start = pos;
                        pos = 0;
                    }
                }
                return;
            }
        }
    }