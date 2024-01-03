#include <mutex>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "myproto/frameVideo.grpc.pb.h"
#include "Queue.h"

class classFrameService final : public framevideoPackage::FrameVideoService::Service {
private:
	std::mutex m_mutex;
	VideoFrameQueue& frameQueue = VideoFrameQueue::getInstance();
public:
	virtual ::grpc::Status getFrame(::grpc::ServerContext* context, const ::framevideoPackage::frame* request, ::framevideoPackage::frameResponse* response);
	void pushToTheDataStruct(const cv::Mat& frame, const std::string& timeStamp);
	cv::Mat convertToImage(const framevideoPackage::frame& frameMsg);
};