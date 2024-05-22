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

namespace ClientGUI
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.LoginSubmit.Click += new RoutedEventHandler(LoginSubmit_Click);
        }

        private void LoginSubmit_Click(object sender, RoutedEventArgs e)
        {
            string username = this.Username.Text;
            string password = this.Password.Text;

            if (username == "admin" && password == "admin")
            {
                MessageBox.Show("Login Successful");
            }
            else
            {
                MessageBox.Show("Login Failed");
            }
        }
    }
}