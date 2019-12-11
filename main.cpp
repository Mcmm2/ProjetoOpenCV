#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
//#include <opencv2/xfeatures2d.hpp>//se der erro nesse import acho q é só tirar e fica d boa
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
using namespace cv;
using namespace std;

int main() {
    
    Mat image1, image2, imageAux;
    VideoCapture cap("blais/blais.mp4");//aqui vai o numero da webcam ou o path do video
    if (!cap.isOpened()) { //verifica se cap abriu como esperado
        cout << "camera ou arquivo em falta";
        return 1;
    }
    cap.read(image1);//aqui vai o path da imagem
    cv::Rect2d r = cv::selectROI(image1);
    cv::Mat img = image1(r) ;
    cv::destroyAllWindows();

    if (img.empty()) { //verifica a imagem1
        cout << "imagem 1 vazia";
        return 1;
    }

    while (true) {
        cap >> image2;
        if (image2.empty()) {
            cout << "imagem 2 vazia";
            return 1;
        }

        vector<KeyPoint> kp1, kp2;
        Mat descriptor1, descriptor2;
    
        //acho que o ORB e o unico q funciona no PC do cin
        //Ptr<Feature2D> orb = xfeatures2d::SIFT::create(400);
        //Ptr<Feature2D> orb = xfeatures2d::SURF::create(400);
        Ptr<Feature2D> orb = ORB::create(1000);

        //detecta as features da imagem
        orb->detectAndCompute(img, Mat(), kp1, descriptor1);
        orb->detectAndCompute(image2, Mat(), kp2, descriptor2);

        //aqui desenha as features
        drawKeypoints(img, kp1, imageAux);
        drawKeypoints(image2, kp2, image2);


        //Drawing matches 
        vector<DMatch> matches;
        cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
        matcher->match(descriptor1, descriptor2, matches, cv::Mat());

        sort(matches.begin(), matches.end());
        const int numGoodMatches = matches.size() * 0.15f;
        matches.erase(matches.begin() + numGoodMatches, matches.end());
        
        cv::Mat imMatches;
        
        drawMatches(img, kp1, image2, kp2, matches, imMatches);
        //imwrite("matches.jpg", imMatches);

        //Get keypoints from good matches
        vector<cv::Point2f> obj, scene;
        for(int i = 0; i < matches.size(); i++){
            obj.push_back(kp1[matches[i].queryIdx].pt);
            scene.push_back(kp2[matches[i].trainIdx].pt);
        }

        //Finding homography
        cv::Mat h = cv::findHomography(obj, scene, cv::RANSAC);

        //Get the corners of object
        vector<cv::Point2f> objCorners(4);
        objCorners[0] = cv::Point2f(0,0);
        objCorners[1] = cv::Point2f(img.cols ,0);
        objCorners[2] = cv::Point2f(img.cols , img.rows);
        objCorners[3] = cv::Point2f(0 ,img.rows);

        vector<cv::Point2f> sceneCorners(4);

        cv::perspectiveTransform(objCorners, sceneCorners, h);

        cv::line(imMatches, sceneCorners[0] + cv::Point2f(img.cols, 0), sceneCorners[1] + cv::Point2f(img.cols, 0), cv::Scalar(0, 255, 0), 4);
        cv::line(imMatches, sceneCorners[1] + cv::Point2f(img.cols, 0), sceneCorners[2] + cv::Point2f(img.cols, 0), cv::Scalar(0, 255, 0), 4);
        cv::line(imMatches, sceneCorners[2] + cv::Point2f(img.cols, 0), sceneCorners[3] + cv::Point2f(img.cols, 0), cv::Scalar(0, 255, 0), 4);
        cv::line(imMatches, sceneCorners[3] + cv::Point2f(img.cols, 0), sceneCorners[0] + cv::Point2f(img.cols, 0), cv::Scalar(0, 255, 0), 4);

        //cv::imshow("Good Matches", matches);

        //cv::perspectiveTransform(point1, point2, h);
        //Removing not so good matches
        //const int nummatches = matches.size() * 0.15f;
        //matches.erase(matches.begin() + nummatches, matches.end());


        //o nome da imagem e case sensitive, cria janela pra 'teste' e usa a ja existente pra 'Teste'
        imshow("teste", imMatches);
        //imshow("Teste", imageAux);
        if (waitKey(1) == 27) {//sai no esc
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
