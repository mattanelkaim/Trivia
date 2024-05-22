using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;

namespace ClientGUI
{
    internal class Communicator
    {
        static private TcpClient client;
        static private NetworkStream stream;
        static private string IP = "127.0.0.0";
        static private short port = 7777;

        public static void Connect()
        {
            client = new TcpClient(IP, port);
            stream = client.GetStream();
        }
    }
}
