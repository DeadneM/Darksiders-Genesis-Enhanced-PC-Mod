using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Web.Script.Serialization;
using System.Runtime.InteropServices;

namespace DarksidersGenesisUpdater {
    internal sealed class Package {
        internal int Major, Minor;
        internal string FileName, DownloadUrl, GitBlobSha, Channel;
        internal long Size;
        internal string Version { get { return "V" + Major + "." + Minor.ToString("00"); } }
        internal string Token { get { return Channel+"|"+Major+"|"+Minor+"|"+GitBlobSha+"|"+FileName; } }
    }

    internal static class UpdateEngine {
        private const string Repo="DeadneM/Darksiders-Genesis-Enhanced-PC-Mod";
        internal const string GameExe="DarksidersGenesis-Win64-Shipping.exe";
        private const string GameSha256="9f4702024df5eea1d51df7745b0ad1ea95b97009982f73ddc1218c53dff33d54";
        private const long MaxZip=32L*1024*1024;
        internal static readonly string[] Binaries={"dxgi.dll","DarksidersGenesisMod.asi"};
        private static readonly string[] LegacyFiles={"dxgi.dll","DarksidersGenesisMod.asi",
            "DarksidersGenesisMod.ini","README.md"};
        private static readonly string DataRoot=Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
            "DarksidersGenesisEnhancedUpdater");

        internal static string StateFolder(string folder) {
            using(var hash=SHA256.Create()){
                byte[] digest=hash.ComputeHash(Encoding.UTF8.GetBytes(
                    Path.GetFullPath(folder).TrimEnd('\\').ToLowerInvariant()));
                return Path.Combine(DataRoot,BitConverter.ToString(digest,0,12).Replace("-",""));
            }
        }
        internal static string BackupRoot(string folder) {return Path.Combine(StateFolder(folder),"backups");}

        [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]
        private static extern bool WritePrivateProfileString(string section,string key,string value,string path);
        [DllImport("kernel32.dll",CharSet=CharSet.Unicode)]
        private static extern uint GetPrivateProfileInt(string section,string key,int fallback,string path);

        internal static int ApplyGraphicsAdapterAfterExit(string folder) {
            ValidateFolder(folder);
            string engine=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                @"THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\Engine.ini");
            return ApplyGraphicsAdapterFiles(Path.Combine(folder,"DarksidersGenesisMod.ini"),engine,Running);
        }
        // Same Win32 INI writer as the former ASI function, now called only
        // after UE has stopped. Latest saved choice wins; no startup write.
        internal static int ApplyGraphicsAdapterFiles(string modIni,string engineIni,Func<bool> running) {
            if(running())throw new InvalidOperationException("Game is running; Engine.ini left untouched");
            if(!File.Exists(modIni)||!File.Exists(engineIni))throw new FileNotFoundException("Existing mod INI and Engine.ini required");
            int adapter=unchecked((int)GetPrivateProfileInt("System","GraphicsAdapter",-1,modIni));
            if(adapter<0||adapter>4)throw new InvalidDataException("GraphicsAdapter must be 0..4");
            byte[] original=File.ReadAllBytes(engineIni);
            if(running())throw new InvalidOperationException("Game restarted; Engine.ini left untouched");
            File.Copy(engineIni,engineIni+".DGE-GPU.bak",true);
            try {
                if(running())throw new InvalidOperationException("Game restarted before GPU write; Engine.ini left untouched");
                if(!WritePrivateProfileString("SystemSettings","r.GraphicsAdapter",adapter.ToString(),engineIni))
                    throw new IOException("Engine.ini write failed",new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error()));
                WritePrivateProfileString(null,null,null,engineIni);
                if(unchecked((int)GetPrivateProfileInt("SystemSettings","r.GraphicsAdapter",-1,engineIni))!=adapter)
                    throw new IOException("Engine.ini GPU readback mismatch");
                return adapter;
            }catch {
                if(!running()){WritePrivateProfileString(null,null,null,engineIni);File.WriteAllBytes(engineIni,original);}
                throw;
            }
        }
        // V1.177: only these verified native CVars are writable. No runtime memory calls.
        private static readonly string[] RenderKeys={"MaxAnisotropy","ScreenPercentage","ShadowQuality","ShadowMaxResolution","ShadowDistanceScale","ViewDistanceScale","SSRQuality"};
        private static readonly string[] RenderCVars={"r.MaxAnisotropy","r.ScreenPercentage","r.ShadowQuality","r.Shadow.MaxResolution","r.Shadow.DistanceScale","r.ViewDistanceScale","r.SSR.Quality","r.AllowHDR","r.HDR.EnableHDROutput","r.HDR.Display.OutputDevice","r.HDR.Display.ColorGamut","r.HDR.UI.CompositeMode","r.HDR.UI.Level","r.DefaultBackBufferPixelFormat"};
        private static readonly double[] RenderMin={1,25,0,64,0,0,0,0,0,0,0,0,0,0},RenderMax={16,200,5,8192,5,5,4,1,1,6,4,1,4,4};
        private static readonly bool[] RenderInt={true,false,true,true,false,false,true,true,true,true,true,true,false,true};
        [DllImport("kernel32.dll",CharSet=CharSet.Unicode)]
        private static extern uint GetPrivateProfileString(string section,string key,string fallback,StringBuilder value,uint size,string path);
        private static string RenderRead(string section,string key,string path) {
            const string missing="__DGE_ABSENT_177__";var value=new StringBuilder(4096);
            uint n=GetPrivateProfileString(section,key,missing,value,4096,path);
            if(n>=4095)throw new InvalidDataException("INI value too long: "+key);
            string text=value.ToString();return text==missing?null:text;
        }
        private static void RenderWrite(string section,string key,string value,string path) {
            if(!WritePrivateProfileString(section,key,value,path))throw new IOException("Render INI write failed: "+key);
        }
        private static void RenderStateWrite(string path,byte[] bytes) {
            string temp=path+".tmp";File.WriteAllBytes(temp,bytes);
            if(File.Exists(path))File.Replace(temp,path,null);else File.Move(temp,path);
        }
        // Identical experimental engine requests on Intel, AMD and NVIDIA.
        // Vendor-specific paths in the game are not a mod-side restriction.
        private static double[] HdrRequestValues(string modIni) {
            string rawMode=RenderRead("HDR","OutputMode",modIni),rawBrightness=RenderRead("HDR","UIBrightness",modIni);
            int mode=-1;double brightness=-1;
            if(rawMode!=null&&(!int.TryParse(rawMode,System.Globalization.NumberStyles.Integer,System.Globalization.CultureInfo.InvariantCulture,out mode)||mode < -1||mode > 4))
                throw new InvalidDataException("HDR OutputMode must be Native (-1), SDR (0), or a profile 1..4");
            if(rawBrightness!=null&&(!double.TryParse(rawBrightness,System.Globalization.NumberStyles.Float,System.Globalization.CultureInfo.InvariantCulture,out brightness)||double.IsNaN(brightness)||double.IsInfinity(brightness)||brightness>4))
                throw new InvalidDataException("Invalid HDR UI brightness");
            int buffer=0;string rawBuffer=RenderRead("HDR","BackBuffer",modIni);
            if(rawBuffer!=null&&(!int.TryParse(rawBuffer,out buffer)||buffer<0||buffer>3))throw new InvalidDataException("Invalid HDR back buffer choice");
            int composition=-1;string rawComposition=RenderRead("HDR","UIComposition",modIni);
            if(rawComposition!=null&&(!int.TryParse(rawComposition,out composition)||composition < -1||composition>1))
                throw new InvalidDataException("Invalid HDR interface composition");
            bool pq=mode==1||mode==2;
            string signaling=RenderRead("HDR","DXGISignaling",modIni);
            int toggle;
            // Keep the startup encoding SDR until the ASI has a working PQ swap chain.
            bool managedPq=pq&&(signaling==null||(int.TryParse(signaling,out toggle)&&toggle!=0));
            var values=new double[]{-1,-1,-1,-1,-1,-1,-1};
            // Preserve a conflicting choice without applying it to the renderer.
            if(mode>0&&buffer>=2&&buffer!=(pq?2:3))return values;
            if(mode>=0){
                values[0]=values[1]=mode>0?1:0;
                values[2]=mode>0&&!managedPq?mode+2:0;
                values[3]=pq&&!managedPq?2:0;
            }
            if(mode>0&&buffer!=1)values[6]=pq?4:3;
            if(mode>0)values[4]=composition>=0?composition:brightness>=0?1:-1;
            return values;
        }
        internal static int ApplyRenderQualityAfterExit(string folder) {
            ValidateFolder(folder);
            string engine=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                @"THQ Nordic\Darksiders Genesis\Saved\Config\WindowsNoEditor\Engine.ini");
            return ApplyRenderQualityFiles(Path.Combine(folder,"DarksidersGenesisMod.ini"),engine,Running);
        }
        internal static int ApplyRenderQualityFiles(string modIni,string engineIni,Func<bool> running) {
            if(running())throw new InvalidOperationException("Game is running; render settings untouched");
            if(!File.Exists(modIni)||!File.Exists(engineIni))throw new FileNotFoundException("Existing INIs required");
            string ledger=engineIni+".DGE-render-owned.json";
            var serializer=new JavaScriptSerializer();
            byte[] oldLedger=File.Exists(ledger)?File.ReadAllBytes(ledger):null;
            var owned=oldLedger==null?new Dictionary<string,string[]>():serializer.Deserialize<Dictionary<string,string[]>>(Encoding.UTF8.GetString(oldLedger));
            if(owned==null||owned.Any(x=>!RenderCVars.Contains(x.Key)||x.Value==null||x.Value.Length!=2||x.Value[1]==null))
                throw new InvalidDataException("Invalid render ownership ledger; no changes");
            var writes=new Dictionary<string,string>();
            double[] hdr=HdrRequestValues(modIni);
            for(int i=0;i<RenderCVars.Length;i++) {
                double desired=-1;
                if(i<RenderKeys.Length){
                    string raw=RenderRead("RenderQuality",RenderKeys[i],modIni);
                    if(raw!=null&&(!double.TryParse(raw,System.Globalization.NumberStyles.Float,System.Globalization.CultureInfo.InvariantCulture,out desired)||double.IsNaN(desired)||double.IsInfinity(desired)))
                        throw new InvalidDataException("Invalid rendering preference: "+RenderKeys[i]);
                }else desired=hdr[i-RenderKeys.Length];
                if(desired>=0&&(desired<RenderMin[i]||desired>RenderMax[i]||(RenderInt[i]&&desired!=Math.Floor(desired))))
                    throw new InvalidDataException("Rendering preference outside supported range: "+RenderCVars[i]);
                string key=RenderCVars[i],current=RenderRead("SystemSettings",key,engineIni);string[] prior;owned.TryGetValue(key,out prior);
                // Live overrides use the ASI. Release values owned by older helpers.
                bool live=key=="r.ScreenPercentage"||key=="r.SSR.Quality"||key=="r.HDR.UI.Level";
                if(desired<0||live) {
                    // External edits win when releasing an owned value.
                    if(prior!=null&&current==prior[1]&&current!=prior[0])writes[key]=prior[0];
                    owned.Remove(key);continue;
                }
                string value=desired.ToString("0.###",System.Globalization.CultureInfo.InvariantCulture);
                string original=prior!=null&&current==prior[1]?prior[0]:current;
                if(current!=value)writes[key]=value;
                // Already equal and never ours: no ownership to claim or restore.
                if(prior!=null||current!=value)owned[key]=new[]{original,value};
            }
            var journal=oldLedger==null?new Dictionary<string,string[]>():serializer.Deserialize<Dictionary<string,string[]>>(Encoding.UTF8.GetString(oldLedger));
            foreach(var item in owned)journal[item.Key]=item.Value;
            byte[] nextLedger=Encoding.UTF8.GetBytes(serializer.Serialize(owned));
            byte[] journalBytes=Encoding.UTF8.GetBytes(serializer.Serialize(journal));
            if(writes.Count==0&&((oldLedger==null&&owned.Count==0)||(oldLedger!=null&&oldLedger.SequenceEqual(nextLedger))))return 0;
            byte[] originalEngine=File.ReadAllBytes(engineIni);
            if(running())throw new InvalidOperationException("Game restarted; render settings untouched");
            if(writes.Count>0)File.Copy(engineIni,engineIni+".DGE-render.bak",true);
            try {
                if(running())throw new InvalidOperationException("Game restarted before render write");
                // Journal ownership first: interruption cannot leave an untracked successful write.
                RenderStateWrite(ledger,journalBytes);
                foreach(var write in writes) {
                    if(running())throw new InvalidOperationException("Game restarted during render write");
                    RenderWrite("SystemSettings",write.Key,write.Value,engineIni);
                }
                WritePrivateProfileString(null,null,null,engineIni);
                foreach(var write in writes)if(RenderRead("SystemSettings",write.Key,engineIni)!=write.Value)
                    throw new IOException("Render readback mismatch: "+write.Key);
                if(owned.Count==0)File.Delete(ledger);else RenderStateWrite(ledger,nextLedger);
                return writes.Count;
            }catch {
                if(!running()) {
                    WritePrivateProfileString(null,null,null,engineIni);File.WriteAllBytes(engineIni,originalEngine);
                    if(oldLedger==null){if(File.Exists(ledger))File.Delete(ledger);}else RenderStateWrite(ledger,oldLedger);
                }
                throw;
            }
        }
        private static string BinaryFingerprint(string folder) {
            var result=new StringBuilder();
            foreach(string name in new[]{"DarksidersGenesisMod.asi","dxgi.dll"}){
                string path=Path.Combine(folder,name);
                if(!File.Exists(path))return "";
                using(var stream=File.OpenRead(path))
                using(var hash=SHA256.Create())
                    result.Append(name).Append(':').Append(
                        BitConverter.ToString(hash.ComputeHash(stream)).Replace("-","")).Append('\n');
            }
            return result.ToString().Trim();
        }
        internal static string ReadState(string folder) {
            // A manual ASI/DXGI install invalidates the updater's old version ledger.
            // Treat unbound legacy ledgers as manual installs, requiring confirmation.
            try {
                string dir=StateFolder(folder),file=Path.Combine(dir,"installed.txt");
                string fingerprint=Path.Combine(dir,"installed-binaries.sha256");
                if(!File.Exists(file)||!File.Exists(fingerprint))return "";
                string actual=BinaryFingerprint(folder);
                return actual.Length>0&&actual==File.ReadAllText(fingerprint).Trim()
                    ?File.ReadAllText(file).Trim():"";
            }catch(IOException){return "";}
             catch(UnauthorizedAccessException){return "";}
        }
        private static void SaveState(string folder,string value) {
            string dir=StateFolder(folder);
            Directory.CreateDirectory(dir);
            string actual=BinaryFingerprint(folder);
            if(actual.Length==0&&!string.IsNullOrEmpty(value))throw new IOException("Installed binaries missing; version state not saved");
            File.WriteAllText(Path.Combine(dir,"installed.txt"),value??"",Encoding.UTF8);
            File.WriteAllText(Path.Combine(dir,"installed-binaries.sha256"),actual,Encoding.UTF8);
        }
        private static WebClient Client() {
            ServicePointManager.SecurityProtocol |= SecurityProtocolType.Tls12;
            var c=new WebClient();
            c.Headers[HttpRequestHeader.UserAgent]="DarksidersGenesisUpdater/1.0";
            c.Headers[HttpRequestHeader.Accept]="application/vnd.github+json";
            return c;
        }
        internal static Package Latest(string channel) {
            if(channel!="main" && channel!="debug")throw new ArgumentException("Invalid update channel");
            string json;
            using(var c=Client())
                json=c.DownloadString("https://api.github.com/repos/"+Repo+"/contents/downloads?ref="+channel);
            var entries=new JavaScriptSerializer().DeserializeObject(json) as object[];
            if(entries==null)throw new InvalidDataException("GitHub did not return a downloadable ZIP listing");
            var rx=new Regex(@"^DarksidersGenesis_V(\d+)\.(\d+)(?:_.*)?\.zip$",
                RegexOptions.IgnoreCase|RegexOptions.CultureInvariant);
            Package best=null;
            foreach(object item in entries) {
                var obj=item as Dictionary<string,object>;
                if(obj==null || !obj.ContainsKey("name") || !obj.ContainsKey("download_url"))continue;
                string name=Convert.ToString(obj["name"]);
                Match m=rx.Match(name);
                int major,minor;
                if(!m.Success || !int.TryParse(m.Groups[1].Value,out major) ||
                    !int.TryParse(m.Groups[2].Value,out minor))continue;
                string url=Convert.ToString(obj["download_url"]);
                string allowed="https://raw.githubusercontent.com/"+Repo+"/"+channel+"/downloads/";
                if(!url.StartsWith(allowed,StringComparison.Ordinal))continue;
                long size=obj.ContainsKey("size")?Convert.ToInt64(obj["size"]):0;
                string sha=obj.ContainsKey("sha")?Convert.ToString(obj["sha"]):"";
                if(size<1000 || size>MaxZip ||
                    !Regex.IsMatch(sha,"^[a-f0-9]{40}$",RegexOptions.IgnoreCase))continue;
                var p=new Package{Major=major,Minor=minor,FileName=name,DownloadUrl=url,
                    GitBlobSha=sha,Size=size,Channel=channel};
                if(best==null || p.Major>best.Major ||
                    (p.Major==best.Major && p.Minor>best.Minor) ||
                    (p.Major==best.Major && p.Minor==best.Minor &&
                     string.CompareOrdinal(p.FileName,best.FileName)>0))best=p;
            }
            if(best==null)throw new InvalidDataException("No versioned mod ZIP found on "+channel);
            return best;
        }
        internal static bool Running() {
            Process[] list=Process.GetProcessesByName("DarksidersGenesis-Win64-Shipping");
            foreach(var p in list)p.Dispose();
            return list.Length>0;
        }
        internal static void ValidateFolder(string folder) {
            if(string.IsNullOrWhiteSpace(folder)||!Directory.Exists(folder))
                throw new DirectoryNotFoundException("Select the ProjectMayhem\\Binaries\\Win64 folder");
            string exe=Path.Combine(folder,GameExe);
            if(!File.Exists(exe))throw new FileNotFoundException("Game EXE not found",exe);
            if(Running())throw new InvalidOperationException("Game is running. Close it before installing.");
            using(var stream=File.OpenRead(exe))
            using(var hash=SHA256.Create()){
                if(stream.Length!=62113280L)
                    throw new InvalidDataException("Unsupported game executable size; nothing installed");
                string sha=BitConverter.ToString(hash.ComputeHash(stream)).Replace("-","").ToLowerInvariant();
                if(sha!=GameSha256)
                    throw new InvalidDataException("Unsupported game executable SHA-256; nothing installed");
            }
        }
        private static void Download(Package p,string target) {
            string allowed="https://raw.githubusercontent.com/"+Repo+"/"+p.Channel+"/downloads/";
            if(!p.DownloadUrl.StartsWith(allowed,StringComparison.Ordinal))
                throw new InvalidDataException("Unexpected download host");
            using(var c=Client())
            using(var stream=c.OpenRead(p.DownloadUrl))
            using(var output=new FileStream(target,FileMode.CreateNew,FileAccess.Write,FileShare.None)){
                byte[] buffer=new byte[32768];
                long count=0;
                int n;
                while((n=stream.Read(buffer,0,buffer.Length))>0){
                    count+=n;
                    if(count>MaxZip)throw new InvalidDataException("ZIP exceeds maximum allowed size");
                    output.Write(buffer,0,n);
                }
                if(count!=p.Size)throw new InvalidDataException("ZIP length changed after GitHub listing");
            }
            byte[] payload=File.ReadAllBytes(target);
            byte[] prefix=Encoding.ASCII.GetBytes("blob "+payload.Length+"\0");
            using(var sha=SHA1.Create()){
                sha.TransformBlock(prefix,0,prefix.Length,prefix,0);
                sha.TransformFinalBlock(payload,0,payload.Length);
                string actual=BitConverter.ToString(sha.Hash).Replace("-","").ToLowerInvariant();
                if(!string.Equals(actual,p.GitBlobSha,StringComparison.OrdinalIgnoreCase))
                    throw new InvalidDataException("GitHub git-blob checksum mismatch; no files installed");
            }
        }
        private static void CheckPe(string path) {
            using(var file=File.OpenRead(path))
            using(var reader=new BinaryReader(file)){
                if(file.Length<256 || reader.ReadUInt16()!=0x5A4D)
                    throw new InvalidDataException(Path.GetFileName(path)+" missing MZ header");
                file.Position=0x3C;
                int pe=reader.ReadInt32();
                if(pe<64||pe>file.Length-24)throw new InvalidDataException("Bad PE offset");
                file.Position=pe;
                if(reader.ReadUInt32()!=0x00004550 || reader.ReadUInt16()!=0x8664)
                    throw new InvalidDataException("Not a Windows AMD64 module");
            }
        }
        internal static void Extract(string zipfile,string stage) {
            Directory.CreateDirectory(stage);
            var found=new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            using(var zip=ZipFile.OpenRead(zipfile)){
                foreach(ZipArchiveEntry e in zip.Entries){
                    string name=e.FullName;
                    if(!LegacyFiles.Any(x=>string.Equals(x,name,StringComparison.Ordinal)) ||
                        !found.Add(name))
                        throw new InvalidDataException("Unexpected, nested or duplicate ZIP entry: "+name);
                    long max=name=="README.md"?3L*1024*1024:
                        name=="DarksidersGenesisMod.ini"?1024*1024:16L*1024*1024;
                    if(e.Length<1 || e.Length>max)throw new InvalidDataException("Invalid entry size");
                    using(var input=e.Open())
                    using(var dest=new FileStream(Path.Combine(stage,name),FileMode.CreateNew,
                        FileAccess.Write,FileShare.None))input.CopyTo(dest);
                }
            }
            if(!found.SetEquals(Binaries)&&!found.SetEquals(LegacyFiles))
                throw new InvalidDataException("Expected two root binaries or the exact legacy four-file layout");
            CheckPe(Path.Combine(stage,"dxgi.dll"));
            CheckPe(Path.Combine(stage,"DarksidersGenesisMod.asi"));
        }
        private static bool EqualBinary(string x,string y) {
            if(!File.Exists(x)||!File.Exists(y))return false;
            using(var sha=SHA256.Create())
            using(var a=File.OpenRead(x))
            using(var b=File.OpenRead(y)){
                return sha.ComputeHash(a).SequenceEqual(sha.ComputeHash(b));
            }
        }
        private static string FileDigest(string file) {
            using(var stream=File.OpenRead(file))
            using(var sha=SHA256.Create())
                return BitConverter.ToString(sha.ComputeHash(stream)).Replace("-","");
        }
        internal static void CompleteBackup(string folder,string backup,string[] changed,string previous) {
            Directory.CreateDirectory(backup);
            File.WriteAllText(Path.Combine(backup,"state.txt"),previous??"",Encoding.UTF8);
            var manifest=new Dictionary<string,string>(StringComparer.Ordinal);
            foreach(string name in changed){
                if(!LegacyFiles.Contains(name))throw new InvalidDataException("Invalid backup filename");
                string source=Path.Combine(folder,name),saved=Path.Combine(backup,name);
                if(File.Exists(source)){
                    File.Copy(source,saved);
                    if(!EqualBinary(source,saved))throw new IOException("Backup verification failed: "+name);
                    manifest.Add(name,FileDigest(saved));
                }else{
                    File.WriteAllText(saved+".missing","");manifest.Add(name,"MISSING");
                }
            }
            manifest.Add("state.txt",FileDigest(Path.Combine(backup,"state.txt")));
            // Publish completion LAST. A failed copy never becomes a candidate
            // for rollback and the installation has not started at this point.
            string temporary=Path.Combine(backup,"backup.pending.json");
            File.WriteAllText(temporary,new JavaScriptSerializer().Serialize(manifest),Encoding.UTF8);
            File.Move(temporary,Path.Combine(backup,"backup.complete.json"));
            VerifyBackup(backup);
        }
        internal static Dictionary<string,string> VerifyBackup(string backup) {
            string complete=Path.Combine(backup,"backup.complete.json");
            if(!File.Exists(complete))throw new InvalidDataException("Backup is incomplete or predates verified backups");
            var manifest=new JavaScriptSerializer().Deserialize<Dictionary<string,string>>(File.ReadAllText(complete));
            if(manifest==null||manifest.Count<3||manifest.Count>5||
               !Binaries.Concat(new[]{"state.txt"}).All(manifest.ContainsKey)||
               (manifest.Count>3&&!manifest.ContainsKey("README.md")))
                throw new InvalidDataException("Incomplete backup manifest");
            foreach(var entry in manifest){
                if(entry.Key!="state.txt"&&!LegacyFiles.Contains(entry.Key))throw new InvalidDataException("Unexpected backup filename");
                string saved=Path.Combine(backup,entry.Key);
                if(entry.Value=="MISSING"&&entry.Key!="state.txt"){
                    if(File.Exists(saved)||!File.Exists(saved+".missing"))throw new InvalidDataException("Invalid missing-file record");
                }else if(!Regex.IsMatch(entry.Value??"","^[A-F0-9]{64}$")||!File.Exists(saved)||
                    FileDigest(saved)!=entry.Value||File.Exists(saved+".missing"))
                    throw new InvalidDataException("Backup integrity check failed: "+entry.Key);
            }
            return manifest;
        }
        internal static void RestoreVerifiedBackup(string folder,string backup) {
            var manifest=VerifyBackup(backup); // check ALL files before any write
            foreach(var entry in manifest){
                if(!Binaries.Contains(entry.Key))continue;
                string dest=Path.Combine(folder,entry.Key);
                if(entry.Value=="MISSING"){
                    if(File.Exists(dest))File.Delete(dest);
                }else File.Copy(Path.Combine(backup,entry.Key),dest,true);
            }
            foreach(var entry in manifest){
                if(!Binaries.Contains(entry.Key))continue;
                string dest=Path.Combine(folder,entry.Key);
                if(entry.Value=="MISSING"?File.Exists(dest):!File.Exists(dest)||FileDigest(dest)!=entry.Value)
                    throw new IOException("Restored file verification failed: "+entry.Key);
            }
            // Never bind an old version token to a partially restored pair.
            SaveState(folder,File.ReadAllText(Path.Combine(backup,"state.txt")));
        }
        internal static string Install(Package p,string folder,Action<string> progress) {
            ValidateFolder(folder);
            string temp=Path.Combine(Path.GetTempPath(),"DGEUpdater-"+Guid.NewGuid().ToString("N"));
            Directory.CreateDirectory(temp);
            try {
                string archive=Path.Combine(temp,"mod.zip");
                string stage=Path.Combine(temp,"extracted");
                progress("Downloading "+p.FileName+"...");
                Download(p,archive);
                Extract(archive,stage);
                ValidateFolder(folder);
                progress("GitHub checksum, ZIP layout and both x64 binaries verified.");
                bool hasIni=File.Exists(Path.Combine(folder,"DarksidersGenesisMod.ini"));
                if(EqualBinary(Path.Combine(stage,"dxgi.dll"),Path.Combine(folder,"dxgi.dll")) &&
                    EqualBinary(Path.Combine(stage,"DarksidersGenesisMod.asi"),Path.Combine(folder,"DarksidersGenesisMod.asi"))){
                    SaveState(folder,p.Token);
                    return "Installed files already match "+p.Version+". INI left untouched.";
                }
                string root=BackupRoot(folder);
                Directory.CreateDirectory(root);
                string backup=Path.Combine(root,DateTime.Now.ToString("yyyyMMdd-HHmmss-fff")+"-"+
                    Guid.NewGuid().ToString("N").Substring(0,6));
                string previous=ReadState(folder);
                var changed=Binaries;
                CompleteBackup(folder,backup,changed,previous);
                progress("Previous files backed up under LocalAppData.");
                try {
                    ValidateFolder(folder);
                    foreach(string name in changed){
                        string dest=Path.Combine(folder,name);
                        string incoming=dest+".dgupdater-new";
                        if(File.Exists(incoming))throw new IOException("Temporary install file already exists");
                        File.Copy(Path.Combine(stage,name),incoming);
                        try {
                            if(File.Exists(dest))File.Replace(incoming,dest,null);
                            else File.Move(incoming,dest);
                        } finally {if(File.Exists(incoming))File.Delete(incoming);}
                    }
                    SaveState(folder,p.Token);
                    return "Installed "+p.Version+" from "+p.Channel+
                        (hasIni?". Existing INI preserved.":". The ASI will create its INI on the next game launch.");
                } catch {
                    try {RestoreVerifiedBackup(folder,backup);}
                    catch(Exception ex){throw new IOException("Install failed; restore incomplete. Verified backup retained at "+backup,ex);}
                    throw;
                }
            } finally {try{Directory.Delete(temp,true);}catch{}}
        }
        internal static string Rollback(string folder) {
            ValidateFolder(folder);
            string root=BackupRoot(folder);
            if(!Directory.Exists(root))throw new InvalidOperationException("No previous version saved");
            string[] backups=Directory.GetDirectories(root).Where(x=>
                File.Exists(Path.Combine(x,"backup.complete.json"))&&
                !File.Exists(Path.Combine(x,"rollback.restored"))).ToArray();
            Array.Sort(backups,StringComparer.Ordinal);
            if(backups.Length==0)throw new InvalidOperationException("No previous version saved");
            string last=backups[backups.Length-1];
            // Keep the backup on disk, including after a failed restoration.
            // A personal INI absent from the manifest cannot be overwritten.
            VerifyBackup(last);
            ValidateFolder(folder);
            RestoreVerifiedBackup(folder,last);
            File.WriteAllText(Path.Combine(last,"rollback.restored"),DateTime.UtcNow.ToString("O"));
            return "Previous binaries restored; personal INI retained.";
        }
    }
}
