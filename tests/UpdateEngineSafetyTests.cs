using System;
using System.IO;
using System.Reflection;
using DarksidersGenesisUpdater;
internal static class UpdateEngineSafetyTests {
    private static int failures;
    private static void Check(bool ok,string label){if(!ok){failures++;Console.WriteLine("FAIL "+label);}}
    private static int Main(){
        string folder=Path.Combine(Path.GetTempPath(),"DGE-State-Test-"+Guid.NewGuid().ToString("N"));
        Directory.CreateDirectory(folder);
        string state=UpdateEngine.StateFolder(folder);
        string asi=Path.Combine(folder,"DarksidersGenesisMod.asi"),dll=Path.Combine(folder,"dxgi.dll");
        string ini=Path.Combine(folder,"DarksidersGenesisMod.ini");
        string ledger=Path.Combine(state,"installed.txt"),fingerprint=Path.Combine(state,"installed-binaries.sha256");
        try {
            File.WriteAllText(asi,"test-asi");File.WriteAllText(dll,"test-dxgi");
            Directory.CreateDirectory(state);
            File.WriteAllText(ledger,"debug|1|39|test|old.zip");
            Check(UpdateEngine.ReadState(folder)=="","legacy ledger cannot trigger automatic downgrade after manual install");
            var save=typeof(UpdateEngine).GetMethod("SaveState",BindingFlags.NonPublic|BindingFlags.Static);
            const string token="debug|1|41|test|current.zip";
            save.Invoke(null,new object[]{folder,token});
            Check(UpdateEngine.ReadState(folder)==token,"matching installed pair retains updater state");
            File.WriteAllText(asi,"manual-new-asi");
            Check(UpdateEngine.ReadState(folder)=="","manual ASI replacement invalidates state");
            File.WriteAllText(asi,"test-asi");
            Check(UpdateEngine.ReadState(folder)==token,"restored pair matches recorded state");
            File.WriteAllText(dll,"manual-new-loader");
            Check(UpdateEngine.ReadState(folder)=="","manual loader replacement invalidates state");
            File.WriteAllText(dll,"test-dxgi");File.WriteAllText(ini,"MenuKey=Insert");
            Check(UpdateEngine.ReadState(folder)==token,"personal INI edits do not invalidate binary version");
            File.Delete(asi);
            Check(UpdateEngine.ReadState(folder)=="","missing installed binary invalidates state");
        }catch(Exception ex){Console.WriteLine(ex);failures++;}
        finally {
            // Remove only this test's known files and now-empty directories; no recursion.
            foreach(string file in new[]{asi,dll,ini,ledger,fingerprint})if(File.Exists(file))File.Delete(file);
            if(Directory.Exists(folder))Directory.Delete(folder);
            if(Directory.Exists(state))Directory.Delete(state);
        }
        Console.WriteLine("Updater binary state: {0} failures",failures);
        return failures==0?0:1;
    }
}
