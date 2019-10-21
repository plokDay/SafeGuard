#include "stdafx.h"
#include "CClientSocket.h"
#include <Ws2tcpip.h>
#include <string.h>
 
 CStringA CClientSocket::CStrW2CStrA(const CStringW &cstrSrcW)
 {
 	int len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, NULL, 0, NULL, NULL);
 	char *str = new char[len];
 	memset(str, 0, len);
 	WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, str, len, NULL, NULL);
 	CStringA cstrDestA = str;
 	delete[] str;
 
 	return cstrDestA;
 }

void CClientSocket::starConnect()
{
	// 1. ��ʼ��
	initSocket();
	// 2. �����׽���
	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 3. ���÷���˵ĵ�ַ�Ͷ˿�
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10086);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	// 4. ���ӵ������
	if (SOCKET_ERROR == connect(m_hSocket, (sockaddr*)&addr, sizeof(addr)))
	{
		m_responce = "����������ʧ��";
	}
	else m_responce = "���������ӳɹ�";
}

CClientSocket::CClientSocket()
{
	
}




CClientSocket::~CClientSocket()
{
}
//  ��ʼ�������׽���ģ��
void CClientSocket::initSocket()
{
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wd);
}

void CClientSocket::send(DataPackType type, const char* data, int size /*= -1*/)
{
 	if (size == -1) {
 		size = strlen(data);
 	}
 	DataPack dp = { type,size };
 	::send(m_hSocket, (char*)&dp, sizeof(dp), 0);
	//����
	int nSized = strlen(data);
	char* sendBufd = (char*)calloc(1,nSized);
	strcpy_s(sendBufd, nSized+1,data);
	for (int i = 0; i < nSized; ++i)
	{
		sendBufd[i] ^= 5;
	}
	::send(m_hSocket, sendBufd, size,0);

}

void CClientSocket::recv()
 {
 	 
 	DataPackResult head = { 0 };
 	if (::recv(m_hSocket, (char*)&head, sizeof(head) - 1, 0) != sizeof(head) - 1) {
 		return;
 	}
 	DataPackResult* pBuff = (DataPackResult*)malloc(sizeof(DataPackResult) + head.size);
 	memset(pBuff, 0, head.size + sizeof(DataPackResult));
 	memcpy(pBuff, &head, sizeof(head));
 	if (::recv(m_hSocket, pBuff->data, pBuff->size, 0) != pBuff->size) {
 		free(pBuff);
 		return;
 	}
 	
 	if (pBuff==NULL)
 	{
 		m_responce = "����ʧ��";
 	}
	//����pBuff->data
	int nSized = strlen(pBuff->data);
	char* recBufd = (char*)calloc(1, nSized);
	strcpy_s(recBufd, nSized + 1, pBuff->data);
	for (int i = 0; i < nSized; ++i)
	{
		pBuff->data[i] ^= 5;
	}
 	m_responce = pBuff->data;
 	
	m_respPack = (DataPackResult*)pBuff;
 
 }

void CClientSocket::freeResult(DataPackResult* p)
{
	free(p);
}

