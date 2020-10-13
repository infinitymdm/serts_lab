namespace serts_gui
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.File_List = new System.Windows.Forms.ListBox();
            this.Show_Files = new System.Windows.Forms.Button();
            this.Send_File = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // serialPort
            // 
            this.serialPort.PortName = "COM5";
            // 
            // File_List
            // 
            this.File_List.FormattingEnabled = true;
            this.File_List.ItemHeight = 31;
            this.File_List.Location = new System.Drawing.Point(12, 78);
            this.File_List.Name = "File_List";
            this.File_List.Size = new System.Drawing.Size(776, 345);
            this.File_List.TabIndex = 0;
            // 
            // Show_Files
            // 
            this.Show_Files.Location = new System.Drawing.Point(12, 12);
            this.Show_Files.Name = "Show_Files";
            this.Show_Files.Size = new System.Drawing.Size(271, 56);
            this.Show_Files.TabIndex = 1;
            this.Show_Files.Text = "Show Files";
            this.Show_Files.UseVisualStyleBackColor = true;
            this.Show_Files.Click += new System.EventHandler(this.Show_Files_Click);
            // 
            // Send_File
            // 
            this.Send_File.Location = new System.Drawing.Point(289, 12);
            this.Send_File.Name = "Send_File";
            this.Send_File.Size = new System.Drawing.Size(271, 56);
            this.Send_File.TabIndex = 2;
            this.Send_File.Text = "Send File";
            this.Send_File.UseVisualStyleBackColor = true;
            this.Send_File.Click += new System.EventHandler(this.Send_File_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(16F, 31F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Send_File);
            this.Controls.Add(this.Show_Files);
            this.Controls.Add(this.File_List);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.ListBox File_List;
        private System.Windows.Forms.Button Show_Files;
        private System.Windows.Forms.Button Send_File;
    }
}

