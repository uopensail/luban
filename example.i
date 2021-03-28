// example.i
%module example
%{
// 此部分会直接拷贝到wrapper文件中，用于编译
extern int fact(int);
%}
// 比部分是SWIG需要进行Wrapper的部分
extern int fact(int);