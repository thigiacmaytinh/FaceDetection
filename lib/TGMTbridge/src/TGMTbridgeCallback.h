//vohungvi: this class use for example callback

#pragma once

using namespace System::Drawing;
using System::IntPtr;
using System::Runtime::InteropServices::Marshal;
using namespace::System;

namespace TGMT
{
	public delegate void CallbackDelegate(Bitmap^ bmp);
	public ref class TGMTbridgeCallback
	{
		CallbackDelegate^ m_handler;
	public:
		void LoadBitmap(String^ filePath);



		//using in C# with syntax when SetCallback is static :
		/*TGMTbridgeCallback.SetCallback(bmp => pictureBox1.Image = bmp); */

		//or syntax when SetCallback is member function
		/*
			TGMTbridgeCallback bridge = new TGMTbridgeCallback();
            bridge.SetCallback(bmp => picCamera.Image = bmp);
            bridge.LoadBitmap("D:\\image.jpg");
		*/

		void __stdcall SetCallback(CallbackDelegate^ handler)
		{			
			m_handler = handler;
		};
	};

}