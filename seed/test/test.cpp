#include <iostream>
#include "rapidxml/rapidxml.hpp"  
#include "rapidxml/rapidxml_utils.hpp"  
#include "rapidxml/rapidxml_print.hpp"
#include <time.h> 
#include <stdio.h>
using namespace rapidxml;
using namespace std;

#define FILENUM 500

typedef struct
{
	char id[128];
	char inpath[128];
	char outpath[128];
	char filename[128];
}FileXml;



void rapidxml::parse_error_handler(const char *what, void *where)
{
	return ;
}

int main()
{
	FileXml xml;
	for(int i=0;i<FILENUM;i++)
	{
		sprintf(xml.id,"72878D77-801C-4D04-BF3F-12F68B728%04d",i);
		sprintf(xml.inpath,"http://192.168.3.145:8080/src/jinglun/%d.mp4",rand()%22);
		sprintf(xml.outpath,"/opt/succ/2012/05/01/jinglun/400/%03d.mp4",i);
		sprintf(xml.filename,"/opt/transcodec/src/xml/%d.xml",i);
		xml_document<> doc;
		xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		doc.append_node(rot);
		
		xml_node<>* node =   doc.allocate_node(node_element,"task_info","information");    
		doc.append_node(node);
		
		xml_node<>* id = doc.allocate_node(node_element,"id",xml.id);
		node->append_node(id);

		xml_node<>* progress = doc.allocate_node(node_element,"progress","http://192.168.3.145:3456/progress");
		node->append_node(progress);
		xml_node<>* result = doc.allocate_node(node_element,"result","http://192.168.3.145:3456/result");
		node->append_node(result);
		xml_node<>* priority = doc.allocate_node(node_element,"priority","1");
		node->append_node(priority);

		xml_node<>* input = doc.allocate_node(node_element,"input",NULL);
		node->append_node(input);
		{
			xml_node<>* infile = doc.allocate_node(node_element,"file",NULL);
			input->append_node(infile);
			{
				xml_node<>* no = doc.allocate_node(node_element,"no","1");
				infile->append_node(no);
				xml_node<>* path = doc.allocate_node(node_element,"path",xml.inpath);
				infile->append_node(path);
			}
		}

		xml_node<>* output = doc.allocate_node(node_element,"output",NULL);
		node->append_node(output);
		{

			xml_node<>* outfile = doc.allocate_node(node_element,"file",NULL);
			output->append_node(outfile);
			{
				outfile->append_node(doc.allocate_node(node_element,"no","1"));
				outfile->append_node(doc.allocate_node(node_element,"format","mp4"));
				outfile->append_node(doc.allocate_node(node_element,"out_path",xml.outpath));
				
				xml_node<>* video = doc.allocate_node(node_element,"video",NULL);
				outfile->append_node(video);
				{
					video->append_node(doc.allocate_node(node_element,"codec","h264"));
					video->append_node(doc.allocate_node(node_element,"width","640"));
					video->append_node(doc.allocate_node(node_element,"height","480"));
					video->append_node(doc.allocate_node(node_element,"profile","mid"));
					video->append_node(doc.allocate_node(node_element,"bitrate","400"));
					video->append_node(doc.allocate_node(node_element,"aspect","4:3"));
				}

				xml_node<>* audio = doc.allocate_node(node_element,"audio",NULL);
				outfile->append_node(audio);
				{
					audio->append_node(doc.allocate_node(node_element,"codec","aac+"));
					audio->append_node(doc.allocate_node(node_element,"sample_rate","48000"));
					audio->append_node(doc.allocate_node(node_element,"channels","2"));
					audio->append_node(doc.allocate_node(node_element,"bitrate","32"));
				}
			}
		}
		std::ofstream out(xml.filename);
		out << doc;
	}
	for(int i=FILENUM;i<FILENUM+FILENUM;i++)
	{
		sprintf(xml.id,"72878D77-801C-4D04-BF3F-12F68B728%04d",i);
		sprintf(xml.inpath,"http://192.168.3.145:8080/src/jinglun/%d.mp4",rand()%22);
		sprintf(xml.outpath,"/opt/succ/2012/05/01/jinglun/500/%03d.mp4",i);
		sprintf(xml.filename,"/opt/transcodec/src/xml/%d.xml",i);
		xml_document<> doc;
		xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		doc.append_node(rot);
		
		xml_node<>* node =   doc.allocate_node(node_element,"task_info","information");    
		doc.append_node(node);
		
		xml_node<>* id = doc.allocate_node(node_element,"id",xml.id);
		node->append_node(id);

		xml_node<>* progress = doc.allocate_node(node_element,"progress","http://192.168.3.145:3456/progress");
		node->append_node(progress);
		xml_node<>* result = doc.allocate_node(node_element,"result","http://192.168.3.145:3456/result");
		node->append_node(result);
		xml_node<>* priority = doc.allocate_node(node_element,"priority","1");
		node->append_node(priority);

		xml_node<>* input = doc.allocate_node(node_element,"input",NULL);
		node->append_node(input);
		{
			xml_node<>* infile = doc.allocate_node(node_element,"file",NULL);
			input->append_node(infile);
			{
				xml_node<>* no = doc.allocate_node(node_element,"no","1");
				infile->append_node(no);
				xml_node<>* path = doc.allocate_node(node_element,"path",xml.inpath);
				infile->append_node(path);
			}
		}

		xml_node<>* output = doc.allocate_node(node_element,"output",NULL);
		node->append_node(output);
		{

			xml_node<>* outfile = doc.allocate_node(node_element,"file",NULL);
			output->append_node(outfile);
			{
				outfile->append_node(doc.allocate_node(node_element,"no","1"));
				outfile->append_node(doc.allocate_node(node_element,"format","mp4"));
				outfile->append_node(doc.allocate_node(node_element,"out_path",xml.outpath));
				
				xml_node<>* video = doc.allocate_node(node_element,"video",NULL);
				outfile->append_node(video);
				{
					video->append_node(doc.allocate_node(node_element,"codec","h264"));
					video->append_node(doc.allocate_node(node_element,"width","640"));
					video->append_node(doc.allocate_node(node_element,"height","480"));
					video->append_node(doc.allocate_node(node_element,"profile","mid"));
					video->append_node(doc.allocate_node(node_element,"bitrate","500"));
					video->append_node(doc.allocate_node(node_element,"aspect","16:9"));
				}

				xml_node<>* audio = doc.allocate_node(node_element,"audio",NULL);
				outfile->append_node(audio);
				{
					audio->append_node(doc.allocate_node(node_element,"codec","aac+"));
					audio->append_node(doc.allocate_node(node_element,"sample_rate","48000"));
					audio->append_node(doc.allocate_node(node_element,"channels","2"));
					audio->append_node(doc.allocate_node(node_element,"bitrate","32"));
				}
			}
		}
		std::ofstream out(xml.filename);
		out << doc;
	}
	for(int i=FILENUM+FILENUM;i<FILENUM+FILENUM+FILENUM;i++)
	{
		sprintf(xml.id,"72878D77-801C-4D04-BF3F-12F68B728%04d",i);
		sprintf(xml.inpath,"http://192.168.3.145:8080/src/chuzhong/%d.mpg",rand()%24);
		sprintf(xml.outpath,"/opt/succ/2012/05/01/chuzhong/400/%03d.mp4",i);
		sprintf(xml.filename,"/opt/transcodec/src/xml/%d.xml",i);
		xml_document<> doc;
		xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		doc.append_node(rot);
		
		xml_node<>* node =   doc.allocate_node(node_element,"task_info","information");    
		doc.append_node(node);
		
		xml_node<>* id = doc.allocate_node(node_element,"id",xml.id);
		node->append_node(id);

		xml_node<>* progress = doc.allocate_node(node_element,"progress","http://192.168.3.145:3456/progress");
		node->append_node(progress);
		xml_node<>* result = doc.allocate_node(node_element,"result","http://192.168.3.145:3456/result");
		node->append_node(result);
		xml_node<>* priority = doc.allocate_node(node_element,"priority","2");
		node->append_node(priority);

		xml_node<>* input = doc.allocate_node(node_element,"input",NULL);
		node->append_node(input);
		{
			xml_node<>* infile = doc.allocate_node(node_element,"file",NULL);
			input->append_node(infile);
			{
				xml_node<>* no = doc.allocate_node(node_element,"no","1");
				infile->append_node(no);
				xml_node<>* path = doc.allocate_node(node_element,"path",xml.inpath);
				infile->append_node(path);
			}
		}

		xml_node<>* output = doc.allocate_node(node_element,"output",NULL);
		node->append_node(output);
		{

			xml_node<>* outfile = doc.allocate_node(node_element,"file",NULL);
			output->append_node(outfile);
			{
				outfile->append_node(doc.allocate_node(node_element,"no","1"));
				outfile->append_node(doc.allocate_node(node_element,"format","mp4"));
				outfile->append_node(doc.allocate_node(node_element,"out_path",xml.outpath));
				
				xml_node<>* video = doc.allocate_node(node_element,"video",NULL);
				outfile->append_node(video);
				{
					video->append_node(doc.allocate_node(node_element,"codec","h264"));
					video->append_node(doc.allocate_node(node_element,"width","640"));
					video->append_node(doc.allocate_node(node_element,"height","480"));
					video->append_node(doc.allocate_node(node_element,"profile","mid"));
					video->append_node(doc.allocate_node(node_element,"bitrate","400"));
					video->append_node(doc.allocate_node(node_element,"aspect","16:9"));
				}

				xml_node<>* audio = doc.allocate_node(node_element,"audio",NULL);
				outfile->append_node(audio);
				{
					audio->append_node(doc.allocate_node(node_element,"codec","aac+"));
					audio->append_node(doc.allocate_node(node_element,"sample_rate","48000"));
					audio->append_node(doc.allocate_node(node_element,"channels","2"));
					audio->append_node(doc.allocate_node(node_element,"bitrate","32"));
				}
			}
		}
		std::ofstream out(xml.filename);
		out << doc;
	}
	for(int i=FILENUM+FILENUM+FILENUM;i<FILENUM+FILENUM+FILENUM+FILENUM;i++)
	{
		sprintf(xml.id,"72878D77-801C-4D04-BF3F-12F68B728%04d",i);
		sprintf(xml.inpath,"http://192.168.3.145:8080/src/chuzhong/%d.mpg",rand()%24);
		sprintf(xml.outpath,"/opt/succ/2012/05/01/chuzhong/500/%03d.mp4",i);
		sprintf(xml.filename,"/opt/transcodec/src/xml/%d.xml",i);
		xml_document<> doc;
		xml_node<>* rot = doc.allocate_node(rapidxml::node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		doc.append_node(rot);
		
		xml_node<>* node =   doc.allocate_node(node_element,"task_info","information");    
		doc.append_node(node);
		
		xml_node<>* id = doc.allocate_node(node_element,"id",xml.id);
		node->append_node(id);

		xml_node<>* progress = doc.allocate_node(node_element,"progress","http://192.168.3.145:3456/progress");
		node->append_node(progress);
		xml_node<>* result = doc.allocate_node(node_element,"result","http://192.168.3.145:3456/result");
		node->append_node(result);
		xml_node<>* priority = doc.allocate_node(node_element,"priority","3");
		node->append_node(priority);

		xml_node<>* input = doc.allocate_node(node_element,"input",NULL);
		node->append_node(input);
		{
			xml_node<>* infile = doc.allocate_node(node_element,"file",NULL);
			input->append_node(infile);
			{
				xml_node<>* no = doc.allocate_node(node_element,"no","1");
				infile->append_node(no);
				xml_node<>* path = doc.allocate_node(node_element,"path",xml.inpath);
				infile->append_node(path);
			}
		}

		xml_node<>* output = doc.allocate_node(node_element,"output",NULL);
		node->append_node(output);
		{

			xml_node<>* outfile = doc.allocate_node(node_element,"file",NULL);
			output->append_node(outfile);
			{
				outfile->append_node(doc.allocate_node(node_element,"no","1"));
				outfile->append_node(doc.allocate_node(node_element,"format","mp4"));
				outfile->append_node(doc.allocate_node(node_element,"out_path",xml.outpath));
				
				xml_node<>* video = doc.allocate_node(node_element,"video",NULL);
				outfile->append_node(video);
				{
					video->append_node(doc.allocate_node(node_element,"codec","h264"));
					video->append_node(doc.allocate_node(node_element,"width","640"));
					video->append_node(doc.allocate_node(node_element,"height","480"));
					video->append_node(doc.allocate_node(node_element,"profile","mid"));
					video->append_node(doc.allocate_node(node_element,"bitrate","500"));
					video->append_node(doc.allocate_node(node_element,"aspect","4:3"));
				}

				xml_node<>* audio = doc.allocate_node(node_element,"audio",NULL);
				outfile->append_node(audio);
				{
					audio->append_node(doc.allocate_node(node_element,"codec","aac+"));
					audio->append_node(doc.allocate_node(node_element,"sample_rate","48000"));
					audio->append_node(doc.allocate_node(node_element,"channels","2"));
					audio->append_node(doc.allocate_node(node_element,"bitrate","32"));
				}
			}
		}
		std::ofstream out(xml.filename);
		out << doc;
	}
	return 0;
}

