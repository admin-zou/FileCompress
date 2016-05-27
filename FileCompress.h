#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include "Huffman.h"
#include <assert.h>
using namespace std;

struct FileInformation
{
	unsigned char		_data;         //�ַ�
	size_t			_appnums;    //���ִ���
	string			_coding;    //��Ӧ����������

	FileInformation(unsigned char ch='\0')
		: _data(ch)
		, _appnums(0)
		, _coding("")
	{}

	FileInformation(size_t times)
		: _data('\0')
		, _appnums(times)
		, _coding("")
	{}

	//������Ӧ�Ĺ��ܣ���Щ����Ϊ��������
	//ע��const�������const����
	bool operator > (const FileInformation& com)const 
	{
		return this->_appnums > com._appnums;
	}

	bool operator != (const FileInformation com)const
	{
		return this->_appnums != com._appnums;
	}

	FileInformation operator + (const FileInformation& com)const
	{
		return FileInformation(this->_appnums + com._appnums);
	}
};

class FileCompress
{
	typedef FileInformation Info;
	typedef long long FileLenType;
public:
	FileCompress()
	{ //Ĭ�Ϲ��캯��
		for (size_t i = 0; i < 256; ++i)
		{
			_File[i]._data = i;
		}
	}
	 
	//�ļ�ѹ��
	void compressFile(const char* InputFile)
	{
		// 1.����
		FILE *pr = fopen(InputFile, "rb");
		FileLenType total = 0;

		//ͳ�Ƹ����ַ����ֵĴ���
		unsigned char ch = 0;
		while ((ch = fgetc(pr)) != EOF)
		{
			_File[ch]._appnums++;
			++total; //����Ϣ��������Ϊ��ѹ���ṩ��Ϣ
		}
		
		//ͳ�ƺ����ַ����ִ������������ΪȨֵ������������
		const FileInformation filter((unsigned char)0);  //�Ƿ�ֵ
		HuffmanTree<FileInformation> Ht(_File, 256, filter);

		//���ɹ���������
		string tmp;
		//�ݹ���б������ҳ����б���
		_CreateHuffmanCode(Ht.Root(), tmp);

		//2. ѹ��
		fseek(pr, 0, SEEK_SET); //�ص��ļ�ͷ��

		string file = InputFile;
		file += ".huffman"; //ѹ������ļ���
		FILE* pw = fopen(file.c_str(), "wb");
		if (pw == NULL)
		{
			cout << "open" << file.c_str() << "failed" << endl;
			exit(-1);
		}

		unsigned char value = 0;
		int index = 0; //��ǵ�ǰλ
		while ((ch = fgetc(pr)) != EOF)
		{
			string coding = _File[ch]._coding;
			for (size_t i = 0; i < coding.size(); ++i)
			{ //ע��С�˴����Ĺ���
				value <<= 1;
				if (coding[i] == '1')
				{
					value |= 1;
				}
				else{
					//do nothing
				}
				//��д��һ���ֽ���
				if (++index == 8)
				{
					fputc(value, pw);
					value = 0;
					index = 0;
				}
			}
		}
		//��������value�ܿ���û�б�д�������ǻ�����Ҫ����д��ѹ���ļ���
		if (index != 0)
		{//˵��δ����
			value <<= (8 - index);
			fputc(value, pw);
		}

		//ʹ�������ļ����ַ��ĸ���д��ȥ���Լ������ַ����ֵĴ���(�����ؽ���������)
		string configFile = InputFile;
		configFile += ".config";
		FILE *pInfo = fopen(configFile.c_str(), "wb");		
		if (pInfo == NULL)
		{
			cout << "open" << configFile.c_str() << "failed" << endl;
			exit(-1);
		}
		//���ַ��ܵĸ���д�������ļ���total �������� long long����Ҫע��
		char infoStr[32];		
		_itoa(total >> 32, infoStr, 10); //(itoa������) 
		fputs(infoStr, pInfo);
		fputc('\n', pInfo);
		_itoa(total & 0xFFFFFFFF, infoStr,10); //ȡ��ʣ�µ�
		fputs(infoStr, pInfo);
		fputc('\n', pInfo);
		//�����ֹ����ַ��Լ�����ֵĴ���д�������ļ�
		string strInfo;
		for (int i = 0; i < 256; ++i)
		{
			if (_File[i]._appnums > 0)
			{
				strInfo.push_back((unsigned char)i);
				strInfo.push_back(',');
				_itoa(_File[i]._appnums, infoStr, 10);
				strInfo += infoStr;
				fputs(strInfo.c_str(), pInfo);
				fputc('\n', pInfo);
				strInfo.clear();     //�ǵ�����string
			}
		}
		//�ǵùرմ򿪵��ļ�
		fclose(pr);
		fclose(pw);
		fclose(pInfo);
	}
	
	void UncompressFile(const char* InputFile)
	{
		unsigned char ch = 0;
		string config = InputFile;
		config += ".config";
		FILE* pInfo = fopen(config.c_str(),"rb");
		assert(pInfo);
		//��ȡ�ַ�����Ŀ
		FileLenType length = 0;  
		string oneline;
		Readline(pInfo, oneline);  //��ȡһ��
		length += atoi(oneline.c_str());
		length <<= 32; //��λ
		oneline.clear();
		Readline(pInfo, oneline); //��λ
		length += atoi(oneline.c_str());
		oneline.clear();  //<��һ������Ҫ©��>

		//��ȡ�����ַ����ֵĴ���
		while (Readline(pInfo, oneline))
		{
			if (!oneline.empty())
			{   //�ļ����汣��ĸ�ʽ���ơ�a,123��
				ch = oneline[0]; //��ȡchar
				//ʹ��string::substr(pos)������ȡ�ַ����ֵĴ�������ʹ��sscanf����
				_File[(unsigned char)ch]._appnums = atoi(oneline.substr(2).c_str());
				oneline.clear();
			}
			else
			{	/*
				*��Ҫע����ǣ�����Ҫ����һ��������Ǿ��ǡ�\n,123�������� \n ʱ��onelineΪ��
				*�����ʱ�������ֽ��丳ֵΪ��\n��(��Ϊ���������'\n'),�������������Ĵ�����
				*/
				oneline = '\n';  
			}
		}
		//���½�����������
		const FileInformation filter((unsigned char)0);
		HuffmanTree<FileInformation> Ht(_File, 256, filter);
		HuffmanNode<Info> * root = Ht.Root();  //��������ͷ���
		//��ѹ������ԭ�ļ�
		string output = InputFile;
		output += ".uncompress";
		FILE *pout = fopen(output.c_str(), "wb");
		assert(pout);
		//����ѹ���ļ���ԭ�ļ�
		string huffuamnfile = InputFile;
		huffuamnfile += ".huffman";
		FILE *pcompress = fopen(huffuamnfile.c_str(), "rb");
		assert(pcompress);
		//���ù��������ڵ����ṩ��Ϣ
		HuffmanNode<Info> * cur = root;
		ch = fgetc(pcompress);
		int pos = 8;
		while (length > 0)
		{//ÿ��chռ8λ						
			while (cur->_left && cur->_right)
			{
				if (pos == 0)
				{ //С�˴洢
					ch = fgetc(pcompress);
					pos = 8;
				}
				--pos;
				if (ch & (1<<pos) )
				{ //��posλΪ 1
					cur = cur->_right;
				}
				else 
				{  //ch�ĵ�posλ�� 0
					cur = cur->_left;
				}
			}
			//�ҵ�����Ӧ����
			if (cur)
			{
				fputc(cur->_data._data, pout);
				cur = root;
				--length;  //�ַ�����Ŀ��1
			}
		}
		cout << "completed!" << endl;
		fclose(pInfo);
		fclose(pout);
		fclose(pcompress);
	}

	//��ȡ�ļ���һ�У������䱣�浽ret��
	bool Readline(FILE* pf,string & ret)
	{ 
		assert(pf);
		char ch = fgetc(pf);
		if (ch == EOF)
		{
			return false;
		}

		while (ch != '\n')
		{
			ret += ch;
			ch = fgetc(pf);
		}
		return true;
	}

protected:
	//�ݹ������������(������)���ҵ������б��룬��д��_File[n]._coding��
	void _CreateHuffmanCode(HuffmanNode<Info>* root,string tmp)
	{
		if (root == NULL)
		{
			return ;
		}
		if (root->_left == NULL && root->_right==NULL)
		{ //Ҷ�ӽڵ㣬��Ӧ�ű���
			int index = (root->_data)._data;
			_File[index]._coding = tmp;
		}
		else
		{
			if (root->_left)
			{ //��ߴ��ڽڵ㣬��tmp�������'0'
				tmp.push_back('0');
				_CreateHuffmanCode(root->_left, tmp);
				tmp.pop_back();
			}
			if (root->_right)
			{ //�ұߴ��ڽڵ㣬��tmp�������'1'
				tmp.push_back('1');
				_CreateHuffmanCode(root->_right, tmp);
				tmp.pop_back();
			}
		}
	}

private:
	Info   _File[256];
};



void TestFuffmanCode()
{
	FileCompress Fc;
	Fc.compressFile("input");
	Fc.UncompressFile("input");
}