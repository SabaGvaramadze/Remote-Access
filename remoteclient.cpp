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
	
	getaddrinfo("127.0.0.1","8200",&hints,&res);
	
	int server = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

	if(connect(server,res->ai_addr,res->ai_addrlen) == -1){
		cout << "connect failed: " << WSAGetLastError() << endl;
		return -1;
	}
	
	string input;
	char *msg = (char*)malloc(100);
	char *buf = (char*)malloc(1024);
	while(input != "quit"){
		string input;
		cout << ": ";
		getline(cin,input);
		memset(msg,0,100);
		memcpy(msg,input.c_str(),input.length());
		if(send(server,msg,100,0) != 100)cout << "couldn't send all of the data";
		recv(server,buf,1024,0);
		while(*buf == 1){
			cout << (buf+1);
			recv(server,buf,1024,0);
		}
		cout << endl;
	}

	
}
