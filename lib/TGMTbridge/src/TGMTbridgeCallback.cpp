#include "TGMTbridgeCallback.h"

using namespace TGMT;

void TGMTbridgeCallback::LoadBitmap(String^ filePath)
{
	Bitmap^ bmp = gcnew Bitmap(filePath);
	m_handler(bmp);
}
