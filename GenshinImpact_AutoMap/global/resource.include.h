#pragma once
#include <opencv2/opencv.hpp>
#include <map>
#include <optional>

class resource_image
{
public:
    virtual ~resource_image() {}
public:
    virtual void load() = 0;
    virtual std::vector<std::string> get_image_names() 
	{ 
        std::vector<std::string> names;	
        for (auto& [name, image] : images)
            names.push_back(name);
        return names;
	}
	
    virtual std::optional<cv::Mat> get_image(std::string name)
	{
        if (images.find(name) != images.end())
            return images[name];
        return std::nullopt;
	}
	
public:
    std::map<std::string, cv::Mat> images;
};

static cv::Mat to_mat(const std::pair<void*, size_t>& in)
{
	return cv::imdecode(cv::Mat(1, in.second, CV_8UC1, in.first), cv::IMREAD_UNCHANGED);
}

class resource
{
	resource();
public:
	~resource() = default;
	static std::shared_ptr<resource> get_instance();
public:
	cv::Mat main_mask;
	cv::Mat gi_map;
	cv::Mat gi_avatar;
	cv::Mat gi_avatar_mask;
	cv::Mat gi_star;
	cv::Mat gi_star_mask;
	std::map<int, cv::Mat> gi_obj_icon;
	std::map<int, cv::Mat> gi_obj_icon_mask;
	std::map<int, cv::Mat> gi_obj_flag_icon;
	std::map<int, cv::Mat> gi_obj_flag_icon_mask;
	std::map<int, cv::Mat> gi_paimon;
	std::map<int, cv::Mat> gi_uid_num;
private:
    std::vector<std::shared_ptr<resource_image>> resources;
};

#define glo_res resource::get_instance()