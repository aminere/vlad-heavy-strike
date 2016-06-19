using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace CppToCSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            string input = "stream.Serialize(PT_Bool, \"Visible\", &m_bVisible);";
            Match match = Regex.Match(input, "stream.Serialize\\(PT_Bool, \"([a-zA-Z0-9_]+)\", &([a-zA-Z0-9_]+)\\);");            
            Console.WriteLine("match.Groups.Count: " + match.Groups.Count);
            foreach (Group g in match.Groups)
            {
                Console.WriteLine("Group: " + g.Value);
            }
            if (match.Success && match.Groups.Count == 3)
            {
                string output = match.Groups[2] + " = stream.Serialize<bool>(\"" + match.Groups[1] + "\");";
                Console.WriteLine("output: " + output);
            }
            Console.Read();
        }
    }
}
