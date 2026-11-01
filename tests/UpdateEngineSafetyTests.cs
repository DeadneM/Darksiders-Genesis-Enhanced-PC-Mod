using System;
using System.IO;
using System.Reflection;
using System.IO.Compression;
using System.Text;
using DarksidersGenesisUpdater;
internal static class UpdateEngineSafetyTests {
    private static int failures;
    private static void Check(bool ok,string label){if(!ok){failures++;Console.WriteLine("FAIL "+label);}}
    private static void PackageCases(string folder){
        string[][] layouts={
            new[]{"dxgi.dll","DarksidersGenesisMod.asi"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi","DarksidersGenesisMod.ini","README.md"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi","README.md"},
            new[]{"dxgi.dll"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi","unexpected.exe"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi","../escape.txt"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi","dxgi.dll"},
            new[]{"DXGI.dll","DarksidersGenesisMod.asi"},
            new[]{"dxgi.dll","DarksidersGenesisMod.asi"}
        };
        for(int i=0;i<layouts.Length;i++){
            string zipPath=Path.Combine(folder,"package-"+i+".zip"),stage=Path.Combine(folder,"stage-"+i);
            try{
                using(var zip=ZipFile.Open(zipPath,ZipArchiveMode.Create))foreach(string name in layouts[i]){
                    byte[] data;
                    if(name.EndsWith(".dll",StringComparison.OrdinalIgnoreCase)||name.EndsWith(".asi",StringComparison.Ordinal)){
                        data=new byte[512];data[0]=0x4D;data[1]=0x5A;BitConverter.GetBytes(128).CopyTo(data,0x3C);
                        data[128]=0x50;data[129]=0x45;data[132]=0x64;data[133]=0x86;
                        if(i==8&&name.EndsWith(".asi",StringComparison.Ordinal))data[132]=0x4C;
                    }else data=Encoding.UTF8.GetBytes("legacy-reference-only");
                    using(var output=zip.CreateEntry(name).Open())output.Write(data,0,data.Length);
                }
                bool rejected=false;try{UpdateEngine.Extract(zipPath,stage);}catch(InvalidDataException){rejected=true;}
                Check(rejected==(i>=2),"strict binary/legacy layout, PE and traversal case "+i);
                Check(!File.Exists(Path.Combine(folder,"escape.txt")),"ZIP entries cannot escape staging");
                if(i==0)Check(Directory.GetFiles(stage).Length==2&&!File.Exists(Path.Combine(stage,"DarksidersGenesisMod.ini")),"new ZIP extracts only binaries");
            }finally{
                if(File.Exists(zipPath))File.Delete(zipPath);
                if(Directory.Exists(stage)){foreach(string file in Directory.GetFiles(stage))File.Delete(file);Directory.Delete(stage);}
            }
        }
    }
    private static int Main(){
        string testRoot=Environment.GetEnvironmentVariable("DGE_UPDATER_TEST_ROOT");
        if(!string.IsNullOrEmpty(testRoot))typeof(UpdateEngine).GetField("DataRoot",BindingFlags.NonPublic|BindingFlags.Static)
            .SetValue(null,Path.Combine(testRoot,"updater-state"));
        string folder=Path.Combine(string.IsNullOrEmpty(testRoot)?Path.GetTempPath():testRoot,"DGE-State-Test-"+Guid.NewGuid().ToString("N"));
        Directory.CreateDirectory(folder);
        string state=UpdateEngine.StateFolder(folder);
        string asi=Path.Combine(folder,"DarksidersGenesisMod.asi"),dll=Path.Combine(folder,"dxgi.dll");
        string ini=Path.Combine(folder,"DarksidersGenesisMod.ini");
        string ledger=Path.Combine(state,"installed.txt"),fingerprint=Path.Combine(state,"installed-binaries.sha256");
        string backup=Path.Combine(folder,"verified-backup"),partial=Path.Combine(folder,"partial-backup"),binaryBackup=Path.Combine(folder,"binary-backup");
        string readme=Path.Combine(folder,"README.md");
        try {
            PackageCases(folder);
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
            File.WriteAllText(asi,"test-asi");File.WriteAllText(readme,"test-readme");
            Directory.CreateDirectory(partial);File.WriteAllText(Path.Combine(partial,"dxgi.dll"),"test-old-loader");
            bool rejected=false;
            try{UpdateEngine.RestoreVerifiedBackup(folder,partial);}catch(InvalidDataException){rejected=true;}
            Check(rejected&&File.ReadAllText(dll)=="test-dxgi"&&File.ReadAllText(asi)=="test-asi",
                "incomplete legacy/failed-copy backup is rejected before either binary changes");
            UpdateEngine.CompleteBackup(folder,backup,new[]{"dxgi.dll","DarksidersGenesisMod.asi","README.md"},token);
            Check(UpdateEngine.VerifyBackup(backup).Count==4,"complete manifest covers every replaced file and the old version token");
            File.WriteAllText(asi,"new-asi");File.WriteAllText(dll,"new-dxgi");File.WriteAllText(ini,"personal-new-binding");
            File.WriteAllText(readme,"personal-readme");
            File.WriteAllText(Path.Combine(backup,"DarksidersGenesisMod.asi"),"corrupt-saved-asi");
            rejected=false;
            try{UpdateEngine.RestoreVerifiedBackup(folder,backup);}catch(InvalidDataException){rejected=true;}
            Check(rejected&&File.ReadAllText(asi)=="new-asi"&&File.ReadAllText(dll)=="new-dxgi"&&UpdateEngine.ReadState(folder)=="",
                "corrupt backup cannot mix pairs or falsely bind an old version");
            File.WriteAllText(Path.Combine(backup,"DarksidersGenesisMod.asi"),"test-asi");
            UpdateEngine.RestoreVerifiedBackup(folder,backup);
            Check(File.ReadAllText(asi)=="test-asi"&&File.ReadAllText(dll)=="test-dxgi"&&UpdateEngine.ReadState(folder)==token,
                "complete verified rollback restores both binaries and the matching version");
            Check(File.ReadAllText(ini)=="personal-new-binding","rollback never touches an existing personal INI");
            Check(File.ReadAllText(readme)=="personal-readme","legacy rollback no longer replaces local documentation");
            UpdateEngine.CompleteBackup(folder,binaryBackup,UpdateEngine.Binaries,token);
            Check(UpdateEngine.VerifyBackup(binaryBackup).Count==3,"new backup covers exactly the binary pair and state");
            File.WriteAllText(asi,"new-again");File.WriteAllText(dll,"new-loader-again");File.Delete(ini);
            UpdateEngine.RestoreVerifiedBackup(folder,binaryBackup);
            Check(File.ReadAllText(asi)=="test-asi"&&File.ReadAllText(dll)=="test-dxgi"&&!File.Exists(ini),"binary-only rollback restores pair without creating an INI");
        }catch(Exception ex){Console.WriteLine(ex);failures++;}
        finally {
            // Remove only this test's known files and now-empty directories; no recursion.
            foreach(string dir in new[]{backup,partial,binaryBackup})if(Directory.Exists(dir)){
                foreach(string file in new[]{"dxgi.dll","DarksidersGenesisMod.asi","README.md","state.txt","backup.complete.json","backup.pending.json"}){
                    string path=Path.Combine(dir,file);if(File.Exists(path))File.Delete(path);
                }
                Directory.Delete(dir);
            }
            foreach(string file in new[]{asi,dll,ini,readme,ledger,fingerprint})if(File.Exists(file))File.Delete(file);
            if(Directory.Exists(folder))Directory.Delete(folder);
            if(Directory.Exists(state))Directory.Delete(state);
        }
        Console.WriteLine("Updater binary state: {0} failures",failures);
        return failures==0?0:1;
    }
}
