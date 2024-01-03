#include <opencv2/opencv.hpp>
#include "ServerService.h"

::grpc::Status classFrameService::getFrame(::grpc::ServerContext* context, const::framevideoPackage::frame* request, ::framevideoPackage::frameResponse* response)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << "i got!!" << std::endl;
	pushToTheDataStruct(convertToImage(*request), request->timestamp());
	response->set_success(true);
	response->set_message("I got it thanks!");
	return grpc::Status::OK;
}

void classFrameService::pushToTheDataStruct(const cv::Mat& frame, const std::string& timeStamp)
{
	VideoFrameData currentFrame;
	currentFrame.timestamp = timeStamp;
	currentFrame.frame = frame;
	frameQueue.push(currentFrame);
}

cv::Mat classFrameService::convertToImage(const framevideoPackage::frame& frameMsg)
{
	// Decode the image data
	std::vector<uchar> imageData(frameMsg.imagedata().begin(), frameMsg.imagedata().end());
	cv::Mat image = cv::imdecode(imageData, cv::IMREAD_COLOR);

	// Ensure that the image channels match the expected channels on the server
	if (image.channels() != frameMsg.channels())
		cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

	// Resize the image if necessary
	if (image.cols != frameMsg.width() || image.rows != frameMsg.height())
		cv::resize(image, image, cv::Size(frameMsg.width(), frameMsg.height()));
	return image;
}