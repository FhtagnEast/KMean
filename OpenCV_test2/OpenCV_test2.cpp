// OpenCV_test2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "imgproc/imgproc.hpp"
#include "highgui/highgui.hpp"
#include <math.h>
#include <iostream>
#include <time.h>
//1
//cv::Mat kmean(int num_of_segments, cv::Mat &img);
void kmean(int num_of_segments, cv::Mat &img);
int find_min(std::vector<int> &v);


int main()
{
	int num_of_segments;
	std::cout << "Input segments number: ";
	std::cin >> num_of_segments;
	cv::Mat img;
	img = cv::imread("C:\\pic\\Tulips.jpg", 0);
	kmean(num_of_segments, img);
	/*cv::Mat img1;
	img1 = cv::imread("C:\\1.png", 0);
	kmean(num_of_segments, img1);*/
	return 0;
}

int find_min(std::vector<int> &v)
{
	int size = v.size();
	int unsorted = 1;
	std::vector<int> nums;
	nums.clear();
	int minPos = 0;
	for (int i = 0; i < size; i++)
	{
		if (v[i] < v[minPos])
			minPos = i;
	}

	/*for (int i = 0; i < size; i++)
	{
		nums.push_back(i+1);
	}
	while (unsorted)
	{
		unsorted = 0;
		for (int i = 0; i<(size-1); i++)
		{
			if (v[i] > v[i + 1])
			{
				int temp = v[i + 1];
				v[i + 1] = v[i];
				v[i] = temp;
				unsorted = 1;
				temp = nums[i + 1];
				nums[i + 1] = nums[i];
				nums[i] = temp;
			}

		}
	}*/
	//int number_of = nums[0];
	//std::cout << minPos << std::endl;
	return minPos+1;
}

//cv::Mat kmean(int num_of_segments, cv::Mat &img)
void kmean(int num_of_segments, cv::Mat &img)
{
	//std::cout << num_of_segments << std::endl;

	int treshold = 255 / num_of_segments;
	//std::cout << treshold << std::endl;
	int flag = 1;
	srand(time(NULL));
	std::vector<int> segments;
	std::vector<std::vector<int>> clusters;
	for (int i = 0; i < num_of_segments; i++)
	{
		segments.push_back((rand() % treshold + 1) + i*treshold);
		//std::cout << num_of_segments << std::endl;
	}

	cv::Mat new_img = cv::Mat::zeros(img.rows, img.cols, CV_8U);
	while (flag)
	{
		
		std::vector<std::vector<std::vector<int>>> top_array;
		for (int y = 0; y < img.rows; y++)
		{
			std::vector<std::vector<int>> frame;
			for (int x = 0; x < img.cols; x++)
			{
				std::vector<int> row;
				for (int i = 0; i < num_of_segments; i++)
				{
					int diff = abs(img.at<unsigned char>(y, x) - segments[i]);
					row.push_back(diff);
				}
				frame.push_back(row);
				row.clear();
			}
 			top_array.push_back(frame);
			frame.clear();
		}
		clusters.clear();
		//std::cout << num_of_segments << std::endl;
		for (int y = 0; y < img.rows; y++)
		{
			std::vector<int> cluster_row;
			for (int x = 0; x < img.cols; x++)
			{
				int clust = find_min(top_array[y][x]);
  				cluster_row.push_back(clust);
			}
			clusters.push_back(cluster_row);
			cluster_row.clear();
		}
		std::vector<double> acc;
		std::vector<double> nums_of_elements;
		std::vector<int> mean;
		int dummy = 0;
		for (int i=0; i < num_of_segments; i++)
		{
			acc.push_back(dummy);
			nums_of_elements.push_back(dummy);
			mean.push_back(dummy);
		}
		for (int i = 0; i < num_of_segments; i++)
		{
			for (int y = 0; y < img.rows; y++)
			{
				for (int x = 0; x < img.cols; x++)
				{
					if (clusters[y][x] == i + 1)
					
					{
						acc[i]+= img.at<unsigned char>(y, x);
						nums_of_elements[i]++;
					}
				}
			}
		}
		//std::cout << num_of_segments << std::endl;
		for (int i = 0; i < num_of_segments; i++)
		{
			mean[i] = acc[i] / nums_of_elements[i];
		}
		double delta = 0;
		
		for (int i = 0; i < num_of_segments; i++)
		{
			delta += abs(mean[i] - segments[i]);
			
		}
		std::cout << delta << std::endl;
		if (delta <= 0)
			flag = 0;
		for (int i = 0; i < num_of_segments; i++)
		{
			segments[i] = mean[i];
		}
		top_array.clear();
		acc.clear();
	//	clusters.clear();
		nums_of_elements.clear();
		mean.clear();
	}

	for (int i = 0; i < num_of_segments; i++)
	{
		
		for (int y = 0; y < img.rows; y++)
		{
			for (int x = 0; x < img.cols; x++)
			{
				
				if (clusters[y][x] == i + 1)
				{
					new_img.at<unsigned char>(y, x) = i*treshold;
				}
			}
		}
	}
	cv::imshow("new_img",new_img);
	cv::waitKey();

}

