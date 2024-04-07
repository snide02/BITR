using OpenJigWare;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace WifiGamepad
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        float x1 = 115;
        float y1 = 115;
        float x2 = 115;
        float y2 = 115;

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
            }
            //Hybrid Mode
            else if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button5) == true)
            {
                ch8.Text = "Hybrid Mode";
                ch8.ForeColor = Color.DarkRed;
            }
            //Drive Mode
            else if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button4) == false && m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button5) == false)
            {
                ch8.ForeColor = Color.IndianRed;
                ch8.Text = "Drive Mode";
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
            }
            else
            {
                Ch5Up.ForeColor = Color.Black;
            }

            //Enable Switch
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button2) == true)
            {
                ch7Up.ForeColor = Color.Red;
                ch7Up.Text = "Enabled";
            }
            else
            {
                ch7Up.ForeColor = Color.Black;
                ch7Up.Text = "Disabled";
            }

            //E-Stop
            if (m_CJoy.IsDown(Ojw.CJoystick.PadKey.Button3) == true)
            {
                resetButtom.ForeColor = Color.Red;
            }
            else
            {
                resetButtom.ForeColor = Color.Black;
            }
            
            radioButton1.Location = new Point((int)(x1 * 1.8 * m_CJoy.dY1),(int)(y1*1.8* m_CJoy.Slide));
            radioButton2.Location = new Point((int)(x2 * 1.8 * m_CJoy.dX0), (int)(y2 * 1.8 * m_CJoy.dY0));
            textBox1.Text = Convert.ToString("X = " + m_CJoy.dY1 + "\r\nY = " + m_CJoy.Slide);
            textBox2.Text = Convert.ToString("X = " + m_CJoy.dX0 + "\r\nY = " + m_CJoy.dY0);
            radioButton3.Location = new Point((int)(168 -m_CJoy.dX1 * 168),10);

            
            int lJoyX, lJoyY;
            lJoyX = (int)(100 * m_CJoy.dY1); //Value of first 2 decimals of the Left Joystick's X Axis
            lJoyY = (int)(100 * m_CJoy.Slide); //Value of first 2 decimals of the Left Joystick's Y Axis
            textBox1.Text = Convert.ToString("X = " + lJoyX + "\r\nLetter X = " + Convert.ToChar(lJoyX) + "\r\nY = " + lJoyY + "\r\nLetter Y = " + Convert.ToChar(lJoyY));

            int rJoyX, rJoyY;
            rJoyX = (int)(100 * m_CJoy.dX0); //Value of first 2 decimals of the Left Joystick's X Axis
            rJoyY = (int)(100 * m_CJoy.dY0); //Value of first 2 decimals of the Left Joystick's Y Axis
            textBox2.Text = Convert.ToString("X = " + rJoyX + "\r\nLetter X = " + Convert.ToChar(rJoyX) + "\r\nY = " + rJoyY + "\r\nLetter Y = " + Convert.ToChar(rJoyY));

        }

        private void timer1_Tick_1(object sender, EventArgs e)
        {
            // 조이스틱 정보 갱신 -> Joystick information update
            m_CJoy.Update();
            // 조이스틱이 살아 있는지 체크하는 함수 -> Function to check if the joystick is alive
            FJoystick_Check_Alive();
            // 조이스틱 데이타 체크 -> Joystick data check
            FJoystick_Check_Data();
        }
    }
}
