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
            serialPort1.Open();
            if (!serialPort1.IsOpen)
            {
                Debug.WriteLine("Unable to open serial port");
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
            serialPort1.Write("R");
        }

        private void buttonGreen_Click(object sender, EventArgs e)
        {
            serialPort1.Write("G");
        }
    }
}
