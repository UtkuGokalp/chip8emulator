using System.IO;
using Avalonia.Controls;
using System.ComponentModel;
using Avalonia.Interactivity;
using System.Collections.Generic;

/*
IMPORTANT NOTICE:
Many codes in this file assumes the 4 element arrays to be structured as the following:
    Index 0: Demos
    Index 1: Games
    Index 2: Hires
    Index 3: Programs
Not following this order might cause weird bugs that might be hard to debug. The arrays that
follow this order include the dictionaries array, the comboboxes array and the folder names array.
*/

namespace Chip8EmulatorLauncher
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        //Handle OS dependent const variables
#if WINDOWS
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
        private const string ROMS_PATH = "..\\..\\..\\..\\chip8emulator\\roms";
#elif LINUX
        private const string EXECUTABLE_PATH = "..\\..\\..\\..\\x64\\Debug\\chip8emulator.exe";
        private const string ROMS_PATH = "..\\..\\..\\..\\chip8emulator\\roms";
#endif
        public string? SelectedROMPath { get; set; }

        //This boolean is used to distinguish between combobox items being changed via the user
        //vs the program. If the program changes the SelectedIndex (which it needs to do in order
        //to set the old combobox item to NONE), OnSelectionChanged events will be raised over and
        //over again, causing bugs. So if this boolean is false, that means the change must have come
        //from the user. In that case, the code will immediately set this variable to true, indicating
        //that SelectedIndex or SelectedItem will be changed and it will NOT be made by the user, so the
        //other comboboxes shouldn't bother executing the event function (the events will still be raised).
        //Once the required execution is complete, this boolean will be set to false again so that once the
        //user chooses a new ROM, it will be registered correctly.
        private bool codeWillChangeSelectedItem = false;
        private Dictionary<string, string>[] romName2romPath =
        [
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
            new Dictionary<string, string>(),
        ];

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;

            //Load ROM paths and add ROM names to comboboxes
            ComboBox[] cbxs = GetAllComboBoxes();
            string[] folderNames = { "demos", "games", "hires", "programs" };
            for (int i = 0; i < cbxs.Length; i++)
            {
                cbxs[i].Items.Add("NONE");
                cbxs[i].SelectedIndex = 0;

                string directoryPath = Path.Combine(ROMS_PATH, folderNames[i]);
                if (Directory.Exists(directoryPath))
                {
                    string[] filePaths = Directory.GetFiles(directoryPath, "*.ch8", SearchOption.AllDirectories);
                    foreach (string filePath in filePaths)
                    {
                        string fileName = Path.GetFileName(filePath);
                        romName2romPath[i].Add(fileName, filePath);
                        cbxs[i].Items.Add(fileName);
                    }
                }
            }
            //Register events to comboboxes
            cbxDemos.SelectionChanged += cbxSelectionChanged;
            cbxGames.SelectionChanged += cbxSelectionChanged;
            cbxHires.SelectionChanged += cbxSelectionChanged;
            cbxPrograms.SelectionChanged += cbxSelectionChanged;
        }

        private void btnLaunchEmulator_Click(object? sender, RoutedEventArgs e)
        {

        }

        private void btnKillInstance_Click(object? sender, RoutedEventArgs e)
        {

        }

        private void btnOpenWorkingDir_Click(object? sender, RoutedEventArgs e)
        {
            
        }

        private void cbxSelectionChanged(object? sender, RoutedEventArgs e)
        {
            if (codeWillChangeSelectedItem || sender == null)
            {
                return;
            }
            codeWillChangeSelectedItem = true;
            ComboBox source = (ComboBox)sender;
            ComboBox[] cbxs = GetAllComboBoxes();
            for (int i = 0; i < cbxs.Length; i++)
            {
                ComboBox cbx = cbxs[i];
                if (cbx.Name == source.Name) //This is the combobox that we changed the selection in
                {
                    //Update the ROM path
                    bool success = romName2romPath[i].TryGetValue(cbx.SelectedItem.ToString(), out string romPath);
                    if (success)
                    {
                        SelectedROMPath = romPath;
                    }
                }
                else
                {
                    //Set the selected item to NONE
                    cbx.SelectedIndex = 0;
                }
            }
            codeWillChangeSelectedItem = false;
        }

        private ComboBox[] GetAllComboBoxes()
        {
            ComboBox[] cbxs = { cbxDemos, cbxGames, cbxHires, cbxPrograms };
            return cbxs;
        }
    }
}