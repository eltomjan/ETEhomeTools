# Gason C#
Is a C# translation of following one.<br>
It uses UTF-8 encoded Byte[] to parse (1/3 time to read, 1/2 memory) and 2 256B long tables to simply recognize Byte type (Definitions.cs / SearchTables).<br>
Rest is almost same except there new **DumpValueIterative** w/o recursion and some C# sugar like **VisualNode3** showing human readable content with complete browse ability and JSON preview.<br>
If **DEBUGGING** defined, there is also **extensive info** during deserialize (but it is aprox. 3x slower in debug mode then).<br>
If **KEY_SPLIT** defined, there is a new feature **splitting long (DB) arrays** @end of JSON file (example in Program.cs).<br>
There is also a lot of testcases in different formats and also 4 formatting checks.<br>
You can see in [Benchmark results](https://github.com/eltomjan/gason/blob/master/src/BenchmarkReport.txt), it is **5-8x faster** comparing 2 famous **Newtonsoft's Json.NET** and uses similar **less memory**.<br>
But a bit slower than C++ original - allocation hacks not translated (C++ using common new/delete would be even slower then Newtonsoft).<br>
<br>
Also format a bit changed - there are 2 Int32 for **position** in source array and **length**, but 4 double there is double value instead, double is not shortened to accommodate Tag # (guess C# code & speed overhead is not worth it).<br>
And final option is **reentrancy preparation** - in case, main parse loop can exit before end of parsing and run again (split, etc.).<br>
<br>
Some duplicated testcase were removed from code and pass/fail changed for some (according 2 http://json.org/JSON_checker/ & https://jsonformatter.curiousconcept.com/).<br>

Repo moved to a forked one [here](https://github.com/eltomjan/gason)