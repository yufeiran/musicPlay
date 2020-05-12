
#include<Windows.h>
#include<digitalv.h>
#include<mmsystem.h>
#include<Mmsystem.h>
#include<string>
#include<iostream>
#include<atlstr.h>
#pragma comment(lib,"WINMM.LIB")

using namespace std;
MCIDEVICEID wDevioceId;


DWORD MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
	DWORD opReturn;
	opReturn = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)&mciPP);
	return opReturn;

}

DWORD MCIStatus_music_length()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_LENGTH;
	DWORD staReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_alltime = mciStatus.dwReturn / 1000; //音乐总时间
	return music_alltime;
}

DWORD MCIStatus_music_pos()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	DWORD stalReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_timeing = mciStatus.dwReturn / 1000;//音乐当前播放时间
	return music_timeing;
}

void send_music_from_pos()
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwItem = MCI_STATUS_POSITION;
	DWORD stalReturn = mciSendCommand(wDevioceId, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
	DWORD music_timeing = mciStatus.dwReturn / 1000; //音乐当前播放时间
	MCI_PLAY_PARMS PlayParms;
	PlayParms.dwFrom = music_timeing; //指定从什么时间播放
	DWORD ret1 = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&PlayParms);
}

void MCISeek(int time)
{
	DWORD music_timeing =time / 1000; //音乐当前播放时间
	MCI_PLAY_PARMS PlayParms;
	PlayParms.dwFrom = music_timeing; //指定从什么时间播放
	DWORD ret1 = mciSendCommand(wDevioceId, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&PlayParms);

}

void close_mci()
{
	DWORD retclose = mciSendCommand(wDevioceId, MCI_CLOSE, NULL, NULL);
}

//调节音量
DWORD MCISetVolume(DWORD dwVol)
{
	MCI_DGV_SETAUDIO_PARMS mciSetAudioPara;
	mciSetAudioPara.dwCallback = NULL;
	mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetAudioPara.dwValue = dwVol; //音量0-1000
	DWORD dwReturn = mciSendCommand(wDevioceId,MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
	return dwReturn;
}


DWORD MCIOpen(const CString& strPath)
{
	//打开MCI设备
	MCI_OPEN_PARMS mciOP;
	DWORD opReturn;
	MCIDEVICEID wIDDevioce; //保存设备ID

	mciOP.lpstrDeviceType = NULL;
	mciOP.lpstrElementName = strPath; //设置打开的文件的名称
	opReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOP);
	if (opReturn == 0)
	{
		wIDDevioce = mciOP.wDeviceID; //播放设备ID
		wDevioceId = mciOP.wDeviceID;

		//设置设备信息
		MCI_SET_PARMS mciSet;
		mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;  //设置时间单位为毫秒
		mciSendCommand(wDevioceId, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&mciSet);

		//设置当前播放位置跳转60秒
		//MCISeek(100);

		//音频播放
		MCIPlay();

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwItem = MCI_STATUS_TIME_FORMAT;
		DWORD sta2Return = mciSendCommand(wIDDevioce, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatus);
		DWORD music_playstyle = mciStatus.dwReturn; //获得当前播放工作模式 播放 526 暂停 525
	}
	else
		wIDDevioce = NULL;
	return opReturn;

}



int main()
{
	cout << MCIOpen("sound/starlight.mp3") << endl;

	while (1);

	return 0;
}