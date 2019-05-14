#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;


#define THICKNESS_VALUE 4

// List of points
typedef vector<Point> contour_t;
// List of contours
typedef vector<contour_t> contour_vector_t;


const int threshold_slider_max = 255;
int threshold_slider = 0;


static void on_trackbar(int pos, void* slider_value) {
	*((int*)slider_value) = pos;
}


int main1() {
	Mat frame;
	VideoCapture cap(0);

	const string streamWindow = "Stream";

	if (!cap.isOpened()) {
		cout << "No webcam, using video file" << endl;
		cap.open("MarkerMovie.MP4");
		if (cap.isOpened() == false) {
			cout << "No video!" << endl;
			exit(0);
		}
	}

	const string contoursWindow = "Contours";
	const string UI = "Threshold";
	namedWindow(contoursWindow, CV_WINDOW_FREERATIO);


	int slider_value = 170;
	createTrackbar(UI, contoursWindow, &slider_value, 255, on_trackbar, &slider_value);

	Mat imgFiltered;

	while (cap.read(frame)) {

		// --- Process Frame ---

		Mat grayScale;
		imgFiltered = frame.clone();
		cvtColor(imgFiltered, grayScale, COLOR_BGR2GRAY);

		threshold(grayScale, grayScale, slider_value, 255, THRESH_BINARY);

		contour_vector_t contours;

		findContours(grayScale, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		for (size_t k = 0; k < contours.size(); k++) {

			// -------------------------------------------------

			// --- Process Contour ---

			contour_t approx_contour;

			approxPolyDP(contours[k], approx_contour, arcLength(contours[k], true) * 0.02, true);

			Scalar QUADRILATERAL_COLOR(0, 0, 255);
			Scalar colour;

			Rect r = boundingRect(approx_contour);

			if (approx_contour.size() == 4) {
				colour = QUADRILATERAL_COLOR;
			}
			else {
				continue;
			}

			if (r.height < 20 || r.width < 20 || r.width > imgFiltered.cols - 10 || r.height > imgFiltered.rows - 10) {
				continue;
			}

			polylines(imgFiltered, approx_contour, true, colour, THICKNESS_VALUE);

			// -----------------------------

			// --- Process Corners ---

			for (size_t i = 0; i < approx_contour.size(); ++i) {
				circle(imgFiltered, approx_contour[i], 3, CV_RGB(0, 255, 0), -1);

				double dx = ((double)approx_contour[(i + 1) % 4].x - (double)approx_contour[i].x) / 7.0;
				double dy = ((double)approx_contour[(i + 1) % 4].y - (double)approx_contour[i].y) / 7.0;

				for (int j = 1; j < 7; ++j) {
					double px = (double)approx_contour[i].x + (double)j * dx;
					double py = (double)approx_contour[i].y + (double)j * dy;

					Point p;
					p.x = (int)px;
					p.y = (int)py;
					circle(imgFiltered, p, 2, CV_RGB(0, 0, 255), -1);
				}
			}

			// -----------------------------

			// -----------------------------
		}

		//imshow(streamWindow, frame);
		imshow(contoursWindow, imgFiltered);

		if (waitKey(10) == 27) {
			break;
		}
	}

	//destroyWindow(streamWindow);
	destroyWindow(contoursWindow);
}