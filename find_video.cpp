#include "find_video.h"

using namespace std;

Mat getContour(Mat src, int & cnt)
{
	Mat contour, coordinates;
	int sens = 60;
	do {
		sens -= 10;
		Canny(src, contour, sens, sens);
		cnt = countNonZero(contour);
		if (sens == 0) break;
	} while (cnt < 46000);
	while (cnt > 48000) {
		sens += 10;
		Canny(src, contour, sens, sens);
		cnt = countNonZero(contour);
	}
	findNonZero(contour, coordinates);
	imshow("edge", contour);
	return coordinates;
}

void getVariance(Mat src, int cnt, double m_x, double m_y, double & v_x, double & v_y)
{
	v_x = v_y = 0;
	for (int i = 0; i < cnt; i++) {
		v_x += (src.at<Point>(i).x - m_x)*(src.at<Point>(i).x - m_x);
		v_y += (src.at<Point>(i).y - m_y)*(src.at<Point>(i).y - m_y);
	}
	if (cnt == 0)
		v_x = v_y = 0;
	else {
		v_x /= cnt;
		v_y /= cnt;
	}
	printf(" V : %f %f\n", v_x, v_y);
}

void getAverage(Mat src, int cnt, double & x, double & y)
{
	x = y = 0;
	for (int i = 0; i < cnt; i++) {
		x += src.at<Point>(i).x;
		y += src.at<Point>(i).y;
	}
	if (cnt == 0)
		x = y = 0;
	else {
		x /= cnt;
		y /= cnt;
	}
	printf(" A : %f %f\n", x, y);
}

void analyzeData(void)
{
	Mat coordinates;
	int in_cnt, db_cnt, cnt = 0;
	double in_m_x, in_m_y, in_v_x, in_v_y;
	double db_m_x, db_m_y, db_v_x, db_v_y;

	VideoCapture video;
	video.open("C:/Users/KSY/Documents/OpenCV/video/accepted_1.avi");
	if (!video.isOpened()) {
		cout << "failed to open video!" << endl;
		return;
	}

	while (true) {
		Mat frame, roi;
		int width, height;
		video.read(frame);
		if (frame.empty())
			break;
		width = frame.size().width / 3;
		height = frame.size().height / 3;

		for (int i = 0; i <= frame.size().height - height; i += height) {
			for (int j = 0; j <= frame.size().width - width; j += width) {
				roi = frame(Rect(j, i, width, height));
		
				coordinates = getContour(roi, db_cnt);
				getAverage(coordinates, db_cnt, db_m_x, db_m_y);
				// 평균 db에 저장하기
		
				getVariance(coordinates, db_cnt, db_m_x, db_m_y, db_v_x, db_v_y);
				// 분산 db에 저장하기
			}
		}

		imshow("frame", frame);
		imshow("roi", roi);
		char c = (char)waitKey(1);
		if (c == 27) break;
	}

	video.release();
	destroyAllWindows();
}