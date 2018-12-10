#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<iostream>
#include<stdlib.h>
using namespace cv;
using namespace std;

void erosion(Mat *img3,Mat *img4)
{
	for(int i=0;i<(*img3).rows;i++)
	{
		for(int j=0;j<(*img3).cols;j++)
		{
		    int arr[10];
			int sum=0;
			int count=0;
			int ans;
			for(int x=0;x<3;x++)
			{
				for(int y=0;y<3;y++)
				{
					if(i-1+x>=0&&j-1+y>=0&&i-1+x<(*img3).rows&&j-1+y<(*img3).cols)
					{
						arr[count]=(*img3).at<uchar>(i-1+x,j-1+y);
						count++;
					}
				}
			}
			ans=arr[0];
			for(int x=1;x<count;x++)
			{
				if(arr[x]<ans)
					ans=arr[x];
			}	
			(*img4).at<uchar>(i,j)=ans;
		}
	}
}
void dilation(Mat *img3,Mat *img4)
{
	for(int i=0;i<(*img3).rows;i++)
	{
		for(int j=0;j<(*img3).cols;j++)
		{
		    int arr[10];
			int sum=0;
			int count=0;
			int ans;
			for(int x=0;x<3;x++)
			{
				for(int y=0;y<3;y++)
				{
					if(i-1+x>=0&&j-1+y>=0&&i-1+x<(*img3).rows&&j-1+y<(*img3).cols)
					{
						arr[count]=(*img3).at<uchar>(i-1+x,j-1+y);
						count++;
					}
				}
			}
			ans=arr[0];
			for(int x=1;x<count;x++)
			{
				if(arr[x]>ans)
					ans=arr[x];
			}	
			(*img4).at<uchar>(i,j)=ans;
		}
	}
}
int main()
{
Mat frame;
VideoCapture vid(0);
 while(1)
 {

    vid>>frame;
    Mat img2(frame.rows,frame.cols,CV_8UC1,Scalar(0));
	for(int i=0;i<frame.rows;i++)
	{
		for(int j=0;j<frame.cols;j++)
		{
			int a=frame.at<Vec3b>(i,j)[0];
            int b=frame.at<Vec3b>(i,j)[1];
             int c=frame.at<Vec3b>(i,j)[2];
			if(a<=110&&a>=0&&b<=227&&b>=0&&c<=255&&c>=175)
					img2.at<uchar>(i,j)=255;
			else
					img2.at<uchar>(i,j)=0;
		}
	}
	Mat img3=img2.clone();
	Mat img4(img3.rows,img3.cols,CV_8UC1,Scalar(0));
	erosion(&img3,&img4);
	Mat img5(img3.rows,img3.cols,CV_8UC1,Scalar(0));
	dilation(&img4,&img5);
	dilation(&img5,&img4);
	dilation(&img4,&img5);
	dilation(&img5,&img4);
	
	Mat img6(img4.rows,img4.cols,CV_8UC1,Scalar(0));
	Canny(img4,img6,40,120,3);
	
	 vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;
  	findContours(img5,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
  	if(contours.size()<=0)continue;
  	vector<Moments>mu(contours.size());
  	for(int i=0;i<contours.size();i++)
  	{
  		mu[i]=moments(contours[i],false);
  	}
  	vector<Point2f> mc(contours.size());
  	for(int i=0;i<contours.size();i++)
  	{
		mc[i]=Point2f(mu[i].m10/mu[i].m00,mu[i].m01/mu[i].m00);
	}
	 Mat drawing = Mat::zeros( img5.rows,img5.cols,CV_8UC3);
  	for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar(255,255,255);
       if(contours.size()>0)
       		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
       circle(drawing,mc[i],4,color,-1,8,0);

     }
     float max_area=contourArea(contours[0]);
     int max_index=0;
     for(unsigned int i=1;i<contours.size();i++)
     {
     	if(contourArea(contours[i])>max_area)
     	{
			max_area=contourArea(contours[i]);
			max_index=i;
		}
	}
	Scalar color = Scalar(255,255,255);
	Mat img7(img3.rows,img3.cols,CV_8UC1,Scalar(0));
  	if(contours.size()>0)
  		drawContours( img7,contours,max_index,color,2,8,hierarchy,0,Point());
	int al=10000;int ah=150000;
	int yl=150;int yh=500;
	int x=(254*(max_area-al)/(ah-al))-127;
	int y=(254*(mc[max_index].x-yl)/(yh-yl))-127;
	if(x>=-127&&x<=127&&y>=-127&&y<=127)
	{cout<<x<<" "<<y<<endl;}
  	namedWindow("win7",WINDOW_NORMAL);
	imshow("win7",img7);
  	waitKey(100);
}
return 0;
}
