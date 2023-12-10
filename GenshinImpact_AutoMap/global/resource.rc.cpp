#include "resource.rc.h"
#include "../resource.h"

cv::Mat get_png_resource(int idb)
{
	HMODULE module_handle = NULL;
	HRSRC gi_map_handle = NULL;
	HGLOBAL gi_map_resource_handle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	module_handle = GetModuleHandle(NULL);

	gi_map_handle = FindResource(module_handle, MAKEINTRESOURCE(IDB_PNG_GIMAP), L"PNG");
	gi_map_resource_handle = LoadResource(module_handle, gi_map_handle);
	pImageFile = LockResource(gi_map_resource_handle);
	imageFileSize = SizeofResource(module_handle, gi_map_handle);
	return cv::imdecode(cv::Mat(1, imageFileSize, CV_8UC1, pImageFile), cv::IMREAD_UNCHANGED);
}

cv::Mat get_bmp_resource(int idb,bool is_alpha = false)
{	
	auto bmp_handle = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(idb));
	BITMAP bmp;
	GetObject(bmp_handle, sizeof(BITMAP), &bmp);
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	int depth = bmp.bmBitsPixel == 1 ? 1 : 8;
	
	cv::Mat v_mat;
	v_mat.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8UC3, nChannels));
	GetBitmapBits(bmp_handle, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);
	if (nChannels == 4 && is_alpha == false)
		cv::cvtColor(v_mat, v_mat, cv::COLOR_RGBA2RGB);
    return v_mat;
}

resource_rc::resource_rc()
{
	load();
}

void resource_rc::load()
{
	auto map = get_png_resource(IDB_PNG_GIMAP);
	images.emplace("map", map);
	auto mask = get_bmp_resource(IDB_MAINMASK);
	images.emplace("mask", mask);

}
