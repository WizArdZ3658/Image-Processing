#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include<cmath>
#include <bits/stdc++.h>
using namespace std;
int w, h;
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
void Write_PGM(vector< vector<unsigned char> > &image, int H, int W)
{
	ofstream fout("zoom.pgm", ios::binary);
    if(!fout.is_open())
    {
        cerr << "Error! Unable to open file .\n";
    }
    fout << "P5\n" << W << " " << H << "\n" << 255 << "\n";

    char ch;
    for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
        	ch = (char)(image[i][j]);
            fout.write(&ch, 1);
        }
    }
    fout.close();
}
void zoom_in(vector< vector<unsigned char> > &image)
{
	vector< vector<unsigned char> > zoomed_image(h*2, vector<unsigned char>(w*2));
	for(int i = 0; i < h; i++)
    {
    	for(int j = 0; j < w; j++)
        {
        	zoomed_image[2*i][2*j] = image[i][j];
			zoomed_image[2*i+1][2*j+1] = image[i][j];        	
			zoomed_image[2*i][2*j+1] = image[i][j];
			zoomed_image[2*i+1][2*j] = image[i][j];
        }
    }
    Write_PGM(zoomed_image, h+h, w+w);
}
void zoom_out(vector< vector<unsigned char> > &image)
{
	vector< vector<unsigned char> > zoomed_image(h/2, vector<unsigned char>(w/2));
	int H = h/2;
	int W = w/2;
	for(int i = 0; i < H; i++)
    {
        for(int j = 0; j < W; j++)
        {
        	zoomed_image[i][j] = (image[2*i][2*j]+image[2*i+1][2*j]+image[2*i][2*j+1]+image[2*i+1][2*j+1])/4;
        }
    }
    Write_PGM(zoomed_image, H, W);
}

int main()
{
	cout << "Enter path along with the file name : \n";
	string path;
	cin >> path;
	vector<vector<unsigned char>> image = Read_PGM(path);

	int option;
	cout << "enter 1 to zoom in and 2 to zoom out\n";
	cin >> option;
	if (option==1)
	{
		zoom_in(image);
	}
	else if (option==2)
	{
		zoom_out(image);
	}
	return 0;
}