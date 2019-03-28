using System;
using System.Diagnostics;
using System.Windows.Forms;
using System.Xml;

namespace PlantWatch
{

    public partial class Form1 : Form
    {

        private event EventHandler<PortEvent> MessageReceived;

        public Form1()
        {
            InitializeComponent();

            MessageReceived += Form1_MessageReceived;
        }

        private void Form1_MessageReceived(object sender, PortEvent e)
        {
            Debug.WriteLine("Receieved message!");

            if (e.Type == 1)
            {
                SetLabelText(label1, e.Message);
            }
        }

        private void SetLabelText(Label c, string text)
        {
            if (c.InvokeRequired)
            {
                c.Invoke(new Action(() => SetLabelText(c, text)));
            }
            else
            {
                c.Text = text;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM26";
            serialPort1.DataReceived += SerialPort1_DataReceived;
        }

        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            if (e.EventType == System.IO.Ports.SerialData.Chars)
            {
                try
                {
                    // read until new line
                    var xml = serialPort1.ReadTo("\r\n");
                    var doc = new XmlDocument();
                    doc.LoadXml("<?xml version=\"1.0\"?>" + xml);

                    // should only be a single element with this name before the \r\n delimiter
                    var events = doc.GetElementsByTagName("event");
                    foreach (XmlElement ev in events)
                    {
                        int type = int.Parse(ev.SelectSingleNode("type").InnerText);
                        string message = ev.SelectSingleNode("message").InnerText;

                        MessageReceived?.Invoke(this, new PortEvent()
                        {
                            Type = type,
                            Message = message,
                            ReceivedUtc = DateTime.UtcNow
                        });

                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("Failed to read message from port.", ex);
                }

            }

        }

        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.Open();
            button1.Enabled = false;
            button2.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort1.DiscardInBuffer();
            serialPort1.DiscardOutBuffer();
            serialPort1.Close();
            button1.Enabled = true;
            button2.Enabled = false;
        }
    }
}
