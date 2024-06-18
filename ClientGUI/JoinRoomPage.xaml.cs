﻿using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
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
using System.Xml.Linq;
using static ClientGUI.Helper;

namespace ClientGUI
{
    /// <summary>
    /// Interaction logic for JoinRoomPage.xaml
    /// </summary>
    public partial class JoinRoomPage : Page
    {
        public struct Room
        {
            public string name { get; set; }
            public uint maxPlayers { get; set; }
            public uint questionCount { get; set; }
            public uint questionTimeout { get; set; }
            public int state { get; set; }
        }        

        private Dictionary<string, Room> RoomsData;

        private Thread requestThread;
        private bool isThreadRunning = true;

        public JoinRoomPage()
        {
            InitializeComponent();
            this.DataContext = this;            
                        
            requestThread = new(ContinuouslyUpdateRooms)
            {
                IsBackground = true
            };
            requestThread.Start();
        }

        private void ContinuouslyUpdateRooms()
        {            
            while (isThreadRunning)
            {
                this.Dispatcher.Invoke(() => { RoomsData = FetchRoomsFromDB(); });
                this.Dispatcher.Invoke(this.ClearRooms);
                this.Dispatcher.Invoke(this.ShowRooms);
                Thread.Sleep(3000);
            }
        }

        public static Dictionary<string, Room> FetchRoomsFromDB()
        {
            return Helper.SendGetRoomsRequest();
        }

        private void HomeButton_Click(object sender, RoutedEventArgs? e)
        {
            isThreadRunning = false;
            this.NavigationService.Navigate(new MenuPage());
        }

        private void JoinRoom(object sender, RoutedEventArgs? e)
        {
            string id = ((Button)sender).Name.Substring(6); // Remove "button" from the name
            Helper.ResponseType status = Helper.SendJoinRoomRequest(id); // Button name is the room id

            switch (status)
            {
                case Helper.ResponseType.OK:
                    isThreadRunning = false;
                    this.NavigationService.Navigate(new WaitingRoomPage(RoomsData[id]));
                    break;
                default:
                    MessageBox.Show("Cannot join room!");
                    break;
            }
        }

        private void HighlightRoom_Hover(object sender, MouseEventArgs e)
        {
            if (sender is Grid roomRow) // Every row is a grid by itself
            {
                ColorAnimation animation = new()
                {
                    To = Color.FromArgb(128, 173, 216, 255),
                    Duration = TimeSpan.FromMilliseconds(200)
                };
                roomRow.Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
            }
        }

        private void ClearRooms()
        {
            this.RoomsStackPanel.Children.Clear();
        }

        private void ShowRooms()
        {
            if (RoomsData.Count == 0) 
                return;

            foreach ((string id, Room room) in RoomsData)
            {
                // Adding all properties to the grid to match the xaml reference
                Grid roomGrid = new()
                {
                    Name = "room" + id,
                    Background = Brushes.Transparent
                };
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(50) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });
                roomGrid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(100) });

                Polygon polygon1 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(42, 0), new Point(50, 32), new Point(0, 32)],
                    Margin = new Thickness(0) // Override style
                };
                roomGrid.Children.Add(polygon1);
                Grid.SetColumn(polygon1, 0);


                Image image = new()
                {
                    Width = 25,                    
                    VerticalAlignment = VerticalAlignment.Center,
                };

                if (room.state == (int)RoomStatus.OPEN)
                {
                    image.Source = new BitmapImage(new Uri("Images/White/enter.png", UriKind.Relative));
                    Button button = new()
                    {
                        Name = "button" + id, // Affects JoinRoom()
                        BorderThickness = new Thickness(0),
                        BorderBrush = new SolidColorBrush(Colors.Transparent),
                        Background = new SolidColorBrush(Colors.Transparent),
                        ToolTip = "Enter this room!",
                        Cursor = Cursors.Hand,
                        Foreground = new SolidColorBrush(Colors.Transparent),
                        Content = image                        
                    };

                    button.Click += JoinRoom;

                    Helper.RemoveButtonHighlighting(button);
                    roomGrid.Children.Add(button);
                }
                else
                {
                    image.Source = new BitmapImage(new Uri("Images/White/closed.png", UriKind.Relative));
                    image.ToolTip = "Too late to enter...";
                    image.Cursor = Cursors.Help;
                    roomGrid.Children.Add(image);
                }

                Polygon polygon2 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(138, 0), new Point(146, 32), new Point(8, 32)]
                };
                roomGrid.Children.Add(polygon2);
                Grid.SetColumn(polygon2, 1);

                TextBlock textBlock1 = new()
                {
                    Text = room.name,
                    Style = (Style)FindResource("cell")
                };
                roomGrid.Children.Add(textBlock1);
                Grid.SetColumn(textBlock1, 1);

                Polygon polygon3 = new()
                {
                    Style = (Style)FindResource("parallelogram"),
                    Points = [new Point(0, 0), new Point(100, 0), new Point(108, 32), new Point(8, 32)],
                    Margin = new Thickness(-18, 0, 0, 0),
                    Fill = (Brush)FindResource("gradient")
                };
                roomGrid.Children.Add(polygon3);
                Grid.SetColumn(polygon3, 2);

                TextBlock textBlock2 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Margin = new Thickness(-10, 0, 0, 0),
                    Text = Helper.SendGetPlayersInRoomRequest(id).Count + " / " + room.maxPlayers
                };
                roomGrid.Children.Add(textBlock2);
                Grid.SetColumn(textBlock2, 2);

                Polygon polygon4 = new()
                {
                    Points = [new Point(0, 0), new Point(110, 0), new Point(118, 32), new Point(8, 32)],
                    Style = (Style)FindResource("parallelogram"),
                    Margin = new Thickness(-18, 0, 0, 0)
                };
                roomGrid.Children.Add(polygon4);
                Grid.SetColumn(polygon4, 3);

                TextBlock textBlock3 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Text = room.questionCount.ToString(),
                    Margin = new Thickness(-10, 0, 0, 0)
                };
                roomGrid.Children.Add(textBlock3);
                Grid.SetColumn(textBlock3, 3);

                Polygon polygon5 = new()
                {
                    Points = [new Point(0, 0), new Point(108, 0), new Point(108, 32), new Point(8, 32)],
                    Margin = new Thickness(-8, 0, 0, 0),
                    Style = (Style)FindResource("parallelogram")
                };
                roomGrid.Children.Add(polygon5);
                Grid.SetColumn(polygon5, 4);

                TextBlock textBlock4 = new()
                {
                    Style = (Style)FindResource("cell"),
                    Text = room.questionTimeout.ToString()
                };
                roomGrid.Children.Add(textBlock4);
                Grid.SetColumn(textBlock4, 4);


                // Create EventTrigger objects for MouseEnter and MouseLeave
                EventTrigger enterTrigger = new()
                {
                    RoutedEvent = Button.MouseEnterEvent  // Set the event to trigger on
                };

                EventTrigger leaveTrigger = new()
                {
                    RoutedEvent = Button.MouseLeaveEvent
                };

                // Add BeginStoryboard actions to the timelines
                BeginStoryboard enterAction = new()
                {
                    Storyboard = (Storyboard)FindResource("fadeIn")  // Reference the fadeIn Storyboard
                };
                enterTrigger.Actions.Add(enterAction);

                BeginStoryboard leaveAction = new()
                {
                    Storyboard = (Storyboard)FindResource("fadeOut") // Reference the fadeOut Storyboard
                };
                leaveTrigger.Actions.Add(leaveAction);

                // Add the triggers to the element
                roomGrid.Triggers.Add(enterTrigger);
                roomGrid.Triggers.Add(leaveTrigger);

                RoomsStackPanel.Children.Add(roomGrid);
            }
        }
    }
}
