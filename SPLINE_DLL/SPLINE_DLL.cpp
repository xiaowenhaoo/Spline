// SPLINE_DLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


//功能：5阶B样条插值
//参数t[]：为时间节点数组，长度为6；
//参数v[]：为采样点数组，长度为6，即t[i]时刻的采样值为v[i]；
//参数interval:采样周期时间间隔；
//参数 x :为t[4]至t[5]之间的时刻值，x的取值间隔根据平台的控制周期而定；
//返回值：为x时刻的控制值，此值直接发送到下位机平台；
__declspec(dllexport) float spline(float t[], float v[], float interval, float x)
{
	int k = 6;  //5阶样条
	int n = 6;  //6个控制点

	float knots[12];  //时间节点序列
	float coefs[6];  //控制点序列

	int index = 0;

	for (int i = 0; i < n + k; i++)
	{
		if (i < n)
			knots[i] = t[i];
		else
			knots[i] = t[5] + (i - 5)*interval;
	}

	for (int i = 0; i < n; i++)
	{
		coefs[i] = v[i];
	}

	//de-boor
	float tx[10];  //tx[]的长度为2*(k-1)；
	float b[6];   //b[]的长度为k；

	//插值点x必须在节点序列[k,n-1]之间
	if (x < knots[k - 1])
		x = knots[k - 1];
	else if (x > knots[n])
		x = knots[n];

	for (int i = k - 1; i < n; i++)
	{
		if (x < knots[i])
		{
			index = i;
			break;
		}
		else if (x >= knots[n - 1])
		{
			index = n;
		}
	}

	for (int i = 0; i < 2 * (k - 1); i++)
	{
		tx[i] = knots[i + 1 - k + index];
		tx[i] -= x;
	}


	for (int i = 0; i < k; i++)
	{
		b[i] = coefs[i - k + index];
	}

	for (int r = 0; r < k - 1; r++)
	{
		for (int i = 0; i < k - r - 1; i++)
		{
			b[i] = (tx[i + k - 1] * b[i] - tx[i + r] * b[i + 1]) / (tx[i + k - 1] - tx[i + r]);
		}
	}

	return b[0];
}

//三次B样条插值计算
//knots[]:节点向量，长度必须为8
//coefs[]:控制定点，长度必须为4
//x:插值点
//返回值:对应于x的
__declspec(dllexport) float fnval(float knots[], float coefs[], float x)
{
	int k = 4;
	int n = 4;
	int index = 0;

	//de-boor
	float tx[6];  //tx[]的长度为2*(k-1)；
	float b[4];   //b[]的长度为k；

	//插值点x必须在节点序列[k,n-1]之间
	if (x < knots[k - 1])
		x = knots[k - 1];
	else if (x > knots[n])
		x = knots[n];

	for (int i = k - 1; i < n; i++)
	{
		if (x < knots[i])
		{
			index = i;
			break;
		}
		else if (x >= knots[n - 1])
		{
			index = n;
		}
	}

	for (int i = 0; i < 2 * (k - 1); i++)
	{
		tx[i] = knots[i + 1 - k + index];
		tx[i] -= x;
	}


	for (int i = 0; i < k; i++)
	{
		b[i] = coefs[i - k + index];
	}

	for (int r = 0; r < k - 1; r++)
	{
		for (int i = 0; i < k - r - 1; i++)
		{
			b[i] = (tx[i + k - 1] * b[i] - tx[i + r] * b[i + 1]) / (tx[i + k - 1] - tx[i + r]);
		}
	}

	return b[0];
}
