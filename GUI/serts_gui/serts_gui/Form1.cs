using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace serts_gui
{
    public partial class Form1 : Form
    {

        readonly int ShowFiles = 1;
        readonly int StartFileList = 2;
        readonly int EndFileList = 3;
        readonly int SetFile = 4;

        public Form1()
        {
            InitializeComponent();

            // Set up Threads
            Thread PC_Comm = new Thread(new ThreadStart(PC_comm_method));
            PC_Comm.Priority = ThreadPriority.Lowest;
            PC_Comm.IsBackground = true;

            // Set up the serial port
            try
            {
                serialPort.Open();
            }
            catch
            {
                Debug.WriteLine("Failed to open serial port");
            }

            // Make sure the window gets created
            if (!IsHandleCreated)
            {
                CreateHandle();
            }

            // Start Threads
            PC_Comm.Start();
        }

        public delegate void ListBoxDelegate(int command, string str);

        public void ListBoxDelMethod(int command, string str)
        {
            if (command == StartFileList)
            {
                File_List.Items.Clear();
            }
            else if (command == EndFileList)
            {
                File_List.Items.Add(str);
            }
        }

        private void PC_comm_method()
        {
            ListBoxDelegate FileListDel = ListBoxDelMethod;
            string serialData = "";
            while (true)
            {
                if (serialPort.IsOpen)
                {
                    try
                    {
                        serialData = serialPort.ReadLine();
                    }
                    catch 
                    {
                        Debug.WriteLine("Failed to open serial port");
                    }
                    if (serialData.Equals(StartFileList.ToString()))
                    {
                        File_List.Invoke(FileListDel, StartFileList, "");
                        try
                        {
                            serialData = serialPort.ReadLine();
                        }
                        catch
                        {
                            Debug.WriteLine("Failed to read from serial port");
                        }
                        while (!serialData.Equals(EndFileList.ToString()))
                        {
                            File_List.Invoke(FileListDel, EndFileList, serialData);
                            try
                            {
                                serialData = serialPort.ReadLine();
                            }
                            catch
                            {
                                Debug.WriteLine("Failed to read from serial port");
                            }
                        }
                    }
                }
                else
                {
                    Thread.Sleep(500);
                }
            }
        }

        private void Show_Files_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write(ShowFiles.ToString());
            }
        }

        private void Send_File_Click(object sender, EventArgs e)
        {
            byte[] b = { 0 };
            if (File_List.SelectedIndex != -1)
            {
                serialPort.Write(SetFile.ToString());
                serialPort.Write(File_List.SelectedItem.ToString());
                serialPort.Write(b, 0, 1);
            }
        }
    }
}
