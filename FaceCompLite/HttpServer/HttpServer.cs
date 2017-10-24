using System;
using System.Collections;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Drawing;
using TGMT;

// offered to the public domain for any use with no restriction
// and also with no warranty of any kind, please enjoy. - David Jeske. 

// simple HTTP explanation
// http://www.jmarshall.com/easy/http/

namespace Bend.Util
{


    public class HttpProcessor
    {
        public TcpClient m_socket;
        public HttpServer m_server;

        private Stream m_inputStream;
        public StreamWriter m_outputStream;

        public String http_method;
        public String http_url;
        public String http_protocol_versionstring;
        public Hashtable httpHeaders = new Hashtable();


        private static int MAX_POST_SIZE = 10 * 1024 * 1024; // 10MB

        public HttpProcessor(TcpClient s, HttpServer srv)
        {
            this.m_socket = s;
            this.m_server = srv;
        }


        private string StreamReadLine(Stream inputStream)
        {
            int next_char;
            string data = "";
            while (true)
            {
                next_char = inputStream.ReadByte();
                if (next_char == '\n') { break; }
                if (next_char == '\r') { continue; }
                if (next_char == -1) { Thread.Sleep(1); continue; };
                data += Convert.ToChar(next_char);
            }
            return data;
        }
        public void process()
        {
            // we can't use a StreamReader for input, because it buffers up extra data on us inside it's
            // "processed" view of the world, and we want the data raw after the headers
            m_inputStream = new BufferedStream(m_socket.GetStream());

            // we probably shouldn't be using a streamwriter for all output from handlers either
            m_outputStream = new StreamWriter(new BufferedStream(m_socket.GetStream()));
            try
            {
                ParseRequest();
                ReadHeaders();
                if (http_method.Equals("GET"))
                {
                    HandleGETRequest();
                }
                else if (http_method.Equals("POST"))
                {
                    HandlePOSTRequest();
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception: " + e.ToString());
                WriteFailure();
            }
            m_outputStream.Flush();
            // bs.Flush(); // flush any remaining output
            m_inputStream = null; m_outputStream = null; // bs = null;            
            m_socket.Close();
        }

        public void ParseRequest()
        {
            String request = StreamReadLine(m_inputStream);
            string[] tokens = request.Split(' ');
            if (tokens.Length != 3)
            {
                throw new Exception("invalid http request line");
            }
            http_method = tokens[0].ToUpper();
            http_url = tokens[1];
            http_protocol_versionstring = tokens[2];
        }

        public void ReadHeaders()
        {
            String line;
            while ((line = StreamReadLine(m_inputStream)) != null)
            {
                if (line.Equals(""))
                {
                    return;
                }

                int separator = line.IndexOf(':');
                if (separator == -1)
                {
                    throw new Exception("invalid http header line: " + line);
                }
                String name = line.Substring(0, separator);
                int pos = separator + 1;
                while ((pos < line.Length) && (line[pos] == ' '))
                {
                    pos++; // strip any spaces
                }

                string value = line.Substring(pos, line.Length - pos);
                //Console.WriteLine("header: {0}:{1}", name, value);
                httpHeaders[name] = value;
            }
        }

        public void HandleGETRequest()
        {
            m_server.HandleGETRequest(this);
        }

        private const int BUF_SIZE = 4096;
        public void HandlePOSTRequest()
        {
            // this post data processing just reads everything into a memory stream.
            // this is fine for smallish things, but for large stuff we should really
            // hand an input stream to the request processor. However, the input stream 
            // we hand him needs to let him see the "end of the stream" at this content 
            // length, because otherwise he won't know when he's seen it all! 

            int content_len = 0;
            MemoryStream ms = new MemoryStream();
            if (this.httpHeaders.ContainsKey("Content-Length") || this.httpHeaders.ContainsKey("content-length"))
            {
                content_len = Convert.ToInt32(this.httpHeaders["Content-Length"]);
                if (content_len == 0)
                    content_len = Convert.ToInt32(this.httpHeaders["content-length"]);
                if (content_len > MAX_POST_SIZE)
                {
                    throw new Exception(
                        String.Format("POST Content-Length({0}) too big for this simple server",
                          content_len));
                }
                byte[] buf = new byte[BUF_SIZE];
                int to_read = content_len;
                while (to_read > 0)
                {
                    int numread = this.m_inputStream.Read(buf, 0, Math.Min(BUF_SIZE, to_read));
                    if (numread == 0)
                    {
                        if (to_read == 0)
                        {
                            break;
                        }
                        else
                        {
                            throw new Exception("client disconnected during post");
                        }
                    }
                    to_read -= numread;
                    ms.Write(buf, 0, numread);
                }
                ms.Seek(0, SeekOrigin.Begin);
            }

            m_server.HandlePOSTRequest(this, ms);

        }

        public void WriteSuccess(string content_type = "text/html")
        {
            m_outputStream.WriteLine("HTTP/1.0 200 OK");
            m_outputStream.WriteLine("Content-Type: " + content_type);
            m_outputStream.WriteLine("Connection: close");
            m_outputStream.WriteLine("");
        }

        public void WriteFailure()
        {
            m_outputStream.WriteLine("HTTP/1.0 404 File not found");
            m_outputStream.WriteLine("Connection: close");
            m_outputStream.WriteLine("");
        }
    }

    public class HttpServer
    {
        FaceCompBridge faceComp;// = new FaceCompBridge();

        protected int port;
        TcpListener listener;
        bool is_active = true;

        public HttpServer(int port)
        {
            faceComp = new FaceCompBridge();
            Console.WriteLine("Server ready");
            this.port = port;
        }

        public void Listen()
        {
            listener = new TcpListener(port);
            listener.Start();
            while (is_active)
            {
                TcpClient s = listener.AcceptTcpClient();
                HttpProcessor processor = new HttpProcessor(s, this);
                Thread thread = new Thread(new ThreadStart(processor.process));
                thread.Start();
                Thread.Sleep(1);
            }
        }

        public void HandleGETRequest(HttpProcessor p)
        {
            Console.WriteLine("request: {0}", p.http_url);
            p.WriteSuccess();
            p.m_outputStream.WriteLine("<html><body><h1>test server</h1>");
            p.m_outputStream.WriteLine("Current Time: " + DateTime.Now.ToString());
            p.m_outputStream.WriteLine("url : {0}", p.http_url);
        }

        public void HandlePOSTRequest(HttpProcessor p, MemoryStream inputData)
        {
            Console.WriteLine(">>>>>>>>>>>>>>>>>>>>>>>>>new POST request: {0}", p.http_url);
            var watch = System.Diagnostics.Stopwatch.StartNew();


            bool cropped = false;
            if (p.httpHeaders.ContainsKey("cropped") && (string)p.httpHeaders["cropped"] == "true")
            {
                cropped = true;
            }
            Bitmap bmp;
            string result = "{\"confident\":0,\"error\":\"\",\"person\":\"\"}";
            try
            {
                bmp = new Bitmap(inputData);
                bmp = TGMTimage.CorrectOrientation(bmp);                

                result = faceComp.RecogImage(bmp, cropped);
            }
            catch (Exception ex)
            {
                result = "{\"confident\":0,\"error\":\"Can not read image\",\"person\":\"\"}";
            }
            p.WriteSuccess();

            p.m_outputStream.WriteLine(result);
            watch.Stop();
            Console.WriteLine("Elapsed time: " + watch.ElapsedMilliseconds);
        }
    }

    public class TestMain
    {
        public static int Main(String[] args)
        {


            HttpServer httpServer;
            if (args.GetLength(0) > 0)
            {
                httpServer = new HttpServer(Convert.ToInt16(args[0]));
            }
            else
            {
                httpServer = new HttpServer(8080);
            }
            Thread thread = new Thread(new ThreadStart(httpServer.Listen));
            thread.Start();
            return 0;
        }

    }

}



