namespace Chip8EmulatorLauncher
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            btnLaunch = new Button();
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            label4 = new Label();
            cbxDemos = new ComboBox();
            cbxGames = new ComboBox();
            cbxHires = new ComboBox();
            cbxPrograms = new ComboBox();
            btnKillInstance = new Button();
            btnOpenWorkingDirectory = new Button();
            SuspendLayout();
            // 
            // btnLaunch
            // 
            btnLaunch.Location = new Point(31, 150);
            btnLaunch.Name = "btnLaunch";
            btnLaunch.Size = new Size(626, 53);
            btnLaunch.TabIndex = 1;
            btnLaunch.Text = "Launch Emulator";
            btnLaunch.UseVisualStyleBackColor = true;
            btnLaunch.Click += btnLaunch_Click;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(31, 20);
            label1.Name = "label1";
            label1.Size = new Size(44, 15);
            label1.TabIndex = 2;
            label1.Text = "Demos";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(357, 20);
            label2.Name = "label2";
            label2.Size = new Size(43, 15);
            label2.TabIndex = 2;
            label2.Text = "Games";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(58, 86);
            label3.Name = "label3";
            label3.Size = new Size(34, 15);
            label3.TabIndex = 2;
            label3.Text = "Hires";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(347, 86);
            label4.Name = "label4";
            label4.Size = new Size(58, 15);
            label4.TabIndex = 2;
            label4.Text = "Programs";
            // 
            // cbxDemos
            // 
            cbxDemos.DropDownStyle = ComboBoxStyle.DropDownList;
            cbxDemos.FormattingEnabled = true;
            cbxDemos.Location = new Point(31, 41);
            cbxDemos.Name = "cbxDemos";
            cbxDemos.Size = new Size(310, 23);
            cbxDemos.TabIndex = 3;
            cbxDemos.SelectedIndexChanged += cbxDemos_SelectedIndexChanged;
            // 
            // cbxGames
            // 
            cbxGames.DropDownStyle = ComboBoxStyle.DropDownList;
            cbxGames.FormattingEnabled = true;
            cbxGames.Location = new Point(347, 41);
            cbxGames.Name = "cbxGames";
            cbxGames.Size = new Size(310, 23);
            cbxGames.TabIndex = 3;
            cbxGames.SelectedIndexChanged += cbxGames_SelectedIndexChanged;
            // 
            // cbxHires
            // 
            cbxHires.DropDownStyle = ComboBoxStyle.DropDownList;
            cbxHires.FormattingEnabled = true;
            cbxHires.Location = new Point(31, 107);
            cbxHires.Name = "cbxHires";
            cbxHires.Size = new Size(310, 23);
            cbxHires.TabIndex = 3;
            cbxHires.SelectedIndexChanged += cbxHires_SelectedIndexChanged;
            // 
            // cbxPrograms
            // 
            cbxPrograms.DropDownStyle = ComboBoxStyle.DropDownList;
            cbxPrograms.FormattingEnabled = true;
            cbxPrograms.Location = new Point(347, 107);
            cbxPrograms.Name = "cbxPrograms";
            cbxPrograms.Size = new Size(310, 23);
            cbxPrograms.TabIndex = 3;
            cbxPrograms.SelectedIndexChanged += cbxPrograms_SelectedIndexChanged;
            // 
            // btnKillInstance
            // 
            btnKillInstance.Location = new Point(31, 209);
            btnKillInstance.Name = "btnKillInstance";
            btnKillInstance.Size = new Size(626, 53);
            btnKillInstance.TabIndex = 1;
            btnKillInstance.Text = "Kill Instance";
            btnKillInstance.UseVisualStyleBackColor = true;
            btnKillInstance.Click += btnKillInstance_Click;
            // 
            // btnOpenWorkingDirectory
            // 
            btnOpenWorkingDirectory.Location = new Point(31, 268);
            btnOpenWorkingDirectory.Name = "btnOpenWorkingDirectory";
            btnOpenWorkingDirectory.Size = new Size(626, 53);
            btnOpenWorkingDirectory.TabIndex = 1;
            btnOpenWorkingDirectory.Text = "Open Working Directory";
            btnOpenWorkingDirectory.UseVisualStyleBackColor = true;
            btnOpenWorkingDirectory.Click += btnOpenWorkingDirectory_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(679, 332);
            Controls.Add(cbxPrograms);
            Controls.Add(cbxHires);
            Controls.Add(cbxGames);
            Controls.Add(cbxDemos);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            Controls.Add(btnOpenWorkingDirectory);
            Controls.Add(btnKillInstance);
            Controls.Add(btnLaunch);
            MaximizeBox = false;
            Name = "Form1";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Chip-8 Emulator Launcher";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion
        private Button btnLaunch;
        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private ComboBox cbxDemos;
        private ComboBox cbxGames;
        private ComboBox cbxHires;
        private ComboBox cbxPrograms;
        private Button btnKillInstance;
        private Button btnOpenWorkingDirectory;
    }
}
