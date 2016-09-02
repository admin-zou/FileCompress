项目分析
  项目的常见问题总结与分析的相关部分我放到了博客里面去了，大家可以去我的博客看看吧，博客地址：
  
  http://blog.csdn.net/zr1076311296/article/details/51519807
  http://blog.csdn.net/zr1076311296/article/details/51527540


  项目简介：利用哈夫曼编码实现文件压缩与解压
  项目描述：
    > 编码：以字符出现的频率为权值，利用Heap与贪心算法建立哈夫曼树
    > 压缩：读取文件并根据编码压缩，统计字符信息写进配置文件便于恢复二叉树
    > 解压：根据配置文件重建哈夫曼编码树，利用编码树对被压缩文件进行解压
    > 测试：对不同文件进行测试，通过Beyond Compare软件进行进行验证
  项目总结：
    > 开发环境：VS2013/Linux，vim，gcc/g++，gdb，heap等
    > URL：https://github.com/admin-zou/FileCompress
