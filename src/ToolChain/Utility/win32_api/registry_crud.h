#include "../pch.h"
#include <tchar.h>

//COM
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// defines GUID
#include <initguid.h>

// the SetupDiXXX api (from the DDK)
#include <setupapi.h>
#include <cfgmgr32.h>
#include <devpkey.h>

#pragma comment(lib, "Setupapi.lib")

namespace my_util
{

class SampleOfRegistry
{

public:
    //@param classGuid -> {4d36e96f-e325-11ce-bfc1-08002be10318} <- Mouse
    //https://docs.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors
    std::vector<wstring> EnumHardwareId(BSTR classGuid)
    {
        std::vector<wstring> ret;
        GUID guid;
        CLSIDFromString(classGuid, &guid);
        HDEVINFO devInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT);
        if (devInfo == INVALID_HANDLE_VALUE)
        {
            return ret;
        }
        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        for (int deviceIndex = 0;
             SetupDiEnumDeviceInfo(devInfo, deviceIndex, &devInfoData);
             deviceIndex++)
        {

            wchar_t devID[MAX_PATH];
            SP_DEVINFO_LIST_DETAIL_DATA devInfoListDetail;

            devInfoListDetail.cbSize = sizeof(devInfoListDetail);
            if (!SetupDiGetDeviceInfoListDetail(devInfo, &devInfoListDetail))
                break;

            if (CM_Get_Device_ID_ExW(devInfoData.DevInst, (PWSTR)devID, MAX_DEVICE_ID_LEN, 0, devInfoListDetail.RemoteMachineHandle) != CR_SUCCESS)
                break;

            wstring currentDevicePath = devID;
            ret.push_back(currentDevicePath);
        }
        return ret;
    }
};
} // namespace my_util