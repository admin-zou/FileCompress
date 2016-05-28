/*
	优化版
	1.文件的长度可以不用写进配置文件了，而是通过哈夫曼树的根节点的权值确定
	2.用feof来判断是否到文件末尾了
*/
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include "Huffman.h"
#include <assert.h>
#include <time.h>
using namespace std;

typedef unsigned long long FileLenType;

struct FileInformation
{
	unsigned char		_data;         //字符
	FileLenType	    _appnums;    //出现次数
	string			_coding;    //对应哈弗曼编码

	FileInformation(unsigned char ch = '\0')
		: _data(ch)
		, _appnums(0)
		, _coding("")
	{}

	FileInformation(FileLenType times)
		: _data('\0')
		, _appnums(times)
		, _coding("")
	{}

	//重载相应的功能，这些功能为后面所用
	//注意const对象调用const方法
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
public:
	FileCompress()
	{ //默认构造函数
		for (size_t i = 0; i < 256; ++i)
		{
			_File[i]._data = i;
		}
	}

	//文件压缩
	void compressFile(const char* InputFile)
	{
		// 1.编码
		FILE *pr = fopen(InputFile, "rb");

		//统计各个字符出现的次数
		unsigned char ch = 0;
		ch = fgetc(pr);
		while (!feof(pr))
		{
			_File[ch]._appnums++;
			ch = fgetc(pr);
		}

		//统计好了字符出现次数，以其次数为权值建立哈弗曼树
		const FileInformation filter((unsigned char)0);  //非法值
		HuffmanTree<FileInformation> Ht(_File, 256, filter);

		//生成哈弗曼编码
		string tmp;
		//递归进行遍历，找出所有编码
		_CreateHuffmanCode(Ht.Root(), tmp);

		//2. 压缩
		fseek(pr, 0, SEEK_SET); //回到文件头部

		string file = InputFile;
		file += ".huffman";      //压缩后的文件名
		FILE* pw = fopen(file.c_str(), "wb");
		if (pw == NULL)
		{
			cout << "open" << file.c_str() << "failed" << endl;
			exit(-1);
		}

		unsigned char value = 0;
		int index = 0; //标记当前位
		ch = fgetc(pr);
		while (!feof(pr))
		{	
			string coding = _File[ch]._coding;
			for (size_t i = 0; i < coding.size(); ++i)
			{ //注意小端存贮的规律
				value <<= 1;
				if (coding[i] == '1')
				{
					value |= 1;
				}
				else{
					//do nothing
				}
				//若写满一个字节了
				if (++index == 8)
				{
					fputc(value, pw);
					value = 0;
					index = 0;
				}
			}
			ch = fgetc(pr);
		}
		//最后出来后value很可能没有被写满，但是还是需要将其写入压缩文件中
		if (index != 0)
		{//说明未填满
			value <<= (8 - index);
			fputc(value, pw);
		}

		//使用配置文件将字符的个数写进去，以及各个字符出现的次数(便于重建哈弗曼树)
		string configFile = InputFile;
		configFile += ".config";
		FILE *pInfo = fopen(configFile.c_str(), "wb");
		if (pInfo == NULL)
		{
			cout << "open" << configFile.c_str() << "failed" << endl;
			exit(-1);
		}
				
		//重建的哈夫曼树的根节点的权值就是字符的总数目，所以不必再去将字符的次数写进配置文件了
		//将出现过的字符以及其出现的次数写进配置文件
		string strInfo;
		char infoStr[32];
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
				strInfo.clear();     //记得清理string
			}
		}
		//记得关闭打开的文件
		fclose(pr);
		fclose(pw);
		fclose(pInfo);
	}

	void UncompressFile(const char* InputFile)
	{
		unsigned char ch = 0;
		string config = InputFile;
		config += ".config";
		FILE* pInfo = fopen(config.c_str(), "rb");
		assert(pInfo);

		string oneline;
		//获取各个字符出现的次数
		while (Readline(pInfo, oneline))
		{
			if (!oneline.empty())
			{   //文件里面保存的格式类似“a,123”
				ch = oneline[0]; //获取char
				//使用string::substr(pos)函数提取字符出现的次数，比使用sscanf更简单
				_File[(unsigned char)ch]._appnums = atoi(oneline.substr(2).c_str());
				oneline.clear();
			}
			else
			{	/*
				*需要注意的是，我们要考虑一种情况，那就是“\n,123”，读到 \n 时，oneline为空
				*但这个时候我们现将其赋值为‘\n’(因为它本身就是'\n'),接下里会读到它的次数的
				*/
				oneline = '\n';
			}
		}
		//重新建立哈弗曼树
		const FileInformation filter((unsigned char)0);
		HuffmanTree<FileInformation> Ht(_File, 256, filter);
		HuffmanNode<Info> * root = Ht.Root();  //哈弗曼树头结点
		//解压缩，还原文件
		string output = InputFile;
		output += ".uncompress";
		FILE *pout = fopen(output.c_str(), "wb");
		assert(pout);
		//根据压缩文件还原文件
		string huffuamnfile = InputFile;
		huffuamnfile += ".huffman";
		FILE *pcompress = fopen(huffuamnfile.c_str(), "rb");
		assert(pcompress);
		//利用哈弗曼树节点来提供信息
		HuffmanNode<Info> * cur = root;
		ch = fgetc(pcompress);
		int pos = 8;

		//哈夫曼树的根节点的权值就是字符的总的次数
		FileLenType length = root->_data._appnums;

		while (length > 0)
		{//每个ch占8位						
			while (cur->_left && cur->_right)
			{
				if (pos == 0)
				{ //小端存储
					ch = fgetc(pcompress);
					pos = 8;
				}
				--pos;
				if (ch & (1 << pos))
				{ //第pos位为 1
					cur = cur->_right;
				}
				else
				{  //ch的第pos位是 0
					cur = cur->_left;
				}
			}
			//找到最后对应编码
			if (cur)
			{
				fputc(cur->_data._data, pout);
				cur = root;
				--length;  //字符总数目减1
			}
		}
		//cout << "completed!" << endl;
		fclose(pInfo);
		fclose(pout);
		fclose(pcompress);
	}

	//读取文件的一行，并将其保存到ret中
	bool Readline(FILE* pf, string & ret)
	{
		assert(pf);
		char ch = fgetc(pf);
		if (feof(pf))
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
	//递归遍历哈弗曼树(二叉树)，找到其所有编码，并写到_File[n]._coding中
	void _CreateHuffmanCode(HuffmanNode<Info>* root, string tmp)
	{
		if (root == NULL)
		{
			return;
		}
		if (root->_left == NULL && root->_right == NULL)
		{ //叶子节点，对应着编码
			int index = (root->_data)._data;
			_File[index]._coding = tmp;
		}
		else
		{
			if (root->_left)
			{ //左边存在节点，给tmp后面添加'0'
				tmp.push_back('0');
				_CreateHuffmanCode(root->_left, tmp);
				tmp.pop_back();
			}
			if (root->_right)
			{ //右边存在节点，给tmp后面添加'1'
				tmp.push_back('1');
				_CreateHuffmanCode(root->_right, tmp);
				tmp.pop_back();
			}
		}
	}

private:
	Info   _File[256];
};


void TestOptFuffmanCode()
{
	FileCompress Fc;

	//计算压缩和解压缩的时间
	int ct = 0;
	int ut = 0;
	
	//Fc.compressFile("input");
	//ct = clock();
	//printf("compress file takes %f secodes \n", (float)ct / CLOCKS_PER_SEC);
	//Fc.UncompressFile("input");
	//ut = clock();
	//printf("uncompress file takes %f secodes \n", (float)ut / CLOCKS_PER_SEC);
	

	Fc.compressFile("Input.BIG");
	ct = clock();
	printf("compress file takes %f secodes \n", (float)ct / CLOCKS_PER_SEC);
	Fc.UncompressFile("Input.BIG");
	ut = clock();
	printf("uncompress file takes %f secodes \n", (float)ut / CLOCKS_PER_SEC);
}