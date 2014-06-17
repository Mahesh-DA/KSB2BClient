// Client.cpp : Defines the entry point for the console application.
// 2006.11.12 by wangliang
//

#include <stdio.h>
#include <winsock2.h>

#define MSG_MAXLEN 600

void handle_message(SOCKET hSockContral, char * szClientMsg, int nLen)
{
	int nReturn;
	char szServerMsg[MSG_MAXLEN];
	memset(szServerMsg, 0, sizeof(szServerMsg));

	// send message to server
	nReturn = send(hSockContral, szClientMsg, nLen, 0);
	if (nReturn == -1)
	{
		perror("send");
		exit(1);
	}
	printf("%s\n", szClientMsg);


	// Receive from server
	nReturn = recv(hSockContral, szServerMsg, sizeof(szServerMsg), 0);
	if (nReturn == -1)
	{
		perror("recv");
		exit(1);
	}
	printf("%s\n", szServerMsg);
	strcpy(szClientMsg, szServerMsg);
}


int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <IP Addresss> <PORT>\n", argv[0]);
		exit(1);
	}

	// initialize
	SOCKET hSockContral;
	sockaddr_in tagSockAddrServer;
	int nReturn;

	// WSAStartup	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup() failed, Error Number: %d", GetLastError());
		exit(1);
	}

	// Create the socket
	hSockContral = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSockContral == INVALID_SOCKET)
	{
		perror("socket");
		exit(1);
	}

	// Set values to server sockaddr
	memset(&tagSockAddrServer, 0, sizeof(tagSockAddrServer));
	tagSockAddrServer.sin_family = AF_INET;
	tagSockAddrServer.sin_port = htons(atoi(argv[2]));
	tagSockAddrServer.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	//tagSockAddrServer.sin_port = htons(17990);
	//tagSockAddrServer.sin_addr.S_un.S_addr = inet_addr ("10.253.44.94");

	// connect to server
	nReturn = connect(hSockContral, (sockaddr*)&tagSockAddrServer, sizeof(tagSockAddrServer));
	if (nReturn == -1)
	{
		perror("connect failed, sys error message:");
		exit(1);
	}

	char szClientMsg[MSG_MAXLEN];
	memset(szClientMsg, 0, sizeof(szClientMsg));

	/******************************************************
	*	request-response examples
	*******************************************************/

	// login
	sprintf(szClientMsg, "%s", "R|||6011|||200889|1|10.253.44.94|1|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//query fund status
	sprintf(szClientMsg, "%s", "R|||6012|||200889|1|20081010|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//历史状态资金查询
	_snprintf(szClientMsg, sizeof(szClientMsg) - 1, "%s", "R|||6032|||200889|1|20081010|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//成交明细查询
	_snprintf(szClientMsg, sizeof(szClientMsg) - 1, "%s", "R|||6053|||200889|1|20090202|20090209|||1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// 下单
	sprintf(szClientMsg, "%s", "R|||6021|||40000003|1|B|a0907|0|0|0|10|1500|00000004|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// 查询委托状态
	sprintf(szClientMsg, "%s", "R|||6020|||200889|1|60||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// 撤单
	sprintf(szClientMsg, "%s", "R|||6022|||200889|1||||||||60|10||sfe|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// 查资金
	sprintf(szClientMsg, "%s", "R|||6012|||200889|1||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey mapno (multi-records)
	sprintf(szClientMsg, "%s", "R|||6040|||200889|1|||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey quotation
	sprintf(szClientMsg, "%s", "R|||6017|||200889|1|A|al0906|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey position(multi-records)
	sprintf(szClientMsg, "%s", "R|||6014|||200889|1|20081028|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6015查交易统计 查询出错
	sprintf(szClientMsg, "%s", "R|||6015|||200889|1|20081025|20081109|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6016出入金查询
	sprintf(szClientMsg, "%s", "R|||6016|||200889|1|20081028|20081029|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey order(multi-records)
	printf("\n委托单查询\n");
	//sprintf(szClientMsg, "%s", "R|||6019|||7963229|63229||||||");
	sprintf(szClientMsg, "%s", "R|||6019|||200889|1||||||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey deal(multi-records)
	sprintf(szClientMsg, "%s", "R|||6053|||200889|1|20090105|20090105|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey contract(multi-records)
	sprintf(szClientMsg, "%s", "R|||6018|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey cust info(multi-records)
	sprintf(szClientMsg, "%s", "R|||6045|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey futures-bank account (multi-records)
	sprintf(szClientMsg, "%s", "R|||6042|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// futures-bank trans
	sprintf(szClientMsg, "%s", "R|||6041|||200889|1|1|18800|01|000001|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// qurey futures-bank trans(multi-records)
	sprintf(szClientMsg, "%s", "R|||6043|||200889|1||||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// modify trade password
	sprintf(szClientMsg, "%s", "R|||6023|||200889|2|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// modify fund password
	sprintf(szClientMsg, "%s", "R|||6024|||200889|1|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6030 读系统日期
	sprintf(szClientMsg, "%s", "R|||6030|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6031 读营业部名称
	sprintf(szClientMsg, "%s", "R|||6031|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6040 查询客户交易编码
	sprintf(szClientMsg, "%s", "R|||6040|||200889|1|A||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6041 银期转账命令
	sprintf(szClientMsg, "%s", "R|||6041|||200889|1|123456|10000|01|01|11111|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6044 查询最大可开仓手数
	sprintf(szClientMsg, "%s", "R|||6044|||200889|1|al0811|A|120|0|0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6068 查询最大可平仓手数
	sprintf(szClientMsg, "%s", "R|||6068|||200889|1|al0901|A|0|0|sfe|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6045 查询客户信息
	sprintf(szClientMsg, "%s", "R|||6045|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6092 交割单信息查询
	sprintf(szClientMsg, "%s", "R|||6092|||200889|1|20081027|20081029|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6025 查询追保强平结束语句 没有实现的功能
	sprintf(szClientMsg, "%s", "R|||6025|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6071 确认结帐单  drtp 没开
	sprintf(szClientMsg, "%s", "R|||6071|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//6072 下载结帐单  drtp 没开
	sprintf(szClientMsg, "%s", "R|||6072|||200889|1|1||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	/********************************组合委托部分*****************************/
	//下组合单
	sprintf(szClientMsg, "%s", "R|||6101|||40000003|1|00000004|B|dce|0|2|1|1|349|SP|0|0|0|0|0|0|a0905|a0909|||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//组合单撤单
	sprintf(szClientMsg, "%s", "R|||6102|||40000003|1|dce|00000004|1000036|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//查询订单类型
	sprintf(szClientMsg, "%s", "R|||6103|||200889|1||1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//查询组合策略
	sprintf(szClientMsg, "%s", "R|||6104|||200889||||||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//查询组合合约
	sprintf(szClientMsg, "%s", "R|||6105|||200889|1||SP|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//查询组合持仓
	sprintf(szClientMsg, "%s", "R|||6106|||200889|1|200200889|||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//组合委托查询
	sprintf(szClientMsg, "%s", "R|||6107|||200889|1||||||");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));
	sprintf(szClientMsg, "%s", "R|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	//查询组合可开可平手数
	sprintf(szClientMsg, "%s", "R|||6108|||200889|1|200200889|B|dce|SPC|1|0|a0907|a0909|||0|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	/*********************************************************************/

	// logout
	sprintf(szClientMsg, "%s", "R|||6061|||200889|1|");
	handle_message(hSockContral, szClientMsg, strlen(szClientMsg));

	// close socket
	nReturn = closesocket(hSockContral);

	// WSACleanup
	if (WSACleanup())
	{
		perror("WSACleanup");
		exit(1);
	}

	getchar();

	return 0;
}