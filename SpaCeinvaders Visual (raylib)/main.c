
//mover al header
#include "include/raylib.h"
#include <stdio.h>
#include "pthread.h"
#include "main.h"


Texture2D ovni;
Texture2D calamar;
Texture2D crab;
Texture2D pulpo;
Texture2D nave;


int posx=0;
int posy=0;

int navex = 0;
int navey = 800;
int currentLaser=0;
int laserlimit = 0;

int z = 1;


//borrar boton!!
typedef struct {
    Rectangle rect;   // Rectángulo que define la posición y tamaño del botón
    const char *text; // Texto que se muestra en el botón
} Button;

struct Laser {
    Vector2 position;
    Vector2 direction;
    bool active;
    bool colision;
    Rectangle laser_rec;
};

struct Ovnis {
    Texture2D texture;
    int velocidad;
    int tipo; //1calamar - 2cangrejo 4pulpo
    bool active;
    bool colision;
    Rectangle ovni_rec; //x y determinan posicion
};


struct Laser lasers[10];
int lasers_size = 10;
struct Ovnis ovnis[8][11];





//________________________________________________________________________________________________________-


void load_crab(){
    Image imagen = LoadImage("img/crab 50x50.png");  
    crab = LoadTextureFromImage(imagen);      
    UnloadImage(imagen);                                    
    imagen = LoadImageFromTexture(crab);                 
    UnloadTexture(crab);                               
    crab = LoadTextureFromImage(imagen);                
}
void load_calamar(){
    Image imagen = LoadImage("img/calamar 50x50.png");  
    calamar = LoadTextureFromImage(imagen);      
    UnloadImage(imagen);                                   
    imagen = LoadImageFromTexture(calamar);                
    UnloadTexture(calamar);                               
    calamar = LoadTextureFromImage(imagen);                
}
void load_ovni(){
    Image image = LoadImage("img/ovni.png"); 
    ovni = LoadTextureFromImage(image);      
    UnloadImage(image);                                   
    image = LoadImageFromTexture(ovni);                
    UnloadTexture(ovni);                            
    ovni = LoadTextureFromImage(image);              
}


void load_spaceship(){
    Image image = LoadImage("img/spaceship100x100.png");  
    nave = LoadTextureFromImage(image);    
    UnloadImage(image);                                    
    image = LoadImageFromTexture(nave);                 
    UnloadTexture(nave);                             
    nave = LoadTextureFromImage(image);                
}





//Creación de una fila de cangrejos
void create_crabs(int fila){
     int temp = 0;
    int temp_fila = 0;
    if (fila == 1){
        temp_fila = 0;
        }else if (fila == 2){
            temp_fila = 50;
            }else if (fila == 3){
                temp_fila = 100;
                }else if (fila == 4){
                    temp_fila = 150;
                    }else if (fila == 5){
                        temp_fila = 200;
                        }else if (fila == 6){
                            temp_fila = 250;
                            }else if (fila == 7){
                                temp_fila = 300;
                                }else{
                                    temp_fila = 350;
                                    }
    for (int i = 0; i <8 ; i++)
    {
        ovnis[fila-1][i].texture = crab;
        ovnis[fila-1][i].active=true;
        ovnis[fila-1][i].tipo = 2;
        ovnis[fila-1][i].ovni_rec=(Rectangle){posx+temp,posy+temp_fila,50,50};
        temp+=75;
     }
    }
void create_calamars(int fila){
    int temp = 0;
    int temp_fila = 0;
    if (fila == 1){
        temp_fila = 0;
        }else if (fila == 2){
            temp_fila = 50;
            }else if (fila == 3){
                temp_fila = 100;
                }else if (fila == 4){
                    temp_fila = 150;
                    }else if (fila == 5){
                        temp_fila = 200;
                        }else if (fila == 6){
                            temp_fila = 250;
                            }else if (fila == 7){
                                temp_fila = 300;
                                }else{
                                    temp_fila = 350;
                                    }
    
    for (int i = 0; i <11 ; i++)
    {
        ovnis[fila-1][i].texture = calamar;
        ovnis[fila-1][i].active=true;
        ovnis[fila-1][i].tipo = 1;
        ovnis[fila-1][i].ovni_rec.x=posx+temp;
        ovnis[fila-1][i].ovni_rec.y=posy+temp_fila;
        ovnis[fila-1][i].ovni_rec.height=50;
        ovnis[fila-1][i].ovni_rec.width=50;
        temp+=75;
        
     }
     
}

    
//Coloca todos los lasers en la posición inicial de la nave
void reset_lasers(){
    for (int i = 0; i < lasers_size; i++)
        {
            lasers[i].position.x=navex - (nave.width/2);
            lasers[i].position.y=820;{}
            lasers[i].laser_rec=(Rectangle){navex - (nave.width/2),820,5,10};
        }
}
void reset_pos_ovnis(){
     for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                ovnis[i][j].ovni_rec.x =posx;
                ovnis[i][j].ovni_rec.y =posy;
                
            }
        }

}

void *change_posx_posy(){
    while (1)
    { 
        if (ovnis[0][7].ovni_rec.x<750)
        {
            WaitTime(0.1);
            posx+=10;
        }
        if(ovnis[0][7].ovni_rec.x>750){
            posx=0;
            printf("AAAXXXX: %d\n",posy);
            posy+=50;
            reset_pos_ovnis();
        }
        
       
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 900;

    
    InitWindow(screenWidth, screenHeight, "SpaCEinvaders");
    SetTargetFPS(20);


    load_crab();
    load_calamar();

    //create_crabs(1);
  
    create_calamars(1);
    create_calamars(2);
    create_calamars(3);
    //create_calamars(4);
    //create_calamars(5);
    //create_calamars(6);
    //create_calamars(7);
    //create_calamars(8);
    




    load_spaceship();
    navex=screenWidth/2-(nave.width/2);
    navey = screenHeight-(nave.height);
    reset_lasers();
    pthread_t thread1; 
    pthread_create(&thread1,NULL,&change_posx_posy,NULL);
    

    while (!WindowShouldClose()) {
        
        if (IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)){
            if (navex<695){
                 navex+=15;
            }
            } 
        
        if (IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A)){
            if (navex>5)
            {
                navex-=15;
            }
            }
    
        if (IsKeyPressed(KEY_SPACE)){
        {
            if (laserlimit<2){ //Limite de cuantos disparos hay activos
                laserlimit+=1;
                if (currentLaser >= 2){
                    currentLaser = 0;
                    reset_lasers();
                }
                lasers[currentLaser].active = true;
                lasers[currentLaser].position.x= navex+nave.width/2;
                currentLaser++;
                printf("\n%d",currentLaser); 
            }
                
        }
        }

        //actualiza la posicion de todos los lasers activos
        for (int i = 0; i < lasers_size; i++)
        {
            if (lasers[i].active){
                lasers[i].position.y -= 10;
                lasers[i].laser_rec.y-=10;
                lasers[i].laser_rec.x = lasers[i].position.x;
                 for (int i = 0; i < 8; i++)
                 {
                        for (int j = 0; j < 11; j++){
                            if(ovnis[i][j].active){
                           //printf("\n%f",ovnis[i]->ovni_rec.x);
                            if (CheckCollisionRecs(ovnis[i][j].ovni_rec,lasers[i].laser_rec)){
                                lasers[i].active = false;
                                ovnis[i][j].active = false;
                                laserlimit-=1;
                                printf("COLISIOOOOON\n");
                            }                                    
                        }
                    }
                 }
            

               
                // Borrar el laser si sale de la pantalla
                if (lasers[i].position.y < 0)
                {
                    lasers[i].active = false;
                    laserlimit-=1;
                    printf("laser borrado");
                }

                }
            }

        int tempx=0;
        int tempy = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                ovnis[i][j].ovni_rec.x =posx+tempx;
                ovnis[i][j].ovni_rec.y =posy+tempy;
                tempx+=75;
            }
            tempx=0;
            tempy+=50;
            }
       
        
        


        
        //Drawing
        BeginDrawing();
        
       
        
        DrawTexture(crab,0,0, WHITE);
        DrawTexture(nave,navex, navey, WHITE);
       


        
        //DrawRectangle(lasers[0].laser_rec.x, lasers[0].laser_rec.y, lasers[0].laser_rec.width, lasers[0].laser_rec.height, BLUE);
        
         for (int i = 0; i < lasers_size; i++){
            if (lasers[i].active){
                DrawLine(lasers[i].position.x,lasers[i].position.y,lasers[i].position.x,lasers[i].position.y+10,GREEN);
                DrawLine(lasers[i].position.x+1,lasers[i].position.y,lasers[i].position.x+1,lasers[i].position.y+10,GREEN);
                DrawLine(lasers[i].position.x+2,lasers[i].position.y,lasers[i].position.x+2,lasers[i].position.y+10,GREEN);
                DrawLine(lasers[i].position.x-1,lasers[i].position.y,lasers[i].position.x-1,lasers[i].position.y+10,GREEN);
                DrawLine(lasers[i].position.x-2,lasers[i].position.y,lasers[i].position.x-2,lasers[i].position.y+10,GREEN);
            }
        }

         //Dibuja los Ovnis
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 11; j++)
            {
               if (ovnis[i][j].active)
               {
                
                DrawTexture(ovnis[i][j].texture,ovnis[i][j].ovni_rec.x, ovnis[i][j].ovni_rec.y, WHITE);
                
                //DrawRectangle(ovnis[i][j].ovni_rec.x,ovnis[i][j].ovni_rec.y, ovnis[i][j].ovni_rec.width,ovnis[i][j].ovni_rec.height, WHITE);
               }
            }
            
        }
        // DrawRectangle(ovnis[5][5].ovni_rec.x,ovnis[5][5].ovni_rec.y, ovnis[5][5].ovni_rec.width,ovnis[5][5].ovni_rec.height, WHITE);

        ClearBackground(BLACK);
        
       


        EndDrawing();
    }
    pthread_exit(NULL);
    CloseWindow();
    return 0;
}


