using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for GamePage.xaml
    /// </summary>
    public partial class GamePage : Page
    {
        private Thread TimerThread;
        private bool isTimerRunning = true;
        public GamePage()
        {
            InitializeComponent();
            Helper.RemoveButtonHighlighting(this.ans1);
            Helper.RemoveButtonHighlighting(this.ans2);
            Helper.RemoveButtonHighlighting(this.ans3);
            Helper.RemoveButtonHighlighting(this.ans4);

            // TODO - show question
            
            // start timer
            this.TimerThread = new Thread(TimerThreadWrapper)
            {
                IsBackground = true
            };
            TimerThread.Start();            
        }

        private void Button_MouseEnter(object sender, MouseEventArgs? e)
        {
            Helper.ButtonGotHoverEffect((Button)sender);
        }

        private void Button_MouseLeave(object sender, MouseEventArgs? e)
        {
            Helper.ButtonLostHoverEffect((Button)sender);
        }

        private void Answer_Click(object sender, RoutedEventArgs? e)
        {

        }

        private void TimerThreadWrapper()
        {
            bool HasTimerEnded = false;
            while(isTimerRunning)
            {
                this.Dispatcher.Invoke(() => { this.TimerTextBlock.Text = (int.Parse(this.TimerTextBlock.Text) - 1).ToString(); });
                this.Dispatcher.Invoke(() => { HasTimerEnded = this.TimerTextBlock.Text == "0"; });
                if (HasTimerEnded)
                    MessageBox.Show("Time Ended!"); // TODO - remove and do somthing
                Thread.Sleep(1000);
            }
        }
    }
}
