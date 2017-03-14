#include <stdio.h>
#include "highgui.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <memory.h>
#include <io.h>  
#include <math.h>
#include "PreProduce.h"
#include <fstream>
#include <wchar.h>
#include <tchar.h>
#include <Windows.h> 
#include <CString>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )


#define N 360//Hֱ��ͼ�ļ���
  
using namespace std;  

struct img{
	string filename;
	string dst;
	int hist1[N];
	int hist2[20]; 
	int hist3[20];
	double dist ;
	int tag;//H��ֱ��ͼ�еķ�ֵ
	int flag;//S��ֱ��ͼ�еķ�ֵ
	int mark;
	int a;
	int hij;
	int sij;
    };
struct img every[110];//ͼƬ��ͼƬ
struct img exam;//����ͼƬ
int num=0;//ͼƬ���ͼƬ����



double fangcha(int x[], int n)
{
 //������x������n��Ԫ�أ��ķ���:S=(<x^2>-<x>)^0.5
 int i;
 double xaver=0.0, x2aver=0.0;
 for(i=0;i<n;++i)
 {
  xaver+=x[i]; x2aver+=x[i]*x[i];
 }
 xaver/=n; x2aver/=n; //��x��ƽ����x^2��ƽ��
 return sqrt(x2aver-xaver*xaver);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����HV��ֱ��ͼ�ķ�ֵtag��mark
//����Ϊ100ʱ�������ͼƬ�ģ��������ͼƬ����ĳ��ͼƬ��
void mostvalue(int n)
{
	int i,t=0,t1=0,t2=0,c;

	if(n<100)//����ͼƬ����ĳ��ͼƬ��
	{
	
		for(i=0;i<N;i++)
			if(every[n].hist1[t]<every[n].hist1[i] )
				t=i;
		every[n].hij=t;

			//double b=(t*36/360);

		if(t>=10&&t<115)
			every[n].tag=(int)(t*36/360);
		else if(t>=115&&t<185)
			every[n].tag= 12;
		else if(t>=185&&t<195)
			every[n].tag= 14;
		else if(t>=195&&t<219)
			every[n].tag= 14;
		else if(t>=219&&t<230)
			every[n].tag =16;
		else if(t>=230&&t<240)
			every[n].tag =18;
		else if(t>=240&&t<260)
			every[n].tag =19;
		else 
			every[n].tag = 0;
			

		t=0;

		for(i=0;i<20;i++)
			if(every[n].hist2[t]<every[n].hist2[i] )
				t=i;
		every[n].flag=t;

		t=0;

		for(i=0;i<20;i++)
			if(every[n].hist3[t]<every[n].hist3[i] )
				t=i;
		every[n].mark=t;


		

	}

	//�������ͼƬ��
	else
	{
		//H
		for(i=0;i<N;i++)
		{
			if(exam.hist1[t]<exam.hist1[i])
				t=i;
		}
		exam.hij=t;

			if(t>=10&&t<115)
				exam.tag=(int)(t*36/360);
			else if(t>=115&&t<185)
				exam.tag= 12;
			else if(t>=185&&t<195)
				exam.tag= 14;
			else if(t>=195&&t<214)
				exam.tag= 14;
			else if(t>=214&&t<230)
				exam.tag =16;
			else if(t>=230&&t<240)
				exam.tag =18;
			else if(t>=240&&t<260)
				exam.tag =19;
			else 
				exam.tag = 0;

		t=0;

	
		for(i=0;i<20;i++)
			if(exam.hist2[t]<exam.hist2[i] )
				t=i;
		exam.flag=t;

		t=0;
		for(i=0;i<20;i++)
			if(exam.hist3[t]<exam.hist3[i] )
				t=i;
		exam.mark=t;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����ֵ
int max_val(int a, int b, int c)
{
	int m;
	if(a>b)
		m=a;
	else
		m=b;
	if(m<c)
		m=c;
	return m;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����Сֵ
int min_val(int a, int b, int c)
{
	int m;
	if(a<b)
		m=a;
	else
		m=b;
	if(m>c)
		m=c;
	return m;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RGBתHSV
void change(int r, int g, int b, float *h, float *s, float *v)
{
	int max,min;
	max=max_val(r,g,b);
	min=min_val(r,g,b);

	if(max==min)
	{
		*h=0;
	}
	else
	{
		if(r==max)
			*h=(float)(g-b)/(float)(max-min);
		else if(g==max)
			*h=2+(float)(b-r)/(float)(max-min);
		else
			*h=4+(float)(r-g)/(float)(max-min);
		*h=*h*60;
		if(*h<0)
			*h+=360;
	}

	*v=max;
	*s=((float)(max-min))/((float)max);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HSVȡֵ��Χ�涨��
int nomal(float c, int n)
{
	int tem;
	if(n==1)
		return (int)c;
	else if(n==2)
		return (int)(c/255*20+0.5)-1;
	else
		return (int)(c*20+0.5)-1;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RGBֱ��ͼ(ʵ����û���õ�)
void creathist1(char *name, int hist1[], int hist2[], int hist3[])
{
	IplImage  *img=NULL;
	int height,width;
	int i,j,r,g,b;
	CvScalar c;

	img = cvLoadImage(name);
	height=img->height-30;
	width=img->width-30;

	for(int i = 0;i < height;i++)
	{
        for(int j = 0;j < width;j++)
        {
		   c = cvGet2D(img,i,j); // get the (i,j) pixel value
		   r=c.val[0];
		   g=c.val[1];
		   b=c.val[2]; 
		   hist1[r]++;
		   hist2[g]++;
		   hist3[b]++;
        }
	}

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HSVֱ��ͼ
void creathist(char *name, int hist1[], int hist2[],int hist3[])
{
	IplImage  *img=NULL;
	Mat mat,mat1;
	int height,width;
	int i,j,r,g,b,nh,nv,ns;
	float H, V, S;
	float *h=&H, *v=&V, *s=&S;
	CvScalar c;

	img = cvLoadImage(name);
	//PreProduce(name,mat);
	
	//img=&IplImage(src);

	
    //mat1=mat.clone();// �������ݵ���ȸ��ƣ��Է���mat���ݵĸ���
    //img=cvCreateImage(cvSize(mat.cols,mat.rows),8,3); //����ʵ�ʽ��г�ʼ��
    //img->imageData=(char*)mat1.data;

	height=img->height -32 ;
	width=img->width -32 ;

	for(int i = 32;i < height;i++)
	{
        for(int j = 32;j < width;j++)
        {
		   c = cvGet2D(img,i,j); // ��ȡͼ�����ص��ֵ
		   r=c.val[0];
		   g=c.val[1];
		   b=c.val[2]; 
		   
		   if(r!=0||g!=0||b!=0)//
		  { 
				change(r,g,b,h,s,v);
				nh=nomal(H,1);
				ns=nomal(S,3);
				nv=nomal(V,2);
				/*if(nh==0)
					printf("%d,%d,%d,%d,%d\n",r,g,b,i,j);*/
				
				hist1[nh]++;
				hist2[ns]++;
				hist3[nv]++;
		   }
        }
	}

}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ļ����е�ÿ��ͼ����д���
void everyhist(char *road,char *dst)
{
	char result[200]="C:\\��ͬѧ��\\���";

	string fileFolderPath = road;
	string fileFolderPathDst=dst;
	string fileExtension = "bmp";
	string fileFolderPathRes=result;
	
   
      
    string fileFolder = fileFolderPath + "\\*." + fileExtension;  

	// �����ļ���  
    char fileName[2000];  
	char fileNameDst[2000];
  
    struct _finddata_t fileInfo;    // �ļ���Ϣ�ṹ��  
  
    // 1. ��һ�β���  
    long findResult = _findfirst(fileFolder.c_str(), &fileInfo);            
    if (findResult == -1)  
    {  
        _findclose(findResult);   
        return ;  
    }  
	// 2. ѭ������  
    do   
    {  
        sprintf(fileName, "%s\\%s", fileFolderPath.c_str(), fileInfo.name); 
		sprintf(fileNameDst,"%s\\%s",fileFolderPathDst.c_str(), fileInfo.name);
		sprintf(result,"%s\\%s",fileFolderPathRes.c_str(), fileInfo.name);
  
        if ( fileInfo.attrib == _A_ARCH)  // �Ǵ浵�����ļ�  
        {  
           /* cout<<"fileName: "<<fileName<<endl;  */
			every[num].filename = fileName;
			every[num].dst=result;
			
			//����HVֱ��ͼ
			PreProduce(fileName,fileNameDst);

			creathist(fileNameDst,every[num].hist1,  every[num].hist2,every[num].hist3);
			num++;
        }  

		//����tag��flag,mark
		mostvalue(num-1);

    }
	while (!_findnext(findResult, &fileInfo));    
  
  
    _findclose(findResult);   
  
    return ;  
} 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�Ƚ�ͼƬ��������ͼ�������ͼ��ľ���
void compare()
{
	int i;
	
	
	for(i=0; i<num; i++)
	{
		if(every[i].mark<=5)
		{
			every[i].a=0;
		}
		else if(every[i].mark>=10&&every[i].flag<=2)
			every[i].a=2;
		else
			every[i].a=1;

		if(exam.mark<=5)
		{
			exam.a=0;
		}
		else if(exam.mark>=10&&exam.flag<=2)
			exam.a=2;
		else
			exam.a=1;

		if(every[i].tag>=exam.tag)
		{
			if((every[i].tag-exam.tag)<(20/2))
			{
				every[i].hij = every[i].hij-exam.hij;
				every[i].hij *= (every[i].tag-exam.tag);
			}
			else
			{	every[i].hij = 360-(every[i].hij-exam.hij);
				every[i].hij *=(20-(every[i].tag-exam.tag));
			}
		}
		else if(every[i].tag<exam.tag)
		{
			if((exam.tag-every[i].tag)<(20/2))
			{
				every[i].hij = exam.hij -every[i].hij;
				every[i].hij *= (exam.tag-every[i].tag);
			}
			else
			{
				every[i].hij = 360-(exam.hij-every[i].hij);
				every[i].hij *=(360-(exam.tag-every[i].tag));
			}
		}

		
		if(every[i].flag>=exam.flag)
			every[i].sij=every[i].flag-exam.flag;
		else
			every[i].sij=exam.flag-every[i].flag;


		every[i].sij*=1;

		if(every[i].a==0&&exam.a==0)
		{
			if(every[i].mark>=exam.mark)
				every[i].dist=10.2*(every[i].mark-exam.mark)+every[i].hij;
			else
				every[i].dist=10.2*(exam.mark-every[i].mark)+every[i].hij;

		}
		else if(every[i].a==2&&exam.a==2)
		{
			if(every[i].mark>=exam.mark)
				every[i].dist=(every[i].mark-exam.mark);
			else
				every[i].dist=(exam.mark-every[i].mark);
		
		}
		else if(fabs((double)(every[i].a-exam.a))==2)
		{
			every[i].dist=16.2*every[i].hij+1.9*every[i].sij+300;
		}
		else if(every[i].a==0||exam.a==0)
		{
			if(every[i].mark>=exam.mark)
				every[i].dist=0.22*every[i].hij+every[i].sij+2.2*(every[i].mark-exam.mark);
			else
				every[i].dist=0.22*every[i].hij+every[i].sij+2.2*(exam.mark-every[i].mark);
		}
		else if(every[i].a==2||exam.a==2)
		{
			if(every[i].mark>=exam.mark)
				every[i].dist=every[i].hij+every[i].sij+(every[i].mark-exam.mark);
			else
				every[i].dist=every[i].hij+every[i].sij+(exam.mark-every[i].mark);
		}
		else 
		{
			every[i].dist=every[i].hij+every[i].sij;
			if(every[i].mark>=exam.mark)
				every[i].dist+=(every[i].mark-exam.mark);
			else
				every[i].dist+=(exam.mark-every[i].mark);
	
			
		}
	
			
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������img�ṹ�������ֵ
void exchange(int i)
{
	string t;
	int h,j,f;
	double d;

	//filename
	t = every[i].filename;
	every[i].filename = every[i+1].filename;
	every[i+1].filename = t;

	t = every[i].dst;
	every[i].dst = every[i+1].dst;
	every[i+1].dst = t;


	//dist
	d = every[i].dist;
	every[i].dist = every[i+1].dist;
	every[i+1].dist = d;

	//tag
	f = every[i].tag;
	every[i].tag = every[i+1].tag;
	every[i+1].tag = f;

	//flag
	f = every[i].flag;
	every[i].flag = every[i+1].flag;
	every[i+1].flag = f;

	f = every[i].mark;
	every[i].mark = every[i+1].mark;
	every[i+1].mark = f;

	f = every[i].a;
	every[i].a = every[i+1].a;
	every[i+1].a = f;

	f = every[i].hij;
	every[i].hij = every[i+1].hij;
	every[i+1].hij = f;

	f = every[i].sij;
	every[i].sij = every[i+1].sij;
	every[i+1].sij = f;
	//hist1
	for(j=0;j<N;j++)
	{
		h = every[i].hist1[j];
		every[i].hist1[j] = every[i+1].hist1[j];
		every[i+1].hist1[j] = h;
	}

	//hist2
	for(j=0;j<20;j++)
	{
		h = every[i].hist2[j];
		every[i].hist2[j] = every[i+1].hist2[j];
		every[i+1].hist2[j] = h;
	}

	//hist3
	for(j=0;j<20;j++)
	{
		h = every[i].hist3[j];
		every[i].hist3[j] = every[i+1].hist3[j];
		every[i+1].hist3[j] = h;
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���ݾ�������
void sort()
{
	int i,j,t=0;

	for(i=0; i<num-1; i++)
		for(j=0; j<num-i-1; j++)
		{
			if(every[j].dist>every[j+1].dist)
			
				exchange(j);
			
		}
}

int CopyFile(char *SourceFile,char *NewFile)
{
ifstream in;
ofstream out;
in.open(SourceFile,ios::binary);//��Դ�ļ�
if(in.fail())//��Դ�ļ�ʧ��
{
   cout<<"Error 1: Fail to open the source file."<<endl;
   in.close();
   out.close();
   return 0;
}
out.open(NewFile,ios::binary);//����Ŀ���ļ� 
if(out.fail())//�����ļ�ʧ��
{
   cout<<"Error 2: Fail to create the new file."<<endl;
   out.close();
   in.close();
   return 0;
}
else//�����ļ�
{
   out<<in.rdbuf();
   out.close();
   in.close();
   return 1;
}
}
char *w2c(char *pcstr,const wchar_t *pwstr, size_t len)  
{  
    int nlength=wcslen(pwstr);  
    //��ȡת����ĳ���  
    int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL,0,NULL, NULL );   
    if(nbytes>len)   nbytes=len;  
    // ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�  
    WideCharToMultiByte( 0,0, pwstr, nlength,   pcstr, nbytes, NULL,   NULL );  
    return pcstr ;  
}  

int main()
{
	char name[200] = "C:\\��ͬѧ��\\ͼƬ��";
	char namedst[200]="C:\\��ͬѧ��\\ȥ����ͼƬ��";
	char test[200] = "C:\\��ͬѧ��\\ʾ��ͼƬ";
	char testdst[200]="C:\\��ͬѧ��\\ȥ����ʾ��ͼƬ";
	char result[200]="C:\\��ͬѧ��\\���";
	
	char test1[200];
	char test2[200];
	char Path[2000];
	char Path2[2000];

	string examPath=test;
	string examPath2=testdst;

	int i;



	char amount[50];
	wchar_t lastname[50];


	FILE *fp1;
	FILE *fp2;
	
	if((fp1=_wfopen(L"C:\\Users\\RubyLyu\\Documents\\Visual Studio 2010\\ͼƬ����\\ͼƬ����\\1.txt",L"rt+,ccs=UTF-8"))==NULL)
	{
		
		printf("File1 Name Error.\n");
		 system("pause");
		return 0;
	}
	fgetws(lastname,49,fp1);
	fclose(fp1);


	char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(lastname)+1));  
    memset(pcstr , 0 , 2 * wcslen(lastname)+1 );  
    w2c(pcstr,lastname,2 * wcslen(lastname)+1) ;  
     

	if((fp2=fopen("C:\\Users\\RubyLyu\\Documents\\Visual Studio 2010\\ͼƬ����\\ͼƬ����\\2.txt","r"))==NULL)
	{
		printf("File2 Name Error.\n");
		 system("pause");                
		return 0;
	}
	fgets(amount,49,fp2);
	fclose(fp2);

	sprintf(Path, "%s\\%s", examPath.c_str(),pcstr); 
	sprintf(Path2,"%s\\%s",examPath2.c_str(),pcstr);

	
	free(pcstr);

	everyhist(name,namedst);

	exam.filename = Path;

	for(i=0;Path[i]!=10;i++)
	{
		test1[i]=Path[i];
	}
	test1[i]='\0';
	for(i=0;Path2[i]!=10;i++)
	{
		test2[i]=Path2[i];
	}
	test2[i]='\0';

	PreProduce(test1,test2);

	creathist(test2, exam.hist1, exam.hist2,exam.hist3);

	mostvalue(100);

	compare();

	sort();

	int a=0;
	for(int i=0;amount[i]!=10;i++)
		a=a*10+amount[i]-'0';

	for(int i=0;i<a;i++)
	{
		char temp1[200],temp2[200];
		int n=every[i].filename.length();
		strcpy(temp1,every[i].filename.c_str());
		strcpy(temp2,every[i].dst.c_str());
		CopyFile(temp1,temp2);
	}              
	return 0;
}
 