#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include <fstream>

#define rc_foot_y(rhead_y,rc_scale) rhead_y + 300 * rc_scale
#define lc_foot_y(lhead_y,lc_scale) lhead_y + 300 * lc_scale
#define rc_foot_x(rhead_x,rc_scale) rhead_x - 80 * rc_scale
#define lc_foot_x(lhead_x,lc_scale) lhead_x + 80 * lc_scale
#define shoot_radius 190
#define DEGREE15 1
#define DEGREE45 2
#define DEGREENEGATIVE15 3
#define power_cooldown 500
#define square(x) x*x
#define numberOfPowers 3
#define fps 50
#define gametime 120

enum powers{
PUNCH,
KICK_FIRE_BALL,
INVISIBLE_BALL
};

using namespace std;
//action functions
void window_color(SDL_Renderer *Renderer, int R, int G, int B);
void rect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian);
void testellipse(int x,int y,int radius);
void my_line(SDL_Renderer *Renderer, int x_1, int y_1, int L,double theta,int widht, int R, int G, int B );
void picLoader (SDL_Renderer* renderer,int x,int y,int width,int height,const char* address); //used for loading pictures
bool btn_clicked(int index, int mouse_x, int mouse_y);//checks if button is clicked by mouse
void collision(SDL_Renderer* renderer);//checks if 2 objects intersect
void movement(SDL_Renderer* renderer);
void cordinatefinder(); //used for debugging(prints mouse coordinates)
void resetvalues(bool fullrestart); //restarts variables when restart button is clicked or a goal is scored
string intToString(int a);
void gradient(SDL_Renderer* m_renderer, short int vx[], short int vy[]);//used in pause menu
void shoot(); // this function is called when shoot button is pressed
short checkshoot(float tangent);// checks angle of shooting
void print_one_digit(int X,int Y,int R);
void print_two_digits(int x,int y,int r);
void loadgame();
void goal(bool left); // if left = 1 means that left player scored a goal and if 0 right player...
float my_abs(float num);
void nameinputs();

//stage functions
short startMenu(SDL_Renderer* renderer,int page_width,int page_height);
void play(SDL_Renderer* renderer);
void tutorial(SDL_Renderer* renderer);
void setting(SDL_Renderer* renderer);
int PauseMenu(SDL_Renderer* m_renderer, SDL_Texture* m_texture, int W, int H, int ump_w);
bool endmenu();

//global variables
const int nbtn = 50,zamin_y=590;
int delay=1000/fps;

int rc_shoot=-20,lc_shoot=-20;
long long int t=0;      // time(increases when every frame is shown
int btn_array[nbtn][4]; // X0, Y0, X1, Y1
bool Ljump;
bool Rjump;

bool music_flag=true;

const char* ume_picAddress [] = {"info1.jpg","info2.jpg","info3.jpg","info4.jpg","info5.jpg"};//address
const char* players_rc[]= {"1rc.png", "2rc.png", "3rc.png"}; //address
const char* players_lc[]= {"1lc.png", "2lc.png", "3lc.png"}; //address
const char* shoes_rc[]= {"1shoe.png", "3shoe.png", "2shoe.png"}; //address
const char* shoes_lc[]= {"1shoel.png", "3shoel.png", "2shoel.png"}; //address

const char* fields[]= {"a.png", "b.png", "c.png", "d.png"};  //address
const char* balls[]= {"ball1.png", "ball2.png"};             //address
const char* musics[]= {"tsubasa.mp3","euro2008.mp3","champions.mp3"}; //address

//Aimation_timer anime_timer;
SDL_Event *event = new SDL_Event();
SDL_Renderer* renderer;
const Uint8 *state = SDL_GetKeyboardState(NULL);

string rc_name=" ",lc_name=" ";

float ball_x=640,ball_y=120,ball_radius=30,ball_dx=0,ball_ddx=0,ball_dy=-8,ball_ddy=0.5, //toop
      lc_dy=0,lc_ddy=1.6,lc_scale=0.3,lc_x=165,lc_y=zamin_y-500*lc_scale,         //character chap
      lhead_x=185*lc_scale+lc_x,lhead_y=165*lc_scale+lc_y                                             //character chap
      ,rc_dy=0,rc_ddy=1.6,rc_scale=0.3,rc_x=1000,rc_y=zamin_y-500*rc_scale,//character rast
      rhead_x=210*rc_scale+rc_x,rhead_y=165*rc_scale+rc_y;                                       //character rast

float lc_power_timer = 0,rc_power_timer = 0 , lc_punching_timer = 0     //timers
,rc_punching_timer = 0 , lc_freezing_timer = 0 , rc_freezing_timer = 0     //timers
,invisible_ball_timer = 0 ,lc_kick_fire_timer = 0 , rc_kick_fire_timer = 0
,lhead_timer=0,rhead_timer=0,lbody_timer = 0,rbody_timer=0,
lshootsound_timer=0,rshootsound_timer=0;
; // timers
int rc_index=0 , lc_index=0 , field_index=0 , ball_index=0 , lc_power_index = KICK_FIRE_BALL ,rc_power_index=PUNCH;//power ha
int rc_goals = 0 ,lc_goals =0;

int background_music_index=0;

SDL_Rect punch_rect;
SDL_Texture* punch_img = NULL;
SDL_Rect rpunch_rect;
SDL_Texture* rpunch_img = NULL;
SDL_Rect qmark_rect;
SDL_Texture* qmark_img = NULL;
SDL_Rect img_rect;
SDL_Texture* m_img = NULL;
SDL_Rect ltr_fire_rect;
SDL_Texture* ltr_fire_img = NULL;
SDL_Rect rtl_fire_rect;
SDL_Texture* rtl_fire_img = NULL;
SDL_Rect ball_rect;
SDL_Texture* ball_texture = NULL;
SDL_Rect tir_rect;
SDL_Texture* tir = NULL;
SDL_Rect lc_rect;
SDL_Texture* lc_texture = NULL;
SDL_Rect rc_rect;
SDL_Texture* rc_texture = NULL;
SDL_Rect left_foot_rect;
SDL_Texture* left_foot_texture = NULL;
SDL_Rect right_foot_rect;
SDL_Texture* right_foot_texture = NULL;
SDL_Point left_foot_axle={10,20};
SDL_Point right_foot_axle={350 * rc_scale/2,20};
SDL_Point ball_point={ball_radius,ball_radius};

int main( int argc, char * argv[] )
{

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,8,2046);

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

short startMenu(SDL_Renderer* renderer,int page_width,int page_height){
      nameinputs(); //gereftan nam ha

      //music_theme
      Mix_Music *backgroundSound = Mix_LoadMUS(musics[background_music_index]);
      if(music_flag){
      Mix_PlayMusic(backgroundSound,-1);
      music_flag=false;}

      int stng_x=880,stng_y=280,stng_size=150, //setting btn
      stbtn_x=870,stbtn_y=70,stbtn_size=170, //start btn
      info_x=876,info_y=470,info_size=150,       //tutorial btn
      exitbtn_x=1180,exitbtn_y=0,exitbtn_size=90, //exit btn
      mouse_x,mouse_y;

      bool changeMade=1;

      btn_array[10][0]=stbtn_x;btn_array[10][1]=stbtn_y;   //start button
      btn_array[10][2]=stbtn_x+stbtn_size;btn_array[10][3]=stbtn_y+stbtn_size;
      btn_array[11][0]=stng_x,btn_array[11][1]=stng_y;         //setting button
      btn_array[11][2]=stng_x+stng_size;btn_array[11][3]=stng_y+stng_size;
      btn_array[14][0]=info_x;btn_array[14][1]=info_y; //guide button
      btn_array[14][2]=info_x+info_size;btn_array[14][3]=info_y+info_size;
      btn_array[15][0]=exitbtn_x;btn_array[15][1]=exitbtn_y; //exit button
      btn_array[15][2]=exitbtn_x+exitbtn_size;btn_array[15][3]=exitbtn_y+exitbtn_size;
      while(1){
           SDL_PollEvent(event);
           if(SDL_GetMouseState(&mouse_x,&mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)){ // left click
            if(btn_clicked(10,mouse_x,mouse_y)) return 0; //start got clicked
            if(btn_clicked(11,mouse_x,mouse_y)) return 2; //setting got clicked
            if(btn_clicked(14,mouse_x,mouse_y)) return 1; //guide clicked
             if(btn_clicked(15,mouse_x,mouse_y))  exit(0);   //exit clicked
              }

      if(changeMade){
            picLoader(renderer,0,0,page_width,page_height,"background.png");
            picLoader(renderer, info_x, info_y, info_size+10, info_size, "manual.png");            //guide
            picLoader(renderer, stng_x, stng_y, stng_size, stng_size, "settings (1).png");      //game icon
            picLoader(renderer, stbtn_x, stbtn_y, stbtn_size, stbtn_size, "startbtn.png");      // start button
            picLoader(renderer, exitbtn_x, exitbtn_y, exitbtn_size+10, exitbtn_size, "exit.png");            //exit

            SDL_RenderPresent(renderer);
            changeMade=0;
      }
            SDL_Delay(delay);
      }

}

void play(SDL_Renderer* renderer){
loadgame();// loading images

Mix_Chunk *whistle = Mix_LoadWAV("wistle.wav");
Mix_PlayChannel(-1,whistle,0);

        //crowd_sound
        Mix_Music *crowd = Mix_LoadMUS("stadiumcrowd.mp3");
        Mix_PlayMusic(crowd,-1);

while(lc_goals < 5  && rc_goals <5  && t/fps < gametime){
//      cordinatefinder();//comment this line in final version
      SDL_RenderCopy(renderer, m_img, NULL, &img_rect); //background
      movement(renderer);
      shoot();
      collision(renderer);


      if(rc_power_timer < power_cooldown) rc_power_timer+=1;
      if(lc_power_timer < power_cooldown) lc_power_timer+=1;
      if(lshootsound_timer > 0) lshootsound_timer--;
      if(rshootsound_timer > 0) rshootsound_timer--;
      boxRGBA(renderer,510,0,160 + 350 * (lc_power_timer) / power_cooldown,60,0,0,0,120);//navare ghodrat
      boxRGBA(renderer,767,0,1118 - 350 * (rc_power_timer) / power_cooldown,60,0,0,0,120);//navare ghodrat
      print_two_digits(605,10,t/fps);//timer balaye safhe
      print_one_digit(530,10,lc_goals);//tedad goal chapi
      print_one_digit(708,10,rc_goals);//tedad goal rasti

      if(rc_freezing_timer > 0) //right player is freezed
      {
            qmark_rect.x = rc_x + 50 * rc_scale;
            qmark_rect.y = rc_y - 220 * rc_scale;
            qmark_rect.w = 300 * (rc_scale );
            qmark_rect.h = 300 * (rc_scale );
            SDL_RenderCopy(renderer,qmark_img,NULL,&qmark_rect);
            rc_freezing_timer --;
      }

       if(lc_freezing_timer > 0) //left player is freezed
      {
            qmark_rect.x = lc_x + 50 * rc_scale;
            qmark_rect.y = lc_y - 220 * rc_scale;
            qmark_rect.w = 300 * (lc_scale );
            qmark_rect.h = 300 * (lc_scale );
            SDL_RenderCopy(renderer,qmark_img,NULL,&qmark_rect);
            lc_freezing_timer --;
      }

      if(lc_punching_timer > 0) { //animation mosht zadane chapi
            punch_rect.x=lc_x + 200 * lc_scale;
            punch_rect.y=lc_y + 150 * lc_scale;
            punch_rect.h=300 * lc_scale;
            punch_rect.w=500 * square(lc_scale - lc_punching_timer);
            SDL_RenderCopy(renderer,punch_img,NULL,&punch_rect);
            lc_punching_timer--;
      }

       if(rc_punching_timer > 0) { //animation mosht zadane rasti
            rpunch_rect.x=rc_x -  250 * rc_scale;
            rpunch_rect.y=rc_y + 150 * rc_scale;
            rpunch_rect.h=300 * rc_scale;
            rpunch_rect.w=500 * square(rc_scale + rc_punching_timer);
            SDL_RenderCopy(renderer,rpunch_img,NULL,&rpunch_rect);
            rc_punching_timer--;
      }

      if(state[SDL_SCANCODE_Q] && lc_power_timer == power_cooldown && lc_freezing_timer == 0){
                  //power chapi
            switch(lc_power_index){
                  case PUNCH:
                        lc_power_timer = 0;
                        lc_punching_timer = 8;
                        if(rc_x - lc_x < 350 * lc_scale + 480 * square(lc_scale))
                              rc_freezing_timer = 150; // rasti mosht khord
                        break;

                  case INVISIBLE_BALL:
                        lc_power_timer = 0;
                        if ((lc_foot_x(lhead_x,lc_scale) - ball_x)*(lc_foot_x(lhead_x,lc_scale) - ball_x) + (lc_foot_y(lhead_y,lc_scale) - ball_y) * (lc_foot_y(lhead_y,lc_scale) - ball_y)
                        <=  ((shoot_radius + ball_radius) * lc_scale)*((shoot_radius+ball_radius) * lc_scale)){
                        lc_shoot = 8;
                        invisible_ball_timer = fps * 2;
                        }
                        break;
                  case KICK_FIRE_BALL:
                        if ((lc_foot_x(lhead_x,lc_scale) - ball_x)*(lc_foot_x(lhead_x,lc_scale) - ball_x) + (lc_foot_y(lhead_y,lc_scale) - ball_y) * (lc_foot_y(lhead_y,lc_scale) - ball_y)
                        <= 2 * ((shoot_radius + ball_radius) * lc_scale)*((shoot_radius+ball_radius) * lc_scale))
                        lc_kick_fire_timer =50;
                        lc_power_timer = 0;
                        break;
            }
      }
       if(state[SDL_SCANCODE_RSHIFT] && rc_power_timer == power_cooldown && rc_freezing_timer == 0){
                 //power chapi
            switch(rc_power_index){
                  case PUNCH:
                        rc_power_timer = 0;
                        rc_punching_timer = 8;
                        if(rc_x - lc_x < 350 * lc_scale + 480 * square(rc_scale))
                              lc_freezing_timer = 150; // rasti mosht khord
                        break;
                  case INVISIBLE_BALL:
                        rc_power_timer = 0;
                         if((rc_foot_x(rhead_x,rc_scale) - ball_x)*(rc_foot_x(rhead_x,rc_scale) - ball_x) + (rc_foot_y(rhead_y,rc_scale) - ball_y) * (rc_foot_y(rhead_y,rc_scale) - ball_y)
                        <=  ((shoot_radius + ball_radius) * rc_scale)*((shoot_radius+ball_radius) * rc_scale)){
                        invisible_ball_timer = fps * 2;
                        rc_shoot = 8;
                        }
                        break;
                  case KICK_FIRE_BALL:
                             if ((rc_foot_x(rhead_x,rc_scale) - ball_x)*(rc_foot_x(rhead_x,rc_scale) - ball_x) + (rc_foot_y(rhead_y,rc_scale) - ball_y) * (rc_foot_y(rhead_y,rc_scale) - ball_y)
                        <= 2 * ((shoot_radius + ball_radius) * rc_scale)*((shoot_radius+ball_radius) * rc_scale))
                        rc_kick_fire_timer =50;
                        rc_power_timer = 0;
                        break;
            }
            }


      //left player
            lc_rect.x = lc_x;        //left player coordinates
            lc_rect.y = lc_y;        //left player coordinates
            SDL_RenderCopy(renderer,lc_texture, NULL, &lc_rect);

      //right palyer
            rc_rect.x = rc_x;        //left player coordinates
            rc_rect.y = rc_y;        //left player coordinates
            SDL_RenderCopy(renderer,rc_texture, NULL, &rc_rect);

      //shooting
      if(state[SDL_SCANCODE_E] && lc_shoot==-20 && lc_freezing_timer ==0){
           lc_shoot=8;
           }

      if(lc_shoot>0){
            left_foot_rect.x=lhead_x-15;
            left_foot_rect.y=lhead_y+40;
            SDL_RenderCopyEx(renderer,left_foot_texture,NULL,&left_foot_rect,(-50 + (lc_shoot)*(lc_shoot)),&left_foot_axle,SDL_FLIP_NONE);
      }

      if(state[SDL_SCANCODE_SLASH] && rc_shoot==-20 && rc_freezing_timer == 0){
           rc_shoot=8;
           }
      if(rc_shoot>0){
            right_foot_rect.x=rhead_x-220*rc_scale;
            right_foot_rect.y=rhead_y+120*rc_scale;
            SDL_RenderCopyEx(renderer,right_foot_texture,NULL,&right_foot_rect,50-(rc_shoot)*(rc_shoot)*2,&right_foot_axle,SDL_FLIP_NONE);
}


//shooting


//ball

      if(invisible_ball_timer > 0){
         invisible_ball_timer--;
      }
      else if(lc_kick_fire_timer == 0 && rc_kick_fire_timer == 0){
            ball_rect.x=ball_x - ball_radius;
            ball_rect.y=ball_y - ball_radius;
            SDL_RenderCopyEx(renderer, ball_texture,NULL,&ball_rect, t%20 * ball_dx , &ball_point ,SDL_FLIP_NONE);
      }


SDL_RenderCopy(renderer, tir, NULL, &tir_rect);//aks tirak ha rooye akse asli

SDL_RenderPresent(renderer);//presente nahayi

      if(state[SDL_SCANCODE_ESCAPE]) //pause kardan
      switch(PauseMenu(renderer,m_img,1280,800,100)){
            case 0:
                  break;//resume kardan
            case 1:
                  resetvalues(1);
                  Mix_FreeMusic(crowd);
                  return play(renderer);//restart
            case 2:
                  resetvalues(1);
                  Mix_FreeMusic(crowd);
                  music_flag = true;
                  return ;//quit
             }
t++;
SDL_Delay(delay);
}
if (endmenu() == 0)  {
            Mix_FreeMusic(crowd);
            resetvalues(1);
            return play(renderer);//restart
}
else {
            Mix_FreeMusic(crowd);
            resetvalues(1);
            return;
}

}


bool endmenu(){
      int restartbtn_x = 700,restartbtn_y=400,restartbtn_size=100,
      exitbtn_x = 550,exitbtn_y=400,exitbtn_size=100,
      mouse_x,mouse_y;
      /////////////////////////////////////////////////////////////////////////// dokme ha
      btn_array[16][0] = restartbtn_x;
      btn_array[16][1] = restartbtn_y;//restart button
      btn_array[16][2] = restartbtn_x + restartbtn_size;
      btn_array[16][3] = restartbtn_y + restartbtn_size;
      btn_array[17][0] = exitbtn_x;//exit button
      btn_array[17][1] = exitbtn_y;
      btn_array[17][2] = exitbtn_x + exitbtn_size;
      btn_array[17][3] = exitbtn_y +exitbtn_size;
      ////////////////////////////////////////////////////////////////////////////dokme ha

      ///////////////////////////////////////////////////////////////////////////load aks
      bool changemade = 1;
      picLoader(renderer,0,0,1280,800,"end.png");
      picLoader(renderer,exitbtn_x,exitbtn_y,exitbtn_size,exitbtn_size,"quit.png");
      picLoader(renderer,restartbtn_x,restartbtn_y,restartbtn_size,restartbtn_size,"undo.png");
      if(lc_goals > rc_goals) textRGBA(renderer,455,150,"LEFT PLAYER WON",2,50,0,0,0,255);
      else if(rc_goals > lc_goals) textRGBA(renderer,455,150,"RIGHT PLAYER WON",2,50,0,0,0,255);
      else textRGBA(renderer,530,150,"DRAW",2,100,0,0,0,255);
      SDL_RenderPresent(renderer);
      ///////////////////////////////////////////////////////////////////////////load aks


      ///////////////////////////////////////////////////////////////////////////file
//      ofstream outputfile ("memory.txt");
//      outputfile <<"#"<<lc_name<<"@"<<intToString(lc_goals)<<"@";
//      outputfile <<"#"<<rc_name<<"@"<<intToString(lc_goals)<<"@";









      ///////////////////////////////////////////////////////////////////////////file

      while (1){
            SDL_PollEvent(event) ;
           if(SDL_GetMouseState(&mouse_x,&mouse_y)&SDL_BUTTON(SDL_BUTTON_LEFT)){
           if(btn_clicked(16,mouse_x,mouse_y) )
                return 0;
           if(btn_clicked(17,mouse_x,mouse_y) )
                music_flag=true;
                return 1;
                  }
      }

}


void tutorial(SDL_Renderer* renderer){
      short numberOfPics = sizeof(ume_picAddress)/sizeof(ume_picAddress[0]);// gereftan tedad aks haye safhe aval
      int rarrow_x=900,rarrow_y=530,rarrow_size=250, //right arrow
      lerrow_x=100,lerrow_y=530,lerrow_size=250,
      home_x=560,home_y=560,home_size=140,
      mouse_x,mouse_y;

      short index=0;
      bool changeMade=1,click=0;

      btn_array[12][0]=lerrow_x;btn_array[12][1]=lerrow_y; //left arrow
      btn_array[12][2]=lerrow_x+lerrow_size;btn_array[12][3]=lerrow_y+lerrow_size;
      btn_array[13][0]=rarrow_x;btn_array[13][1]=rarrow_y; //right arrow
      btn_array[13][2]=rarrow_x+rarrow_size;btn_array[13][3]=rarrow_y+rarrow_size;
      btn_array[15][0]=home_x;btn_array[15][1]=home_y; //home
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

void nameinputs(){

int i=0;
SDL_Event *event = new SDL_Event();
bool btnup=1;
bool isRc = 1;
const Uint8 *state = SDL_GetKeyboardState(NULL);
                  window_color(renderer,40,40,40);
                  textRGBA(renderer,100,600,"/press backspace to delete name/",2,50,50,100,100,200);
                  textRGBA(renderer,100,650,"/press space to change player/",2,50,50,100,100,200);
                  textRGBA(renderer,100,700,"/press esc to confirm/",2,50,50,100,100,200);
                  textRGBA(renderer,100,50,"RIGHT PLAYER NAME:",2,50,200,200,200,150+ 80 * isRc);
                  textRGBA(renderer,200,140,rc_name.c_str(),2,50,100,200,100,255);
                  textRGBA(renderer,100,300,"LEFT PLAYER NAME:",2,50,200,200,200,150 + 80 * abs(1- isRc));
                  textRGBA(renderer,200,350,lc_name.c_str(),2,50,250,100,100,255);
while(1){
      SDL_PumpEvents();

      for(i=0;i<=29 && btnup;i++){
                  if (state[i]==1) {
//          cout<<(char) (i +93 );

if(isRc) rc_name = rc_name + (char) (i +93 );
else lc_name = lc_name + (char) (i +93 );
          btnup=0;
      }
      }

if(state[SDL_SCANCODE_BACKSPACE])//pak kardan
      if(isRc) rc_name = " ";
      else        lc_name = " ";

if(state[SDL_SCANCODE_SPACE])//avaz kardan player
            if(isRc) isRc=0;
            else isRc=1;
if(state[SDL_SCANCODE_ESCAPE])//sabt
      return;


      if(SDL_PollEvent(event))
            if(event->type==SDL_KEYUP){
                  if(state[i]==0) btnup=1;
                  window_color(renderer,40,40,40);
                  textRGBA(renderer,100,600,"/press backspace to delete name/",2,50,50,100,100,200);
                  textRGBA(renderer,100,650,"/press space to change player/",2,50,50,100,100,200);
                   textRGBA(renderer,100,700,"/press esc to confirm/",2,50,50,100,100,200);
                  textRGBA(renderer,100,50,"RIGHT PLAYER NAME:",2,50,200,200,200,150+ 80 * isRc);
                  textRGBA(renderer,200,140,rc_name.c_str(),2,50,100,200,100,255);
                  textRGBA(renderer,100,300,"LEFT PLAYER NAME:",2,50,200,200,200,150 + 80 * abs(1- isRc));
                  textRGBA(renderer,200,350,lc_name.c_str(),2,50,250,100,100,255);
            }
                  SDL_RenderPresent(renderer);
                  }



}



void setting(SDL_Renderer* renderer){

short numberofplayers=sizeof(players_rc)/sizeof(players_rc[0]);
short numberofballs=sizeof(balls)/sizeof(balls[0]);
short numberoffields=sizeof(fields)/sizeof(fields[0]);

bool changemade = 1 ; // jelogiri az render ziad
int lc_x=380 ,lc_y=400 ,rc_x=690 ,rc_y=400 ;
int rc_size=200 ,lc_size=200 ;
int ballpic_x=585 ,ballpic_y=80 ,ballpic_width=100 ,ballpic_height=100;
int fieldpic_x=0 ,fieldpic_y=0 ,fieldpic_width=1280 ,fieldpic_height=800;
window_color(renderer,10,20,30);
SDL_Event *e = new SDL_Event();

while(1)
{
//      cout<<"r"<<rc_power_index<<endl<<"L"<<lc_power_index<<endl;

//      cout<<ball_index<<endl;
    if(SDL_PollEvent(e))
    {
        if(e->type==SDL_KEYDOWN)
        {
            switch(e->key.keysym.sym)
            {
                case SDLK_1: background_music_index=0;
                    music_flag=true;
                    break;
                case SDLK_2: background_music_index=1;
                    music_flag=true;
                    break;
                case SDLK_3: background_music_index=2;
                    music_flag=true;
                    break;
                case SDLK_a:
                    changemade = 1 ;
                    if(lc_index>=1)
                        lc_index--;
                    else
                        lc_index=numberofplayers-1;
                    break;
                case SDLK_w:
                    changemade = 1 ;
                    if(ball_index<numberofballs-1)
                        ball_index++;
                    else
                        ball_index=0;
                    break;
                case SDLK_s:
                    changemade = 1 ;
                    if(ball_index>=1)
                        ball_index--;
                    else
                        ball_index=numberofballs-1;
                    break;
                case SDLK_d:
                    changemade = 1 ;
                    if(lc_index<numberofplayers-1)
                        lc_index++;
                    else
                        lc_index=0;
                    break;
                case SDLK_LEFT:
                     changemade = 1 ;
                    if(rc_index>=1)
                        rc_index--;
                    else
                        rc_index=numberofplayers-1;
                    break;
                case SDLK_RIGHT:
                    changemade = 1 ;
                    if(rc_index<numberofplayers-1)
                        rc_index++;
                    else
                        rc_index=0;
                    break;
                case SDLK_UP:
                    changemade = 1 ;
                    if(field_index<numberoffields-1)
                        field_index++;
                    else
                        field_index=0;
                    break;
                case SDLK_DOWN:
                    changemade = 1 ;
                    if(field_index>=1)
                        field_index--;
                    else
                        field_index=numberoffields-1;
                    break;
                case SDLK_e:
                     changemade = 1 ;
                     if(lc_power_index < numberOfPowers - 1)
                        lc_power_index++;
                     else
                        lc_power_index = 0;
                     break;

                case SDLK_q:
                     changemade = 1 ;
                     if(lc_power_index>=1)
                        lc_power_index--;
                     else
                        lc_power_index=numberOfPowers-1;
                        break;

                case SDLK_RSHIFT:
                     changemade = 1 ;
                     if(rc_power_index < numberOfPowers - 1)
                          rc_power_index++;
                     else
                        rc_power_index = 0;
                     break;

                case SDLK_RCTRL:
                     changemade = 1 ;
                     if(rc_power_index>=1)
                        rc_power_index--;
                     else
                        rc_power_index=numberOfPowers-1;
                     break;
                case SDLK_ESCAPE:
                    return;
            }
        }
    }

//    window_color(renderer,10,20,30);
if(changemade){
    changemade = 0;
    picLoader(renderer,fieldpic_x,fieldpic_y,fieldpic_width,fieldpic_height,fields[field_index]);
    picLoader(renderer,rc_x,rc_y,rc_size,rc_size,players_rc[rc_index]);
    picLoader(renderer,lc_x,lc_y,lc_size,lc_size,players_lc[lc_index]);
    picLoader(renderer,ballpic_x,ballpic_y,ballpic_width,ballpic_height,balls[ball_index]);

    //ghodrat ha
     switch(lc_power_index){
     case INVISIBLE_BALL:
                  ellipse(renderer,480,290,60,60,0,0,0,1); break;
     case PUNCH:
                  picLoader(renderer,280,180,300,200,"punch.png");break;
     case KICK_FIRE_BALL:
                  picLoader(renderer,240,200,300,200,"ltrFireball.png");break;
     }
          switch(rc_power_index){
     case INVISIBLE_BALL:
                  ellipse(renderer,800,290,60,60,0,0,0,1);break;
     case PUNCH:
                  picLoader(renderer,740,180,300,200,"punch_right.png");break;
     case KICK_FIRE_BALL:
                  picLoader(renderer,740,200,300,200,"rtlFireball.png");break;

     }
    SDL_RenderPresent(renderer);
}
}
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


void collision(SDL_Renderer* renderer){
//toop va divar ha
      if(ball_x>1280-ball_radius&&ball_dx>0) ball_dx*=-0.9;   //raste safhe
      if((ball_y>590-ball_radius)&&ball_dy>0) ball_dy*=-0.7;   //kafe zamin
      if(ball_y<0+ball_radius&&ball_dy<0) ball_dy*=-1;      //khorde be saghf
      if(ball_x<0+ball_radius&&ball_dx<0) ball_dx*=-0.9;      //chape safhe
      if((ball_y<390 && ball_y>360) && (ball_x<120 || ball_x>1110)){ball_y-=ball_dy;ball_dy*=-1;}//tir ofoghi

//goal shodan
if(ball_x > 1200 && ball_y > 400) goal(1);
if(ball_x < 80  && ball_y > 400)      goal(0);

if(lhead_timer > 0) lhead_timer--;
if(rhead_timer > 0) rhead_timer--;

//kale va toop
      if((ball_x - lhead_x)*(ball_x - lhead_x) + (ball_y - lhead_y)*(ball_y - lhead_y) <=
         (ball_radius + 100*lc_scale) * (ball_radius + 100*lc_scale)
         && ball_dx!=15 && lhead_timer == 0)
         {
               //toop khord be kale chapi
                     if(ball_x > lhead_x && ball_dx >= 0){ //raste kale chapi va harekat toop be rast
                              if(my_abs(ball_dx) < 0.2){
                                    ball_dx = 3;
                                    ball_dy=-my_abs(ball_dy);
                              }
                              else{
                              ball_dx *= 1;
                              ball_dy *= -1;
                              }
                     }
                     else if(ball_x > lhead_x && ball_dx < 0){//raste kale chapi va harekat toop be chap
                               ball_dx *= -0.8;
                               ball_dy *= -0.8;
                     }
                     else if (ball_x > lhead_x && ball_dx < 0){//raste kale chapi va harekat toop be chap
                               ball_dx *=-0.8;
                               ball_dy *=-0.8;
                        }
                     else{ //chape kale chapi va harekat toop be chap
                                ball_dx *=0.8;
                               ball_dy *=-0.8;
                     }
            lhead_timer =15;
        }

      if((ball_x - rhead_x)*(ball_x - rhead_x) + (ball_y - rhead_y)*(ball_y - rhead_y) <=
   (ball_radius + 100*rc_scale) * (ball_radius + 100*rc_scale)
   && ball_dx != -15 && rhead_timer == 0){
        //toop khord be kale rasti
                      if(ball_x > rhead_x && ball_dx > 0){ //raste kale rasti va harekat toop be rast
                               ball_dx *=0.8;
                               ball_dy *=-0.8;
                     }
                     else if(ball_x > rhead_x && ball_dx < 0){//raste kale rasti  va harekat toop be chap
                               ball_dx *= 0.8;
                               ball_dy *=-0.8;
                     }
                     else if (ball_x < rhead_x && ball_dx <=0){//chape kale rasti  va harekat toop be chap
                                  if(my_abs(ball_dx) < 0.2){
                                    ball_dx = -3;
                                    ball_dy=-my_abs(ball_dy);
                                  }
                           else{
                                     ball_dx *= 1;
                                     ball_dy *= -1;
                           }
                     }
                     else{ //chape kale rasti  va harekat toop be rast
                              ball_dx *= -0.8;
                              ball_dy *= -0.8;
                     }
      rhead_timer = 15;
      }

//badan va toop

if(lbody_timer > 0) lbody_timer--;
if(rbody_timer > 0) rbody_timer--;
//cout<<ball_dx<<endl;
if(ball_x < rhead_x && ball_x > rhead_x - 350/2 * rc_scale && ball_y < rc_y + 500*rc_scale && ball_y>rc_y + 250*rc_scale && ball_dx != -15  && rbody_timer ==0 ){
//chape badane rasti
            if(ball_dx > 0)
                  ball_dx *= -0.3;
           else  if(my_abs(ball_dx)<5)
                        ball_dx = -6;

                  rbody_timer=5;
}
if(ball_x > rhead_x && ball_x < rhead_x - 350/2 * rc_scale && ball_y < rc_y + 500*rc_scale && ball_y>rc_y && ball_dx != -15 && rbody_timer ==0 ){
//raste badane rasti
if(ball_dx > 0){
          ball_dx *= -0.3;
      rbody_timer=5;

}

}


if(ball_x < lhead_x && ball_x > lhead_x - 350/2 * lc_scale && ball_y < lc_y + 500*lc_scale && ball_y>lc_y && ball_dx != 15 &&lbody_timer ==0 ){
//chape badane chapi
      if(ball_dx > 0)
            ball_dx *= -0.3;

            lbody_timer = 5;
}
if(ball_x > lhead_x && ball_x < lhead_x + 350/2 * lc_scale && ball_y < lc_y + 500*lc_scale && ball_y>lc_y + 250*lc_scale && ball_dx != 15 && lbody_timer ==0){
//raste badane chapi
      if(ball_dx < 0)
            ball_dx *=-0.3;
      else if(my_abs(ball_dx)<5 )
            ball_dx = 6;

      lbody_timer = 5;

}

//badan va toop

}

void movement(SDL_Renderer* renderer){

if(lc_kick_fire_timer > 0) {
if(ball_x < 1200) ball_x +=10;
if(rc_x < ball_x  && ball_x - rhead_x < 100 && ball_y > rhead_y-100 * rc_scale){
      rc_x = ball_x;
      rc_freezing_timer=fps*2;
}
ltr_fire_rect.x = ball_x - ball_radius -100;
ltr_fire_rect.y = ball_y - ball_radius -25;
ltr_fire_rect.w = ball_rect.w + 100;
ltr_fire_rect.h = ball_rect.h +60;
lc_kick_fire_timer--;
SDL_RenderCopy(renderer,ltr_fire_img,NULL,&ltr_fire_rect);
}

else if(rc_kick_fire_timer >0){
if(ball_x > 80)ball_x -=10;
if(lc_x + 350*lc_scale > ball_x  && lhead_x - ball_x < 100 && ball_y > lhead_y-100 * lc_scale){
      lc_x = ball_x - 350 * lc_scale;
      lc_freezing_timer = fps *2;
}
rtl_fire_rect.x = ball_x - ball_radius ;
rtl_fire_rect.y = ball_y - ball_radius-25;
rtl_fire_rect.w = ball_rect.w + 100;
rtl_fire_rect.h = ball_rect.h +60;
rc_kick_fire_timer--;

SDL_RenderCopy(renderer,rtl_fire_img,NULL,&rtl_fire_rect);

}

else {
      ball_x+=ball_dx;              //harekat toop
      ball_dx+=ball_ddx;            //harekat toop

      if(ball_y<zamin_y){
      ball_y+=ball_dy;
      ball_dy+=ball_ddy;
      }
}


if(lc_y < zamin_y - 500*lc_scale || Ljump){
lc_y+=lc_dy;
lc_dy+=lc_ddy;
Ljump=0;
}

if(rc_y<zamin_y - 500*rc_scale || Rjump){
rc_y+=rc_dy;
rc_dy+=rc_ddy;
Rjump=0;
}

lhead_x=185*lc_scale+lc_x;      //kale donbale badan biad
lhead_y=165*lc_scale+lc_y;      //kale donbale badan biad
rhead_x=210*rc_scale+rc_x;    //kale donbale badan biad
rhead_y=165*rc_scale+rc_y;    //kale donbale badan biad

      SDL_PumpEvents();
      if(lc_freezing_timer ==0){
            if (state[SDL_SCANCODE_A] && lhead_x>40) {
                lc_x-=6;
            }
            if (state[SDL_SCANCODE_D]&&lc_x<1120 && lc_x+300*lc_scale < rc_x) {
                lc_x+=6;
            }
                 if (state[SDL_SCANCODE_W]  &&  lc_y>=zamin_y - 500*lc_scale) {
                Ljump=1;
                lc_dy=-15;
            }
      }

      if(rc_freezing_timer == 0){
            if (state[SDL_SCANCODE_LEFT] && rc_x>30 && lc_x+300*lc_scale < rc_x) {
                rc_x-=6;
            }
            if (state[SDL_SCANCODE_RIGHT]&&rhead_x<1250) {
                rc_x+=6;
            }
                 if (state[SDL_SCANCODE_UP] && rc_y>=zamin_y - 500*rc_scale) {
                Rjump=1;
                rc_dy=-15;
            }
      }


}


void ellipse(SDL_Renderer *Renderer, int x, int y, int Radius1, int Radius2, int R, int G, int B, int fill_boolian)

{
    if(fill_boolian==1)
        filledEllipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,120);
    if(fill_boolian==0)
        ellipseRGBA(Renderer,x,y,Radius1,Radius2,R,G,B,120);
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



void shoot() {

//     cout<<square(ball_x-lc_foot_x(lhead_x,lc_scale))+ square(ball_y-lc_foot_y(lhead_y,lc_scale))<<endl;
if(lc_shoot>-20) {
     lc_shoot--;
      if((lc_foot_x(lhead_x,lc_scale) - ball_x)*(lc_foot_x(lhead_x,lc_scale) - ball_x) + (lc_foot_y(lhead_y,lc_scale) - ball_y) * (lc_foot_y(lhead_y,lc_scale) - ball_y)
         <=  ((shoot_radius + ball_radius) * lc_scale)*((shoot_radius+ball_radius) * lc_scale) ) {
            if( lc_shoot > 0 && lshootsound_timer == 0){
                 lshootsound_timer = 10;
                Mix_Chunk *ball_kick = Mix_LoadWAV("ballkick.wav");
                Mix_PlayChannel(-1,ball_kick,0);
            }
               switch(checkshoot(ball_y-lc_foot_y(lhead_y,lc_scale)/ball_x-lc_foot_x(lhead_x,lc_scale))){
                  case DEGREE15 :
                        ball_dx = 15;
                        ball_dy = -3.25;
                        break;

                  case DEGREE45 :
                         ball_dx = 15;
                         ball_dy = -13;
                         break;
                  case DEGREENEGATIVE15 :
                         ball_dx=15;
                         ball_dy=-3.75;

               }

     }
}


if(rc_shoot>-20) {
      rc_shoot--;
       if((rc_foot_x(rhead_x,rc_scale) - ball_x)*(rc_foot_x(rhead_x,rc_scale) - ball_x) + (rc_foot_y(rhead_y,rc_scale) - ball_y) * (rc_foot_y(rhead_y,rc_scale) - ball_y)
         <=  ((shoot_radius + ball_radius) * rc_scale)*((shoot_radius+ball_radius) * rc_scale) ) {
                if( rc_shoot >0 && rshootsound_timer == 0){
                rshootsound_timer = 10;
                Mix_Chunk *ball_kick = Mix_LoadWAV("ballkick.wav");
                Mix_PlayChannel(-1,ball_kick,0);
                }
                           switch(-checkshoot(ball_y-rc_foot_y(rhead_y,rc_scale)/ball_x-rc_foot_x(rhead_x,rc_scale))){
                  case DEGREE15 :
                        ball_dx = -15;
                        ball_dy = -3.25;
                        break;

                  case DEGREE45 :
                         ball_dx = -15;
                         ball_dy = -13;
                         break;
                  case DEGREENEGATIVE15 :
                         ball_dx=-15;
                         ball_dy=-3.75;

               }
           }
}
}

short checkshoot(float tangent){
//                Mix_Chunk *ball_kick = Mix_LoadWAV("ballkick.wav");
//                Mix_PlayChannel(-1,ball_kick,0);
if(tangent<0.15 && tangent>0) return DEGREE15;
if(tangent>=0.15 ) return DEGREE45;
if(tangent<0 && tangent>-60) return DEGREENEGATIVE15;
}

void testellipse(int x,int y,int radius){
ellipse(renderer,x,y,radius,radius,120,123,123,1);
}
void print_one_digit(int X,int Y,int R){
    if (R==0) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+40);
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+40);
    }
    if (R==1) {
        SDL_RenderDrawLine(renderer, X+17, Y+40, X+17, Y+10);
    }
    if (R==2) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+5, Y+40);
    }
    if (R==3) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+40);
    }
    if (R==4) {
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+20);
        SDL_RenderDrawLine(renderer, X+5, Y+20, X+30, Y+20);
    }
    if (R==5) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+25);
        SDL_RenderDrawLine(renderer, X+30, Y+25, X+30, Y+40);
    }
    if (R==6) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+30, Y+25, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+40);
    }
    if (R==7) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+5, Y+40);
    }
    if (R==8) {
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
        SDL_RenderDrawLine(renderer, X+5, Y+25, X+30, Y+25);
        SDL_RenderDrawLine(renderer, X+5, Y+40, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+40);
    }
    if (R==9) {
        SDL_RenderDrawLine(renderer, X+30, Y+10, X+30, Y+40);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+5, Y+20);
        SDL_RenderDrawLine(renderer, X+5, Y+20, X+30, Y+20);
        SDL_RenderDrawLine(renderer, X+5, Y+10, X+30, Y+10);
    }
}

void print_two_digits(int x,int y,int r){
print_one_digit(x+30, y , r%10);
print_one_digit(x , y , (r/10) % 10);
}

void loadgame(){
//load kardan aks ha
       m_img = IMG_LoadTexture(renderer,fields[field_index]);  // aks field
       img_rect.x = 0;        //pictures coordinates
       img_rect.y = 0;        //pictures coordinates
       img_rect.w = 1280;    //pictures size
       img_rect.h = 800;


       tir = IMG_LoadTexture(renderer,"tir.png"); //akse tirak ha
       tir_rect.x = 0;        //pictures coordinates
       tir_rect.y = 0;        //pictures coordinates
       tir_rect.w = 1280;    //pictures size
       tir_rect.h = 800;


       lc_texture = IMG_LoadTexture(renderer,players_lc[lc_index]); //akse character chapi
       lc_rect.w = 350 * lc_scale;
       lc_rect.h = 500 * lc_scale;

       rc_texture = IMG_LoadTexture(renderer,players_rc[rc_index]); //akse character rasti
       rc_rect.w = 350 * lc_scale;
       rc_rect.h = 500 * lc_scale;

       ball_texture = IMG_LoadTexture(renderer,balls[ball_index]);//akse toop
       ball_rect.w = ball_radius * 2;
       ball_rect.h = ball_radius * 2;


       left_foot_texture = IMG_LoadTexture(renderer,shoes_rc[lc_index]);//akse paye chapi(baraye animation shoot)
       left_foot_rect.h=300*lc_scale;
       left_foot_rect.w=300*lc_scale;

       right_foot_texture = IMG_LoadTexture(renderer,shoes_lc[rc_index]);//akse paye rasti(baraye animation shoot)
       right_foot_rect.h=300*lc_scale;
       right_foot_rect.w=300*lc_scale;

       punch_img  = IMG_LoadTexture(renderer,"punch.png");               //akse mosht
       rpunch_img = IMG_LoadTexture(renderer,"punch_right.png");//akse mosht
       qmark_img  = IMG_LoadTexture(renderer,"qmark.png");              //akse alamat saol
       rtl_fire_img= IMG_LoadTexture(renderer,"rtlFireball.png");       //akse toop atishi
       ltr_fire_img= IMG_LoadTexture(renderer,"ltrFireball.png");       //akse toop atishi

//load kardan aks ha
}

void resetvalues(bool fullrestart){
if(fullrestart) {
      lc_power_timer = 0;
      rc_power_timer = 0 ;
      rc_goals = 0 ;
      lc_goals =0  ;
      t=0;
}
rc_shoot=-20;
lc_shoot=-20;
lc_punching_timer = 0;
rc_punching_timer = 0 ;
lc_freezing_timer = 0 ;
rc_freezing_timer = 0;
invisible_ball_timer = 0 ;
lc_kick_fire_timer = 0;
rc_kick_fire_timer = 0;
ball_x=640;
ball_y=120;
ball_radius=30;
ball_dx=0;
ball_ddx=0;
ball_dy=-8;
ball_ddy=0.5;
lc_dy=0;
lc_ddy=1.6;
lc_scale=0.3;
lc_x=165;
lc_y=zamin_y-500*lc_scale;
lhead_x=185*lc_scale+lc_x,lhead_y=165*lc_scale+lc_y;
rc_dy=0;
rc_ddy=1.6;
rc_scale=0.3;
rc_x=1000;
rc_y=zamin_y-500*rc_scale;
rhead_x=210*rc_scale+rc_x;
rhead_y=165*rc_scale+rc_y;
}

void goal(bool left){
Mix_Chunk *goalsound = Mix_LoadWAV("goal.wav");
Mix_PlayChannel(-1,goalsound,0);
      if(left)    {
                  lc_goals++;
                  if(lc_power_timer < (2* power_cooldown/3) )
                  lc_power_timer+= power_cooldown/3;
                  else
                  lc_power_timer = power_cooldown;
      }
      else          {
         rc_goals++;
         if(rc_power_timer < (2* power_cooldown/3) )
                  rc_power_timer+= power_cooldown/3;
                  else
                  rc_power_timer = power_cooldown;

      }

      SDL_Rect  goal_rect;
      goal_rect.x  = 450;
      goal_rect.y  = 200;
      goal_rect.h  = 300;
      goal_rect.w =  400;
      resetvalues(0);
      SDL_Texture* goal_img = NULL;
      goal_img = IMG_LoadTexture(renderer,"goal.png");
      SDL_Point  goal_point={200,150};
      float angle = 0;
      int i = 0;
      while(i < 40){
                  if(i <= 30 )
                  {
                              angle = 900 - square(i);
                              goal_rect.h  = 10 * i;
                              goal_rect.w =  14 * i;
                  }
                  else angle =0;

                  SDL_RenderCopy(renderer, m_img, NULL, &img_rect); //background
                  SDL_RenderCopyEx(renderer, ball_texture,NULL,&ball_rect,t%20 * ball_dx,&ball_point,SDL_FLIP_NONE);
                  SDL_RenderCopy(renderer, tir, NULL, &tir_rect);//aks tirak ha rooye akse asli

                  SDL_RenderCopyEx(renderer, goal_img , NULL , &goal_rect , angle , &goal_point,SDL_FLIP_NONE);//akse goal
                  SDL_RenderPresent(renderer);
                        i++;
                        SDL_Delay(40);
}


SDL_DestroyTexture(goal_img);
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

float my_abs(float num){
if (num >= 0) return num;
if(num < 0)      return -num;


}
