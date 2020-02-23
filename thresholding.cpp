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
int Write_PGM(vector< vector<unsigned char> > &image, int thresh)
{
	ofstream fout("output.pgm", ios::binary);
    if(!fout.is_open())
    {
        cerr << "Error! Unable to open file .\n";
        return 0;
    }
    fout << "P5\n" << w << " " << h << "\n" << 255 << "\n";

    int temp = 256/thresh;
    char ch;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
        	int temp2 = temp;
        	while(image[i][j]>=temp2)
        	{
        		temp2+=temp;
        	}

        	ch = (char)(temp2-temp);
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
	int thresh;
	cout << "Enter the value to break into levels : \n";
	cin >> thresh;
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