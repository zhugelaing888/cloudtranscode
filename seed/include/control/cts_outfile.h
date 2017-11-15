#ifndef CTS_OUTFILE_H__
#define CTS_OUTFILE_H__

#include<iostream>
#include<string.h>
 
using namespace std;

typedef enum profile{low,mid,high} Level;

typedef struct Video
{
	char codec[16];       //视频编码
	char aspect[16];       //比例
	int width;         //视频宽度
	int height;        //视频高度
	int bitrate;    //码流，单位 kbps
	Level lev;     //编码档次
}Video;

typedef struct Audio
{
	char codec[16];        //音频编码方式
	int sample_rate;     //音频采样率
	int channels;        //声道数目
	int bitrate;         //码流，单位 kbps
}Audio;

typedef struct Logo
{
	char file[32];        //水印文件，支持 HTTP/FTP 和本地文件
	int width;          //水印宽度，0不变换
	int height;         //水印高度，0不变换
	int x;              //水印横向位置，大于0从左算起，小于0从右算起
	int y;              //水印纵向位置，大于0从顶部算起，小于0从底部算起
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
	char *_name;/*生成文件的名字*/
	char *_format;/* 输出格式 mp4/flv/ts/3gp/mkv/avi/mp3 */
	char *_out_path;/* 文件保存路径，可以实现路径和文件名的自定义；如果节点不存在，文件名则自动生成，并在转码结束后，反馈具体地址 */
	string cachefile;
	int _num;/* 编号 */
	Video _video;/* 视频设定 */
	Audio _audio;/* 音频设定 */
	Logo _logo;/* 台标叠加，如不需要贴加台标的，字段省去 */
};

#endif //__OUTFILE_H__
