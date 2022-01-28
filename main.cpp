#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include<SDL2/SDL_ttf.h>

#define rc_foot_y(rhead_y,rc_scale) rhead_y + 300 * rc_scale
#define lc_foot_y(lhead_y,lc_scale) lhead_y + 300 * lc_scale
#define rc_foot_x(rhead_x,rc_scale) rhead_x - 80 * rc_scale
#define lc_foot_x(lhead_x,lc_scale) lhead_x + 80 * lc_scale
#define shoot_radius 190
#define DEGREE15 1
#define DEGREE45 2
#define DEGREENEGATIVE15 3

using namespace std;
//action functions
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void picLoader (SDL_Renderer* renderer,int x,int y,int width,int height,const char* address); //used for loading pictures
bool btn_clicked(int index, int mouse_x, int mouse_y);//checks if button is clicked by mouse
void draw(SDL_Renderer* renderer);     //useless
void collision(SDL_Renderer* renderer);//checks if 2 objects intersect
void movement(SDL_Renderer* renderer);
void cordinatefinder(); //used for debugging(prints mouse coordinates)
void resetvalues(bool fullrestart); //restarts variables when restart button is clicked or a goal is scored
string intToString(int a);
void gradient(SDL_Renderer* m_renderer, short int vx[], short int vy[]);//used in pause menu
void shoot(); // this function is called when shoot button is pressed
short checkshoot(float tangent);// checks angle of shooting
bool sign(int a);//returns sign of a number(0 if negative)


//stage functions
short startMenu(SDL_Renderer* renderer,int page_width,int page_height);
void play(SDL_Renderer* renderer);
void tutorial(SDL_Renderer* renderer);
void setting(SDL_Renderer* renderer);
int PauseMenu(SDL_Renderer* m_renderer, SDL_Texture* m_texture, int W, int H, int ump_w);


//global variables
int fps=50,delay=1000/fps,rc_shoot=-10,lc_shoot=-10;
long long int t=0;      // time(increases when every frame is shown
const int nbtn = 50,zamin_y=590;
int btn_array[nbtn][4]; // X0, Y0, X1, Y1
bool Ljump;
bool Rjump;
const char* ume_picAddress [] = {"1.jpg" , "2.jpg" ,"3.jpg"}; //address of pictures for start menu
const Uint8 *state = SDL_GetKeyboardState(NULL);

SDL_Event *event = new SDL_Event();
SDL_Renderer* renderer;

float ball_x=200,ball_y=40,ball_radius=30,ball_dx=3,ball_ddx=0,ball_dy=2,ball_ddy=0.5, //toop
      lc_dy=0,lc_ddy=1.6,lc_scale=0.3,lc_x=100,lc_y=zamin_y-500*lc_scale,//character chap
      lhead_x=185*lc_scale+lc_x,lhead_y=165*lc_scale+lc_y                //character chap
      ,rc_dy=0,rc_ddy=1.6,rc_scale=0.3,rc_x=700,rc_y=zamin_y-500*rc_scale,//character rast
      rhead_x=210*rc_scale+rc_x,rhead_y=165*rc_scale+rc_y;               //character rast


int main( int argc, char * argv[] )
{

     int W = 800; //ertefa safhe
     int L = 1280;//tool safhe
    // be ina kari nadashte bashid
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;//| SDL_WINDOW_FULLSCREEN_DESKTOP;//SDL_WINDOW_BORDERLESS ;
    SDL_Window *m_window;
    SDL_Init( SDL_flags );
    SDL_CreateWindowAndRenderer( L, W, WND_flags, &m_window, &renderer );
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen res olution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    // be ina kari nadashte bashid

bool exit=0;
while(!exit){

switch(startMenu(renderer,L,W)){
case 0://play button pressed
      play(renderer);
      break;
case 1://info button pressed
      tutorial(renderer);
      break;
case 2://setting button pressed
      setting(renderer);
      break;
case 3:
      exit=1;
      break;

}

}


                        SDL_DestroyWindow( m_window );
                        SDL_DestroyRenderer( renderer );
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
      SDL_Delay(100);
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
bool lc_textureDestroyed = 1,rc_textureDestroyed = 1;
//load kardan pas zamine
       SDL_Rect img_rect;
       SDL_Texture* m_img = NULL;
       m_img = IMG_LoadTexture(renderer,"a.png");
       img_rect.x = 0;        //pictures coordinates
       img_rect.y = 0;        //pictures coordinates
       img_rect.w = 1280;    //pictures size
       img_rect.h = 800;

       SDL_Rect tir_rect;
       SDL_Texture* tir = NULL;
       tir = IMG_LoadTexture(renderer,"tir.png");
       tir_rect.x = 0;        //pictures coordinates
       tir_rect.y = 0;        //pictures coordinates
       tir_rect.w = 1280;    //pictures size
       tir_rect.h = 800;

       SDL_Rect lc_rect;
       SDL_Texture* lc_texture = NULL;
       lc_texture = IMG_LoadTexture(renderer,"1norm.png");
       SDL_Rect rc_rect;
       SDL_Texture* rc_texture = NULL;
       rc_texture = IMG_LoadTexture(renderer,"1norml.png");
       lc_rect.w = 350 * lc_scale;
       lc_rect.h = 500 * lc_scale;
       rc_rect.w = 350 * lc_scale;
       rc_rect.h = 500 * lc_scale;

       SDL_Rect ball_rect;
       SDL_Texture* ball_texture = NULL;
       ball_texture = IMG_LoadTexture(renderer,"ball.png");
       ball_rect.w = ball_radius * 2;
       ball_rect.h = ball_radius * 2;
       SDL_Point ball_point={ball_radius,ball_radius};

       SDL_Rect left_foot_rect;
       SDL_Texture* left_foot_texture = NULL;
       left_foot_texture = IMG_LoadTexture(renderer,"1shoe.png");
       left_foot_rect.h=300*lc_scale;
       left_foot_rect.w=300*lc_scale;
//       SDL_Point left_foot_axle={0,30};
       SDL_Point left_foot_axle={10,20};

       SDL_Rect right_foot_rect;
       SDL_Texture* right_foot_texture = NULL;
       right_foot_texture = IMG_LoadTexture(renderer,"1shoel.png");
       right_foot_rect.h=300*lc_scale;
       right_foot_rect.w=300*lc_scale;
       SDL_Point right_foot_axle={350 * rc_scale/2,20};

//load kardan pas zamine


while(1){
picLoader(renderer,ball_x,ball_y,ball_radius,ball_radius,"ball.png");
//cordinatefinder();//comment this line in final version
SDL_RenderCopy(renderer, m_img, NULL, &img_rect);
movement(renderer);
collision(renderer);




//left player
      lc_rect.x = lc_x;        //left player coordinates
      lc_rect.y = lc_y;        //left player coordinates
      SDL_RenderCopy(renderer,lc_texture, NULL, &lc_rect);
//left player

//right palyer
      rc_rect.x = rc_x;        //left player coordinates
      rc_rect.y = rc_y;        //left player coordinates
      SDL_RenderCopy(renderer,rc_texture, NULL, &rc_rect);
//right player

//shooting
if(state[SDL_SCANCODE_E] && lc_shoot==-10){
     lc_shoot=8;
     }
if(lc_shoot>0){
      left_foot_rect.x=lhead_x-15;
      left_foot_rect.y=lhead_y+40;
      SDL_RenderCopyEx(renderer,left_foot_texture,NULL,&left_foot_rect,(-50 + (lc_shoot)*(lc_shoot)),&left_foot_axle,SDL_FLIP_NONE);
}

if(state[SDL_SCANCODE_SLASH] && rc_shoot==-10 ){
     rc_shoot=8;
     }
if(rc_shoot>0){
      right_foot_rect.x=rhead_x-220*rc_scale;
      right_foot_rect.y=rhead_y+120*rc_scale;
//      ellipse(renderer,right_foot_rect.x,right_foot_rect.y,10,10,100,103,2,1);
      SDL_RenderCopyEx(renderer,right_foot_texture,NULL,&right_foot_rect,50-(rc_shoot)*(rc_shoot)*2,&right_foot_axle,SDL_FLIP_NONE);
}
      shoot();

//shooting

//ball
      ball_rect.x=ball_x - ball_radius;
      ball_rect.y=ball_y - ball_radius;
      //ellipse(renderer,ball_x,ball_y,ball_radius,ball_radius,100,0,100,1);//replace with image
      SDL_RenderCopyEx(renderer, ball_texture,NULL,&ball_rect,t%20 * ball_dx,&ball_point,SDL_FLIP_NONE);
//ball


SDL_RenderCopy(renderer, tir, NULL, &tir_rect);

SDL_RenderPresent(renderer);

      if(state[SDL_SCANCODE_ESCAPE])
      switch(PauseMenu(renderer,m_img,1280,800,100)){
            case 0:
                  break;
            case 1:
                  resetvalues(0);
                  SDL_DestroyTexture(m_img);
                  SDL_DestroyTexture(tir);
                  return play(renderer);//restart
            case 2:
                  resetvalues(0);
                  SDL_DestroyTexture(m_img);
                  SDL_DestroyTexture(tir);
                  return ;//quit
             }
t++;
SDL_Delay(delay);
}

SDL_DestroyTexture(m_img);
SDL_DestroyTexture(tir);

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

const char* players[]= {"1norm.png", "2norm.png", "3norm.png"};
int lc_x=200 ,lc_y=300 ,rc_x=800 ,rc_y=300 ;
int rc_size=200 ,lc_size=200 ;
short numberofpics=sizeof(players)/sizeof(players[0]);
short int index_player_L=0;
short int index_player_R=0;
window_color(renderer,10,20,30);
SDL_Event *e = new SDL_Event();
while(1)
{
    if(SDL_PollEvent(e))
    {
        if(e->type==SDL_KEYDOWN)
        {
            switch(e->key.keysym.sym)
            {
                case SDLK_a:
                    if(index_player_L>=1)
                        index_player_L--;
                    else
                        index_player_L=numberofpics;
                    break;
                case SDLK_w:
                    break;
                case SDLK_s:
                    break;
                case SDLK_d:
                    if(index_player_L<numberofpics-1)
                        index_player_L++;
                    else
                        index_player_L=0;
                    break;
                case SDLK_LEFT:
                    if(index_player_R>=1)
                        index_player_R--;
                    else
                        index_player_R=numberofpics;
                    break;
                case SDLK_RIGHT:
                    if(index_player_R<numberofpics-1)
                        index_player_R++;
                    else
                        index_player_R=0;
                    break;
                case SDLK_UP:
                    break;
                case SDLK_DOWN:
                    break;
            }
        }
    }
    SDL_RenderPresent(renderer);
    picLoader(renderer,rc_x,rc_y,rc_size,rc_size,players[index_player_R]);
    picLoader(renderer,lc_x,lc_y,lc_size,lc_size,players[index_player_L]);
}
SDL_RenderPresent(renderer);


}

int PauseMenu(SDL_Renderer* m_renderer, SDL_Texture* m_texture, int W, int H, int ump_w)
{
    SDL_Texture* ump_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W, H);
    SDL_SetRenderTarget(m_renderer, ump_texture);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    //0 for resume, 1 for restart, 2 for quit
    int i = 0;
    //menu background
    short int vx[4] = { W / 2 + ump_w,W,W,W / 2 - ump_w }, vy[4] = { 0,0,H,H };
    filledPolygonColor(m_renderer, vx, vy, 4, 0xff330000);
    //score
    const char* ump_time = intToString(t/fps).c_str();
    textRGBA(m_renderer, W / 2 + ump_w + W / 10, 100, "Time:", 2, 40, 255, 255, 255, 255);
    textRGBA(m_renderer, W / 2 + ump_w + W / 6 + 80, 150, ump_time, 2, 40, 255, 255, 255, 255);
    SDL_SetRenderTarget(m_renderer, NULL);
    SDL_Event* e = new SDL_Event();
    bool ump_nbp = true; // avoid unnecessary rendering
    bool hover=false; //mouse is hovering over button

    // start which run once
    // first button
        btn_array[0][0] = ump_w + W / 2 + 10 - (640 * ump_w) / H;
        btn_array[0][1] = 320;
        btn_array[0][2] = W;
        btn_array[0][3] = 420;
    // second button
        btn_array[1][0] = ump_w + W / 2 + 10 - (840 * ump_w) / H;
        btn_array[1][1] = 420;
        btn_array[1][2] = W;
        btn_array[1][3] = 520;
    // third button
        btn_array[2][0] = ump_w + W / 2 + 10 - (1040 * ump_w) / H;
        btn_array[2][1] = 520;
        btn_array[2][2] = W;
        btn_array[2][3] = 620;
    int mx=0, my=0;
    while (1)
    {
        e->type = NULL;
        SDL_PollEvent(e);
        if (e->type==SDL_MOUSEMOTION)
        {
            mx = e-> motion.x;
            my = e-> motion.y;
        }
        else
        {
            mx=0;
            my=0;
        }


        //hover effect
        for (int j=0;j<3;j++)
        {
            if (btn_clicked(j,mx,my) && i!=j)
            {
                hover=true;
                i=j;
                ump_nbp=true;
                break;
            }
        }

        if (e->type==SDL_MOUSEBUTTONDOWN && hover)
        {
            SDL_DestroyTexture(ump_texture);
            return i;
        }

        else if (e->type == SDL_KEYDOWN )
        {
            if (e->key.keysym.sym == SDLK_RETURN)
            {
                SDL_DestroyTexture(ump_texture);
                return i;
            }
            else if (e->key.keysym.sym == SDLK_DOWN)
            {
                ump_nbp = true;
                i++;
                i %= 3;
            }
            else if (e->key.keysym.sym == SDLK_UP)
            {
                ump_nbp = true;
                i--;
                i = (3 + i) % 3;
            }
        }
        if (ump_nbp)
        {
            ump_nbp = false;
            SDL_RenderCopy(m_renderer, ump_texture, NULL, NULL);
            //buttons
            if (i == 0)
            {
                short int ump_vx[4] = { ump_w + W / 2 + 10 - 2 * 320 * ump_w / H,W,W,ump_w + W / 2 + 10 - 2 * 420 * ump_w / H };
                short int ump_vy[4] = { 320,320,420,420 };
                filledPolygonRGBA(m_renderer, ump_vx, ump_vy, 4, 0, 150, 150, 255);
                gradient(m_renderer, ump_vx, ump_vy);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 350, "Resume", 2, 40, 0, 0, 0, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 450, "Restart", 2, 40, 255, 255, 255, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 550, "Quit", 2, 40, 255, 255, 255, 255);
            }
            else if (i == 1)
            {
                short int ump_vx[4] = { ump_w + W / 2 + 10 - 2 * 420 * ump_w / H,W,W,ump_w + W / 2 + 10 - 2 * 520 * ump_w / H };
                short int ump_vy[4] = { 420,420,520,520 };
                filledPolygonRGBA(m_renderer, ump_vx, ump_vy, 4, 0, 150, 150, 255);
                gradient(m_renderer, ump_vx, ump_vy);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 450, "Restart", 2, 40, 0, 0, 0, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 350, "Resume", 2, 40, 255, 255, 255, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 550, "Quit", 2, 40, 255, 255, 255, 255);
            }
            else
            {
                short int ump_vx[4] = { ump_w + W / 2 + 10 - 2 * 520 * ump_w / H,W,W,ump_w + W / 2 + 10 - 2 * 620 * ump_w / H };
                short int ump_vy[4] = { 520,520,620,620 };
                filledPolygonRGBA(m_renderer, ump_vx, ump_vy, 4, 0, 150, 150, 255);
                gradient(m_renderer, ump_vx, ump_vy);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 550, "Quit", 2, 40, 0, 0, 0, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 350, "Resume", 2, 40, 255, 255, 255, 255);
                textRGBA(m_renderer, W / 2 + ump_w + 80, 450, "Restart", 2, 40, 255, 255, 255, 255);
            }
            SDL_RenderPresent(m_renderer);

        }

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
void draw(SDL_Renderer* renderer){
//picLoader(renderer,lc_x,lc_y,350*lc_scale,500*lc_scale,"1norm.png");
//picLoader(renderer,rc_x,rc_y,350*rc_scale,500*rc_scale,"1norml.png");
}

void collision(SDL_Renderer* renderer){
//toop va divar ha
      if(ball_x>1280-ball_radius&&ball_dx>0) ball_dx*=-1;   //raste safhe
      if((ball_y>590-ball_radius)&&ball_dy>0) ball_dy*=-0.7;  //kafe zamin
      if(ball_y<0+ball_radius&&ball_dy<0) ball_dy*=-1;      //khorde be saghf
      if(ball_x<0+ball_radius&&ball_dx<0) ball_dx*=-1;      //chape safhe
      if((ball_y<390 && ball_y>360) && (ball_x<120 || ball_x>1110)) {ball_y-=ball_dy;ball_dy*=-1;}//tir ofoghi
//toop va divar ha

//kale va toop
      if((ball_x - lhead_x)*(ball_x - lhead_x) + (ball_y - lhead_y)*(ball_y - lhead_y) <=
         (ball_radius + 100*lc_scale) * (ball_radius + 100*lc_scale)
         && sign(lhead_x-ball_x) == sign(ball_dx) && lc_shoot<0){
            ball_dx *=-0.8;
            ball_dy *=-0.8;
            }

      if((ball_x - rhead_x)*(ball_x - rhead_x) + (ball_y - rhead_y)*(ball_y - rhead_y) <=
   (ball_radius + 100*rc_scale) * (ball_radius + 100*rc_scale)
   && sign(rhead_x-ball_x) == sign(ball_dx) && rc_shoot<0){
         if(ball_dx < 5) ball_dx += (rhead_x - ball_x) * 0.2;
      ball_dx *=-0.8;
      ball_dy *=-0.8;
      }
//kale va toop

//badan va toop
if(ball_x>rc_x  &&  ball_x <rc_x + 100*rc_scale && ball_y>rc_y && ball_y<rc_y + 500*rc_scale &&  ball_dx > 0)
      ball_dx=-2;//barkhorde toop be chape badan
if(ball_x<rc_x+350*rc_scale && ball_x > rc_x + 200*rc_scale && ball_y>rc_y && ball_y<rc_y + 500*rc_scale && ball_dx < 0)
      ball_dx=2;

if(ball_x>lc_x  &&  ball_x <lc_x + 100*lc_scale && ball_y>lc_y && ball_y<lc_y + 500*lc_scale &&  ball_dx > 0)
      ball_dx*=-1;
if(ball_x<lc_x+350*lc_scale && ball_x > lc_x + 200*lc_scale && ball_y>lc_y && ball_y<lc_y + 500*lc_scale && ball_dx < 0)
      ball_dx*=-1;
//badan va toop

}

void movement(SDL_Renderer* renderer){
//      if(sqrt(ball_dy)<0.01 && ball_y>590-ball_radius) {ball_ddy=0;ball_dy=0;ball_y=590-ball_radius;}
ball_x+=ball_dx;              //kinematic toop
ball_dx+=ball_ddx;            //kinematic toop
lhead_x=185*lc_scale+lc_x;    //kale donbale badan biad
lhead_y=165*lc_scale+lc_y;    //kale donbale badan biad

rhead_x=210*rc_scale+rc_x;    //kale donbale badan biad
rhead_y=165*rc_scale+rc_y;    //kale donbale badan biad

if(ball_y<zamin_y){
ball_y+=ball_dy;
ball_dy+=ball_ddy;
}

if(lc_y<zamin_y - 500*lc_scale|| Ljump){
lc_y+=lc_dy;
lc_dy+=lc_ddy;
Ljump=0;
}

if(rc_y<zamin_y - 500*rc_scale|| Rjump){
rc_y+=rc_dy;
rc_dy+=rc_ddy;
Rjump=0;
}
      SDL_PumpEvents();
      if (state[SDL_SCANCODE_A] && lc_x>30) {
          lc_x-=6;
      }
      if (state[SDL_SCANCODE_D]&&lc_x<1120 ) {
          lc_x+=6;
      }
           if (state[SDL_SCANCODE_W] && lc_y>=zamin_y - 500*lc_scale) {
          Ljump=1;
          lc_dy=-15;
      }

      if (state[SDL_SCANCODE_LEFT] && rc_x>30) {
          rc_x-=6;
      }
      if (state[SDL_SCANCODE_RIGHT]&&rc_x<1120) {
          rc_x+=6;
      }
           if (state[SDL_SCANCODE_UP] && rc_y>=zamin_y - 500*rc_scale) {
          Rjump=1;
          rc_dy=-15;
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

string intToString(int a){
    int m = 1;
    string ret = "";
    while(a != 0){
        ret = char((a % 10) + '0') + ret;
        a /= 10;
    }
    return ret;
}
void gradient(SDL_Renderer* m_renderer, short int vx[], short int vy[])
{
    int n = 40;
    for (int i = 0; i < n; i++)
    {
        aalineRGBA(m_renderer, vx[0] + i * (vx[0] - vx[3]) / (vy[0] - vy[3]), vy[0] + i, vx[1] - i, vy[1] + i, 255, 255, 255, 255 * (n - i) / n);
        aalineRGBA(m_renderer, vx[1] - i, vy[1] + i, vx[2] - i, vy[2] - i, 255, 255, 255, 255 * (n - i) / n);
        aalineRGBA(m_renderer, vx[2] - i, vy[2] - i, vx[3] - i * (vx[0] - vx[3]) / (vy[0] - vy[3]), vy[3] - i, 255, 255, 255, 255 * (n - i) / n);
        //        aalineRGBA(m_renderer,vx[0]+i,vy[0]+i,vx[3]+i,vy[3]-i,255,255,255,255*(n-i)/n);
    }
}

void resetvalues(bool fullrestart){
t=0;
ball_x=200,ball_y=40,ball_radius=30,ball_dx=2,ball_ddx=0,ball_dy=50,ball_ddy=0.5, //toop
lc_dy=0,lc_ddy=0.5,lc_scale=0.3,lc_x=100,lc_y=zamin_y-500*lc_scale,lhead_x=185*lc_scale+lc_x,lhead_y=165*lc_scale+lc_y;
}
bool sign(int a){
if (a>=0) return 1;
if (a<0)  return 0;
}



void shoot(){

//     cout<<square(ball_x-lc_foot_x(lhead_x,lc_scale))+ square(ball_y-lc_foot_y(lhead_y,lc_scale))<<endl;
if(lc_shoot>-10) {
     lc_shoot--;
      if((lc_foot_x(lhead_x,lc_scale) - ball_x)*(lc_foot_x(lhead_x,lc_scale) - ball_x) + (lc_foot_y(lhead_y,lc_scale) - ball_y) * (lc_foot_y(lhead_y,lc_scale) - ball_y)
         <=  ((shoot_radius + ball_radius) * lc_scale)*((shoot_radius+ball_radius) * lc_scale)) {

               switch(checkshoot(ball_y-lc_foot_y(lhead_y,lc_scale)/ball_x-lc_foot_x(lhead_x,lc_scale))){
                  case DEGREE15 :
                        ball_dx = 15;
                        ball_dy = -3.25;
                        break;

                  case DEGREE45 :
                         ball_dx = 15;
                         ball_dy = -15;
                         break;
                  case DEGREENEGATIVE15 :
                         ball_dx=15;
                         ball_dy=-3.75;

               }

     }
}


if(rc_shoot>-10) {
      rc_shoot--;
       if((rc_foot_x(rhead_x,rc_scale) - ball_x)*(rc_foot_x(rhead_x,rc_scale) - ball_x) + (rc_foot_y(rhead_y,rc_scale) - ball_y) * (rc_foot_y(rhead_y,rc_scale) - ball_y)
         <=  ((shoot_radius + ball_radius) * rc_scale)*((shoot_radius+ball_radius) * rc_scale)) {
                           switch(-checkshoot(ball_y-rc_foot_y(rhead_y,rc_scale)/ball_x-rc_foot_x(rhead_x,rc_scale))){
                  case DEGREE15 :
                        ball_dx = -15;
                        ball_dy = -3.25;
                        break;

                  case DEGREE45 :
                         ball_dx = -15;
                         ball_dy = -15;
                         break;
                  case DEGREENEGATIVE15 :
                         ball_dx=-15;
                         ball_dy=-3.75;

               }
           }
}


//      ellipse(renderer,rc_foot_x(rhead_x,rc_scale),rc_foot_y(rhead_y,rc_scale),shoot_radius*lc_scale,shoot_radius*lc_scale,10,231,10,0);
//      ellipse(renderer,lc_foot_x(lhead_x,lc_scale),lc_foot_y(lhead_y,lc_scale),shoot_radius*rc_scale,shoot_radius*rc_scale,10,231,10,0);

}

short checkshoot(float tangent){

if(tangent<0.15 && tangent>0) return DEGREE15;
if(tangent>0.15 ) return DEGREE45;
if(tangent<0 && tangent>-60) return DEGREENEGATIVE15;

}
