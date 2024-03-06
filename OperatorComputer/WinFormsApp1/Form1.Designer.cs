namespace WinFormsApp1
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
            textBox1 = new TextBox();
            Start = new Button();
            Pause = new Button();
            Stop = new Button();
            SuspendLayout();
            // 
            // textBox1
            // 
            textBox1.Location = new Point(236, 60);
            textBox1.Multiline = true;
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(286, 224);
            textBox1.TabIndex = 0;
            textBox1.TextChanged += textBox1_TextChanged;
            // 
            // Start
            // 
            Start.Location = new Point(111, 351);
            Start.Name = "Start";
            Start.Size = new Size(75, 23);
            Start.TabIndex = 1;
            Start.Text = "Start";
            Start.UseVisualStyleBackColor = true;
            Start.Click += Start_Click;
            // 
            // Pause
            // 
            Pause.Location = new Point(347, 351);
            Pause.Name = "Pause";
            Pause.Size = new Size(75, 23);
            Pause.TabIndex = 2;
            Pause.Text = "Pause";
            Pause.UseVisualStyleBackColor = true;
            Pause.Click += Pause_Click;
            // 
            // Stop
            // 
            Stop.Location = new Point(576, 351);
            Stop.Name = "Stop";
            Stop.Size = new Size(75, 23);
            Stop.TabIndex = 3;
            Stop.Text = "Exit";
            Stop.UseVisualStyleBackColor = true;
            Stop.Click += Stop_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(Stop);
            Controls.Add(Pause);
            Controls.Add(Start);
            Controls.Add(textBox1);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textBox1;
        private Button Start;
        private Button Pause;
        private Button Stop;
    }
}
