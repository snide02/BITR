namespace WifiGamepad
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.panel2 = new System.Windows.Forms.Panel();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.txtMessage = new System.Windows.Forms.TextBox();
            this.lbJoystick = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.ch8 = new System.Windows.Forms.Label();
            this.resetButtom = new System.Windows.Forms.Label();
            this.Ch5Up = new System.Windows.Forms.Label();
            this.ch7Up = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.radioButton1);
            this.panel1.Location = new System.Drawing.Point(13, 280);
            this.panel1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(220, 220);
            this.panel1.TabIndex = 0;
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(103, 103);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(14, 13);
            this.radioButton1.TabIndex = 0;
            this.radioButton1.TabStop = true;
            this.radioButton1.UseVisualStyleBackColor = true;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Controls.Add(this.radioButton2);
            this.panel2.Location = new System.Drawing.Point(319, 280);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(220, 220);
            this.panel2.TabIndex = 2;
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Checked = true;
            this.radioButton2.Location = new System.Drawing.Point(103, 103);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(14, 13);
            this.radioButton2.TabIndex = 0;
            this.radioButton2.TabStop = true;
            this.radioButton2.UseVisualStyleBackColor = true;
            // 
            // txtMessage
            // 
            this.txtMessage.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtMessage.Location = new System.Drawing.Point(13, 618);
            this.txtMessage.Multiline = true;
            this.txtMessage.Name = "txtMessage";
            this.txtMessage.Size = new System.Drawing.Size(526, 100);
            this.txtMessage.TabIndex = 4;
            // 
            // lbJoystick
            // 
            this.lbJoystick.AutoSize = true;
            this.lbJoystick.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lbJoystick.Font = new System.Drawing.Font("Malgun Gothic", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lbJoystick.Location = new System.Drawing.Point(12, 9);
            this.lbJoystick.Name = "lbJoystick";
            this.lbJoystick.Size = new System.Drawing.Size(28, 34);
            this.lbJoystick.TabIndex = 5;
            this.lbJoystick.Text = "a";
            // 
            // timer1
            // 
            this.timer1.Interval = 40;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick_1);
            // 
            // textBox1
            // 
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox1.Location = new System.Drawing.Point(13, 508);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(220, 104);
            this.textBox1.TabIndex = 6;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(319, 508);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(220, 104);
            this.textBox2.TabIndex = 7;
            // 
            // ch8
            // 
            this.ch8.AutoSize = true;
            this.ch8.BackColor = System.Drawing.SystemColors.Control;
            this.ch8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.ch8.Font = new System.Drawing.Font("Malgun Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ch8.Location = new System.Drawing.Point(14, 71);
            this.ch8.Name = "ch8";
            this.ch8.Size = new System.Drawing.Size(123, 23);
            this.ch8.TabIndex = 8;
            this.ch8.Text = "Mode Selector";
            // 
            // resetButtom
            // 
            this.resetButtom.AutoSize = true;
            this.resetButtom.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.resetButtom.Font = new System.Drawing.Font("Malgun Gothic", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.resetButtom.Location = new System.Drawing.Point(75, 206);
            this.resetButtom.Name = "resetButtom";
            this.resetButtom.Size = new System.Drawing.Size(87, 34);
            this.resetButtom.TabIndex = 9;
            this.resetButtom.Text = "E-Stop";
            // 
            // Ch5Up
            // 
            this.Ch5Up.AutoSize = true;
            this.Ch5Up.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.Ch5Up.Font = new System.Drawing.Font("Malgun Gothic", 18F);
            this.Ch5Up.Location = new System.Drawing.Point(100, 142);
            this.Ch5Up.Name = "Ch5Up";
            this.Ch5Up.Size = new System.Drawing.Size(32, 34);
            this.Ch5Up.TabIndex = 10;
            this.Ch5Up.Text = "A";
            // 
            // ch7Up
            // 
            this.ch7Up.AutoSize = true;
            this.ch7Up.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.ch7Up.Font = new System.Drawing.Font("Malgun Gothic", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.ch7Up.Location = new System.Drawing.Point(363, 142);
            this.ch7Up.Name = "ch7Up";
            this.ch7Up.Size = new System.Drawing.Size(165, 34);
            this.ch7Up.TabIndex = 11;
            this.ch7Up.Text = "Enable Switch";
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.radioButton3);
            this.panel3.Location = new System.Drawing.Point(363, 70);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(180, 30);
            this.panel3.TabIndex = 14;
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Checked = true;
            this.radioButton3.Location = new System.Drawing.Point(85, 9);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(14, 13);
            this.radioButton3.TabIndex = 0;
            this.radioButton3.TabStop = true;
            this.radioButton3.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(555, 730);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.ch7Up);
            this.Controls.Add(this.Ch5Up);
            this.Controls.Add(this.resetButtom);
            this.Controls.Add(this.ch8);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.lbJoystick);
            this.Controls.Add(this.txtMessage);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Malgun Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.TextBox txtMessage;
        private System.Windows.Forms.Label lbJoystick;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label ch8;
        private System.Windows.Forms.Label resetButtom;
        private System.Windows.Forms.Label Ch5Up;
        private System.Windows.Forms.Label ch7Up;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.RadioButton radioButton3;
    }
}

