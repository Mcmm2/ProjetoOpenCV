#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>//se der erro nesse import acho q é só tirar e fica d boa
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
using namespace cv;
using namespace std;

int main() {
    namedWindow("Teste", WINDOW_AUTOSIZE);
    Mat image1, image2, imageAux;
    VideoCapture cap(0);//aqui vai o numero da webcam ou o path do video
    if (!cap.isOpened()) { //verifica se cap abriu como esperado
        cout << "camera ou arquivo em falta";
        return 1;
    }

    image1 = imread("berserk.jpg", IMREAD_GRAYSCALE);//aqui vai o path da imagem

    if (image1.empty()) { //verifica a imagem1
        cout << "imagem 1 vazia";
        return 1;
    }

    while (true) {
        cap >> image2;
        if (image2.empty()) {
            cout << "imagem 2 vazia";
            return 1;
        }

        cvtColor(image2, image2, COLOR_BGR2GRAY);//coloca em grayscale

        vector<KeyPoint> kp1, kp2;
        Mat descriptor1, descriptor2;
    
        //acho que o ORB e o unico q funciona no PC do cin
        //Ptr<Feature2D> orb = xfeatures2d::SIFT::create(400);
        //Ptr<Feature2D> orb = xfeatures2d::SURF::create(400);
        Ptr<Feature2D> orb = ORB::create(400);

        //detecta as features da imagem
        orb->detectAndCompute(image1, Mat(), kp1, descriptor1);
        orb->detectAndCompute(image2, Mat(), kp2, descriptor2);

        //aqui desenha as features
        drawKeypoints(image1, kp1, imageAux);
        drawKeypoints(image2, kp2, image2);

        //o nome da imagem e case sensitive, cria janela pra 'teste' e usa a ja existente pra 'Teste'
        imshow("teste", image2);
        imshow("Teste", imageAux);
        if (waitKey(1) == 27) {//sai no esc
            break;
        }
    }

    destroyAllWindows();
    return 0;
}
