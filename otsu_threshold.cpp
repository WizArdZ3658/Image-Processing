#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<cmath>
#include <bits/stdc++.h>
using namespace std;
int w, h;
int Otsu_Threshold( vector< vector<unsigned char> > &image)
{
	int hist[256] = {0};         
	for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            hist[(int)image[i][j]]++;
        }
    }
    double prob[255+1];
	int no_of_pixels = w*h;
	//double check_sum = 0;
	for (int i = 0; i < 255+1; ++i)
	{
		prob[i] = (double)hist[i]/no_of_pixels;
		//check_sum += prob[i];
	}
	//cout << "Check Sum : " << check_sum << '\n';
	int thresh;
    double p1=0, p2, mean0=0, mean1=0, v=0, u, maxv=-1;
    for(int t=0; t<=255; t++)
    {    	
        mean1 += t*prob[t];
    }
    for(int t=0; t<=255; t++)
    {
        p1 += prob[t];
        p2 = 1-p1;
        mean0 += t*prob[t];
        mean1 -= t*prob[t];
        u = (mean0/p1)-(mean1/p2);
        v = p1*(1.0-p1)*(u*u);
        if(v>maxv)
        {
            maxv=v;
            thresh=t;
        }
    }
    cout << "Thresh Value : " << thresh << '\n';
    return thresh;
}
vector< vector<unsigned char> > Read_PGM(string path)
{
	ifstream file(path, ios::binary);
	string s1;
	getline(file,s1);
	cout << s1 << '\n'; 
	while(getline(file,s1))
	{
		if (s1[0]!='#')
		{
			break;
		}
	}
	string width, height;
	int flag = 1;
	for (int i = 0; i < s1.size(); ++i)
	{
		if (s1[i]==' ')
		{
			flag = 2;
			continue;
		}
		else if (flag==1)
		{
			width += s1[i];
		}
		else if (flag==2)
		{
			height += s1[i];
		}
	}
	w = stoi(width);
	h = stoi(height);
	getline(file, s1);
	cout << "Height : " << h << " Width : " << w << '\n';
	vector< vector<unsigned char> > image(h, vector<unsigned char>(w));
	char ch;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            file.read(&ch, 1);
            image[i][j] = (unsigned char)ch;
        }
    }
	file.close();
	return image;
}
int Write_PGM(vector< vector<unsigned char> > &image, int thresh)
{
	ofstream fout("Otsu_Threshold_image.pgm", ios::binary);
    if(!fout.is_open())
    {
        cerr << "Error! Unable to open file .\n";
        return 0;
    }
    fout << "P5\n" << w << " " << h << "\n" << 255 << "\n";
    char ch;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
        	if (image[i][j]>=thresh)
        	{
        		ch = (char)(255);
        	}
        	else
        	{
        		ch = (char)(0);
        	}
            fout.write(&ch, 1);
        }
    }
    fout.close();
    return 1;
}
int main()
{
	cout << "Enter path along with the file name : \n";
	string path;
	cin >> path;
	vector<vector<unsigned char>> image = Read_PGM(path);
	int thresh = Otsu_Threshold(image);
	if(Write_PGM(image, thresh))
	{
		cout << "Successful Output\n";
	}
	else
	{
		cout << "Failed\n";
	}
	return 0;
}