#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;

int main(int argc,char **argv){
	WSAData wsadata;
	if(WSAStartup(MAKEWORD(2,2), &wsadata) !=0){
		cout << "wsa startup failed" << endl;
		return -1;
	}
	if(LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2){
		cout << "winsock 2.2 not available" << endl;
		WSACleanup();
		return -1;
	}


	struct addrinfo hints;
	struct addrinfo *res;
	
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo("127.0.0.1","8200",&hints,&res);
	
	int serversock;

	
	serversock = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	
	char yes='1';
	
	if(setsockopt(serversock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1){
		//perror("setsockopt");
		return -1;
	}

	if(bind(serversock,res->ai_addr, res->ai_addrlen)){
		//cout << "could not bind: "  <<  WSAGetLastError()<< endl;
		return -1;
	}
	
	if(listen(serversock,1) == -1){
		//cout << "listen failed: " << WSAGetLastError() << endl;
		return -1;
	}
	
	struct sockaddr_storage clientaddr;
	int clientsock;
	socklen_t clientaddr_len = sizeof clientaddr;
	//cout << "waiting for client" << endl;
	clientsock = accept(serversock,(struct sockaddr *)&clientaddr,&clientaddr_len);
	//cout << "client accepted" << endl;
	char *msg = (char*)malloc(100);
	char *buf = (char*)malloc(1024);
	while((string)(msg +1)!= "quit"){
		memset(buf,0,1024);
		recv(clientsock,msg,100,0);
		FILE *cmd = popen(msg,"r");
		if(cmd == NULL){
			//cout << "popen error" << endl;
			return -1;
		}

		*buf = 1;
		bool quit=false;
		while(!quit){
			memset(buf+1,0,1023);
			for(int i=1;i<=1023;i++){
				buf[i]=fgetc(cmd);
				if(buf[i]==EOF){
					buf[i]=0;
					quit=true;
					break;
				}
			}

			send(clientsock,buf,1024,0);
		}
		*buf = 0;
		while(send(clientsock,buf,1024,0) <1);
		
		pclose(cmd);
	}
	


	

}
