using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        static int port = 49002; //tcpPort
        bool comContinue = true;
        //private TextBox textBox1;
        public Form1()
        {
            InitializeComponent();
        }

        public async void Listener()
        {
            try
            {
                textBox1.AppendText("Starting Packet listener..." + "\r\n");

                //Set the IP Address as server's address, and set port  = 49002
                TcpListener server = new TcpListener(IPAddress.Any, port);
                server.Start();

                textBox1.AppendText("Accepting clients on Port " + port.ToString() + "\r\n");

                //If a connection exists, the server will accept it
                //NOTE: If no client exists, and the Async version isn't used,
                //this will cause teh app to hang while it waits
                TcpClient client = await server.AcceptTcpClientAsync();

                //At this point a remote client has been accepted.
                //Get remote client IP address
                string clientIP = client.Client.RemoteEndPoint.ToString();
                textBox1.AppendText("Client connected from IP Address " + clientIP + "\r\n");

                textBox1.AppendText("Instnatiating Networkstream..." + "\r\n");

                //Networkstream is used to send/receive messages
                NetworkStream ns = client.GetStream();

                while (client.Connected && comContinue)
                {
                    //Convert the charater "L" into a decimal equivalent
                    //of "76" and store it in a single element byte[]
                    byte[] writeBuffer = Encoding.ASCII.GetBytes("L");

                    DateTime start = DateTime.Now;

                    //This will write the "76" to the client
                    ns.Write(writeBuffer, 0, writeBuffer.Length);
                    textBox1.AppendText("Sent L to client..." + "\r\n");

                    //the messages arrives as byte array
                    byte[] msg = new byte[4];

                    //reads the message sent by the client using NetworkStream object
                    ns.Read(msg, 0, msg.Length);

                    DateTime end = DateTime.Now;
                    TimeSpan ts = end - start;

                    //Write the message as string
                    textBox1.AppendText("Client response: " + Encoding.Default.GetString(msg));
                    textBox1.AppendText("\r\nRoundTrip time = " + ts.TotalMilliseconds.ToString("F1") + "mSec.");
                    textBox1.AppendText("\r\n \r\n");

                    Thread.Sleep(1000);
                }
            }
            catch
            {
                textBox1.AppendText("\r\n" + "Client disconnected!" + "\r\n");
                return;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Start_Click(object sender, EventArgs e)
        {
            Listener();
        }

        private void Pause_Click(object sender, EventArgs e)
        {
            comContinue = false;
        }

        private void Stop_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
