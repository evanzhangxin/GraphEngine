﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Trinity.Utilities
{
    internal static class AssemblyPath
    {
        private static string my_assembly_path = null;
        internal static string MyAssemblyPath
        {
            get
            {
                if (my_assembly_path == null)
                {
                    //  primary heuristics: find the assembly that calls into Trinity
                    Assembly trinity_asm = Assembly.GetExecutingAssembly();
                    int firstTrinityFrame = 0;
                    for (int skipFrames = 1; ; skipFrames++)
                    {
                        StackFrame stackFrame;
                        try
                        {
                            stackFrame = new StackFrame(skipFrames);
                            if (stackFrame.GetMethod() == null) break;
                        }
                        catch { break; }

                        try
                        {
                            var asm = stackFrame.GetMethod().Module.Assembly;
                            if (asm == trinity_asm) firstTrinityFrame = skipFrames;
                        }
                        catch { continue; }
                    }

                    for (int skipFrames = firstTrinityFrame + 1; ; skipFrames++)
                    {
                        StackFrame stackFrame;
                        try
                        {
                            stackFrame = new StackFrame(skipFrames);
                            if (stackFrame.GetMethod() == null) break;
                        }
                        catch { break; }

                        try
                        {
                            var method = stackFrame.GetMethod();
                            var type   = method.DeclaringType;
                            var asm    = method.Module.Assembly;

                            if (asm == trinity_asm) continue;
                            if (asm.IsDynamic) continue;
                            if (type == typeof(System.RuntimeMethodHandle)) continue;
                            if (type.FullName == "System.Linq.Enumerable+WhereSelectEnumerableIterator`2") continue;
                            if (type.FullName == "System.Linq.Enumerable+WhereEnumerableIterator`1") continue;
                            if (type.FullName == "System.Linq.Enumerable") continue;

                            var path = asm.Location;
                            my_assembly_path = System.IO.Path.GetDirectoryName(path) + Path.DirectorySeparatorChar;
                            break;
                        }
                        catch { continue; }
                    }
                }

                if (my_assembly_path == null)
                {
                    //  last resort heuristic: return the path of the Trinity assembly.
                    my_assembly_path = System.IO.Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + Path.DirectorySeparatorChar;
                }

                return my_assembly_path;
            }
        }
    }
}
