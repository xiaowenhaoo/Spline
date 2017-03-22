// SPLINE_DLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"


//���ܣ�5��B������ֵ
//����t[]��Ϊʱ��ڵ����飬����Ϊ6��
//����v[]��Ϊ���������飬����Ϊ6����t[i]ʱ�̵Ĳ���ֵΪv[i]��
//����interval:��������ʱ������
//���� x :Ϊt[4]��t[5]֮���ʱ��ֵ��x��ȡֵ�������ƽ̨�Ŀ������ڶ�����
//����ֵ��Ϊxʱ�̵Ŀ���ֵ����ֱֵ�ӷ��͵���λ��ƽ̨��
__declspec(dllexport) float spline(float t[], float v[], float interval, float x)
{
	int k = 6;  //5������
	int n = 6;  //6�����Ƶ�

	float knots[12];  //ʱ��ڵ�����
	float coefs[6];  //���Ƶ�����

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
	float tx[10];  //tx[]�ĳ���Ϊ2*(k-1)��
	float b[6];   //b[]�ĳ���Ϊk��

	//��ֵ��x�����ڽڵ�����[k,n-1]֮��
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

//����B������ֵ����
//knots[]:�ڵ����������ȱ���Ϊ8
//coefs[]:���ƶ��㣬���ȱ���Ϊ4
//x:��ֵ��
//����ֵ:��Ӧ��x��
__declspec(dllexport) float fnval(float knots[], float coefs[], float x)
{
	int k = 4;
	int n = 4;
	int index = 0;

	//de-boor
	float tx[6];  //tx[]�ĳ���Ϊ2*(k-1)��
	float b[4];   //b[]�ĳ���Ϊk��

	//��ֵ��x�����ڽڵ�����[k,n-1]֮��
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
