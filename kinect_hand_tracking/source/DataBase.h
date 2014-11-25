#ifndef DATABASE_H
#define DATABASE_H


#include <Kinect.h>
#include <Windows.h>
#include <qtcore/qstring>
#include <QtCore/qbytearray.h>

#include <string>
#include <vector>
#include <utility>
#include <list>

#include "Point3D.h"

using namespace std;

class DataBase
{

private:

public:
	BYTE * ReadDepthWithColor_PLY(string path, string fileName, USHORT * depthData);
	bool WriteListOfPoints(list<Point3D> listOfPoints, string fileName);
};

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

#endif
