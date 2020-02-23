#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<cmath>
#include <bits/stdc++.h>
using namespace std;
int w, h;
void Write_Section(vector< vector<unsigned char> > &image, int start_h, int start_w, int finish_h, int finish_w, int thresh)
{
	for(int i = start_h; i <= finish_h; i++)
    {
        for(int j = start_w; j <= finish_w; j++)
        {
            if((int)image[i][j] > thresh)
			{
				image[i][j] = (unsigned char)(255);
			}
			else
			{
				image[i][j] = (unsigned char)(0);
			}
        }
    }
}
void Otsu_Threshold( vector< vector<unsigned char> > &image, int start_h, int finish_h, int start_w, int finish_w)
{
	int hist[256] = {0};         
	for(int i = start_h; i <= finish_h; i++)
    {
        for(int j = start_w; j <= finish_w; j++)
        {
            hist[(int)image[i][j]]++;
        }
    }
    double prob[255+1];
	int no_of_pixels = (finish_w - start_w + 1)*(finish_h - start_h + 1);
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
    //cout << "Thresh Value : " << thresh << '\t';
    Write_Section(image, start_h, start_w, finish_h, finish_w, thresh);
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
void Localised_Otsu_Threshold(vector< vector<unsigned char> > &image)
{
	int count = 0;
	for(int i = 0; i < h; i+=15)
	{
		for(int j = 0; j < w; j+=15)
		{
			//cout << i << " " << j << " " << (i+14<h ? i+14 : h-1) << " " << (j+14<w ? j+14 : w-1) << '\t';
			Otsu_Threshold(image, i, (i+14<h ? i+14 : h-1), j, (j+14<w ? j+14 : w-1));
			count++; 
		}
		//cout << "\n\n";
	}
	cout << "total threshold values calculated : " << count << '\n';
}
int Write_PGM(vector< vector<unsigned char> > &image)
{
	ofstream fout("localised_otsu_image.pgm", ios::binary);
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
        	ch = (char)(image[i][j]);
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

	Localised_Otsu_Threshold(image);
	if(Write_PGM(image))
	{
		cout << "Successful Output\n";
	}
	else
	{
		cout << "Failed\n";
	}
	return 0;
}