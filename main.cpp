#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>

#define D3DFVF_MYVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

typedef struct
{
    FLOAT x, y, z;
    DWORD kolor;
} MYVERTEX;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetupProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD WINAPI BeepThread(LPVOID lParam);

LPDIRECT3D8 direct = NULL;
LPDIRECT3DDEVICE8 device = NULL;
LPDIRECT3DVERTEXBUFFER8 vBuffer = NULL;
LPD3DXFONT font = NULL;
LPD3DXFONT font2 = NULL;

int iVertex = 100;

int WINAPI WinMain(HINSTANCE hThis, HINSTANCE hPrev, char * pszCmd, int nShow)
{
    D3DDISPLAYMODE mode; ZeroMemory(&mode, sizeof(D3DDISPLAYMODE));
    direct = Direct3DCreate8(D3D_SDK_VERSION);
    
    if(!DialogBoxParam(hThis, "SETUP_DIALOG", NULL, SetupProc, (LPARAM)&mode))
    {
        direct->Release();
        return 0;
    }
        
    WNDCLASSEX wincl;
    wincl.hInstance = hThis;
    wincl.lpszClassName = "class";
    wincl.lpfnWndProc = MainWndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = NULL;
    wincl.hIconSm = NULL;
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)COLOR_BTNSHADOW;
    RegisterClassEx(&wincl);
    
    HWND hwnd = CreateWindowEx(0, "class", "Wnerwiacz Direct3D", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, HWND_DESKTOP, 0, hThis, NULL);
    ShowCursor(FALSE);
    ShowWindow(hwnd, SW_SHOW);
    
    D3DPRESENT_PARAMETERS params; ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));
    params.BackBufferWidth = mode.Width;
    params.BackBufferHeight = mode.Height;
    params.BackBufferFormat = mode.Format;
    params.BackBufferCount = 1;
    params.MultiSampleType = D3DMULTISAMPLE_NONE;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = hwnd;
    params.Windowed = FALSE;
    params.EnableAutoDepthStencil = TRUE;
    params.AutoDepthStencilFormat = D3DFMT_D16;
    params.FullScreen_RefreshRateInHz = mode.RefreshRate;
    
    direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, &device);
    device->SetRenderState(D3DRS_LIGHTING, FALSE);
    
    D3DXMATRIX mWorld;
    D3DXMATRIX mView;
    D3DXMATRIX mCamera;
    
    D3DXMatrixIdentity(&mView);
    device->SetTransform(D3DTS_VIEW, &mView);
    
    D3DXMatrixPerspectiveLH(&mCamera, 800.0f, 600.0f, 1.0f, 100.0f);
    device->SetTransform(D3DTS_PROJECTION, &mCamera);
    device->CreateVertexBuffer(iVertex * sizeof(MYVERTEX), 0, D3DFVF_MYVERTEX, D3DPOOL_DEFAULT, &vBuffer);
    
    MYVERTEX * vertex;
    vBuffer->Lock(0, iVertex * sizeof(MYVERTEX), (BYTE**)&vertex, D3DLOCK_DISCARD);
        vertex[0].x = 0.0f;
        vertex[0].y = 0.0f;
        vertex[0].z = 10.0f;
        vertex[0].kolor = (int)(cos(GetTickCount()) * 1000000);
        for(int i = 1; i < iVertex; i++)
        {
            float kat = (float)i / (float)(iVertex - 2) * 2.0f * D3DX_PI;
            vertex[i].x = cos(kat);
            vertex[i].y = sin(kat);
            vertex[i].z = 10.0f;
            vertex[i].kolor = (int)(i * 10000000 * sin(GetTickCount()));
        }
    vBuffer->Unlock();
    
    D3DXCreateFont(device, CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif"), &font);
    D3DXCreateFont(device, CreateFont(42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif"), &font2);
    RECT rect = {20, 20, mode.Width - 20, 90}; char pszText[1024] = "";
    RECT rcUfo = {20, mode.Height - 100, mode.Width - 20, mode.Height - 20}; char pszUfo[] = "Bo¿e nieee!!! ONI chc¹ pobraæ nasze fekalia!!!";
    
    DWORD time = GetTickCount();
    DWORD licznik = 0;
    
    D3DXMATRIX mRotate;
    D3DXMATRIX mMove;
    D3DXMATRIX mScale;
    
    MSG msg;
    
    DWORD value = 0;
    //HANDLE hThread = CreateThread(NULL, 0, BeepThread, (LPVOID)&msg, 0, &value);
    while(msg.message != WM_QUIT)
    {
        if(GetTickCount() - time > 1000)
        {
            wsprintf(pszText, "WNERWIACZ DIRECT 3D v. 1.0\nCopyright (C) by Xamax Software Patryk Koz³owski.\nAll rights reserved.\n\nNa Einsteina, mamy %i fps!", licznik);
            licznik = 0;
            time = GetTickCount();
        }
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
            TranslateMessage(&msg);
        }
        
        device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
        
        font->Begin();
        font->DrawText(pszText, lstrlen(pszText), &rect, DT_LEFT, D3DCOLOR_XRGB(0, 191, 255));
        font->End();
        
        font2->Begin();
        font2->DrawText(pszUfo, lstrlen(pszUfo), &rcUfo, DT_CENTER, D3DCOLOR_XRGB(255, 0, 255));
        font2->End();
        
        device->BeginScene();
                       
        device->SetVertexShader(D3DFVF_MYVERTEX);
        device->SetStreamSource(0, vBuffer, sizeof(MYVERTEX));
        
        D3DXMatrixRotationZ(&mRotate, (float)GetTickCount() / 379.0f);
        D3DXMatrixTranslation(&mMove, sin(GetTickCount() / 1000.0f), cos(GetTickCount() / 250.0f) * 1.5f, 0.0f);
        D3DXMatrixScaling(&mScale, 2200.0f, -1200, 1.0f);
            
        mWorld = mRotate * mMove * mScale;
        device->SetTransform(D3DTS_WORLD, &mWorld);
        device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, iVertex - 2);
    
        device->EndScene();
        
        device->Present(NULL, NULL, NULL, NULL);
        licznik++;        
    }
    
    //WaitForSingleObject(hThread, INFINITE);
    
    font2->Release();
    font->Release();
    vBuffer->Release();
    device->Release();
    direct->Release();
    return 0;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_KEYDOWN:
        {
            if(LOWORD(wParam) == VK_ESCAPE)
            {
                DestroyWindow(hwnd);
                return 0;
            }
            break; 
        }    
    }    
    return DefWindowProc(hwnd, message, wParam, lParam);
}    

BOOL CALLBACK SetupProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static D3DDISPLAYMODE * mode = NULL;
    static int * mTable = NULL;
    switch(message)
    {
        case WM_INITDIALOG:
        {
            mode = (D3DDISPLAYMODE *)lParam;
            
            int iCount = direct->GetAdapterModeCount(D3DADAPTER_DEFAULT);
            mTable = new int[iCount];
            int * pTable = mTable;
            
            for(int i = 0; i < iCount; i++)
            {
                D3DDISPLAYMODE temp; direct->EnumAdapterModes(D3DADAPTER_DEFAULT, i, &temp);
                int iFormat = 0;
                
                switch(temp.Format)
                {
                    case D3DFMT_A8R8G8B8:
                    case D3DFMT_X8R8G8B8:
                        iFormat = 32; break;
                    case D3DFMT_R8G8B8:
                        iFormat = 24; break;
                    case D3DFMT_R5G6B5:
                        iFormat = 16; break;
                    default:
                        iFormat = 0; break;
                }    
                
                if(temp.Width >= 640 && temp.Width <= 1024 && temp.Height >= 480 && temp.Height <= 768 && temp.RefreshRate == 60 && iFormat > 0)
                {
                    char pszText[1024];
                    wsprintf(pszText, "%ix%i, %i bit", temp.Width, temp.Height, iFormat);
                    SendMessage(GetDlgItem(hwnd, 201), CB_ADDSTRING, 0, (LPARAM)pszText);
                    *pTable = i;
                    pTable++;
                }
            }
            
            SendMessage(GetDlgItem(hwnd, 202), CB_ADDSTRING, 0, (LPARAM)"60 Hz");
            SendMessage(GetDlgItem(hwnd, 202), CB_ADDSTRING, 0, (LPARAM)"75 Hz");
            SendMessage(GetDlgItem(hwnd, 202), CB_ADDSTRING, 0, (LPARAM)"85 Hz");
            return FALSE;
        }
        case WM_DESTROY:
        {
            if(mTable)
                delete [] mTable;
            return TRUE;
        }    
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case IDOK:
                {
                    int iRes = SendMessage(GetDlgItem(hwnd, 202), CB_GETCURSEL, 0, 0);
                    int iMode = SendMessage(GetDlgItem(hwnd, 201), CB_GETCURSEL, 0, 0);
                    
                    if(iMode < 0)
                    {
                        MessageBox(hwnd, "Nie wybra³eœ trybu graficznego!", "DirectX Setup", MB_OK | MB_ICONWARNING);
                        return TRUE;
                    }    
                    
                    if(iRes < 0)
                    {
                        MessageBox(hwnd, "Nie wybra³eœ czêstotliwoœci odœwierzania ekranu!", "DirectX Setup", MB_OK | MB_ICONWARNING);
                        return TRUE;
                    }
                    
                    direct->EnumAdapterModes(D3DADAPTER_DEFAULT, mTable[iMode], mode);
                    
                    switch(iRes)
                    {
                        case 1:
                            mode->RefreshRate = 75; break;
                        case 2:
                            mode->RefreshRate = 85; break;
                        default:
                            mode->RefreshRate = 60; break;                                                        
                    }    
                        
                    EndDialog(hwnd, TRUE);
                    return TRUE;
                }
                case IDCANCEL:
                {
                    EndDialog(hwnd, FALSE);
                    return TRUE;
                }    
            }
            break;
        }                
    }    
    return FALSE;
}    

DWORD WINAPI BeepThread(LPVOID lParam)
{
    while(((MSG *)lParam)->message != WM_QUIT)
        Beep(900 + (int)(cos(GetTickCount() / 100) * 100), 100);
    SetEvent(NULL);
    return 0;
}
