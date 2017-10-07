#include "DiskInfo.h"
DiskInfo::DiskInfo(HANDLE handle,BYTE deviceData[]) 
{
	this->handle = handle;
	convertATAString(deviceData);
	apmSupport = isAPMFeatureSetSupported(deviceData[APM_SUPPORTED_BYTE_NUMBER]);
	apmEnabled = isAPMFeatureEnabled(deviceData[APM_ENABLED_BYTE_NUMBER]);
}

 DiskInfo::~ DiskInfo()
{
}
 void DiskInfo::convertATAString(BYTE deviceData[]) {
	 BYTE rawDeviceName[41];
	 std::copy(deviceData + MODEL_NAME_START_INDEX, deviceData + MODEL_NAME_END_INDEX+1, rawDeviceName);
	 for (int i = 0; i < 40; i += 2) {
		 std::swap(rawDeviceName[i], rawDeviceName[i + 1]);
	 }
	 rawDeviceName[40]='\0';
	 deviceName = reinterpret_cast<char *>(rawDeviceName);
}
 bool DiskInfo::isAPMFeatureSetSupported(BYTE b) {
	 std::bitset<8> device(b);
	 std::bitset<8> mask(0b0001000);
	 device = device.operator&=(mask);
	 return (device.to_ulong() != 0) ? true : false;
 }
 bool DiskInfo::isAPMFeatureEnabled(BYTE b) {
	 std::bitset<8> device(b);
	 std::bitset<8> mask(0b0001000);
	 device = device.operator&=(mask);
	 return (device.to_ulong() != 0) ? true : false;
 }
 int getAPMLevel(BYTE b) {
	 return b;
 }