#include "StdAfx.h"
#include "FileSender.h"


CFileSender::CFileSender()
	: totalFileSize(0), m_pClient(NULL),
	sendedFileSize(0),isSending(FALSE)	
{
	memset(buffer, 0, sizeof(buffer));
}


CFileSender::~CFileSender(void)
{
}

void CFileSender::SetClient(CMyClient *pClient)
{
	m_pClient = pClient;
}

int CFileSender::SendPacket(int iOPCode, const char * data, int iDataLen)
{
	return m_pClient->SendPacket(iOPCode,data,iDataLen);
}


int CFileSender::SendFileInfo(CFile *pFile)
{	
	totalFileSize = pFile->GetLength();
	CString fileName = pFile->GetFileName();

	//�����ڵ� ������ ���� �̸��� UTF-8�� ��ȯ
	TCHAR uniFileName[FILENAMESIZE];
	char utfFileName[FILENAMESIZE];	
	memset(uniFileName, 0, sizeof(uniFileName));
	memset(utfFileName, 0, sizeof(utfFileName));
	_tcscpy(uniFileName, fileName);
	CUtil::UniToUtf(uniFileName, utfFileName);

	//�������ݿ� ���缭 ���� 100����Ʈ�� ���� �̸� ���� 100����Ʈ�� ���� ũ��
	
	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, utfFileName, FILENAMESIZE);
	sprintf(buffer+FILENAMESIZE, "%100llu", totalFileSize);

	sendedFileSize = 0;
	return SendPacket(OP_SENDFILEINFO, buffer, FILENAMESIZE+FILESIZESIZE);
}

int CFileSender::SendFileData(CFile * pFile)
{
	int result = 0;	
	while(totalFileSize > sendedFileSize)
	{
		int iCurrentSendSize = (totalFileSize-sendedFileSize) > MAXDATASIZE ?
			MAXDATASIZE : totalFileSize-sendedFileSize;
		pFile->Read(buffer, iCurrentSendSize);
		result = SendPacket(OP_SENDFILEDATA, buffer, iCurrentSendSize);
		sendedFileSize += iCurrentSendSize;
	}
	pFile->Close();
	return result;
}



void CFileSender::AddSendFile(CFile * pFile)
{
	sendFileList.AddTail((void*)pFile);	
}


BOOL CFileSender::StartSendFile(void)
{
	if(m_pClient == NULL || isSending == TRUE)
		return FALSE;

	isSending = TRUE;
	AfxBeginThread(SendFileThread, this);
	return TRUE;
}


UINT CFileSender::SendFileThread(LPVOID pParam)
{	
	CFileSender *pDlg = (CFileSender *)pParam;	

 	POSITION pos = pDlg->sendFileList.GetHeadPosition();
	while(pos)
	{
		CFile *pFile = (CFile *)pDlg->sendFileList.GetNext(pos);
		pDlg->SendFileInfo(pFile);
		pDlg->SendFileData(pFile);
	}
	pDlg->isSending = FALSE;
	return 0;
}
