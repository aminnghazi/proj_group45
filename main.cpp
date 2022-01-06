#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <SDL2/SDL2_gfx.h>
#include<SDL2/SDL_ttf.h>


using namespace std;
//action functions
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void picLoader (SDL_Renderer* renderer,int x,int y,int width,int height,const char* address);
bool btn_clicked(int index, int mouse_x, int mouse_y);
void draw(SDL_Renderer* renderer);
void collision(SDL_Renderer* renderer);
void movement(SDL_Renderer* renderer);
void cordinatefinder();

//stage functions
short startMenu(SDL_Renderer* renderer,int page_width,int page_height);
void play(SDL_Renderer* renderer);
void tutorial(SDL_Renderer* renderer);
void setting(SDL_Renderer* renderer);



//global variables
int fps=50,delay=1000/fps;
long long int t=0;      // time(increases when every frame is shown
const int nbtn = 50,zamin_y=610;    // number of btn
int btn_array[nbtn][4]; // X0, Y0, X1, Y1
bool Ljump;

const char* ume_picAddress [] = {"1.jpg" , "2.jpg" ,"3.jpg"}; //address of pictures for start menu
const Uint8 *state = SDL_GetKeyboardState(NULL);

SDL_Event *event = new SDL_Event();

float ball_x=200,ball_y=40,ball_size=30,ball_dx=2,ball_ddx=0,ball_dy=50,ball_ddy=0.5, //toop
      lc_dy=0,lc_ddy=0.5,lc_scale=0.3,lc_x=100,lc_y=zamin_y-500*lc_scale,lhead_x=185*lc_scale+lc_x,lhead_y=165*lc_scale+lc_y;//character chap


int main( int argc, char * argv[] )
{

     int W = 800; //ertefa safhe
     int L = 1280;//tool safhe
    // be ina kari nadashte bashid
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;//| SDL_WINDOW_FULLSCREEN_DESKTOP;//SDL_WINDOW_BORDERLESS ;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Init( SDL_flags );
    SDL_CreateWindowAndRenderer( L, W, WND_flags, &m_window, &m_renderer );
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen res olution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    // be ina kari nadashte bashid

bool exit=0;
while(!exit){

switch(startMenu(m_renderer,L,W)){
case 0://play button pressed
      play(m_renderer);
      break;
case 1://info button pressed
      tutorial(m_renderer);
      break;
case 2://setting button pressed
      setting(m_renderer);
      break;
case 3:
      exit=1;
      break;

}

}


                        SDL_DestroyWindow( m_window );
                        SDL_DestroyRenderer( m_renderer );
                        SDL_Quit();
                        return 0;
}










void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L, double theta,int widht, int R, int G, int B )

{
    int x_2 = x_1 + L*cos(theta);
    int y_2 = y_1 - L*sin(theta);
    thickLineRGBA(Renderer,x_1,y_1,x_2,y_2,widht,R,G,B,255);
  //  SDL_RenderPresent(Renderer);
}



void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian )

{
    SDL_Rect rectangle ;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_boolian==1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
}






short startMenu(SDL_Renderer* renderer,int page_width,int page_height){
      int stng_x=880,stng_y=280,stng_size=150,
      stbtn_x=870,stbtn_y=70,stbtn_size=170,
      info_x=876,info_y=470,info_size=150,
      mouse_x,mouse_y;
      bool changeMade=1;

      btn_array[10][0]=stbtn_x;btn_array[10][1]=stbtn_y;   //start button
      btn_array[10][2]=stbtn_x+stbtn_size;btn_array[10][3]=stbtn_y+stbtn_size;
      btn_array[11][0]=stng_x,btn_array[11][1]=stng_y;         //setting button
      btn_array[11][2]=stng_x+stng_size;btn_array[11][3]=stng_y+stng_size;
      btn_array[14][0]=info_x;btn_array[14][1]=info_y; //guide button
      btn_array[14][2]=info_x+info_size;btn_array[14][3]=info_y+info_size;
      //note that btn_array  [15] is reserved for start menu for adding more buttons

      while(1){
           SDL_PollEvent(event);
           if(SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_LEFT)){
            if(btn_clicked(10,mouse_x,mouse_y)) return 0; //start got clicked
            if(btn_clicked(11,mouse_x,mouse_y)) return 2; //setting got clicked
            if(btn_clicked(14,mouse_x,mouse_y)) return 1; //guide clicked
              }


      if(changeMade){
            picLoader(renderer,0,0,page_width,page_height,"background.jpg");
            picLoader(renderer, info_x, info_y, info_size+10, info_size, "manual.png");            //guide
            picLoader(renderer, stng_x, stng_y, stng_size, stng_size, "settings (1).png");      //game icon
            picLoader(renderer, stbtn_x, stbtn_y, stbtn_size, stbtn_size, "startbtn.png");      // start button

            SDL_RenderPresent(renderer);
            changeMade=0;
      }
            SDL_Delay(delay);
      }

}




void play(SDL_Renderer* renderer){
      //load kardan pas zamine
       SDL_Rect img_rect;
       SDL_Texture* m_img = NULL;
       m_img = IMG_LoadTexture(renderer,"a.png");
       img_rect.x = 0;        //pictures coordinates
       img_rect.y = 0;        //pictures coordinates
       img_rect.w = 1280;    //pictures size
       img_rect.h = 800;
             //load kardan pas zamine
       SDL_Rect tir_rect;
       SDL_Texture* tir = NULL;
       tir = IMG_LoadTexture(renderer,"tir.png");
       tir_rect.x = 0;        //pictures coordinates
       tir_rect.y = 0;        //pictures coordinates
       tir_rect.w = 1280;    //pictures size
       tir_rect.h = 800;

while(1){
//cordinatefinder();//comment this line in final version
cout<<state;
SDL_RenderCopy(renderer, m_img, NULL, &img_rect);
movement(renderer);
collision(renderer);
draw(renderer);
SDL_RenderCopy(renderer, tir, NULL, &tir_rect);
SDL_RenderPresent(renderer);
t++;
SDL_Delay(delay);
}

SDL_DestroyTexture(m_img);

}


void tutorial(SDL_Renderer* renderer){
      short numberOfPics = sizeof(ume_picAddress)/sizeof(ume_picAddress[0]);// gereftan tedad aks haye safhe aval
      int rarrow_x=900,rarrow_y=530,rarrow_size=250,
      lerrow_x=100,lerrow_y=530,lerrow_size=250,
      home_x=560,home_y=560,home_size=140,mouse_x,mouse_y;
      short index=0;
      bool changeMade=1,click=0;

      btn_array[12][0]=lerrow_x;btn_array[12][1]=lerrow_y; //left arrow
      btn_array[12][2]=lerrow_x+lerrow_size;btn_array[12][3]=lerrow_y+lerrow_size;
      btn_array[13][0]=rarrow_x;btn_array[13][1]=rarrow_y; //right arrow
      btn_array[13][2]=rarrow_x+rarrow_size;btn_array[13][3]=rarrow_y+rarrow_size;
      btn_array[15][0]=home_x;btn_array[15][1]=home_y; //right arrow
      btn_array[15][2]=home_x+home_size;btn_array[15][3]=home_y+home_size;
      while(1){
           SDL_PollEvent(event) ;
           if(SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_LEFT)){

           if(btn_clicked(12,mouse_x,mouse_y) && click){
                 //left arrow button
                 if(index==0) index=numberOfPics-1;
                 else index--;
//                 cout<<index;
                 changeMade=1;
                 click=0;
            }
            if(btn_clicked(13,mouse_x,mouse_y)&&click) {
                 //right arrow button
                 if(index==numberOfPics-1) index=0;
                 else index++;
//                 cout<<index;
                 changeMade=1;
                 click=0;
            }
            if(btn_clicked(15,mouse_x,mouse_y)&&click) return;
            }
            SDL_PollEvent(event) ;
            if(event -> type==SDL_MOUSEBUTTONUP)
                  click=1;


            if(changeMade){
                  window_color(renderer,100,20,30);
                  picLoader(renderer, 168, 50, 900, 500, ume_picAddress[index]); //tutorial pictures
                  picLoader(renderer,home_x,home_y,home_size,home_size,"home.png" ); //tutorial pictures
                  picLoader(renderer, rarrow_x, rarrow_y, rarrow_size, rarrow_size, "rightarrow.png");//right arrow
                  picLoader(renderer, lerrow_x, lerrow_y, lerrow_size, lerrow_size, "leftarrow.png"); //left arrow
                  SDL_RenderPresent(renderer);
            changeMade=0;
            }
      }

      return;
}


void setting(SDL_Renderer* renderer){
window_color(renderer,10,20,30);
SDL_RenderPresent(renderer);
}


void picLoader (SDL_Renderer* renderer,int x,int y,int width,int height,const char* address){

      //loading images
            SDL_Rect img_rect;
            SDL_Texture* m_img = NULL;
            m_img = IMG_LoadTexture(renderer,address); // loading images from address

            img_rect.x = x;        //pictures coordinates
            img_rect.y = y;        //pictures coordinates
            img_rect.w = width;    //pictures size
            img_rect.h = height;   //pictures size

            SDL_RenderCopy(renderer, m_img, NULL, &img_rect);
            SDL_DestroyTexture(m_img);
      //loading images

}
void draw(SDL_Renderer* renderer){
ellipse(renderer,ball_x,ball_y,ball_size,ball_size,100,0,100,1);//replace with image
picLoader(renderer,lc_x,lc_y,350*lc_scale,500*lc_scale,"1norm.png");
ellipse(renderer,lhead_x,lhead_y,10,10,110,102,31,1);

}

void collision(SDL_Renderer* renderer){
if(ball_x>1280-ball_size&&ball_dx>0) ball_dx*=-1;   //raste safhe
if((ball_y>590-ball_size)&&ball_dy>0) ball_dy*=-0.7;//chape zamin
if(ball_y<0+ball_size&&ball_dy<0) ball_dy*=-1;      //khorde be saghf
if(ball_x<0+ball_size&&ball_dx<0) ball_dx*=-1;      //chape safhe
if((ball_y<390&&ball_y>360)&&(ball_x<100 || ball_x>1160)) ball_dy*=-1; //tir ofoghi


if((ball_x - lhead_x)*(ball_x - lhead_x) + (ball_y - lhead_y)*(ball_y - lhead_y) <=(ball_size + 100*lc_scale) * (ball_size + 100*lc_scale)){

      }
}

void movement(SDL_Renderer* renderer){
//      if(sqrt(ball_dy)<0.01 && ball_y>590-ball_size) {ball_ddy=0;ball_dy=0;ball_y=590-ball_size;}
ball_x+=ball_dx;              //kinematic toop
ball_dx+=ball_ddx;            //kinematic toop
lhead_x=185*lc_scale+lc_x;    //kale donbale badan biad
lhead_y=165*lc_scale+lc_y;    //kale donbale badan biad

if(ball_y<zamin_y){
ball_y+=ball_dy;
ball_dy+=ball_ddy;
}
if(lc_y<zamin_y - 500*lc_scale|| Ljump){
lc_y+=lc_dy;
lc_dy+=lc_ddy;
Ljump=0;
}
      SDL_PumpEvents();
      if (state[SDL_SCANCODE_A] && lc_x>90) {
          lc_x-=3.5;
      }
      if (state[SDL_SCANCODE_D]&&lc_x<1080) {
          lc_x+=3.5;
      }
           if (state[SDL_SCANCODE_W] && lc_y>=zamin_y - 500*lc_scale) {
          Ljump=1;
          lc_dy=-9;
      }
}


void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian)

{
    if(fill_boolian==1)
        filledEllipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,255);
    if(fill_boolian==0)
        ellipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,255);
}

void window_color(SDL_Renderer *Renderer, int R, int G, int B)

{
    SDL_SetRenderDrawColor( Renderer, R, G, B, 255 );
    SDL_RenderClear( Renderer );
}

bool btn_clicked(int index, int mouse_x, int mouse_y){
    if(mouse_x >= btn_array[index][0] && mouse_x <= btn_array[index][2]
    && mouse_y >= btn_array[index][1] && mouse_y <= btn_array[index][3])   return true;
    else    return false;
}

void cordinatefinder(){
int x,y;
SDL_GetMouseState(&x,&y);
cout<<x<<"\t"<<y<<endl;
}
