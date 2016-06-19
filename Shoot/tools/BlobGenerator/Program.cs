using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace BlobGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] cppFiles = Directory.GetFiles(".", "*.cpp", SearchOption.AllDirectories);
            string[] cFiles = Directory.GetFiles(".", "*.c", SearchOption.AllDirectories);

            System.IO.StreamWriter outFile = new System.IO.StreamWriter("BuildFiles.cpp");
            foreach(string cpp in cppFiles)
            {
                string file = cpp.Substring(2, cpp.Length-2);
                outFile.WriteLine("#include \"" + file + "\"");
            }
            outFile.Flush();

            outFile = new System.IO.StreamWriter("BuildFiles.c");
            foreach (string c in cFiles)
            {
                string file = c.Substring(2, c.Length - 2);
                outFile.WriteLine("#include \"" + file + "\"");
            }
            outFile.Flush();
                        
            Console.ReadKey();
        }
    }
}
