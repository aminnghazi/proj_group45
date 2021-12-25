#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <string>

using namespace std;
//action functions
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void picLoader (SDL_Renderer* renderer,int x,int y,int width,int height,const char* address);


//stage functions
void startMenu(SDL_Renderer* renderer,int page_width,int page_height);





//global variables
long long int t=0;      // time(increases when every frame is shown
const int nbtn = 50;    // number of btn
int btn_array[nbtn][4]; // X0, Y0, X1, Y1
const char* ume_picAddress [] = {"1.jpg" , "2.jpg" ,"3.jpg"}; //address of pictures for start menu
int fps=50,delay=1000/fps;



int main( int argc, char * argv[] )
{
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
      }

startMenu(m_renderer);

bool exit=false;
//while(!exit){
//// age mikhay kharej shi exit = 0
//// destroy yadet nare
//// ziad render nakon ke behine she , bara render shart bezar
//
//      window_color(m_renderer,0,200,200);
//            SDL_RenderPresent(m_renderer);
//
//}
//                        SDL_DestroyWindow( m_window );
//                        SDL_DestroyRenderer( m_renderer );
//                        SDL_Quit();



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


void startMenu(SDL_Renderer* renderer,int page_width,int page_height){
      short numberOfPics = sizeof(ume_picAddress)/sizeof(ume_picAddress[0]);// gereftan tedad aks haye safhe aval
      short index=0;
      int ic_x=970,ic_y=150,ic_size=150,stbtn_x=950
      ,stbtn_y=380,stbtn_size=200,rarrow_x=620,rarrow_y=500
      ,rarrow_size=250,lerrow_x=100,lerrow_y=500,lerrow_size=250,mouse_x,mouse_y;
      bool changeMade=1;
      SDL_Event *event = new SDL_Event();

      btn_array[10][0]=stbtn_x;btn_array[10][1]=stbtn_y;   //start button
      btn_array[10][2]=stbtn_x+stbtn_size;btn_array[10][3]=stbtn_x+stbtn_size;
      btn_array[11][0]=ic_x,btn_array[11][1]=ic_y;         //icon button
      btn_array[11][2]=ic_x+ic_size;btn_array[11][3]=ic_y+ic_size;
      btn_array[12][0]=lerrow_x;btn_array[12][1]=lerrow_y; //left arrow
      btn_array[12][2]=lerrow_x+lerrow_size;btn_array[12][3]=lerrow_y+lerrow_size;
      btn_array[13][0]=rarrow_x;btn_array[13][1]=rarrow_y; //right arrow
      btn_array[13][2]=rarrow_x+rarrow_size;btn_array[13][3]=rarrow_y+rarrow_size;
      //note that btn_array [14] & [15] are reserved for start menu for adding more buttons

      while(1){
     //background ?;
            SDL_SetRenderDrawColor(renderer,50,110,50, 255 );
            SDL_RenderClear(renderer);
     //backgronund ?

     SDL_PollEvent(event);
     if(SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_LEFT)){
      if(btn_clicked(10,mouse_x,mouse_y)) cout<<"start clicked ";//start got clicked
      if(btn_clicked(11,mouse_x,mouse_y)) cout<<"icon clicked "; //icon got clicked
      if(btn_clicked(12,mouse_x,mouse_y)){
           if(index==0) index=numberOfPics-1;
           else index--;
           cout<<index;
           changeMade=1;
      }
      if(btn_clicked(13,mouse_x,mouse_y)) {
           if(index==numberOfPics-1) index=0;
           else index++;
           cout<<index;
           changeMade=1;
      }
     }
if(changeMade){
      picLoader(renderer, page_width/10, page_height/6, 700, 400, ume_picAddress[index]); //start menu pictures
      picLoader(renderer, ic_x, ic_y, ic_size, ic_size, "icon.jpg");                      //game icon
      picLoader(renderer, stbtn_x, stbtn_y, stbtn_size, stbtn_size, "startbtn.png");      // start button
      picLoader(renderer, rarrow_x, rarrow_y, rarrow_size, rarrow_size, "rightarrow.png");//right arrow
      picLoader(renderer, lerrow_x, lerrow_y, lerrow_size, lerrow_size, "leftarrow.png"); //left arrow
      SDL_RenderPresent(renderer);
      changeMade=0;
}
      SDL_Delay(100);
      }

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




