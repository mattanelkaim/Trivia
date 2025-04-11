using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
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

        private WaitingRoomPage.RoomData RoomData;
        private Thread TimerThread;
        private bool isTimerRunning = true;
        private bool suspendTimerThread = false;
        private bool hasAnswered = false;

        public GamePage(WaitingRoomPage.RoomData roomData)
        {
            InitializeComponent();
            this.RoomData = roomData;
            Helper.RemoveButtonHighlighting(this.ans1);
            Helper.RemoveButtonHighlighting(this.ans2);
            Helper.RemoveButtonHighlighting(this.ans3);
            Helper.RemoveButtonHighlighting(this.ans4);

            DisplayQuestion(Helper.SendGetNextQuestionRequest());

            // start timer
            this.TimerThread = new Thread(TimerThreadWrapper)
            {
                IsBackground = true
            };
            TimerThread.Start();
            RoomData = roomData;
        }

        private void Button_MouseEnter(object sender, MouseEventArgs? e)
        {
            Helper.ButtonGotHoverEffect((Button)sender);
        }

        private void Button_MouseLeave(object sender, MouseEventArgs? e)
        {
            Helper.ButtonLostHoverEffect((Button)sender);
        }

        private void DisplayQuestion(GetNextQuestionResponse question)
        {            
            this.TimerTextBlock.Text = RoomData.questionTimeout.ToString();
            this.QuestionTextBlock.Text = question.question.ToString();
            string[] answers = question.answers.Values.ToArray();
            ((TextBlock)this.ans1.Content).Text = answers[0];
            ((TextBlock)this.ans2.Content).Text = answers[1];
            ((TextBlock)this.ans3.Content).Text = answers[2];
            ((TextBlock)this.ans4.Content).Text = answers[3];
        }

        private async void Answer_Click(object sender, RoutedEventArgs? e)
        {
            if (this.hasAnswered)
                return;

            int AnswerId = ((Button)sender).Name[3] - '0';
            SubmitAnswerResponse response = Helper.SendSubmitAnswerRequest(AnswerId);

            if (response.status != (int)Helper.ResponseType.OK)
                MessageBox.Show("Error");

            hasAnswered = true;
            suspendTimerThread = true;
            int CorrectAnswerId = response.correctAnsID;
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
                case 1:
                    correctButton = this.ans1;
                    break;
                case 2:
                    correctButton = this.ans2;
                    break;
                case 3:
                    correctButton = this.ans3;
                    break;
                default: // case 4:
                    correctButton = this.ans4;
                    break;
            }

            correctButton.Background = Brushes.Green;

            // wait for next question
            this.WaitForNextQuestion();
            //Task< GetNextQuestionResponse> task = Task.Run(() =>
            //{
            //    Thread.Sleep((int.Parse(this.TimerTextBlock.Text) + 1) * 1000);
            //    return Helper.SendGetNextQuestionRequest();
            //});

            //Helper.GetNextQuestionResponse repsonse2 = await task;

            //// if the 
            //if (repsonse2.status == (int)Helper.ResponseType.NO_MORE_QUESTIONS)
            //{
            //    this.isTimerRunning = false;
            //    //this.NavigationService.Navigate(something);
            //}
            //else
            //{
            //    DisplayQuestion(Helper.SendGetNextQuestionRequest());
            //}
        }

        private void WaitForNextQuestion()
        {
            this.suspendTimerThread = true;
            Helper.GetNextQuestionResponse response;

            while (true)
            {
                response = Helper.SendGetNextQuestionRequest();
                if (response.status == (int)Helper.ResponseType.WAIT_FOR_OTHERS)
                    Thread.Sleep(1000);
                else
                    break;
            }

            this.suspendTimerThread = false;
            this.hasAnswered = false;
            DisplayQuestion(response);
        }

        private void TimerThreadWrapper()
        {
            bool HasTimerEnded = false;
            while(isTimerRunning)
            {
                while (suspendTimerThread) { }
                this.Dispatcher.Invoke(() => { this.TimerTextBlock.Text = (int.Parse(this.TimerTextBlock.Text) - 1).ToString(); });
                this.Dispatcher.Invoke(() => { HasTimerEnded = this.TimerTextBlock.Text == "0"; });
                if (HasTimerEnded)
                    MessageBox.Show("Time Ended!"); // TODO - remove and do somthing
                Thread.Sleep(1000);
            }
        }
    }
}
