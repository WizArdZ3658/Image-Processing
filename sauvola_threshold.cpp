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
int Write_PGM(vector< vector<unsigned char> > &image)
{
    ofstream fout("sauvola_threshold_image.pgm", ios::binary);
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
vector< vector<unsigned char> > Sauvola(vector< vector<unsigned char> > &image)
{
    vector< vector<unsigned char> > image2(h, vector<unsigned char>(w));
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            double mean = 0;
            int x_start = j - 7, x_end = j + 7, y_start = i - 7, y_end = i + 7;
            for (int i2 = x_start; i2 <= x_end; ++i2)
            {
                for (int j2 = y_start; j2 <= y_end; ++j2)
                {
                    int i3 = i2, j3 = j2;
                    if (i3<0)
                    {
                        i3 = abs(i3);
                    }
                    if (i3>=w)
                    {
                        i3 = w - (i3-w+2);
                    }
                    if (j3<0)
                    {
                        j3 = abs(j3);
                    }
                    if (j3>=h)
                    {
                        j3 = h - (j3-h+2);
                    }
                    mean += image[j3][i3];
                }
            }
            mean = mean/225;
            double SD = 0;
            for (int i2 = x_start; i2 <= x_end; ++i2)
            {
                for (int j2 = y_start; j2 <= y_end; ++j2)
                {
                    int i3 = i2, j3 = j2;
                    if (i3<0)
                    {
                        i3 = abs(i3);
                    }
                    if (i3>=w)
                    {
                        i3 = w - (i3-w+2);
                    }
                    if (j3<0)
                    {
                        j3 = abs(j3);
                    }
                    if (j3>=h)
                    {
                        j3 = h - (j3-h+2);
                    }
                    int u = image[j3][i3] - mean;
                    SD += u*u;
                }
            }
            SD = sqrt(SD);
            SD = SD/15;
            int thresh_value = (int)(mean*(1-(0.01*(1-(SD/128)))));
            //cout << i << ":" << j << " "; 
            if (image[i][j]>=thresh_value)
            {
                image2[i][j] = (unsigned char)(255);
            }
            else
            {
                image2[i][j] = (unsigned char)(0);
            }
        }
    }
    return image2;
}
int main()
{
    cout << "Enter path along with the file name : \n";
    string path;
    cin >> path;
    vector<vector<unsigned char>> image = Read_PGM(path);
    vector<vector<unsigned char>> sauvola_binarized_image = Sauvola(image);
    if(Write_PGM(sauvola_binarized_image))
    {
        cout << "Successful Output\n";
    }
    else
    {
        cout << "Failed\n";
    }
    return 0;
}