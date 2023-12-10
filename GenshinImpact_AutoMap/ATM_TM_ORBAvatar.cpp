#include "ATM_TM_ORBAvatar.h"

void ATM_TM_ORBAvatar::setAvatarTemplate(cv::Mat avatarTemplateMat)
{
	_avatarTemplate = avatarTemplateMat;
}

void ATM_TM_ORBAvatar::setAvatarMat(cv::Mat avatarMat)
{
	_avatarMat = avatarMat;
}

void ATM_TM_ORBAvatar::Init()
{
	if (isInit)return;

	isInit = true;
}

bool GreaterSort(cv::DMatch a, cv::DMatch b)
{
	return (a.distance > b.distance);
}

void ATM_TM_ORBAvatar::ORBMatch()
{
	cv::Mat giAvatarRef = _avatarMat;

	resize(giAvatarRef, giAvatarRef, cv::Size(), 2, 2);
	std::vector<cv::Mat> lis;
	split(giAvatarRef, lis);

	threshold(lis[0], gray0, 240, 255, cv::THRESH_BINARY);
	threshold(lis[1], gray1, 212, 255, cv::THRESH_BINARY);
	threshold(lis[2], gray2, 25, 255, cv::THRESH_BINARY_INV);


	bitwise_and(gray1, gray2, and12, gray0);
	resize(and12, and12, cv::Size(), 2, 2, 3);
	Canny(and12, and12, 20, 3 * 20, 3);
	circle(and12, cv::Point(cvCeil(and12.cols / 2), cvCeil(and12.rows / 2)), cvCeil(and12.cols / 4.5), cv::Scalar(0, 0, 0), -1);

	dilate(and12, and12, dilate_element);
	erode(and12, and12, erode_element);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarcy;

	findContours(and12, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	std::vector<cv::Rect> boundRect(contours.size());  //������Ӿ��μ���
	//std::vector<RotatedRect> box(contours.size()); //������С��Ӿ��μ���

	cv::Point2f rect[4];

	std::vector<cv::Point2d> AvatarKeyPoint;
	double AvatarKeyPointLine[3] = { 0 };
	std::vector<cv::Point2f> AvatarKeyLine;
	cv::Point2f KeyLine;

	if (contours.size() != 3)
	{
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		//box[i] = minAreaRect(Mat(contours[i]));  //����ÿ��������С��Ӿ���
		boundRect[i] = cv::boundingRect(cv::Mat(contours[i]));
		AvatarKeyPoint.push_back(cv::Point(cvRound(boundRect[i].x + boundRect[i].width / 2), cvRound(boundRect[i].y + boundRect[i].height / 2)));
	}

	AvatarKeyPointLine[0] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
	AvatarKeyPointLine[1] = dis(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	AvatarKeyPointLine[2] = dis(AvatarKeyPoint[1] - AvatarKeyPoint[0]);



	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[2] && AvatarKeyPointLine[1] >= AvatarKeyPointLine[2])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[2] - AvatarKeyPoint[0]);
	}
	if (AvatarKeyPointLine[0] >= AvatarKeyPointLine[1] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[1])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[0]);
		AvatarKeyLine.push_back(AvatarKeyPoint[1] - AvatarKeyPoint[2]);
	}
	if (AvatarKeyPointLine[1] >= AvatarKeyPointLine[0] && AvatarKeyPointLine[2] >= AvatarKeyPointLine[0])
	{
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[1]);
		AvatarKeyLine.push_back(AvatarKeyPoint[0] - AvatarKeyPoint[2]);
	}

	AvatarKeyLine = Vector2UnitVector(AvatarKeyLine);
	KeyLine = AvatarKeyLine[0] + AvatarKeyLine[1];
	rotationAngle = Line2Angle(KeyLine);
	return;
}

double ATM_TM_ORBAvatar::getRotationAngle()
{
	return rotationAngle;
}

double ATM_TM_ORBAvatar::dis(cv::Point p)
{
	return sqrt(p.x*p.x + p.y*p.y);
}
std::vector<cv::Point2f> ATM_TM_ORBAvatar::Vector2UnitVector(std::vector<cv::Point2f> pLis)
{
	double length = 1;
	std::vector<cv::Point2f> res;
	for (int i = 0; i < pLis.size(); i++)
	{
		length = sqrt(pLis[i].x*pLis[i].x + pLis[i].y*pLis[i].y);
		res.push_back(cv::Point2f((float)(pLis[i].x / length), (float)(pLis[i].y / length)));
	}
	return res;
}

double ATM_TM_ORBAvatar::Line2Angle(cv::Point2f p)
{
	const double rad2degScale = 180 / CV_PI;
	double res = atan2(-p.y, p.x)*rad2degScale;
	res = res - 90; //����Ļ�ռ����ˮƽ��Ϊ0��ת����ֱ����Ϊ0��
	return res;
}
