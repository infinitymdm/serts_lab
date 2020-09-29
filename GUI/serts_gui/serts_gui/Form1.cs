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

        public Form1()
        {
            InitializeComponent();

            // Set up Threads
            Thread thread_0 = new Thread(new ThreadStart(Thread_0));
            thread_0.Priority = ThreadPriority.Lowest;
            thread_0.IsBackground = true;

            // Set up the serial port
            try
            {
                serialPort1.Open();
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
            thread_0.Start();
        }

        public delegate void TextBoxDelegate(String text);


        private void Thread_0()
        {
            TextBoxDelegate TextBoxDel = TextBoxDelMethod;
            while(true)
            {
                if(serialPort1.IsOpen)
                {
                    String r_str = serialPort1.ReadLine();
                    textBox1.Invoke(TextBoxDel, r_str);
                }
            }
        }

        public void TextBoxDelMethod(String text)
        {
            textBox1.Text = text;
        }
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void buttonRed_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Write("1");
            }
            else
            {
                Debug.WriteLine("Failed to write to serial port");
            }
        }

        private void buttonGreen_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Write("2");
            }
            else
            {
                Debug.WriteLine("Failed to write to serial port");
            }
        }

        private void buttonBlue_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                serialPort1.Write("3");
            }
            else
            {
                Debug.WriteLine("Failed to write to serial port");
            }
        }
    }
}
