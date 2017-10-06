#pragma once
#include <Windows.h>
#include <ntddscsi.h>
#include <bitset>
#include <string>
#include <algorithm>
class DiskInfo
{
public:
	DiskInfo(HANDLE handle,BYTE response[]);
	~DiskInfo();
	const int APM_ENABLED_BYTE_NUMBER = 172;
	const int APM_SUPPORTED_BYTE_NUMBER = 166;
	const int MODEL_NAME_START_INDEX = 54;
	const int MODEL_NAME_END_INDEX = 93;
	HANDLE handle;
	bool apmSupport;
	bool apmEnabled;

private:
	std::string deviceName;
	int apmLevel;
	void convertATAString(BYTE deviceData[]);
	bool isAPMFeatureSetSupported(BYTE b);
	bool isAPMFeatureEnabled(BYTE b);
};