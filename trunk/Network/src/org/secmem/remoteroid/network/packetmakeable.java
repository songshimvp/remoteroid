package org.secmem.remoteroid.network;

import java.io.*;

interface packetmakeable{
	public void SendPacket(int iOPCode, byte [] data, int length) throws IOException;
}

interface iFileSendable{
	public void SendFileInfo(File file);
	public void SendFileData(File file);
	}