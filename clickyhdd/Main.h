#pragma once
#include <Windows.h>
#include <ntddscsi.h>
#include <SetupAPI.h>
#include <iostream>
#include <string>
#include <vector>
#include"DiskInfo.h"
struct ATA_PASS_THROUGH_EX_WITH_BUFFER
{
	ATA_PASS_THROUGH_EX apt;
	BYTE dataBuf[512];
};
int getAPMLevel(BYTE b);
void EnumDevices(std::vector<HANDLE> &v);
ATA_PASS_THROUGH_EX_WITH_BUFFER makeBuffer();
void SendIdentifyCommand(HANDLE disk, ATA_PASS_THROUGH_EX_WITH_BUFFER ab, std::vector<DiskInfo> &d);
void setAPMLevel(HANDLE disk, ATA_PASS_THROUGH_EX_WITH_BUFFER ab);
constexpr UCHAR SET_FEATURE = 0xEF;
constexpr UCHAR APM_FEATURE = 0x05;
constexpr UCHAR APM_MAX_PERF = 0xFE;
constexpr UCHAR IDENTIFY_DEVICE = 0xEC;

