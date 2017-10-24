// FaceCompBridge.h

#pragma once
using namespace System::Drawing;
using namespace System;

public ref class FaceCompBridge
{
	
public:
	FaceCompBridge();

	~FaceCompBridge();
	//void Init();
	String^ RecogImage(Bitmap^ bmp, bool cropped);
	String^ RecogImage(String^ imgPath, bool cropped);
};


