using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Savetheegg
{
    public partial class StartScreen : Form
    {
        public StartScreen()
        {
            InitializeComponent();
        }

        private void LoadGame(object sender, EventArgs e)
        {
            Form1 gameWindow = new Form1();
            gameWindow.Show();
        }

        private void HelpPlay(object sender, EventArgs e)
        {
            ajutorjoc gameWindow = new ajutorjoc();
            gameWindow.Show();
        }
    }

    public partial class ajutorjoc : Form
    {
        public ajutorjoc()
        {
            InitializeComponent();
            MessageBox.Show("Bine v-am gasit la jocul Prinde oul!" + Environment.NewLine + "Este un joc foarte simplu de jucat. Trebuie să utilizați doar cele două săgeți: săgeata dreaptă și săgeata stângă ca să va deplasați cu caracterul în direcția dorită!" + Environment.NewLine + "Mult succes");
        }

        private void CloseWindow(object sender, EventArgs e)
        {
            this.Close();
        }
    }

    public partial class Form1 : Form
    {
        bool goLeft, goRight;
        int speed = 8;
        int score = 0;
        int missed = 0;
        Random randomX = new Random();
        Random randomY = new Random();
        PictureBox splash = new PictureBox();
        Timer gameTimer = new Timer();

        public Form1()
        {
            InitializeComponent();
            InitializeGame();
        }

        private void InitializeGame()
        {
            gameTimer.Interval = 100; 
            gameTimer.Tick += MainGameTimerEvent; 
            RestartGame(); 
        }

        private void MainGameTimerEvent(object sender, EventArgs e)
        {
            txtScore.Text = "Salvate: " + score;
            txtMiss.Text = "Pietdute: " + missed;

            if (goLeft == true && player.Left > 0)
            {
                player.Left -= 12;
                player.Image = Properties.Resources.chicken_normal2;
            }
            if (goRight == true && player.Left + player.Width < this.ClientSize.Width)
            {
                player.Left += 12;
                player.Image = Properties.Resources.chicken_normal;
            }

            foreach (Control x in this.Controls)
            {

                if (x is PictureBox && (string)x.Tag == "eggs")
                {

                    x.Top += speed;

                    if (x.Top + x.Height > this.ClientSize.Height)
                    {

                        splash.Image = Properties.Resources.splash;
                        splash.Location = x.Location;
                        splash.Height = 60;
                        splash.Width = 60;
                        splash.BackColor = Color.Transparent;

                        this.Controls.Add(splash);

                        x.Top = randY.Next(80, 300) * -1;
                        x.Left = randX.Next(5, this.ClientSize.Width - x.Width);
                        missed += 1;
                        player.Image = Properties.Resources.chicken_hurt;

                    }


                    if (player.Bounds.IntersectsWith(x.Bounds))
                    {
                        x.Top = randY.Next(80, 300) * -1;
                        x.Left = randX.Next(5, this.ClientSize.Width - x.Width);
                        score += 1;
                    }
                }
            }

            if (score > 10)
            {
                speed = 12;
            }

            if (missed > 5)
            {
                GameTimer.Stop();
                MessageBox.Show("Stop joc!" + Environment.NewLine  + Environment.NewLine + "Apasa sa reincerci");
                RestartGame();
            }
        }

        private void KeyIsDown(object sender, KeyEventArgs e)
        {
               if (e.KeyCode == Keys.Left)
            {
                goLeft = true;
            }
            if (e.KeyCode == Keys.Right)
            {
                goRight = true;
            }
        }

        private void KeyIsUp(object sender, KeyEventArgs e)
        {
             if (e.KeyCode == Keys.Left)
            {
                goLeft = false;
            }
            if (e.KeyCode == Keys.Right)
            {
                goRight = false;
            }
        }

        private void RestartGame()
        {
            foreach (Control x in this.Controls)
            {
                if (x is PictureBox && (string)x.Tag == "eggs")
                {
                    x.Top = randomY.Next(80, 300) * -1;
                    x.Left = randomX.Next(5, this.ClientSize.Width - x.Width);
                }
            }

            player.Left = this.ClientSize.Width / 2;
            player.Image = Properties.Resources.chicken_normal;

            score = 0;
            missed = 0;
            speed = 8;
            goLeft = false;
            goRight = false;

            gameTimer.Start(); // Start the game timer
        }
    }
}
