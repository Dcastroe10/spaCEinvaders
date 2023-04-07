#include <stdio.h>
#include <windows.h>
#include <pthread.h>

#define testbutton 1
#define testlabel 2
#define image 3

HWND window;
HWND image2;
HBITMAP cargar_imagen;
int labelx = 200;
int labely = 300;

DWORD WINAPI move_right(LPVOID lpParam){
    while(labelx < 440) { //Por el tamaño de la imagen de prueba
        SetWindowPos(GetDlgItem(window, image), NULL,
                     labelx + 5, labely, 0, 0,
                     SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        labelx += 5;
        Sleep(10);
    }
    labely+=30;

    while (labelx>0){
        SetWindowPos(GetDlgItem(window, image), NULL,
                     labelx - 5, labely, 0, 0,
                     SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        labelx -= 5;
        Sleep(10);
    }
    labely+=30;
    move_right(NULL);
    printf("DONE");

    return 0;
}

void *move_left(){
    while (1){
        printf("ADIOS \n");
        Sleep(1000);
    }
}

void create_ovni(HINSTANCE hInstanciaActual){
    cargar_imagen = (HBITMAP)LoadImageW(NULL,L"img/HASBULLA.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    image2 = CreateWindowW(L"Static",NULL,WS_VISIBLE|WS_CHILD|SS_BITMAP,200,300,0,0,window,(HMENU)image,hInstanciaActual,NULL);
    SendMessage(image2,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)cargar_imagen);
}

LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstanciaActual,HINSTANCE hInstanciaPrevia,LPSTR lpCmdLinea,int nCmdShow){

    MSG mensaje;
    WNDCLASSEX clase;
    clase.cbSize = sizeof(WNDCLASSEX);
    clase.cbClsExtra = 0;
    clase.style = CS_HREDRAW | CS_VREDRAW;
    clase.lpfnWndProc = ProcediementoVentana;
    clase.hInstance = hInstanciaActual;
    clase.hIcon = LoadIcon(NULL,0);//ICONO
    clase.hIconSm =LoadIcon(NULL,IDI_INFORMATION);
    clase.hCursor = LoadCursor(NULL,IDC_CROSS);//Forma del cursor!!
    clase.lpszClassName = "Space Invaders";
    clase.hbrBackground =CreateSolidBrush( RGB(0,0,0)); //color de la ventana!!!!!!!!!!!
    if(!RegisterClassEx(&clase)){
        MessageBox( NULL,"No se pudo ejecutar la aplicacion","Error",MB_ICONERROR);
        return EXIT_FAILURE;
    }

    //Creación de elementos
    window = CreateWindowEx(0,"Space Invaders","Space Invaders",
                            WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,//Para deshabilitar el cambiar el tamaño de la ventana
                            500,100,
                            720,600,HWND_DESKTOP,NULL,hInstanciaActual,
                            NULL);


    //LABEL FUAP
    CreateWindowW(L"Static",L"FUAP",WS_VISIBLE | WS_CHILD , 90,100,50,20,window,(HMENU)testlabel,
                  hInstanciaActual,NULL);

    //create ovni (IMAGE)
    create_ovni(hInstanciaActual);






    CreateWindowEx(0,"BUTTON","MOVE",BS_CENTER|WS_VISIBLE|WS_CHILD,
                   260,250,150,30,window,(HMENU)testbutton,
                   hInstanciaActual,NULL);



    ShowWindow(window,nCmdShow);
    UpdateWindow(window);
    //creamos el bucle para saber si nuestro mensaje esta interactuando con el usuario, si será cero significa que acabo el programa
    while(GetMessage(&mensaje,NULL,0,0)>0){
        TranslateMessage(&mensaje);
        DispatchMessage(&mensaje);
    }

}




LRESULT CALLBACK ProcediementoVentana(HWND hwnd,UINT msg, WPARAM wParam,LPARAM lParam){

    static HINSTANCE Instancia;
    switch(msg){
        case WM_CREATE:{
            Instancia =((LPCREATESTRUCT)lParam)->hInstance;
            break;
        }
        case WM_COMMAND:{
            switch (LOWORD(wParam)) {
                case testbutton:{
                    DWORD thread1;
                    HANDLE thread_move = CreateThread(NULL,0,move_right,
                                                      NULL,0,&thread1);
/*
                    pthread_t proceso1;
                    pthread_create(&proceso1,NULL,&move_rigth,NULL);
                    pthread_join(proceso1,NULL);
*/

                }
            }
            break;
        }
        case WM_DESTROY:{
            PostQuitMessage(0);
            break;
        }
        default:{
            return DefWindowProc(hwnd, msg,wParam,lParam);
        }

    }
    return 0;
}








