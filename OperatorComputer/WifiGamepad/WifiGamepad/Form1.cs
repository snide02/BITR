using OpenJigWare;
using System;
using System.Drawing;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Threading.Tasks;
using System.ComponentModel;

namespace WifiGamepad
{
    public partial class Form1 : Form
    {
        static int port = 49002; //tcpPort
        bool comContinue = true;

        private Thread trd;

        int lJoyX, lJoyY, rJoyX, rJoyY, eStop, enable, mode, toggle;

        public Form1()
        {
            InitializeComponent();
        }

        float x1 = 115;
        float y1 = 115;
        float x2 = 115;
        float y2 = 115;

        int count = 0;
        int avgTS = 0;

        private void Form1_Load(object sender, EventArgs e)
        {
            Ojw.CMessage.Init(txtMessage);
            panel1.Controls.Add(radioButton1);
            panel2.Controls.Add(radioButton2);
            panel3.Controls.Add(radioButton3);

            timer1.Enabled = true;
            radioButton1.Location = new Point( (int)x1, (int)y1);
            radioButton2.Location = new Point((int)x2, (int)y2);
        }

        private float[] m_afAngle = new float[20];

        private Ojw.CJoystick m_CJoy = new Ojw.CJoystick(Ojw.CJoystick._ID_0); // 조이스틱 선언 -> joystick declaration
        private Ojw.CTimer m_CTmr_Joystick = new Ojw.CTimer(); // 조이스틱의 연결을 주기적으로 체크할 타이머 -> Timer to periodically check the joystick connection

        private void FJoystick_Check_Alive()
        {
            #region Joystick Check

            Color m_colorLive = Color.Green; // 살았을 경우의 색
            Color m_colorDead = Color.Gray; // 죽었을 경우의 색
            if (m_CJoy.IsValid == false)
            {
                #region 조이스틱이 연결되지 않았음을 표시
                if (lbJoystick.ForeColor != m_colorDead)
                {
                    lbJoystick.Text = "Joystick (No Connected)";
                    lbJoystick.ForeColor = m_colorDead;
                }
                #endregion 조이스틱이 연결되지 않았음을 표시

                #region 3초마다 다시 재연결을 하려고 시도
                if (m_CTmr_Joystick.Get() > 3000) // Joystic 이 죽어있다면 체크(3초단위)
                {
                    Ojw.CMessage.Write("Joystick Check again");
                    m_CJoy = new Ojw.CJoystick(Ojw.CJoystick._ID_0);

                    if (m_CJoy.IsValid == false)
                    {
                        Ojw.CMessage.Write("But we can't find a joystick device in here. Check your joystick device");
                        m_CTmr_Joystick.Set(); // 타이머의 카운터를 다시 초기화 한다.
                    }
                    else Ojw.CMessage.Write("Joystick is Connected");
                }
                #endregion 3초마다 다시 재연결을 하려고 시도
            }
            else
            {
                #region 연결 되었음을 표시
                if (lbJoystick.ForeColor != m_colorLive)
                {
                    lbJoystick.Text = "Joystick (Connected)";
                    lbJoystick.ForeColor = m_colorLive;
                }
                #endregion 연결 되었음을 표시
            }
            #endregion Joystick Check
        }

        private void FJoystick_Check_Data()
        {
            //Flight Mode
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button4) == true)
            {
                ch8.ForeColor = Color.Red;
                ch8.Text = "Flight Mode";
                mode = 48;
            }
            //Hybrid Mode
            else if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button5) == true)
            {
                ch8.Text = "Hybrid Mode";
                ch8.ForeColor = Color.DarkRed;
                mode = 50;
            }
            //Drive Mode
            else if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button4) == false && m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button5) == false)
            {
                ch8.ForeColor = Color.IndianRed;
                ch8.Text = "Drive Mode";
                mode = 49;
            }
            else
            {
                ch8.ForeColor = Color.Black;
                ch8.Text = "ERROR! CH8 TOGGLE BROKEN";
            }

            // A Button ¯\_(ツ)_/¯
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button1) == true)
            {
                Ch5Up.ForeColor = Color.Red;
                //comContinue = true;
                toggle = 49;
            }
            else
            {
                Ch5Up.ForeColor = Color.Black;
                //comContinue = false;
                toggle = 48;
            }

            //Enable Switch
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button2) == true)
            {
                ch7Up.ForeColor = Color.Red;
                ch7Up.Text = "Enabled";
                enable = 49;

            }
            else
            {
                ch7Up.ForeColor = Color.Black;
                ch7Up.Text = "Disabled";
                enable = 48;
            }

            //E-Stop
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button3) == true)
            {
                resetButtom.ForeColor = Color.Red;
                //Application.Exit();
                //comContinue = false;
                eStop = 49;
            }
            else
            {
                resetButtom.ForeColor = Color.Black;
                eStop = 48;
            }
            
            radioButton1.Location = new Point((int)(x1 * 1.8 * m_CJoy.dY1),(int)(y1*1.8* m_CJoy.Slide));
            radioButton2.Location = new Point((int)(x2 * 1.8 * m_CJoy.dX0), (int)(y2 * 1.8 * m_CJoy.dY0));
            textBox1.Text = Convert.ToString("X = " + m_CJoy.dY1 + "\r\nY = " + m_CJoy.Slide);
            textBox2.Text = Convert.ToString("X = " + m_CJoy.dX0 + "\r\nY = " + m_CJoy.dY0);
            radioButton3.Location = new Point((int)(168 -m_CJoy.dX1 * 168),10);
            
            lJoyX = (int)(100 * m_CJoy.dY1); //Value of first 2 decimals of the Left Joystick's X Axis
            lJoyY = (int)(100 * m_CJoy.Slide); //Value of first 2 decimals of the Left Joystick's Y Axis
            textBox1.Text = Convert.ToString("X = " + lJoyX + "\r\nLetter X = " + Convert.ToChar(lJoyX) + "\r\nY = " + lJoyY + "\r\nLetter Y = " + Convert.ToChar(lJoyY));

            
            rJoyX = (int)(100 * m_CJoy.dX0); //Value of first 2 decimals of the Left Joystick's X Axis
            rJoyY = (int)(100 * m_CJoy.dY0); //Value of first 2 decimals of the Left Joystick's Y Axis
            textBox2.Text = Convert.ToString("X = " + rJoyX + "\r\nLetter X = " + Convert.ToChar(rJoyX) + "\r\nY = " + rJoyY + "\r\nLetter Y = " + Convert.ToChar(rJoyY));

            txtMessage.Text = Convert.ToString("E-Stop = " + eStop + "  Letter E-Stop = " + Convert.ToChar(eStop) + "\r\nToggle = " + toggle + "  Letter Enable = " + Convert.ToChar(enable) + "\r\nMode = " + mode + "  Letter Mode = " + Convert.ToChar(mode));

        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            // Joystick information update
            m_CJoy.Update();
            //  Function to check if the joystick is alive
            FJoystick_Check_Alive();
            // Joystick data check
            FJoystick_Check_Data();
        }
        public async void Listener()
        {
            try
            {
                if (Start.InvokeRequired)
                {
                    Start.Invoke(new Action(Listener));
                    return;
                }

                textBox3.AppendText("Starting Packet listener..." + "\r\n");

                //Set the IP Address as server's address, and set port  = 49002
                TcpListener server = new TcpListener(IPAddress.Any, port);
                server.Start();

                textBox3.AppendText("Accepting clients on Port " + port.ToString() + "\r\n");

                //If a connection exists, the server will accept it
                //NOTE: If no client exists, and the Async version isn't used,
                //this will cause teh app to hang while it waits
                TcpClient client = await server.AcceptTcpClientAsync();

                //At this point a remote client has been accepted.
                //Get remote client IP address
                string clientIP = client.Client.RemoteEndPoint.ToString();
                textBox3.AppendText("Client connected from IP Address " + clientIP + "\r\n");

                textBox3.AppendText("Instnatiating Networkstream..." + "\r\n");

                //Networkstream is used to send/receive messages
                NetworkStream ns = client.GetStream();

                while (client.Connected && comContinue)
                {
                    // Joystick information update
                    m_CJoy.Update();
                    //  Function to check if the joystick is alive
                    FJoystick_Check_Alive();
                    // Joystick data check
                    FJoystick_Check_Data();

                    //Convert the charater "L" into a decimal equivalent
                    //of "76" and store it in a single element byte[]
                    string sendlX = Convert.ToString(Convert.ToChar(lJoyX));
                    string sendlY = Convert.ToString(Convert.ToChar(lJoyY));
                    string sendrX = Convert.ToString(Convert.ToChar(rJoyX));
                    string sendrY = Convert.ToString(Convert.ToChar(rJoyY));
                    string sendEmStop = Convert.ToString(Convert.ToChar(eStop));
                    string sendToggle = Convert.ToString(Convert.ToChar(toggle));
                    string sendMode = Convert.ToString(Convert.ToChar(mode));
                    string startPack = Convert.ToString('Z');

                    //string send
                    //byte[] writeBuffer = Encoding.ASCII.GetBytes(sendlX + sendlY + sendrX + sendrY);
                    byte[] writeBuffer = Encoding.ASCII.GetBytes(startPack + sendlX + sendlY + sendrX + sendrY + sendEmStop + sendToggle + sendMode);
                    Console.WriteLine("Write Buffer Lenght: " + writeBuffer.Length);
                    Console.WriteLine("Write Buffer:  " + writeBuffer);
                    Console.WriteLine("Sent Packet:  " + startPack + sendlX + sendlY + sendrX + sendrY + sendEmStop + sendToggle + sendMode);

                    DateTime start = DateTime.Now;

                    //This will write the "76" to the client
                    ns.Write(writeBuffer, 0, writeBuffer.Length);
                    textBox3.AppendText("Sent Packet to client..." + "\r\n");

                    //the messages arrives as byte array
                    byte[] msg = new byte[8];

                    //reads the message sent by the client using NetworkStream object
                    ns.Read(msg, 0, msg.Length);

                    DateTime end = DateTime.Now;
                    TimeSpan ts = end - start;
                    
                    count = count + 1;
                    avgTS = (avgTS + (int)ts.TotalMilliseconds);
                    int average = avgTS/ count;

                    //Write the message as string
                    textBox3.AppendText("Client response: " + Encoding.Default.GetString(msg));
                    textBox3.AppendText("\r\nRoundTrip time = " + ts.TotalMilliseconds.ToString("F1") + "mSec.");
                    textBox3.AppendText("\r\nAverage= " + average.ToString("F1") + "mSec.");
                    textBox3.AppendText("\r\n \r\n");

                    //Thread.Sleep(1000);
                }
            }
            catch
            {
                textBox3.AppendText("\r\n" + "Client disconnected!" + "\r\n");
                return;
            }
        }

        private void Start_Click(object sender, EventArgs e)
        {
            //Thread trd = new Thread(new ThreadStart(this.Listener));
            Thread trd = new Thread(Listener);
            trd.IsBackground = true;
            trd.Start();
        }
    }
}
