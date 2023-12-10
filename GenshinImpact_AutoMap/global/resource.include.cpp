#include "resource.include.h"
#include "resource.rc.h"
#include "resource.qrc.h"


resource::resource()
{
	auto rc = resource_rc();
	auto qrc = resource_qrc();
	
	main_mask = rc.get_image("mask").value_or(cv::Mat());
    gi_map = rc.get_image("map").value_or(cv::Mat());
	
}

std::shared_ptr<resource> resource::get_instance()
{
	static std::shared_ptr<resource> instance = nullptr;
	if (instance == nullptr)
	{
		auto deleter = [](resource* p) {delete p; };
		instance = std::shared_ptr<resource>(new resource(), deleter);
	}
	return instance;
}