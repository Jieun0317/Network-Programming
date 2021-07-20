#pragma comment(lib,"wsock32.lib")

#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <time.h>

#define PORT 5500

unsigned __stdcall  LottoGame(void* arg);

int Lottonumber[6];//난수 생성 저장 변수
unsigned threadID;//쓰레드 id 저장 변수

int main() {

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET s_Socket = socket(AF_INET, SOCK_STREAM, 0);//서버 소켓 생성
	printf("서버 소켓을 생성했습니다.\n");

	SOCKADDR_IN serverAddress;//서버 주소 저장하는 구조체 생성
	ZeroMemory(&serverAddress, 0, sizeof(serverAddress));//구조체 초기화
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	bind(s_Socket, (SOCKADDR*)& serverAddress, sizeof(serverAddress));
	listen(s_Socket, 5);
	printf("클라이언트 접속 대기 상태입니다.\n\n");
	
	//난수 생성
	srand((unsigned)time(NULL));	
	for (int i = 0; i < 6; i++)
	{
		Lottonumber[i] = ((rand() % 45) + 1);
		for (int j=i-1; j>=0; j--)
		{
			if (Lottonumber[i] == Lottonumber[j])
			{
				i--;
				break;
			}
		}
	}
	printf("생성된 로또 번호 : %d %d %d %d %d %d\n\n", Lottonumber[0], Lottonumber[1], Lottonumber[2], Lottonumber[3], Lottonumber[4], Lottonumber[5]);

	while (1) {
		int c_Socket = accept(s_Socket, NULL, NULL);
		printf("클라이언트가 접속했습니다.\n");
		printf("로또 번호 맞추기 게임을 시작합니다.\n\n");

		unsigned long hThread = _beginthreadex(NULL, 0, LottoGame, (void*)c_Socket, 0, &threadID);
	}
	return 0;
}

unsigned __stdcall  LottoGame(void* arg) {

	int c_Socket = (int)arg;
	while (1) {
		int clnt_num[6];//입력받은 6개의 숫자를 저장할 배열
		int clntcorr[1];//숫자의 일치 갯수 저장할 배열(클라이언트 전송 위함)
	 
		recv(c_Socket, (char*)clnt_num, sizeof(clnt_num), 0);//클라이언트로부터 데이터 받음
		
		for (int num = 0; num < 6; num++)
			clnt_num[num] = ntohl(clnt_num[num]);
	
		printf("클라이언트의 로또 번호 : %d %d %d %d %d %d\n", clnt_num[0], clnt_num[1], clnt_num[2], clnt_num[3], clnt_num[4], clnt_num[5]);

		int count = 0;//로또 번호 일치하는 갯수 저장 변수

		for (int i = 0; i < 6; i++) {
			int serv_num = Lottonumber[i];

			for (int j = 0; j < 6; j++) {
				int clnt_num_new = clnt_num[j];

				if (serv_num == clnt_num_new) {
					count++;
				}
			}
		}
		printf("%d개가 일치합니다.\n\n", count);
	
		clntcorr[0] = htonl(count);
	
		send(c_Socket, (char*)clntcorr, sizeof(clntcorr), 0);//클라이언트로 데이터 전송

		if (count==6) {//6개의 번호가 일치할 때 종료
			printf("현재 클라이언트와 접속을 종료합니다.\n");
			break;
		}
	}
	closesocket(c_Socket);//클라이언트 소켓 종료
	return 0;
}