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
    /// Interaction logic for WaitingRoomPage.xaml
    /// </summary>
    public partial class WaitingRoomPage : Page
    {
        public struct RoomData
        {
            public bool hasGameBegun { get; set; }
            public List<string> playersInRoom { get; set; }
            public uint questionCount { get; set; }
            public uint questionTimeout { get; set; }
            public int state { get; set; } // Helper.RoomStatus
        }

        private Thread requestThread;
        private bool isRunning;
        private uint maxPlayers;
        private RoomData previousData;

        public WaitingRoomPage(JoinRoomPage.Room rawRoom)
        {
            DataContext = this;
            InitializeComponent();

            this.previousData = new RoomData
            {
                hasGameBegun = (rawRoom.state == (int)Helper.RoomStatus.IN_GAME),
                playersInRoom = [], // Empty list
                questionCount = rawRoom.questionCount,
                questionTimeout = rawRoom.questionTimeout,
                state = rawRoom.state
            };

            isRunning = true;
            requestThread = new Thread(SendRequest)
            {
                IsBackground = true
            };
            requestThread.Start();
        }

        private void SendRequest()
        {
            while (isRunning)
            {
                RoomData newData = Helper.SendGetRoomStateRequest(); // Button name is the room id

                Helper.RoomStatus status = (Helper.RoomStatus)newData.state;

                switch (status)
                {
                    case Helper.RoomStatus.OPEN:
                        this.Dispatcher.Invoke(() =>
                        {
                            this.RefreshRoomData(newData);
                        });
                        break;
                    case Helper.RoomStatus.IN_GAME:
                        this.Dispatcher.Invoke(() =>
                        {
                            //this.NavigationService.Navigate(new GamePage());
                            this.isRunning = false; // Stop the request thread
                            return;
                        });
                        break;
                    case Helper.RoomStatus.CLOSED:
                        this.Dispatcher.Invoke(() =>
                        {
                            MessageBox.Show("Room has been closed!");
                            this.NavigationService.Navigate(new MenuPage());
                            this.isRunning = false; // Stop the request thread
                            return;
                        });
                        break;
                    default:
                        MessageBox.Show("Error fetching room data, try to rejoin!");
                        break;
                }

                Thread.Sleep(3000); // Wait for 3 seconds before sending the next request
            }
        }

        private void DrawRoomAdmin()
        {
            this.nameAdmin.Text = this.previousData.playersInRoom[0];
            this.nameAdmin.Visibility = Visibility.Visible;
        }

        private void UpdatePlayers()
        {
            this.DrawRoomAdmin();
            this.ClearPlayers();

            string[] players = [.. this.previousData.playersInRoom]; // Convert list to array
            int i = 1;

            if (++i > players.Length) return;
            this.name1.Text = players[i-1];
            this.name1.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name2.Text = players[i-1];
            this.name2.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name3.Text = players[i-1];
            this.name3.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name4.Text = players[i-1];
            this.name4.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name5.Text = players[i-1];
            this.name5.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name6.Text = players[i-1];
            this.name6.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name7.Text = players[i-1];
            this.name7.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name8.Text = players[i-1];
            this.name8.Visibility = Visibility.Visible;

            if (++i > players.Length) return;
            this.name9.Text = players[i-1];
            this.name9.Visibility = Visibility.Visible;
        }

        private void ClearPlayers()
        {
            this.name1.Visibility = Visibility.Hidden;
            this.name2.Visibility = Visibility.Hidden;
            this.name3.Visibility = Visibility.Hidden;
            this.name4.Visibility = Visibility.Hidden;
            this.name5.Visibility = Visibility.Hidden;
            this.name6.Visibility = Visibility.Hidden;
            this.name7.Visibility = Visibility.Hidden;
            this.name8.Visibility = Visibility.Hidden;
            this.name9.Visibility = Visibility.Hidden;
        }

        private void RefreshRoomData(RoomData newData)
        {
            this.previousData = newData;
            this.UpdatePlayers();
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs? e)
        {
            Helper.ResponseType status = Helper.SendLeaveRoomRequest(); // Button name is the room id

            switch (status)
            {
                case Helper.ResponseType.OK:
                    this.NavigationService.Navigate(new MenuPage());
                    this.isRunning = false; // Stop the request thread
                    break;
                default:
                    MessageBox.Show("Cannot leave room! You are stuck! Imagine being stuck in a washing machine LMFAO");
                    break;
            }
        }
    }
}
