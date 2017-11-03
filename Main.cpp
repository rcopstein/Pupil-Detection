#define _USE_MATH_DEFINES

#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"

#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;
using std::cout;

double angulo = 0;

int tampleX = 0;
cv::Mat temple;

int zoom = 4;
int thresh = 66;
int matrix[1000][3];
bool visualization = false;

typedef struct _Eye {

	Point* center;
	float pupilSize;

} Eye;

Eye getPupilCenter(Mat frame)
{
    Point* defPoint = new Point(0, 0);
	float defSize = 0;

	Eye eye = { defPoint, defSize };
    Mat subFrame, frame_gray, tempMAT;
 
    //Corte da imagem e conversão de RGB para tons de cinza

	int roiX = frame.cols * 0.30;
	int roiY = frame.rows * 0.20;
	int roiW = frame.cols * 0.50;
	int roiH = frame.rows * 0.60;

    Rect ROI = Rect(roiX, roiY, roiW, roiH);
    subFrame = frame(ROI);
 
    cvtColor(subFrame, frame_gray, CV_RGB2GRAY);
 
    //Limiar
    threshold(frame_gray, frame_gray, thresh, 255, 1);
 
    //Canny
    Canny(frame_gray, frame_gray, 100, 255);
 
    //Morfologia
    Mat kernel;
    int tamanho = 5;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(tamanho, tamanho));
    dilate(frame_gray, frame_gray, kernel, Point(-1, -1), 2, 1, 1);
    erode(frame_gray, frame_gray, kernel, Point(-1, -1), 2, 1, 1);
 
    //Detecção de bordas
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarquia;
 
    findContours(frame_gray, contornos, hierarquia, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(roiX, roiY));
 
    if (contornos.empty())
        return eye;
 
    int indiceDoContorno, areaDoContorno = 0, temp;
    vector<Point> contorno;
    for (unsigned int i = 0; i < contornos.size(); i++)
    {
        contorno = contornos[i];
        temp = contourArea(contorno);
        if (temp >= areaDoContorno)
        {
            indiceDoContorno = i;
            areaDoContorno = temp;
        }
    }
 
    Mat contourMat(frame.rows, frame.cols, CV_8UC3);
 
    //BOUNDING-BOX
    Rect aux = boundingRect(contornos[indiceDoContorno]);
 
    drawContours(contourMat, contornos, indiceDoContorno, Scalar(255, 255, 255), 1, 8, hierarquia, 0);
 
    if (contourArea(contornos[indiceDoContorno]) > 80)
    {
        //Desenho contornos e bounding-box
        rectangle(contourMat, aux, Scalar(0, 255, 0), 1);
 
        Point centro(aux.x + aux.width / 2, aux.y + aux.height / 2);
        eye.pupilSize = aux.width / 2;
        eye.center = &centro;
 
        if (eye.pupilSize == 0) eye.pupilSize = aux.width / 2;
    }
    else{
        //cout << contourArea(contornos[indiceDoContorno]);
        //imshow("Problema", contourMat);
        //waitKey(0);
    }
    return eye;
}

Eye getPupilCenterNew(Mat frame) 
{
	Point* defPoint = new Point(0, 0);
	float defSize = 0;

	Eye eye = { defPoint, defSize };
	Mat subFrame, frame_gray, tempMAT;

	// Set ROI

	int roiX = frame.cols * 0.30;
	int roiY = frame.rows * 0.20;
	int roiW = frame.cols * 0.50;
	int roiH = frame.rows * 0.60;
	subFrame = frame(Rect(roiX, roiY, roiW, roiH));

	//Corte da imagem e conversão de RGB para tons de cinza

	cvtColor(subFrame, frame_gray, CV_RGB2GRAY);

	//Limiar
	threshold(frame_gray, frame_gray, thresh, 255, 1);

	//Canny
	//Canny(frame_gray, frame_gray, 100, 255);

	//Morfologia
	//Mat kernel;
	//int tamanho = 5;
	//kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(tamanho, tamanho));
	//dilate(frame_gray, frame_gray, kernel, Point(-1, -1), 2, 1, 1);
	//erode(frame_gray, frame_gray, kernel, Point(-1, -1), 2, 1, 1);

	//Detecção de bordas
	vector<vector<Point> > contornos;
	vector<Vec4i> hierarquia;

	findContours(frame_gray, contornos, hierarquia, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(roiX, roiY));
	if (contornos.empty()) return eye;

	int indiceDoContorno, areaDoContorno = 0, temp;
	vector<Point> contorno;

	for (unsigned int i = 0; i < contornos.size(); i++)
	{
		contorno = contornos[i];
		temp = contourArea(contorno);
		if (temp >= areaDoContorno)
		{
			indiceDoContorno = i;
			areaDoContorno = temp;
		}
	}

	Mat contourMat(frame.rows, frame.cols, CV_8UC3);

	//BOUNDING-BOX
	Rect aux = boundingRect(contornos[indiceDoContorno]);
	drawContours(contourMat, contornos, indiceDoContorno, Scalar(255, 255, 255), 1, 8, hierarquia, 0);

	if (contourArea(contornos[indiceDoContorno]) > 80)
	{
		//Desenho contornos e bounding-box
		rectangle(contourMat, aux, Scalar(0, 255, 0), 1);

		Point centro(aux.x + aux.width / 2, aux.y + aux.height / 2);
		eye.pupilSize = aux.width / 2;
		eye.center->x = centro.x;
		eye.center->y = centro.y;

		if (eye.pupilSize == 0) eye.pupilSize = aux.width / 2;

	}

	return eye;

}

void compareCenter(VideoCapture capture)
{
	Mat frame;
	capture.read(frame);

	while (true)
	{

		// Pupila usando técnica original

			Mat frame_original = frame.clone();

			Eye original;
			original = getPupilCenter(frame_original);

			// cout << original.center.x << original.center.y << original.pupilSize*2 << endl;;
			circle(frame_original, *(original.center), original.pupilSize, Scalar(0, 0, 255), -1);

			imshow("Original", frame_original);

		// Pupila usando nova técnica

			Mat frame_novo = frame.clone();

			Eye novo;
			novo = getPupilCenterNew(frame_novo);

			// cout << novo.center.x << novo.center.y << novo.pupilSize*2 << endl;;
			circle(frame_novo, *(novo.center), novo.pupilSize, Scalar(0, 0, 255), -1);

			imshow("Novo", frame_novo);

		// Ler resultado da avaliação

			int key = cv::waitKey(0) - 49;
			cout << key << endl;

		// Continuar avaliação

			if (!capture.read(frame)) break;

	}

	cv::waitKey(0);
}

int main()
{
	VideoCapture capture("./Video1.avi");

	if (!capture.isOpened()) { printf("Couldn't load video!"); cv::waitKey(0); return -1; }

	compareCenter(capture);
	return 0;
}

/*

 - Video1 Works
 - Video2 Doesn't
 - Video3 Works
 - Video4 Doesn't
 - Video5 Works

*/