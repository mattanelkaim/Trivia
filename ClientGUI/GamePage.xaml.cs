using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
using static ClientGUI.GamePage;
using static ClientGUI.Helper;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for GamePage.xaml
    /// </summary>
    public partial class GamePage : Page
    {
        public struct Question
        {
            public string Title { get; set; }
            public Dictionary<string, string> Answers { get; set; }
        }


        private Thread TimerThread;
        private bool isTimerRunning = true;
        private bool suspendThread = false;

        public GamePage()
        {
            InitializeComponent();
            Helper.RemoveButtonHighlighting(this.ans1);
            Helper.RemoveButtonHighlighting(this.ans2);
            Helper.RemoveButtonHighlighting(this.ans3);
            Helper.RemoveButtonHighlighting(this.ans4);

            DisplayQuestion(Helper.SendGetNextQuestionRequest().question);

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

        private void DisplayQuestion(Question question)
        {
            this.QuestionTextBlock.Text = question.Title;
            string[] answers = question.Answers.Values.ToArray();
            ((TextBlock)this.ans1.Content).Text = answers[0];
            ((TextBlock)this.ans2.Content).Text = answers[1];
            ((TextBlock)this.ans3.Content).Text = answers[2];
            ((TextBlock)this.ans4.Content).Text = answers[3];
        }

        private void Answer_Click(object sender, RoutedEventArgs? e)
        {
            int AnswerId = ((TextBlock)(((Button)sender).Content)).Text[3] - '0' - 1;
            SubmitAnswerResponse response = Helper.SendSubmitAnswerRequest(AnswerId);

            if (response.status != (int)Helper.ResponseType.OK)
                MessageBox.Show("Error");

            suspendThread = true;
            int CorrectAnswerId = response.correctAnsID + 1;
            if (AnswerId != CorrectAnswerId)
            {
                ((Button)sender).Background = Brushes.Red;
            }
            else
            {
                this.CorrectAnswersTextBlock.Text = (int.Parse(this.CorrectAnswersTextBlock.Text) + 1).ToString();
            }

            Button correctButton;
            switch (CorrectAnswerId) 
            {
                case 0:
                    correctButton = this.ans1;
                    break;
                case 1:
                    correctButton = this.ans2;
                    break;
                case 2:
                    correctButton = this.ans3;
                    break;
                default: // case 3:
                    correctButton = this.ans4;
                    break;
            }

            correctButton.Background = Brushes.Green;

            // wait for next question
            Thread.Sleep((int.Parse(this.TimerTextBlock.Text) + 1) * 1000);
            Helper.GetNextQuestionResponse repsonse = Helper.SendGetNextQuestionRequest();

            // if the 
            if (repsonse.status == (int)Helper.ResponseType.NO_MORE_QUESTIONS)
            {
                this.suspendThread = true;
                this.isTimerRunning = false;
                //this.NavigationService.Navigate(something);
            }
        }

        private void TimerThreadWrapper()
        {
            bool HasTimerEnded = false;
            while(isTimerRunning)
            {
                while (suspendThread) { }
                this.Dispatcher.Invoke(() => { this.TimerTextBlock.Text = (int.Parse(this.TimerTextBlock.Text) - 1).ToString(); });
                this.Dispatcher.Invoke(() => { HasTimerEnded = this.TimerTextBlock.Text == "0"; });
                if (HasTimerEnded)
                    MessageBox.Show("Time Ended!"); // TODO - remove and do somthing
                Thread.Sleep(1000);
            }
        }
    }
}
