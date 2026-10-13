using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using System.Threading.Tasks;
using Microsoft.Win32;

namespace DarksidersGenesisUpdater {
    internal sealed class UpdaterForm : Form {
        private TextBox location, history;
        private Label available, installed, status;
        private CheckBox beta, autoInstall;
        private Button checkButton, installButton, restoreButton, browseButton;
        private Timer closeTimer;
        private Package latest;
        private bool busy, queued, loadingPreferences;
        private readonly string settingsFile=Path.Combine(
            Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
            "DarksidersGenesisEnhancedUpdater","preferences.txt");

        internal UpdaterForm() {
            Text="Darksiders Genesis Enhanced | Mod Updater";
            StartPosition=FormStartPosition.CenterScreen;
            MinimumSize=new Size(710,475);
            Size=new Size(850,555);
            Font=new Font("Segoe UI",9.5f);
            BuildForm();
            loadingPreferences=true;
            ReadPreferences();
            loadingPreferences=false;
            closeTimer=new Timer();
            closeTimer.Interval=2000;
            closeTimer.Tick+=delegate {
                if(queued && !busy && !UpdateEngine.Running()){
                    queued=false;closeTimer.Stop();InstallLatest(true);
                }
            };
            Shown+=delegate {
                RefreshInstalled();
                if(Directory.Exists(location.Text) &&
                    File.Exists(Path.Combine(location.Text,UpdateEngine.GameExe)))CheckLatest(true);
            };
            FormClosing+=delegate {SavePreferences();};
        }
        private void BuildForm() {
            var table=new TableLayoutPanel {Dock=DockStyle.Fill,Padding=new Padding(17),
                ColumnCount=1,RowCount=9};
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,40));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,66));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,38));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,38));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,27));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,27));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,48));
            table.RowStyles.Add(new RowStyle(SizeType.Percent,100));
            table.RowStyles.Add(new RowStyle(SizeType.Absolute,26));
            Controls.Add(table);
            table.Controls.Add(new Label {Dock=DockStyle.Fill,
                Font=new Font("Segoe UI",14,FontStyle.Bold),
                Text="Darksiders Genesis Enhanced PC Mod Updater"},0,0);
            var pathRow=new TableLayoutPanel {Dock=DockStyle.Fill,ColumnCount=2,RowCount=2};
            pathRow.ColumnStyles.Add(new ColumnStyle(SizeType.Percent,100));
            pathRow.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute,125));
            pathRow.RowStyles.Add(new RowStyle(SizeType.Absolute,23));
            pathRow.RowStyles.Add(new RowStyle(SizeType.Absolute,32));
            pathRow.Controls.Add(new Label {Dock=DockStyle.Fill,
                Text="Folder containing DarksidersGenesis-Win64-Shipping.exe"},0,0);
            location=new TextBox {Dock=DockStyle.Fill};
            location.TextChanged+=delegate {latest=null;RefreshInstalled();if(!loadingPreferences)SavePreferences();};
            browseButton=new Button {Text="Browse...",Dock=DockStyle.Fill};
            browseButton.Click+=delegate {
                using(var d=new FolderBrowserDialog()){
                    d.Description="Select ProjectMayhem\\Binaries\\Win64";
                    if(Directory.Exists(location.Text))d.SelectedPath=location.Text;
                    if(d.ShowDialog(this)==DialogResult.OK)location.Text=d.SelectedPath;
                }
            };
            pathRow.Controls.Add(location,0,1);
            pathRow.Controls.Add(browseButton,1,1);
            table.Controls.Add(pathRow,0,1);

            var channels=new FlowLayoutPanel {Dock=DockStyle.Fill,WrapContents=false};
            beta=new CheckBox {Text="Beta (debug branch)",AutoSize=true,Margin=new Padding(0,7,25,0)};
            autoInstall=new CheckBox {Text="Automatically install newer versions after checking",
                AutoSize=true,Checked=true,Margin=new Padding(0,7,0,0)};
            beta.CheckedChanged+=delegate {
                if(loadingPreferences)return;
                SavePreferences();latest=null;
                available.Text="Remote version: not checked";
                AddLog(beta.Checked?"Channel: debug (beta)":"Channel: main (stable)");
                if(!busy)CheckLatest(false);
            };
            autoInstall.CheckedChanged+=delegate {if(!loadingPreferences)SavePreferences();};
            channels.Controls.Add(beta);
            channels.Controls.Add(autoInstall);
            table.Controls.Add(channels,0,2);
            table.Controls.Add(new Label {Dock=DockStyle.Fill,
                Text="Beta OFF downloads from main. Beta ON downloads from debug. Existing mod INI is never overwritten."},0,3);
            available=new Label {Dock=DockStyle.Fill,Text="Remote version: not checked",
                Font=new Font("Segoe UI",9.5f,FontStyle.Bold)};
            installed=new Label {Dock=DockStyle.Fill,Text="Installed version: unknown"};
            table.Controls.Add(available,0,4);
            table.Controls.Add(installed,0,5);

            var commands=new FlowLayoutPanel {Dock=DockStyle.Fill,WrapContents=false};
            checkButton=new Button {Text="Check Updates",Width=144,Height=32};
            installButton=new Button {Text="Install Latest",Width=144,Height=32};
            restoreButton=new Button {Text="Restore Previous",Width=144,Height=32};
            checkButton.Click+=delegate {CheckLatest(false);};
            installButton.Click+=delegate {InstallLatest(false);};
            restoreButton.Click+=delegate {Restore();};
            commands.Controls.Add(checkButton);
            commands.Controls.Add(installButton);
            commands.Controls.Add(restoreButton);
            table.Controls.Add(commands,0,6);

            history=new TextBox {Dock=DockStyle.Fill,ReadOnly=true,Multiline=true,
                ScrollBars=ScrollBars.Vertical,Font=new Font("Consolas",9)};
            table.Controls.Add(history,0,7);
            status=new Label {Dock=DockStyle.Fill,Text="Ready. Close the game before installation."};
            table.Controls.Add(status,0,8);
            SetBusy(false);
        }
        private void AddLog(string message) {
            if(IsDisposed)return;
            if(InvokeRequired){BeginInvoke(new Action<string>(AddLog),message);return;}
            history.AppendText("["+DateTime.Now.ToString("HH:mm:ss")+"] "+message+Environment.NewLine);
            status.Text=message;
        }
        private void SetBusy(bool value) {
            busy=value;
            checkButton.Enabled=!busy;
            installButton.Enabled=!busy && latest!=null;
            restoreButton.Enabled=!busy;
            browseButton.Enabled=!busy;
            beta.Enabled=!busy;
        }
        private static string Value(string[] lines,string name,string fallback) {
            string prefix=name+"=";
            string item=lines.FirstOrDefault(s=>s.StartsWith(prefix,StringComparison.OrdinalIgnoreCase));
            return item==null?fallback:item.Substring(prefix.Length);
        }
        private void ReadPreferences() {
            try {
                if(File.Exists(settingsFile)){
                    string[] lines=File.ReadAllLines(settingsFile);
                    location.Text=Value(lines,"GameDir","");
                    beta.Checked=Value(lines,"Beta","0")=="1";
                    autoInstall.Checked=Value(lines,"Auto","1")=="1";
                } else {
                    string found=DefaultFolder();
                    if(Directory.Exists(found))location.Text=found;
                }
            } catch(Exception ex){AddLog("Could not load preferences: "+ex.Message);}
        }
        private static string DefaultFolder() {
            try {
                using(var key=Registry.CurrentUser.OpenSubKey(@"Software\Valve\Steam")){
                    if(key!=null){
                        var root=key.GetValue("SteamPath");
                        if(root!=null){
                            string result=Path.Combine(Convert.ToString(root).Replace('/','\\'),
                                @"steamapps\common\DarksidersGenesis\ProjectMayhem\Binaries\Win64");
                            if(Directory.Exists(result))return result;
                        }
                    }
                }
            }catch{}
            return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86),
                @"Steam\steamapps\common\DarksidersGenesis\ProjectMayhem\Binaries\Win64");
        }
        private void SavePreferences() {
            try {
                if(location==null || beta==null || autoInstall==null)return;
                Directory.CreateDirectory(Path.GetDirectoryName(settingsFile));
                File.WriteAllLines(settingsFile,new string[]{
                    "GameDir="+location.Text,"Beta="+(beta.Checked?"1":"0"),
                    "Auto="+(autoInstall.Checked?"1":"0")
                });
            }catch{}
        }
        private void RefreshInstalled() {
            if(location==null || installed==null)return;
            if(!Directory.Exists(location.Text)){
                installed.Text="Installed version: unknown (select a game folder)";return;
            }
            string text=UpdateEngine.ReadState(location.Text);
            string[] bits=text.Split('|');
            int minor;
            installed.Text=bits.Length>=5 && int.TryParse(bits[2],out minor)
                ?"Installed by updater: V"+bits[1]+"."+minor.ToString("00")+" ("+bits[0]+")"
                :"Installed by updater: unknown (manual mod left intact)";
        }
        private bool ExistingManualMod() {
            return File.Exists(Path.Combine(location.Text,"dxgi.dll")) ||
                File.Exists(Path.Combine(location.Text,"DarksidersGenesisMod.asi"));
        }
        private static int Compare(Package p,string state) {
            string[] bits=state.Split('|');
            int major,minor;
            if(bits.Length<5 || !int.TryParse(bits[1],out major) ||
                !int.TryParse(bits[2],out minor))return 1;
            if(p.Major!=major)return p.Major.CompareTo(major);
            if(p.Minor!=minor)return p.Minor.CompareTo(minor);
            return string.Equals(p.GitBlobSha,bits[3],StringComparison.OrdinalIgnoreCase)?0:1;
        }
        private async void CheckLatest(bool startup) {
            if(busy)return;
            string channel=beta.Checked?"debug":"main";
            latest=null;
            SetBusy(true);
            AddLog("Checking GitHub "+channel+"...");
            try {
                Package p=await Task.Run(()=>UpdateEngine.Latest(channel));
                if(IsDisposed)return;
                latest=p;
                available.Text="Latest "+channel+": "+p.Version+" | "+p.FileName;
                AddLog("Latest is "+p.Version+" ("+p.Size+" bytes)");
            } catch(Exception ex) {AddLog("Check failed: "+ex.Message);}
            finally {SetBusy(false);}
            if(latest==null || !autoInstall.Checked || !Directory.Exists(location.Text))return;
            string state=UpdateEngine.ReadState(location.Text);
            if(string.IsNullOrEmpty(state) && ExistingManualMod()){
                AddLog("First run: existing manual installation detected. Use Install Latest once.");
                return;
            }
            if(Compare(latest,state)>0)InstallLatest(true);
            else AddLog("No newer mod for this channel.");
        }
        private async void InstallLatest(bool automatic) {
            if(busy || latest==null){
                if(!automatic)AddLog("Check updates before installing.");return;
            }
            string folder=location.Text.Trim();
            string state=Directory.Exists(folder)?UpdateEngine.ReadState(folder):"";
            int cmp=Compare(latest,state);
            if(cmp<0 && automatic)return;
            if(cmp<0 && MessageBox.Show(this,
                "Selected "+latest.Version+" is OLDER than installed version.\nDowngrade anyway?",
                "Confirm Downgrade",MessageBoxButtons.YesNo,MessageBoxIcon.Warning)!=DialogResult.Yes)return;
            if(!automatic && MessageBox.Show(this,
                "Install "+latest.Version+" from "+latest.Channel+"?\nYour personal INI will be preserved.",
                "Confirm Installation",MessageBoxButtons.YesNo,MessageBoxIcon.Question)!=DialogResult.Yes)return;
            if(UpdateEngine.Running()){
                queued=true;closeTimer.Start();
                AddLog("Game is running: installation queued until it closes (keep updater open).");
                return;
            }
            Package selected=latest;
            SetBusy(true);
            try {
                string result=await Task.Run(()=>UpdateEngine.Install(selected,folder,AddLog));
                AddLog(result);
                RefreshInstalled();
            }catch(Exception ex){AddLog("INSTALLATION FAILED: "+ex.Message);}
            finally {SetBusy(false);}
        }
        private async void Restore() {
            if(busy)return;
            if(MessageBox.Show(this,"Restore the previous binaries?\nThe game must be closed.",
                "Confirm Rollback",MessageBoxButtons.YesNo,MessageBoxIcon.Question)!=DialogResult.Yes)return;
            SetBusy(true);
            try {
                string folder=location.Text.Trim();
                string result=await Task.Run(()=>UpdateEngine.Rollback(folder));
                AddLog(result);RefreshInstalled();
            }catch(Exception ex){AddLog("Rollback failed: "+ex.Message);}
            finally {SetBusy(false);}
        }
    }
}
