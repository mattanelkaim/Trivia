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
        static private readonly string IP = "127.0.0.1"; // Loopback
        static private readonly ushort PORT = 7777;
        static private readonly int BUFFER_SIZE = 256;

        /// <summary>
        /// Connects to the server using the specified IP address and port.
        /// </summary>
        public static void Connect()
        {
            client = new TcpClient(IP, PORT);
            stream = client.GetStream();
        }

        /// <summary>
        /// Sends a message to the server.
        /// </summary>
        /// <param name="message">The message to send (as it is).</param>
        public static void Send(string message)
        {
            // Convert the message to bytes and then send it
            byte[] data = Encoding.ASCII.GetBytes(message);
            stream.Write(data, 0, data.Length);

            // Flush the stream to make sure all data is sent
            stream.Flush();
        }

        /// <summary>
        /// Receives a message from the server.
        /// </summary>
        /// <returns>The received message.</returns>
        public static string Receive()
        {
            // Read the data from the socket stream
            byte[] data = new byte[BUFFER_SIZE];
            int bytes = stream.Read(data, 0, data.Length);

            // Convert the bytes to a string
            return Encoding.ASCII.GetString(data, 0, bytes);
        }
    }
}
