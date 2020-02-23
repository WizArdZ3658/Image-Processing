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
int start_BFS(vector< vector<unsigned char> > &image, vector< vector<unsigned char> > &visited, int h1, int w1)
{
	int H[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int W[] = {-1, 0, 1, -1, 1, -1, 0, 1};
	queue<pair <int, int>> q;
	q.push(make_pair(h1,w1));
	int count = 0;

	while(!q.empty())
	{
		pair<int , int> temp =  q.front();
		q.pop();
		int start_h = temp.first;
		int start_w = temp.second;
		bool executed_once = false;
		for (int i = 0; i < 8; ++i)
		{
			if (start_h+H[i] >= 0 && start_h+H[i] < h && start_w+W[i] >= 0 && start_w+W[i] < w && image[start_h+H[i]][start_w+W[i]]!=255 && visited[start_h+H[i]][start_w+W[i]]==0)
			{
				q.push(make_pair(start_h+H[i], start_w+W[i]));
				visited[start_h+H[i]][start_w+W[i]] = 1;
				executed_once = true;
			}
		}

		if (!executed_once)
		{
			image[start_h][start_w] = 255;
			count++;
		}

	}

	return count;
}
int rem_redundant_pxls(vector< vector<unsigned char> > &image)
{
	vector<vector <unsigned char>> visited(h, vector<unsigned char>(w,0));
	int count = 0;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			if (image[i][j] != 255 && visited[i][j]==0)
			{
				count += start_BFS(image, visited, i, j);
			}
		}
	}

	return count;
}
int Write_PGM(vector< vector<unsigned char> > &image)
{
	ofstream fout("redundant2.pgm", ios::binary);
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
	while(true)
	{
		int pixels_removed = rem_redundant_pxls(image);
		if (pixels_removed == 0)
		{
			break;
		}
	}
	
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