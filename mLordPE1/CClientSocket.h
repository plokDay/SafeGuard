#pragma once
#include <iostream>
enum DataPackType {
	scanVirus = 1,//云查杀
	pushVirus = 2,
	done = 3,
	
};

#pragma pack(push,1)
struct DataPack
{
	DataPackType type;
	uint32_t	 size;
};

struct DataPackResult {
	uint32_t	type;
	int32_t		status;
	uint32_t	size;
	char		data[1];
};
#pragma pack(pop)
class CClientSocket
{
public:
	SOCKET m_hSocket;
	CString m_responce;
	DataPackResult* m_respPack;
public:
	CClientSocket();
	
	~CClientSocket();
	static void initSocket();//初始化socket

	void send(DataPackType type, const char* data, int size = -1);//发送消息
	void recv();//接收消息
	void freeResult(DataPackResult* p);//释放
	static CStringA CStrW2CStrA(const CStringW &cstrSrcW);
	void starConnect();
};

