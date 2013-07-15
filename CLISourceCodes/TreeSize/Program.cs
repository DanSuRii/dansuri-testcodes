using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

class DirInfo
{
    public String dirName;
    public long curSize;
    public long aSize;
    public System.Collections.Generic.LinkedList<DirInfo> listChild;

    public DirInfo() { listChild = new LinkedList<DirInfo>(); }

    public void DoPrint(int nDepth = 0)
    {
        Console.WriteLine("");
        String strTabs = new String(' ', nDepth++*2);
        Console.Write(strTabs);
        if(dirName != null)
            Console.Write("- {0} ({1,0:N0})Bytes ", dirName.Substring(dirName.LastIndexOf('\\')), aSize);
        foreach (DirInfo aInfo in listChild)
        {
            aInfo.DoPrint(nDepth);
        }

    }
};

namespace TreeSize
{
    class Program
    {
        static long GetDirectoriesSize(
            String strCurDir,
            System.Collections.Generic.LinkedList<DirInfo> parentList)
        {            
            String[] strFiles = Directory.GetFiles(strCurDir, "*.*");
            long aSize = 0;
            foreach (String s in strFiles)
            {
                FileInfo info = new FileInfo(s);
                aSize += info.Length;
            }


            DirInfo infoCur = new DirInfo();
            infoCur.dirName = strCurDir;
            infoCur.curSize = aSize;
            System.Collections.Generic.LinkedListNode<DirInfo> infoMine = parentList.AddLast(infoCur);


            String[] strDirectories = Directory.GetDirectories(strCurDir);
            foreach (String sDirs in strDirectories)
            {
                aSize += GetDirectoriesSize(sDirs, infoMine.Value.listChild);
            }

            //Console.WriteLine("{0} size({1,0:N0}) Bytes", strCurDir, aSize);
            //infoMine.Value.aSize = aSize;
            infoMine.Value.aSize = aSize;

            return aSize;
        }

        static void Main(string[] args)
        {
            foreach (string s in args)
            {
                Console.WriteLine(s);
            }
            //Console.Write("pause");
            //Console.ReadLine();

            String strCurDir = System.IO.Directory.GetCurrentDirectory();

            //Console.WriteLine(strCurDir);
            DirInfo infoRoot = new DirInfo();
            GetDirectoriesSize(strCurDir, infoRoot.listChild);


            //Console.WriteLine("{0,0:N0}",aSize);

            infoRoot.DoPrint();
            

            Console.WriteLine("Press any key to continue...");
            Console.ReadLine();
        }
    }
}
