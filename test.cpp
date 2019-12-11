#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/xfeatures2d.hpp>//se der erro nesse import acho q é só tirar e fica d boa
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
//#include<opencv2/tracking.hpp>

using namespace std;


int main(){
    //Video Capturing Object
    cv::VideoCapture video("blais/blais.mp4");

    if(!video.isOpened()){
        exit(-1);
    }

    //Saving frame
    cv::Mat frame;

    int framewidth = video.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameheigth = video.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    cout << "Width: " << framewidth << ", Height: " << frameheigth << endl;

    //cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
//
    //video.read(frame);
    //cv::Rect2d trackingBox = cv::selectROI(frame, false);
    //tracker->init(frame, trackingBox);

    while(video.read(frame)){

        //if(tracker->update(frame, trackingBox)){
        //    cv::rectangle(frame, cv::Scala(0,0,255), 2, 8); 
        //}

        //Displaying the frame
        cv::imshow("Video Display", frame);

        //Pressing any key will break the loop
        if(cv::waitKey(25) >= 0){
            break;
        }
    }

    video.release();
    cv::destroyAllWindows();
 
    return 0;
}