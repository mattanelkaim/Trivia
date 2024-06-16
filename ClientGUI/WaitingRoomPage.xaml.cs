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
            InitializeComponent();
            isRunning = true;
            requestThread = new Thread(SendRequest);
            requestThread.Start();

            this.previousData = new RoomData
            {
                hasGameBegun = (rawRoom.state == (int)Helper.RoomStatus.IN_GAME),
                playersInRoom = [], // Empty list
                questionCount = rawRoom.questionCount,
                questionTimeout = rawRoom.questionTimeout,
                state = rawRoom.state
            };
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

        private void RefreshRoomData(RoomData newData)
        {
            this.previousData = newData;
            //this.DrawPlayers();
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
                    MessageBox.Show("Cannot join room!");
                    break;
            }
        }
    }
}
