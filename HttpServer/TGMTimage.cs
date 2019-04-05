using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace TGMTcs
{
    class TGMTimage
    {
        const int BUF_SIZE = 512 * 1024;
        static byte[] buffer = new byte[BUF_SIZE];

        public static Bitmap CorrectOrientation(Bitmap bmp)
        {
            if (Array.IndexOf(bmp.PropertyIdList, 274) > -1)
            {
                var orientation = (int)bmp.GetPropertyItem(274).Value[0];
                switch (orientation)
                {
                    case 1:
                        // No rotation required.
                        break;
                    case 2:
                        bmp.RotateFlip(RotateFlipType.RotateNoneFlipX);
                        break;
                    case 3:
                        bmp.RotateFlip(RotateFlipType.Rotate180FlipNone);
                        break;
                    case 4:
                        bmp.RotateFlip(RotateFlipType.Rotate180FlipX);
                        break;
                    case 5:
                        bmp.RotateFlip(RotateFlipType.Rotate90FlipX);
                        break;
                    case 6:
                        bmp.RotateFlip(RotateFlipType.Rotate90FlipNone);
                        break;
                    case 7:
                        bmp.RotateFlip(RotateFlipType.Rotate270FlipX);
                        break;
                    case 8:
                        bmp.RotateFlip(RotateFlipType.Rotate270FlipNone);
                        break;
                }
                // This EXIF data is now invalid and should be removed.
                bmp.RemovePropertyItem(274);
            }
            return bmp;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string ImageToBase64(string filename)
        {
            FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read);
            byte[] filebytes = new byte[fs.Length];
            fs.Read(filebytes, 0, Convert.ToInt32(fs.Length));
            fs.Close();
            return "data:image/png;base64," + Convert.ToBase64String(filebytes, Base64FormattingOptions.None);

        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static Image Base64ToImage(string base64String)
        {
            // Convert base 64 string to byte[]
            byte[] imageBytes = Convert.FromBase64String(base64String);
            // Convert byte[] to Image
            using (var ms = new MemoryStream(imageBytes, 0, imageBytes.Length))
            {
                Image image = Image.FromStream(ms, true);
                return image;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string ImageToBase64(Image image)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                // Convert Image to byte[]
                image.Save(ms, image.RawFormat);
                byte[] imageBytes = ms.ToArray();

                // Convert byte[] to base 64 string
                string base64String = Convert.ToBase64String(imageBytes);
                return base64String;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool ConvertCr2(string inputPath, string outputPath, bool rotateIfNeeded, Size size)
        {
            if (!File.Exists(inputPath))
                return false;

            FileStream fi = new FileStream(inputPath, FileMode.Open, FileAccess.Read,
                                                   FileShare.Read, BUF_SIZE, FileOptions.None);
            // Start address is at offset 0x62, file size at 0x7A, orientation at 0x6E
            fi.Seek(0x62, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fi);
            uint jpgStartPosition = br.ReadUInt32();  // 62
            br.ReadUInt32();  // 66
            br.ReadUInt32();  // 6A
            uint orientation = br.ReadUInt32() & 0x000000FF; // 6E
            br.ReadUInt32();  // 72
            br.ReadUInt32();  // 76
            int fileSize = br.ReadInt32();  // 7A

            fi.Seek(jpgStartPosition, SeekOrigin.Begin);

            if (fi.ReadByte() != 0xFF || fi.ReadByte() != 0xD8)
            {
                fi.Close();
                return false;
            }
            else
            {
                Bitmap bmp = new Bitmap(new PartialStream(fi, jpgStartPosition, fileSize));

                //resize
                if (!size.IsEmpty)
                {
                    int width = size.Width;
                    int height = size.Height;

                    if (height == 0)
                    {
                        double aspect = (double)bmp.Width / bmp.Height;
                        height = (int)(width / aspect);
                    }

                    Bitmap result = new Bitmap(width, height);
                    using (Graphics g = Graphics.FromImage(result))
                    {
                        g.DrawImage(bmp, 0, 0, width, height);
                    }

                    bmp = result;
                }
                               

                //rotate
                if (rotateIfNeeded)
                {
                    if (orientation == 8)
                        bmp.RotateFlip(RotateFlipType.Rotate270FlipNone);
                    else if (orientation == 6)
                        bmp.RotateFlip(RotateFlipType.Rotate90FlipNone);
                }

                string ext = outputPath.Substring(outputPath.Length - 3);
                if(ext == "jpg")
                {
                    //save jpg
                    bmp.Save(outputPath, ImageFormat.Jpeg);
                }
                else
                {
                    //save png
                    bmp.Save(outputPath, ImageFormat.Png);
                }                
            }

            fi.Close();
            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private static ImageCodecInfo GetJpegCodec()
        {
            foreach (ImageCodecInfo c in ImageCodecInfo.GetImageEncoders())
            {
                if (c.CodecName.ToLower().Contains("jpeg")
                    || c.FilenameExtension.ToLower().Contains("*.jpg")
                    || c.FormatDescription.ToLower().Contains("jpeg")
                    || c.MimeType.ToLower().Contains("image/jpeg"))
                    return c;
            }

            return null;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool IsImage(string fileName)
        {
            if (fileName.Contains(" "))
            {
                return false;
            }
            string ext = Path.GetExtension(fileName).ToLower();
            return (ext == ".jpg" || ext == ".png" || ext == ".bmp");
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public static bool IsBase64(string base64String)
        {
            // Credit: oybek https://stackoverflow.com/users/794764/oybek
            if (base64String == null || base64String.Length == 0 || base64String.Length % 4 != 0
               || base64String.Contains(" ") || base64String.Contains("\t") || base64String.Contains("\r") || base64String.Contains("\n"))
                return false;

            return true;            
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        class PartialStream : Stream  // Fun solution and experiment... probably not the best idea here
        {
            internal PartialStream(FileStream p_f, uint p_start, int p_length)
            {
                m_f = p_f;
                m_start = p_start;
                m_length = p_length;

                m_f.Seek(p_start, SeekOrigin.Begin);
            }

            FileStream m_f;
            uint m_start;
            int m_length;

            public override IAsyncResult BeginRead(byte[] buffer, int offset, int count, AsyncCallback callback, object state)
            {
                return m_f.BeginRead(buffer, offset, count, callback, state);
            }
            public override IAsyncResult BeginWrite(byte[] buffer, int offset, int count, AsyncCallback callback, object state)
            {
                return m_f.BeginWrite(buffer, offset, count, callback, state);
            }
            public override bool CanRead
            {
                get { return m_f.CanRead; }
            }
            public override bool CanSeek
            {
                get { return m_f.CanSeek; }
            }
            public override bool CanTimeout
            {
                get { return m_f.CanTimeout; }
            }
            public override bool CanWrite
            {
                get { return m_f.CanWrite; }
            }
            public override void Close()
            {
                m_f.Close();
            }
            public override System.Runtime.Remoting.ObjRef CreateObjRef(Type requestedType)
            {
                return m_f.CreateObjRef(requestedType);
            }
            protected override void Dispose(bool disposing)
            {
                //m_f.Dispose(disposing); // Can't...
                base.Dispose(disposing);
            }
            public override int EndRead(IAsyncResult asyncResult)
            {
                return m_f.EndRead(asyncResult);
            }
            public override void EndWrite(IAsyncResult asyncResult)
            {
                m_f.EndWrite(asyncResult);
            }
            public override bool Equals(object obj)
            {
                return m_f.Equals(obj);
            }
            public override void Flush()
            {
                m_f.Flush();
            }
            public override int GetHashCode()
            {
                return m_f.GetHashCode();
            }
            public override object InitializeLifetimeService()
            {
                return m_f.InitializeLifetimeService();
            }
            public override long Length
            {
                get { return m_length; }
            }
            public override long Position
            {
                get { return m_f.Position - m_start; }
                set { m_f.Position = value + m_start; }
            }
            public override int Read(byte[] buffer, int offset, int count)
            {
                long maxRead = Length - Position;
                return m_f.Read(buffer, offset, (count <= maxRead) ? count : (int)maxRead);
            }
            public override int ReadByte()
            {
                if (Position < Length)
                    return m_f.ReadByte();
                else
                    return 0;
            }
            public override int ReadTimeout
            {
                get { return m_f.ReadTimeout; }
                set { m_f.ReadTimeout = value; }
            }
            public override void SetLength(long value)
            {
                m_f.SetLength(value);
            }
            public override string ToString()
            {
                return m_f.ToString();
            }
            public override void Write(byte[] buffer, int offset, int count)
            {
                m_f.Write(buffer, offset, count);
            }
            public override void WriteByte(byte value)
            {
                m_f.WriteByte(value);
            }
            public override int WriteTimeout
            {
                get { return m_f.WriteTimeout; }
                set { m_f.WriteTimeout = value; }
            }
            public override long Seek(long offset, SeekOrigin origin)
            {
                return m_f.Seek(offset + m_start, origin);
            }
        }

    }
}
