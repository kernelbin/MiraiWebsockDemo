#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include "..\MiraiWebsock\MiraiWS.h"

VOID PrintMessageChain(MESSAGE_CHAIN* pMessageChain)
{
    for (int i = 0; i < pMessageChain->BlockCnt; i++)
    {
        MESSAGE_BLOCK* pBlock = &(pMessageChain->MessageBlocks[i]);
        switch (pBlock->Type)
        {
            case MB_AT:
            {
                wprintf(L"[@%lld]", pBlock->At.Target);
                break;
            }
            case MB_ATALL:
            {
                wprintf(L"[@all]");
                break;
            }
            case MB_FACE:
            {
                wprintf(L"[EMOJI]");
                break;
            }
            case MB_PLAIN:
            {
                wprintf(L"%ls", pBlock->Plain.Text);
                break;
            }
            case MB_IMAGE:
            {
                wprintf(L"[IMAGE]");
                break;
            }
            case MB_VOICE:
            {
                wprintf(L"[VOICE]");
                break;
            }
            case MB_XML:
            {
                wprintf(L"[XML]");
                break;
            }
            case MB_JSON:
            {
                wprintf(L"[JSON]");
                break;
            }
            case MB_APP:
            {
                wprintf(L"[APP]");
                break;
            }
            case MB_POKE:
            {
                wprintf(L"[POKE]");
                break;
            }
            case MB_DICE:
            {
                wprintf(L"[DICE]");
                break;
            }
            case MB_MARKETFACE:
            {
                wprintf(L"[EMOJI]");
                break;
            }
            case MB_MUSICSHARE:
            {
                wprintf(L"[MUSIC]");
                break;
            }
            case MB_FORWARD:
            {
                wprintf(L"[FORWARDED MESSAGE]");
                break;
            }
            case MB_FILE:
            {
                wprintf(L"[FILE]");
                break;
            }
        }
        wprintf(L" ");
    }
}

VOID MiraiWSCallback(_In_ PMIRAI_WS pMiraiWS, _In_ UINT EventType, _In_ PVOID pInformation)
{
    switch (EventType)
    {
    case MWS_CONNECT:
    {
        MWS_CONNECTINFO* pInfo = (MWS_CONNECTINFO*)pInformation;
        if (pInfo->bSuccess)
        {
            wprintf(L"connected successfully\n\n");
        }
        else
        {
            wprintf(L"failed to connect: %d\n\n", pInfo->dwError);
        }
        break;
    }
    case MWS_NWERROR:
    {
        MWS_NWERRORINFO* pInfo = (MWS_NWERRORINFO*)pInformation;
        wprintf(L"network error: %d\n\n", pInfo->dwError);
        break;
    }
    case MWS_BADMSG:
    {
        MWS_BADMSGINFO* pInfo = (MWS_BADMSGINFO*)pInformation;
        wprintf(L"received an unresolved message: \n"
            L"===== begin =====\n"
            L"%s\n"
            L"====== end ======\n\n", pInfo->Message);
        break;
    }
    case MWS_AUTH:
    {
        MWS_AUTHINFO* pInfo = (MWS_AUTHINFO*)pInformation;

        wprintf(L"authentication code: %lld\n", pInfo->ResponseCode);
        if (pInfo->Session) wprintf(L"session: %s\n", pInfo->Session);
        if (pInfo->Message) wprintf(L"message: %s\n", pInfo->Message);
        wprintf(L"\n");
        break;
    }
    case MWS_FRIENDMSG:
    {
        MWS_FRIENDMSGINFO* pInfo = (MWS_FRIENDMSGINFO*)pInformation;

        wprintf(L"received a message from %s(%s)\n", pInfo->Sender.Nick, pInfo->Sender.Remark);
        PrintMessageChain(&pInfo->MessageChain);
        wprintf(L"\n\n");
        break;
    }
    case MWS_GROUPMSG:
    {
        MWS_GROUPMSGINFO* pInfo = (MWS_GROUPMSGINFO*)pInformation;
        wprintf(L"received a message in group %s from %s\n", pInfo->Sender.Group.Name, pInfo->Sender.MemberName);
        PrintMessageChain(&pInfo->MessageChain);
        wprintf(L"\n\n");
        break;
    }
    }
}

int wmain()
{
    setlocale(LC_ALL, "");
    WCHAR szServerAddr[MAX_PATH] = { 0 }, szVerifyKey[MAX_PATH] = { 0 }, szQQ[MAX_PATH] = { 0 };
    INT Port = 0;

    wprintf(L"your mirai server address: ");
    wscanf(L"%s", szServerAddr);
    wprintf(L"your mirai server websocket port: ");
    wscanf(L"%d", &Port);

    PMIRAI_WS pMiraiWS = CreateMiraiWS(szServerAddr, (INTERNET_PORT)Port, TRUE, MiraiWSCallback);
    if (!pMiraiWS)
    {
        wprintf(L"CreateMiraiWS failed");
        return 0;
    }


    wprintf(L"your mirai-api-http verify key: ");
    wscanf(L"%s", szVerifyKey);
    wprintf(L"your QQ: ");
    wscanf(L"%s", szQQ);
    if (!ConnectMiraiWS(pMiraiWS, szVerifyKey, szQQ))
    {
        wprintf(L"ConnectMiraiWS failed");
        return 0;
    }

    while (1)
    {
        WCHAR command[MAX_PATH] = { 0 };
        wscanf(L"%s", command);
        if (wcscmp(command, L"stop") == 0)
        {
            break;
        }
        else
        {
            wprintf(L"unrecognized command [%s]\n\n", command);
        }
    }
    DestroyMiraiWSAsync(pMiraiWS);

    return 0;
}
