#ifndef DEPTHDATABASE_H
#define DEPTHDATABASE_H

#include "DataBase.h"

class DepthDataBase : DataBase
{
public :
	static const int XResDefault = 512;
	static const int YResDefault = 424;
	static const int maxDepth = 5000;
	static const int minDepth = 500;

	DepthDataBase();
	~DepthDataBase();

	HRESULT InitializeDefaultSensor();
	void Update();
	USHORT getDepthByIndex(int index);
	list<Point3D *>			point3DList;

private:
	IKinectSensor*          kinectSensor;
	IDepthFrameReader*      depthFrameReader;
	RGBQUAD*                depthRGBX;
	USHORT*					depthZArray;
	
	void ProcessDepth(const USHORT* depthZArray, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth);
	void CutDepth(USHORT* depthZArray, int maxDepth, int minDepth, int length);
};

#endif