#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <sstream>
#include "..\Proccessors\taskproc.h"
#include "..\Util\read.h"
#include "..\Util\CommandSeparator.h"
#include "..\Util\metadataseparator.h"

namespace
{
	int run_CREATE_TASK(SOCKET clisock,char *recvbuf)
	{
		memset(recvbuf,0,255);
		char ok[3] = "OK";
		send(clisock,ok,3,0);
		int iResult = util::leer(clisock,recvbuf);
		if (iResult != 0) return 1;
		std::string comando = recvbuf;
		//Divide in words
		std::string Word1 = util::separatewords(comando,0);
		std::string Word2 = util::separatewords(comando,1);
		std::string Word3 = util::separatewords(comando,2);
		std::string Word4 = util::separatewords(comando,3);
		std::string Word5 = util::separatewords(comando,4);
		if (Word2 == "BUILD") //A build task. Yay!
		{
			char *metadatos = new char[255];
			memcpy(metadatos,Word3.c_str(),Word3.size());
			Tag building = util::SeparateTags(metadatos,0);
			std::stringstream var1;
			var1 << "Type=" << building.TagValue;
			Task newtask = Task(var1.str());
			newtask.push();
			delete metadatos;
		}
		/*
		TODO : More tasks
		*/
		send(clisock,ok,3,0);
		return 0;
	}
}