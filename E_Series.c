#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* ===================== SERIES TABLES ===================== */

static const int E3[]   ={100,220,470};
static const int E6[]   ={100,150,220,330,470,680};
static const int E12[]  ={100,120,150,180,220,270,330,390,470,560,680,820};
static const int E24[]  ={100,110,120,130,150,160,180,200,220,240,270,300,
                          330,360,390,430,470,510,560,620,680,750,820,910};

static const int E48[]={100,105,110,115,121,127,133,140,147,154,162,169,
178,187,196,205,215,226,237,249,261,274,287,301,
316,332,348,365,383,402,422,442,464,487,511,536,
562,590,619,649,681,715,750,787,825,866,909,953};

static const int E96[]={100,102,105,107,110,113,115,118,121,124,127,130,
133,137,140,143,147,150,154,158,162,165,169,174,
178,182,187,191,196,200,205,210,215,221,226,232,
237,243,249,255,261,267,274,280,287,294,301,309,
316,324,332,340,348,357,365,374,383,392,402,412,
422,432,442,453,464,475,487,499,511,523,536,549,
562,576,590,604,619,634,649,665,681,698,715,732,
750,768,787,806,825,845,866,887,909,931,953,976};

static const int E192[]={100,101,102,104,105,106,107,109,110,111,113,114,
115,117,118,120,121,123,124,126,127,129,130,132,
133,135,137,138,140,142,143,145,147,149,150,152,
154,156,158,160,162,164,165,167,169,172,174,176,
178,180,182,184,187,189,191,193,196,198,200,203,
205,208,210,213,215,218,221,223,226,229,232,234,
237,240,243,246,249,252,255,258,261,264,267,271,
274,277,280,284,287,291,294,298,301,305,309,312,
316,320,324,328,332,336,340,344,348,352,357,361,
365,370,374,379,383,388,392,397,402,407,412,417,
422,427,432,437,442,448,453,459,464,470,475,481,
487,493,499,505,511,517,523,530,536,542,549,556,
562,569,576,583,590,597,604,612,619,626,634,642,
649,657,665,673,681,690,698,706,715,723,732,741,
750,759,768,777,787,796,806,816,825,835,845,856,
866,876,887,898,909,920,931,942,953,965,976,988};

#define SZ(x) (sizeof(x)/sizeof(x[0]))

/* ===================== GLOBAL ===================== */

const int *SERIES = E3;
int SERIES_SIZE = SZ(E3);
int sig_digits = 2;
char series_name[16] = "E3";

HWND hInput, hOutNearest, hOutError, hLabel;
char suffix_char = 0;

void compute(void);
WNDPROC OldEditProc;

/* ===================== CORE ===================== */

double nearest(double v)
{
    int i, exp;
    double norm, best, bestErr;

    exp = (int)floor(log10(v));
    norm = v / pow(10.0, exp);

    if(norm < 1.0){
        norm *= 10.0;
        exp--;
    }

    best = SERIES[0]/100.0;
    bestErr = fabs((best - norm) / norm);

    for(i=1;i<SERIES_SIZE;i++){
        double val = SERIES[i]/100.0;
        double err = fabs((val - norm) / norm);
        if(err < bestErr){
            bestErr = err;
            best = val;
        }
    }

    {
        double val = 10.0;
        double err = fabs((val - norm) / norm);
        if(err < bestErr){
            best = val;
        }
    }

    return best * pow(10.0, exp);
}

void compute(void)
{
    char buf[128], out[64], errbuf[64];
    double in, result, err;
    int len;
    char *e_ptr, *e_ptr_in;
    char tmp[64];
    int exp_val;
    char exp_char;
    double mant, absval;

    GetWindowText(hInput, buf, sizeof(buf));

    suffix_char = 0;
    e_ptr = strchr(buf,'e');
    if(e_ptr == NULL) e_ptr = strchr(buf,'E');

    len = strlen(buf);

    if(len > 0 && isalpha((unsigned char)buf[len-1])){
        if(e_ptr == NULL || (buf + len - 1) != e_ptr){
            suffix_char = buf[len-1];
            buf[len-1] = 0;
        }
    }

    in = atof(buf);
    if(in <= 0.0) return;

    result = nearest(in);
    err = 100.0 * (result / in - 1.0);

    exp_char = 'e';
    exp_val = 0;

    e_ptr_in = strchr(buf,'e');
    if(e_ptr_in == NULL){
        e_ptr_in = strchr(buf,'E');
        if(e_ptr_in != NULL) exp_char = 'E';
    }

    if(e_ptr_in != NULL)
    {
        exp_val = atoi(e_ptr_in + 1);
        mant = result / pow(10.0, exp_val);

        if(sig_digits == 3)
            sprintf(tmp,(mant>=100)?"%.0f":(mant>=10)?"%.1f":"%.2f",mant);
        else
            sprintf(tmp,(mant>=10)?"%.0f":"%.1f",mant);

        sprintf(out," %s%c%d%c",tmp,exp_char,exp_val,
            suffix_char?suffix_char:'\0');
    }
    else
    {
        absval = fabs(result);

        if(sig_digits == 3)
            sprintf(out,(absval>=100)?" %.0f%c":(absval>=10)?" %.1f%c":" %.2f%c",
                result,suffix_char?suffix_char:'\0');
        else
            sprintf(out,(absval>=10)?" %.0f%c":" %.1f%c",
                result,suffix_char?suffix_char:'\0');
    }

    sprintf(errbuf," %+0.3g%%", err);

    SetWindowText(hOutNearest, out);
    SetWindowText(hOutError, errbuf);

    {
        char label[64];
        sprintf(label,"Nearest %s:", series_name);
        SetWindowText(hLabel, label);
    }
}

/* ===================== EDIT FIX ===================== */

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(msg == WM_CHAR && wParam == '\r')
    {
        /* CLEAR EVERYTHING */
        SetWindowText(hInput, "");
        SetWindowText(hOutNearest, "");
        SetWindowText(hOutError, "");
        return 0;  /* stop beep */
    }

    return CallWindowProc(OldEditProc, hwnd, msg, wParam, lParam);
}

/* ===================== WINDOW ===================== */

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:

        CreateWindow("BUTTON","E3",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,20,80,20,hwnd,(HMENU)(INT_PTR)100,NULL,NULL);
        CreateWindow("BUTTON","E6",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,45,80,20,hwnd,(HMENU)(INT_PTR)101,NULL,NULL);
        CreateWindow("BUTTON","E12",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,70,80,20,hwnd,(HMENU)(INT_PTR)102,NULL,NULL);
        CreateWindow("BUTTON","E24",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,95,80,20,hwnd,(HMENU)(INT_PTR)103,NULL,NULL);
        CreateWindow("BUTTON","E48",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,120,80,20,hwnd,(HMENU)(INT_PTR)104,NULL,NULL);
        CreateWindow("BUTTON","E96",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,145,80,20,hwnd,(HMENU)(INT_PTR)105,NULL,NULL);
        CreateWindow("BUTTON","E192",WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON,
            20,170,80,20,hwnd,(HMENU)(INT_PTR)106,NULL,NULL);

        SendMessage(GetDlgItem(hwnd,100),BM_SETCHECK,BST_CHECKED,0);

        CreateWindow("STATIC","Enter value:",WS_VISIBLE|WS_CHILD,
            120,20,100,20,hwnd,NULL,NULL,NULL);

        hInput = CreateWindow("EDIT","",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            220,20,120,25,hwnd,NULL,NULL,NULL);

        OldEditProc = (WNDPROC)SetWindowLongPtr(hInput, GWLP_WNDPROC, (LONG_PTR)EditProc);

        hLabel = CreateWindow("STATIC","Nearest E3:",
            WS_VISIBLE|WS_CHILD,
            120,60,95,20,hwnd,NULL,NULL,NULL);

        hOutNearest = CreateWindow("STATIC","",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            220,60,120,25,hwnd,NULL,NULL,NULL);

        CreateWindow("STATIC","Error, %:",WS_VISIBLE|WS_CHILD,
            120,100,100,20,hwnd,NULL,NULL,NULL);

        hOutError = CreateWindow("STATIC","",
            WS_VISIBLE|WS_CHILD|WS_BORDER,
            220,100,120,25,hwnd,NULL,NULL,NULL);

        break;

    case WM_COMMAND:
        switch(LOWORD(wParam)){
            case 100: SERIES=E3; SERIES_SIZE=SZ(E3); sig_digits=2; strcpy(series_name,"E3"); break;
            case 101: SERIES=E6; SERIES_SIZE=SZ(E6); sig_digits=2; strcpy(series_name,"E6"); break;
            case 102: SERIES=E12; SERIES_SIZE=SZ(E12); sig_digits=2; strcpy(series_name,"E12"); break;
            case 103: SERIES=E24; SERIES_SIZE=SZ(E24); sig_digits=2; strcpy(series_name,"E24"); break;
            case 104: SERIES=E48; SERIES_SIZE=SZ(E48); sig_digits=3; strcpy(series_name,"E48"); break;
            case 105: SERIES=E96; SERIES_SIZE=SZ(E96); sig_digits=3; strcpy(series_name,"E96"); break;
            case 106: SERIES=E192; SERIES_SIZE=SZ(E192); sig_digits=3; strcpy(series_name,"E192"); break;
        }
        compute();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        ExitProcess(0);
        return 0;
    }

    return DefWindowProc(hwnd,msg,wParam,lParam);
}

/* ===================== MAIN ===================== */

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    memset(&wc,0,sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "EAPP";
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    RegisterClass(&wc);

    hwnd = CreateWindow("EAPP","E-Series Calculator",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        200,200,380,260,
        NULL,NULL,hInst,NULL);

    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

