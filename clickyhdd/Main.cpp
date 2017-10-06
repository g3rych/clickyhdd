#include "Main.h"
#define UNICODE 1
#define _UNICODE 1


int main() {
	std::vector<DiskInfo> ATADisks;
	std::vector<HANDLE> devices;
	EnumDevices(devices);
	for (auto dev : devices) {
		SendIdentifyCommand(dev,makeBuffer(),ATADisks);
	}
	for (auto disk : ATADisks) {
		if (disk.apmSupport && disk.apmEnabled) {
			setAPMLevel(disk.handle, makeBuffer());
		}	
	}
}

void EnumDevices(std::vector<HANDLE> &devices)
{
	HANDLE memory = HeapCreate(NULL, 4096, 0);
	HDEVINFO devinfo = nullptr;
	devinfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_DISK, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	int i = 0;
	SP_DEVICE_INTERFACE_DATA data;
	PSP_DEVICE_INTERFACE_DETAIL_DATA detaildata;
	data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	DWORD requiredSize;
	while (SetupDiEnumDeviceInterfaces(devinfo, NULL, &GUID_DEVINTERFACE_DISK, i, &data)) {
		SetupDiGetDeviceInterfaceDetail(devinfo,&data,NULL,0,&requiredSize,NULL);
		detaildata = (PSP_DEVICE_INTERFACE_DETAIL_DATA)HeapAlloc(memory, HEAP_ZERO_MEMORY, requiredSize);
		detaildata->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail(devinfo, &data, detaildata, requiredSize, &requiredSize, NULL);
		i++;

		HANDLE handle = INVALID_HANDLE_VALUE;
		handle = CreateFile(detaildata->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
		if (handle == INVALID_HANDLE_VALUE) {
			std::wcout << "need admin rights to run";
		}
		else {
			devices.push_back(handle);
		}
	}
}
ATA_PASS_THROUGH_EX_WITH_BUFFER makeBuffer() {
	ATA_PASS_THROUGH_EX_WITH_BUFFER ab; memset(&ab, 0, sizeof(ab));
	ab.apt.Length = sizeof(ATA_PASS_THROUGH_EX);
	ab.apt.TimeOutValue = 10;
	ab.apt.DataBufferOffset = offsetof(ATA_PASS_THROUGH_EX_WITH_BUFFER, dataBuf);
	ab.apt.AtaFlags = ATA_FLAGS_DATA_IN;
	ab.apt.DataTransferLength = 512;
	return ab;
}

void SendIdentifyCommand(HANDLE disk, ATA_PASS_THROUGH_EX_WITH_BUFFER ab, std::vector<DiskInfo>& ATADisks)
{
	ab.apt.CurrentTaskFile[6] = IDENTIFY_DEVICE;
	unsigned size = sizeof(ATA_PASS_THROUGH_EX) + 512;
	DWORD num_out;
	if (DeviceIoControl(disk, IOCTL_ATA_PASS_THROUGH, &ab, size, &ab, size, &num_out, NULL)) {
		ATADisks.push_back(DiskInfo(disk,ab.dataBuf));
	}
}

void setAPMLevel(HANDLE disk,ATA_PASS_THROUGH_EX_WITH_BUFFER ab){
	ab.apt.CurrentTaskFile[6] = SET_FEATURE;
	ab.apt.CurrentTaskFile[0] = APM_FEATURE;
	ab.apt.CurrentTaskFile[1] = APM_MAX_PERF;
	unsigned size = sizeof(ATA_PASS_THROUGH_EX) + 512;
	DWORD num_out;
	DeviceIoControl(disk, IOCTL_ATA_PASS_THROUGH, &ab, size, &ab, size, &num_out, NULL);
}

