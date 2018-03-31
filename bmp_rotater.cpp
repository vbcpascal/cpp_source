#include <iostream>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

bool readbmp(const char *bmpname,const char *outname)
{
	ifstream inFile(bmpname, ios::in | ios::binary);
	if (!inFile)
	{
		cout << "error: can not open this bmp file.";
		return false;
	}
	BITMAPFILEHEADER bitFilInfo; //14
	inFile.read((char*)&bitFilInfo, sizeof(BITMAPFILEHEADER));
	BITMAPINFOHEADER bitMapInfo; //40
	inFile.read((char*)&bitMapInfo, sizeof(BITMAPINFOHEADER));
	unsigned char* p = new unsigned char[84];
	if (bitMapInfo.biBitCount == 32) {
		inFile.read((char*)p, 84);
	}
	int oldWidth = bitMapInfo.biWidth;
	int oldHeight = bitMapInfo.biHeight;
	const int jw = bitMapInfo.biBitCount == 32 ? 4 : 3;
	auto oldDatasize = ((oldWidth * jw + 3)&(~3))*oldHeight;
	unsigned char* oldColorData = new unsigned char[oldDatasize];
	inFile.read((char*)oldColorData, oldDatasize);

	int Width = oldHeight;
	int Height = oldWidth;
	auto DataSize = ((Width * jw + 3)&(~3))*Height;
	unsigned char* colorData = new unsigned char[DataSize];
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			*(colorData + (i*Width + j) * jw) = *(oldColorData + (j*oldWidth + (Height - 1 - i)) * jw);
			*(colorData + (i*Width + j) * jw + 1) = *(oldColorData + (j*oldWidth + (Height - 1 - i)) * jw + 1);
			*(colorData + (i*Width + j) * jw + 2) = *(oldColorData + (j*oldWidth + (Height - 1 - i)) * jw + 2);
			if (jw == 4) {
				*(colorData + (i*Width + j) * jw + 3) = *(oldColorData + (j*oldWidth + (Height - 1 - i)) * jw + 3);
			}
		}
	}
	ofstream outFile(outname, ios::out | ios::binary);
	bitMapInfo.biWidth = Width;
	bitMapInfo.biHeight = Height;
	outFile.write((char*)&bitFilInfo, sizeof(BITMAPFILEHEADER));
	outFile.write((char*)&bitMapInfo, sizeof(BITMAPINFOHEADER));
	if (bitMapInfo.biBitCount == 32) {
		outFile.write((char*)p, 84);
	}
	outFile.write((char*)colorData, DataSize);
	inFile.close();
	outFile.close();
}

int main(int argc,char * argv[]) {
	cout << "Welcome to use Picture_Rotate Tool!" << endl;
	string s1; string s2;
	switch (argc) {
	case 1:
		cout << "From:";
		 cin >> s1;
		cout << "To:";
		 cin >> s2;
		readbmp(s1.c_str(), s2.c_str());
		break;
	case 3:
		cout << "Waiting.." << endl;
		readbmp(argv[1], argv[2]);
		break;
	default:
		cout << "Error Input!" << endl;
		system("pause");
		return 0;
	}
	cout << "Succeed!" << endl;
	system("pause");
}
