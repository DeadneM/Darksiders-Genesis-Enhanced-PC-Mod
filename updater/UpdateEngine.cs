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
        internal static readonly string[] Files={"dxgi.dll","DarksidersGenesisMod.asi",
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
        private static void Extract(string zipfile,string stage) {
            Directory.CreateDirectory(stage);
            var found=new HashSet<string>(StringComparer.OrdinalIgnoreCase);
            using(var zip=ZipFile.OpenRead(zipfile)){
                foreach(ZipArchiveEntry e in zip.Entries){
                    string name=e.FullName;
                    if(!Files.Any(x=>string.Equals(x,name,StringComparison.Ordinal)) ||
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
            if(found.Count!=4)throw new InvalidDataException("Expected precisely four root mod files");
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
                if(!Files.Contains(name))throw new InvalidDataException("Invalid backup filename");
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
            if(manifest==null||manifest.Count<4||manifest.Count>5||
               !new[]{"dxgi.dll","DarksidersGenesisMod.asi","README.md","state.txt"}.All(manifest.ContainsKey))
                throw new InvalidDataException("Incomplete backup manifest");
            foreach(var entry in manifest){
                if(entry.Key!="state.txt"&&!Files.Contains(entry.Key))throw new InvalidDataException("Unexpected backup filename");
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
                if(entry.Key=="state.txt"||entry.Key=="DarksidersGenesisMod.ini")continue;
                string dest=Path.Combine(folder,entry.Key);
                if(entry.Value=="MISSING"){
                    if(File.Exists(dest))File.Delete(dest);
                }else File.Copy(Path.Combine(backup,entry.Key),dest,true);
            }
            foreach(var entry in manifest){
                if(entry.Key=="state.txt"||entry.Key=="DarksidersGenesisMod.ini")continue;
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
                progress("GitHub checksum and four ZIP entries verified.");
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
                var changed=new List<string>{"dxgi.dll","DarksidersGenesisMod.asi","README.md"};
                if(!hasIni)changed.Add("DarksidersGenesisMod.ini");
                CompleteBackup(folder,backup,changed.ToArray(),previous);
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
                        (hasIni?". Existing INI preserved.":". Default INI created.");
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
