#include "DepthDataBase.h"

DepthDataBase::DepthDataBase() :
kinectSensor(NULL),
depthFrameReader(NULL),
depthRGBX(NULL)
{
	depthRGBX = new RGBQUAD[XResDefault * YResDefault];
	depthZArray = new USHORT[XResDefault * YResDefault];
}

HRESULT DepthDataBase::InitializeDefaultSensor()
{
	HRESULT depthResult;

	depthResult = GetDefaultKinectSensor(&kinectSensor);
	if (FAILED(depthResult))
	{
		return depthResult;
	}

	if (kinectSensor)
	{
		IDepthFrameSource* pDepthFrameSource = NULL;
		depthResult = kinectSensor->Open();
		if (SUCCEEDED(depthResult))
		{
			depthResult = kinectSensor->get_DepthFrameSource(&pDepthFrameSource);
		}
		if (SUCCEEDED(depthResult))
		{
			depthResult = pDepthFrameSource->OpenReader(&depthFrameReader);
		}
		SafeRelease(pDepthFrameSource);
	}

	return depthResult;
}

void DepthDataBase::Update()
{
	if (!depthFrameReader)
	{
		return;
	}

	IDepthFrame* pDepthFrame = NULL;
	HRESULT depthResult = depthFrameReader->AcquireLatestFrame(&pDepthFrame);

	if (SUCCEEDED(depthResult))
	{

		INT64 nTime = 0;
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxDistance = 0;
		UINT nBufferSize = 0;
		UINT16 *pBuffer = NULL;
		UINT framePixels;

		depthResult = pDepthFrame->get_RelativeTime(&nTime);

		if (SUCCEEDED(depthResult))
		{
			depthResult = pDepthFrame->get_FrameDescription(&pFrameDescription);
		}
		if (SUCCEEDED(depthResult))
		{
			depthResult = pFrameDescription->get_Width(&nWidth);
		}
		if (SUCCEEDED(depthResult))
		{
			depthResult = pFrameDescription->get_Height(&nHeight);
		}
		if (SUCCEEDED(depthResult))
		{
			depthResult = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}
		if (SUCCEEDED(depthResult))
		{
			// In order to see the full range of depth (including the less reliable far field depth)
			// we are setting nDepthMaxDistance to the extreme potential depth threshold
			nDepthMaxDistance = USHRT_MAX;
			// Note:  If you wish to filter by reliable depth distance, uncomment the following line.
			//// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
		}
		//if (SUCCEEDED(depthResult))
		//{
		//	depthResult = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		//}
		if (SUCCEEDED(depthResult))
		{
			pFrameDescription->get_LengthInPixels(&framePixels);
			pDepthFrame->CopyFrameDataToArray(framePixels, depthZArray);
			this->CutDepth(depthZArray, maxDepth , minDepth , framePixels);
			this->ProcessDepth(depthZArray, nWidth, nHeight, nDepthMinReliableDistance, nDepthMaxDistance);
		}
		SafeRelease(pFrameDescription);
	}
	SafeRelease(pDepthFrame);
}

void DepthDataBase::CutDepth(USHORT* depthZArray, int maxDepth, int minDepth, int length)
{
	for (int i = 0; i < length; ++ i)
	{
		if (depthZArray[i] >= maxDepth)
		{
			depthZArray[i] = 0;
		}
		else if (depthZArray[i] <= minDepth)
		{
			depthZArray[i] = 0;
		}
	}
}

void DepthDataBase::ProcessDepth(const USHORT* depthZArray, int nWidth, int nHeight, USHORT nMinDepth, USHORT nMaxDepth)
{
	if (depthZArray && (nWidth == XResDefault) && (nHeight == YResDefault))
	{
		point3DList.clear();
		for (ushort x = 0; x < nWidth; ++x)
		{
			for (ushort y = 0; y < nHeight; ++y)
			{

				int depthIndex = (y * nWidth) + x;
				ushort z = depthZArray[depthIndex];

				if (z != 0)
				{
					point3DList.push_back(new Point3D(x, y, z));
				}
			}
		}

		//while (pBuffer < pBufferEnd)
		//{
		//	USHORT depth = *pBuffer;



		//	// To convert to a byte, we're discarding the most-significant
		//	// rather than least-significant bits.
		//	// We're preserving detail, although the intensity will "wrap."
		//	// Values outside the reliable depth range are mapped to 0 (black).

		//	// Note: Using conditionals in this loop could degrade performance.
		//	// Consider using a lookup table instead when writing production code.
		//	//BYTE intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth % 256) : 0);

		//	//pRGBX->rgbRed = intensity;
		//	//pRGBX->rgbGreen = intensity;
		//	//pRGBX->rgbBlue = intensity;

		//	++pRGBX;
		//	++pBuffer;
		//}
	}
}

USHORT DepthDataBase::getDepthByIndex(int index)
{
	return depthZArray[index];
}