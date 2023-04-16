
#include "include/raylib.h"
#include <stdio.h>
#include "pthread.h"



Texture2D texture;
Texture2D nave;
//posicion de la imagen(cambiar para que sea un struct por nave)
int posx=0;
int posy=0;

int navex = 300;
int navey = 700;
int currentLaser=0;




typedef struct {
    Rectangle rect;   // Rectángulo que define la posición y tamaño del botón
    const char *text; // Texto que se muestra en el botón
} Button;

typedef struct {
    Vector2 position;
    Vector2 direction;
    bool active;
} Laser;


Laser lasers[50]={0};


void DrawButton(Button button) {
    DrawRectangleRec(button.rect, LIGHTGRAY);
    DrawText(button.text, button.rect.x + button.rect.width/2 - MeasureText(button.text, 20)/2, button.rect.y + button.rect.height/2 - 10, 20, GRAY);
}



Texture2D create_ovni(){
    Image image = LoadImage("img/HASBULLA.png");  // Load image data into CPU memory (RAM)
    texture = LoadTextureFromImage(image);       // Image converted to texture, GPU memory (RAM -> VRAM)
    UnloadImage(image);                                    // Unload image data from CPU memory (RAM)

    image = LoadImageFromTexture(texture);                 // Load image from GPU texture (VRAM -> RAM)
    UnloadTexture(texture);                                // Unload texture from GPU memory (VRAM)

    texture = LoadTextureFromImage(image);                 // Recreate texture from retrieved image data (RAM -> VRAM)
    //UnloadImage(image); 
    return texture;
}

Texture2D create_spaceship(){
    Image image = LoadImage("img/nave_azul.png");  
    nave = LoadTextureFromImage(image);    
    UnloadImage(image);                                    
    image = LoadImageFromTexture(nave);                 
    UnloadTexture(nave);                             
    nave = LoadTextureFromImage(image);                
    return nave;
}

void *change_posx_posy(){
    while (1)
    {
        WaitTime(1);
        posx+=10;
        printf("POSICIÓN X: \n%d",posx);
    }
}


void move_spaceship(int x, int y) {
    navex += x;
    navey += y;

}




int main() {
    const int screenWidth = 700;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "SpaCEinvaders");
    SetTargetFPS(20);
                                //posicion en X posicion en Y, ancho, alto
    Button button = { (Rectangle){ 100, 100, 100, 60 }, "MOVE IT!" };
    create_spaceship();


    //Coloca todos los lasers en la posición inicial de la nave (no sirve aún)
    for (int i = 0; i < 50; i++)
        {
           lasers[i].position.x=navex;
           lasers[i].position.y=navey;
        }
    
    //coords de la nave
    Rectangle rectNave = { 0 };
    rectNave.width = nave.width;
    rectNave.height = nave.height;
    rectNave.x = (screenWidth - nave.width) / 2; //condiciones iniciales de creación de la nave (ajustar a donde se coloca)
    rectNave.y = (screenHeight - nave.height) / 2; 

    while (!WindowShouldClose()) {
        
        //actualiza la posicion de todos los lasers activos
        for (int i = 0; i < 50; i++)
        {
            if (lasers[i].active)
            {
                lasers[i].position.x += lasers[i].direction.x * 10;
                lasers[i].position.y += lasers[i].direction.y * 10;

                // Desactivar el láser si sale de la pantalla
                if (lasers[i].position.x > screenWidth || lasers[i].position.x < 0 ||
                    lasers[i].position.y > screenHeight || lasers[i].position.y < 100)
                {
                    lasers[i].active = false;
                }
            }
        }
        //BUTTON PRESSED
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), button.rect)) {
                //crear thread 
                //create_ovni(); 
                

               printf("naveX: %f\n",rectNave.x);
               printf("naveY: %f\n",rectNave.y);

                //pthread_t thread1; 
                //pthread_create(&thread1,NULL,&change_posx_posy,NULL);  //No se utiliza pthread_join() ya que se está llamando desde
                                                                      //el thread principal, por eso abajo se agrega pthread_exit(NULL); 
                         
            }
        }
        if (IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)){ 
            move_spaceship(10, 0); 
            rectNave.x += 10.0f;
            } 
        if (IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A)){
             move_spaceship(-10, 0);
              rectNave.x += -10.0f;
        }
        if (IsKeyDown(KEY_UP)||IsKeyDown(KEY_W)){
            move_spaceship(0, -10);
             rectNave.y += -10.0f;
        } 
        if (IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S)){
            move_spaceship(0, 10);
             rectNave.y += 10.0f;
        } 

         if (IsKeyPressed(KEY_SPACE))
        {
            lasers[currentLaser].position = (Vector2){ rectNave.x, rectNave.y };
            lasers[currentLaser].direction = (Vector2){ 0, -1 };
            lasers[currentLaser].active = true;

            currentLaser++;
            if (currentLaser >= 50) currentLaser = 0;
        }

        
        //Drawing
        BeginDrawing();
        
        
        //Image (posiciones donde se crean) implica movimiento de las mismas si se moficia posx o posy
        DrawTexture(texture, posx, posy, WHITE);
        DrawTexture(nave,navex, navey, WHITE);
        //DrawTexture(texture, posx+150, 0, WHITE);
        //DrawTexture(texture, posx+300, 0, WHITE);
        
        //Label
        //DrawText("labelText", screenWidth/2 - MeasureText("labelText", 20)/2, screenHeight/2 + 50, 20, BLUE);

         for (int i = 0; i < 50; i++)
        {
            if (lasers[i].active)
            {
                DrawLineEx(lasers[i].position, (Vector2){ lasers[i].position.x + lasers[i].direction.x * 10, lasers[i].position.y + lasers[i].direction.y * 10 }, 3, GREEN);
            }
        }

        
        ClearBackground(BLACK);
        
        DrawButton(button);
        DrawLine(18, 42, screenWidth - 18, 42, WHITE);

        EndDrawing();
    }
    pthread_exit(NULL);
    CloseWindow();
    return 0;
}


