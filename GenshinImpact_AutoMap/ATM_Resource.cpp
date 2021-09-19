#include "ATM_Resource.h"
#include <wincodec.h>

ATM_Resource::ATM_Resource()
{
	hGIOBJICON = new HBITMAP[13];//OBJ
	hGIOBJICONMASK = new HBITMAP[13];//OBJ
	hGIOBJFLAGICON = new HBITMAP[2];
	hGIOBJFLAGICONMASK = new HBITMAP[2];
	hGIPAIMON = new HBITMAP[11];
	hGINUMUID = new HBITMAP[11];//NUM


	GIOBJICON = new Mat[13];//OBJ
	GIOBJICONMASK = new Mat[13];//OBJ
	GIOBJFLAGICON = new Mat[2];
	GIOBJFLAGICONMASK = new Mat[2];
	GIPAIMON = new Mat[4];
	GINUMUID = new Mat[11];//NUM

	loadPngGiMap();
	//loadGiMap();
	loadMainMask();
	loadGiAvatar();
	loadGiAvatarMask();
	loadGiStar();
	loadGiStarMask();
	loadGiPaimon();
	loadGiObjIcon();
	loadGiObjIconMask();
	loadGiObjFlagIcon();
	loadGiObjFlagIconMask();
	loadGiNumUID();
}

ATM_Resource::~ATM_Resource()
{
	delete[] hGIOBJICON;
	delete[] hGIOBJICONMASK;
	delete[] hGIOBJFLAGICON;
	delete[] hGIOBJFLAGICONMASK;
	delete[] hGIPAIMON;
	delete[] hGINUMUID;

	delete[] GIOBJICON;
	delete[] GIOBJICONMASK;
	delete[] GIOBJFLAGICON;
	delete[] GIOBJFLAGICONMASK;
	delete[] GIPAIMON;
	delete[] GINUMUID;
}

void ATM_Resource::loadPngGiMap()
{
	HMODULE hModu = NULL;
	IWICStream *pIWICStream = NULL;
	IWICBitmapDecoder *pIDecoder = NULL;
	IWICBitmapFrameDecode *pIDecoderFrame = NULL;
	IWICImagingFactory *m_pIWICFactory = NULL;
	IWICBitmapSource *bitmap_source = NULL; 
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void *pImageFile = NULL;
	DWORD imageFileSize = 0;

	hModu = GetModuleHandle(0);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pIWICFactory)
	);

	imageResHandle = FindResource(hModu, MAKEINTRESOURCE(IDB_PNG_GIMAP), L"PNG");
	imageResDataHandle = LoadResource(hModu, imageResHandle);
	pImageFile = LockResource(imageResDataHandle);
	imageFileSize = SizeofResource(hModu, imageResHandle);
	m_pIWICFactory->CreateStream(&pIWICStream);

	pIWICStream->InitializeFromMemory(
		reinterpret_cast<BYTE*>(pImageFile),
		imageFileSize);
	m_pIWICFactory->CreateDecoderFromStream(
		pIWICStream,                   // The stream to use to create the decoder
		NULL,                          // Do not prefer a particular vendor
		WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
		&pIDecoder);                   // Pointer to the decoder
	pIDecoder->GetFrame(0, &pIDecoderFrame);

	bitmap_source = pIDecoderFrame;

	UINT width = 0, height = 0, depht = 4;
	bitmap_source->GetSize(&width, &height);

	{
		std::vector<BYTE> buffer(width * height * depht);
		bitmap_source->CopyPixels(NULL, width * depht, buffer.size(), buffer.data());

		hGIMAP = CreateBitmap(width, height, 1, 8 * depht, buffer.data());

		HBitmap2Mat(hGIMAP, GIMAP);

		DeleteObject(hGIMAP);
	}
	DeleteObject(bitmap_source);
	CoUninitialize();

}

//void ATM_Resource::loadGiMap()
//{
//	hGIMAP = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GIMAP));
//	HBitmap2Mat(hGIMAP, GIMAP);
//	DeleteObject(hGIMAP);
//}

void ATM_Resource::loadMainMask()
{
	hMAINMASK = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_MAINMASK));
	HBitmap2Mat(hMAINMASK, MAINMASK);
	DeleteObject(hMAINMASK);
}

void ATM_Resource::loadGiAvatar()
{
	hGIAVATAR = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GIAVATAR));
	//HBitmap2Mat(hGIAVATAR, GIAVATAR);
	HBitmap2MatAlpha(hGIAVATAR, GIAVATAR);
	DeleteObject(hGIAVATAR);
}

void ATM_Resource::loadGiAvatarMask()
{
	hGIAVATARMASK = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GIAVATARMASK));
	HBitmap2MatAlpha(hGIAVATARMASK, GIAVATARMASK);
	DeleteObject(hGIAVATARMASK);
}

void ATM_Resource::loadGiStar()
{
	hGISTAR = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GISTAR));
	HBitmap2Mat(hGISTAR, GISTAR);
	DeleteObject(hGISTAR);
}

void ATM_Resource::loadGiStarMask()
{
	hGISTARMASK = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GISTARMASK));
	HBitmap2Mat(hGISTARMASK, GISTARMASK);
	DeleteObject(hGISTARMASK);
}

void ATM_Resource::loadGiPaimon()
{
	hGIPAIMON[0]= LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_PAIMON1920X1080));
	HBitmap2Mat(hGIPAIMON[0], GIPAIMON[0]);
	DeleteObject(hGIPAIMON[0]);
	hGIPAIMON[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_PAIMON1680X1050));
	HBitmap2Mat(hGIPAIMON[1], GIPAIMON[1]);
	DeleteObject(hGIPAIMON[1]);
	hGIPAIMON[2] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_PAIMON1440X900));
	HBitmap2Mat(hGIPAIMON[2], GIPAIMON[2]);
	DeleteObject(hGIPAIMON[2]);
	hGIPAIMON[3] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_PAIMON1366X768));
	HBitmap2Mat(hGIPAIMON[3], GIPAIMON[3]);
	DeleteObject(hGIPAIMON[3]);
}

void ATM_Resource::loadGiObjIcon()
{
	int i = 0;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON0));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON1));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON2));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON3));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON4));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);

	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON93));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON94));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);

	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON101));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON102));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON103));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON104));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON105));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);

	i++;
	hGIOBJICON[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON205));
	HBitmap2Mat(hGIOBJICON[i], GIOBJICON[i]);
	DeleteObject(hGIOBJICON[i]);
}

void ATM_Resource::loadGiObjIconMask()
{
	int i = 0;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON0MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON1MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);

	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON2MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON3MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON4MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);

	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON93MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON94MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);

	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON101MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON102MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON103MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON104MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);

	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON105MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);

	i++;
	hGIOBJICONMASK[i] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJICON205MASK));
	HBitmap2Mat(hGIOBJICONMASK[i], GIOBJICONMASK[i]);
	Mat2MaskMat(GIOBJICONMASK[i], GIOBJICONMASK[i]);
	DeleteObject(hGIOBJICONMASK[i]);
}

void ATM_Resource::loadGiObjFlagIcon()
{
	hGIOBJFLAGICON[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJFLAGICON0));
	HBitmap2Mat(hGIOBJFLAGICON[0], GIOBJFLAGICON[0]);
	DeleteObject(hGIOBJFLAGICON[0]);
	hGIOBJFLAGICON[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJFLAGICON1));
	HBitmap2Mat(hGIOBJFLAGICON[1], GIOBJFLAGICON[1]);
	DeleteObject(hGIOBJFLAGICON[1]);
}

void ATM_Resource::loadGiObjFlagIconMask()
{
	hGIOBJFLAGICONMASK[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJFLAGICON0MASK));
	HBitmap2Mat(hGIOBJFLAGICONMASK[0], GIOBJFLAGICONMASK[0]);
	Mat2MaskMat(GIOBJFLAGICONMASK[0], GIOBJFLAGICONMASK[0]);
	hGIOBJFLAGICONMASK[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_OBJFLAGICON1MASK));
	HBitmap2Mat(hGIOBJFLAGICONMASK[1], GIOBJFLAGICONMASK[1]);
	Mat2MaskMat(GIOBJFLAGICONMASK[1], GIOBJFLAGICONMASK[1]);
}

void ATM_Resource::loadGiNumUID()
{
	hGINUMUID[0] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID0_ARGB));
	HBitmap2MatAlpha(hGINUMUID[0], GINUMUID[0]);
	hGINUMUID[1] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID1_ARGB));
	HBitmap2MatAlpha(hGINUMUID[1], GINUMUID[1]);
	hGINUMUID[2] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID2_ARGB));
	HBitmap2MatAlpha(hGINUMUID[2], GINUMUID[2]);
	hGINUMUID[3] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID3_ARGB));
	HBitmap2MatAlpha(hGINUMUID[3], GINUMUID[3]);
	hGINUMUID[4] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID4_ARGB));
	HBitmap2MatAlpha(hGINUMUID[4], GINUMUID[4]);
	hGINUMUID[5] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID5_ARGB));
	HBitmap2MatAlpha(hGINUMUID[5], GINUMUID[5]);
	hGINUMUID[6] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID6_ARGB));
	HBitmap2MatAlpha(hGINUMUID[6], GINUMUID[6]);
	hGINUMUID[7] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID7_ARGB));
	HBitmap2MatAlpha(hGINUMUID[7], GINUMUID[7]);
	hGINUMUID[8] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID8_ARGB));
	HBitmap2MatAlpha(hGINUMUID[8], GINUMUID[8]);
	hGINUMUID[9] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID9_ARGB));
	HBitmap2MatAlpha(hGINUMUID[9], GINUMUID[9]);
	hGINUMUID[10] = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_GINUMUID_ARGB));
	HBitmap2MatAlpha(hGINUMUID[10], GINUMUID[10]);
}

void ATM_Resource::loadFromResource()
{
	//:/png/resource/ItemsPng/UIObjList0.png
}

bool ATM_Resource::HBitmap2Mat(HBITMAP & _hBmp, cv::Mat & _mat)
{
	//BITMAP操作
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	//mat操作
	cv::Mat v_mat;
	v_mat.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data);
	_mat = v_mat;
	if (nChannels == 4)
	{
		cv::cvtColor(v_mat, _mat, CV_RGBA2RGB);
		return true;
	}
	return false;
}

//带Alpha通道的32位Bmp图片
bool ATM_Resource::HBitmap2MatAlpha(HBITMAP & _hBmp, cv::Mat & _mat)
{
	//BITMAP操作
	BITMAP bmp;
	GetObject(_hBmp, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	//mat操作
	cv::Mat v_mat;
	v_mat.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, v_mat.data);
	_mat = v_mat;
	return true;
}

bool ATM_Resource::Mat2MaskMat(Mat & in, Mat & out)
{
	std::vector<Mat> mv0;
	//通道分离
	split(in, mv0);
	out = mv0[0];

	return false;
}

QPixmap ATM_Resource::QImage2QPixmap(QImage & in)
{
	QPixmap out;
	return out.fromImage(in);
}

QImage ATM_Resource::QPixmap2QImage(QPixmap & in)
{
	return in.toImage();
}

Mat ATM_Resource::QImage2Mat(QImage & in)
{
	return Mat(in.height(), in.width(), CV_8UC4, (uchar*)in.bits(), in.bytesPerLine());
}

QImage ATM_Resource::Mat2QImage(Mat & in)
{
	Mat rgb;
	cvtColor(in, rgb, CV_BGR2RGB);
	return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
}
