#ifndef CTS_OUTFILE_H__
#define CTS_OUTFILE_H__

#include<iostream>
#include<string.h>
 
using namespace std;

typedef enum profile{low,mid,high} Level;

typedef struct Video
{
	char codec[16];       //��Ƶ����
	char aspect[16];       //����
	int width;         //��Ƶ���
	int height;        //��Ƶ�߶�
	int bitrate;    //��������λ kbps
	Level lev;     //���뵵��
}Video;

typedef struct Audio
{
	char codec[16];        //��Ƶ���뷽ʽ
	int sample_rate;     //��Ƶ������
	int channels;        //������Ŀ
	int bitrate;         //��������λ kbps
}Audio;

typedef struct Logo
{
	char file[32];        //ˮӡ�ļ���֧�� HTTP/FTP �ͱ����ļ�
	int width;          //ˮӡ��ȣ�0���任
	int height;         //ˮӡ�߶ȣ�0���任
	int x;              //ˮӡ����λ�ã�����0��������С��0��������
	int y;              //ˮӡ����λ�ã�����0�Ӷ�������С��0�ӵײ�����
}Logo;

class CTS_OutFile
{
public:
	CTS_OutFile();
	~CTS_OutFile();
	
	char * getName();
	void setName(char *name);
		
	char * getFormat();
	void setFormat(char *format);
	
	char *getOutPath();
	void setOutPath(char* out_path);

	void setCacheFile(std::string cfile);
	const char *  getCacheFile();
	
	int getNum();
	void setNum(int num);
		
	Video getVideo();
	void setVideo(Video video);
	
	Audio getAudio();
	void setAudio(Audio audio);
	
	Logo getLogo();
	void setLogo(Logo logo);
private:
	char *_name;/*�����ļ�������*/
	char *_format;/* �����ʽ mp4/flv/ts/3gp/mkv/avi/mp3 */
	char *_out_path;/* �ļ�����·��������ʵ��·�����ļ������Զ��壻����ڵ㲻���ڣ��ļ������Զ����ɣ�����ת������󣬷��������ַ */
	string cachefile;
	int _num;/* ��� */
	Video _video;/* ��Ƶ�趨 */
	Audio _audio;/* ��Ƶ�趨 */
	Logo _logo;/* ̨����ӣ��粻��Ҫ����̨��ģ��ֶ�ʡȥ */
};

#endif //__OUTFILE_H__
