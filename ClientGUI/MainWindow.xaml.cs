using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.Json;

namespace ClientGUI
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.LoginSubmit.Click += new RoutedEventHandler(LoginSubmit_Click);

            try
            {
                Communicator.Connect();
            }
            catch (Exception e)
            {
                MessageBox.Show("Connection failed: " + e.Message);
            }
        }

        private void LoginSubmit_Click(object sender, RoutedEventArgs e)
        {
            string json = JsonSerializer.Serialize(new { username = this.Username.Text, password = this.Password.Text });

            string msg = Helper.Serialize(json, Helper.MessageType.Login);
            MessageBox.Show("[Sending]: " + msg);
            Communicator.Send(msg);

            string response = Communicator.Receive();
            MessageBox.Show("[Received]: " + response);
        }
    }
}
