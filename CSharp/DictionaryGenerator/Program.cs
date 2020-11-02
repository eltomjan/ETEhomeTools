using System.Speech.Synthesis;
using System.IO;

namespace Speaker
{
    class Program
    {
        static void Main(string[] args)
        {
            var slov = File.ReadAllText("Slovnik.txt").Replace("\r","").Split('\n');
            int i = 1000;
            foreach (var item in slov)
            {
                var pair = item.Split('\t');
                write(pair[0], "a" + i.ToString().Substring(1) + ".wav", false);
                write(pair[1], "c" + i.ToString().Substring(1) + ".wav", true);
                i++;
            }
        }
        static void write(string text, string file, bool czech)
        {
            var speak = new SpeechSynthesizer();
            var v = speak.GetInstalledVoices();
            if (czech) speak.SelectVoice("Microsoft Jakub");
            else
            {
                speak.SelectVoice("Microsoft David Desktop");
                speak.Rate = -3;
            }
            speak.SetOutputToWaveFile(file);
            speak.Speak(text);

        }
    }
}
