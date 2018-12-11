#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
using namespace std;
extern "C" __declspec(dllexport) const char *get_default_gateway();

vector <string> default_gateway;

const char *get_default_gateway()
{

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	PIP_ADAPTER_INFO info_p;

	unsigned long stSize = sizeof(IP_ADAPTER_INFO);

	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	info_p = pIpAdapterInfo;
	
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;

		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];

		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
		info_p = pIpAdapterInfo;
	}
	if (ERROR_SUCCESS == nRel)
	{

		while (info_p)
		{
			IP_ADDR_STRING *pIpAddrString = &(info_p->IpAddressList);
			do
			{
				string gateway_tmp = info_p->GatewayList.IpAddress.String;
				if (gateway_tmp != "0.0.0.0") {
					default_gateway.push_back(info_p->GatewayList.IpAddress.String);
				}
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			info_p = info_p->Next;
		}

	}

	if (pIpAdapterInfo)
	{
		delete []pIpAdapterInfo;
	}

	const char *gateway = default_gateway.at(0).c_str();
	return gateway;// fix me: little memory leak
}