#include <allegro.h>
#include <fstream.h>
#include <string>
#include "pictures.h"
using namespace std;

#define MAX_X 20
#define MAX_Y 20
#define SLOW1 14
#define MED2  12
#define MED3   10
#define MED4    8
#define FAST5    6
#define LEV1 1200             //3200
#define LEV2 900             //2400
#define LEV3 600              //1500
#define LEV4 300              //900
#define LEV5 0             //300 
#define GAME_END 5
#define RIGHT 0      //for the frame setting
#define LEFT 1       //for the frame setting

#define WHITE makecol(255, 255, 255)

void setup();
void options();
void game_intro();
void update_counter();
void create_map();
void player_move();
void reset_values();
void redraw();
void level_intro();
void retry();
void game_end();
void record_scoreboard();
void checkfor_record();

extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile int mouse_z;
extern volatile int mouse_b;
extern volatile int mouse_pos;
extern BITMAP *mouse_sprite;

int counter = 0, options_counter = 0;
int tcounter = 0; 
int total_time = 0, ttc = 0;
int fps = 0;
char printstr[50];
int level_counter = 0;

int user[MAX_X][MAX_Y];
int x = 0, y = 0;
int gamespeed = 0;
int difficulty = 0;
bool rego = 1;
 
struct maze
 {
 int exists;
 }cave[MAX_X][MAX_Y];
 
int grid[MAX_X][MAX_Y];
char test[MAX_X][MAX_Y];

int map = 0;
int a = 20, b = 20;
int show[MAX_X][MAX_Y];
int z = 0;
int next = 0;
int frame = 0;     //to see if the character should be facing right or left

DATAFILE *pictures;

BITMAP *buffer;

SAMPLE *FALL;
SAMPLE *SCREAM;
SAMPLE *ENTER;
SAMPLE *MUSIC;
SAMPLE *MOVE;
SAMPLE *GPAUSE;
SAMPLE *WARNING;
SAMPLE *GROAN;
SAMPLE *HORN;

int main() 
{         
 
 setup();
  
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  draw_sprite(buffer, (BITMAP*)pictures[ TITLE ].dat, 249, 80);
  textout_centre(buffer, font,  "Press enter to proceed", 320, 220, makecol(200, 0, 0));
  textout_centre(buffer, font,  "created by Kyle Clark", 320, 450, makecol(200, 0, 200));
  textout_centre(buffer, font,  "version v1.1 8/13/02", 320, 464, makecol(200, 0, 200));
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  while(!key[KEY_ENTER]);
  ;
  options_counter = 0;
  while(options_counter < 20); 
  ;
  tcounter = 3600; 
  options(); 
  game_intro();
  map = 1;
  counter = tcounter;
  reset_values();
  create_map();
  redraw();     
  counter = tcounter;
  total_time = 0;
 while(!key[KEY_ESC] && rego != 0)
  {
  
  update_counter();
  if(keypressed())
  {
  player_move();  
  }
  redraw();
   
  }
  
  allegro_exit();
  return 0;
}
END_OF_MAIN();

void setup()
{
 ofstream filelog("log.txt");
 allegro_init();
 filelog << "allegro_init initialized";
 install_keyboard();
 filelog << "\ninstall_keyboard initialized";
 install_mouse();
 filelog << "\ninstall_mouse initialized";
 install_timer();
 filelog << "\ninstall_timer initialized";
 set_color_depth(32);
 if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) == 0)
 {
 filelog << "\nset_color_depth(32) initialized";
 filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) initialized"; 
 }
 else if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) != 0)
 {
 filelog << "\nset_color_depth(32) failed";
 filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) failed";
 
  set_color_depth(24);
  if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) == 0)
  {
  filelog << "\nset_color_depth(24) initialized";
  filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) initialized"; 
  }
  else if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) != 0)
  {
  filelog << "\nset_color_depth(24) failed";
  filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) failed"; 
  
   set_color_depth(16);
   if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) == 0)
   {
   filelog << "\nset_color_depth(16) initialized";
   filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) initialized"; 
   }
   else if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 600, 480, 0, 0) != 0)
   {
   filelog << "\nset_color_depth(16) failed";
   filelog << "\nset_gfx_mode(GFX_AUTODETECT_FULLSCREEN 800x600) failed";
   filelog << "\nLight.exe bailing out";
   allegro_exit();
   }
  }
 }
 
 install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
 filelog << "\ninstall_sound(DIGI_AUTODETECT, MIDI_AUTODETECT) initialized";
 install_int_ex( update_counter, BPS_TO_TIMER(60));
 filelog << "\ninstall_int_ex(update_counter, BPS_TO_TIMER(60)) initialized";  
  
 pictures = load_datafile( "pictures.dat" );
 filelog << "\npictures.dat loaded";
 buffer     = create_bitmap(640, 480);
 filelog << "\nbuffer created";
  
  FALL      = load_sample("sounds\\fall.wav");
  SCREAM      = load_sample("sounds\\scream.wav");
  ENTER     = load_sample("sounds\\enter.wav");
  MUSIC      = load_sample("sounds\\music.wav");
  MOVE     = load_sample("sounds\\move.wav");
  GPAUSE     = load_sample("sounds\\pause.wav");
  GROAN     = load_sample("sounds\\groan.wav");
  WARNING     = load_sample("sounds\\warning.wav");
  HORN    = load_sample("sounds\\musicalhorn.wav");
  filelog << "\nsounds\\.wav loaded";
  filelog.close();
 }

void game_intro()
 {
 
 y = 268;
 x = 5;
 
 blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
   
for(int i = 0; i < 25; i++)
  { 
  play_sample(MUSIC, 128, 128, 300, FALSE);
  if(i < 24)
  {
  blit((BITMAP*)pictures[ GINTRO ].dat, buffer, 0, 0, 0, 100, 640, 280); 
  draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, x, y);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  }
  else if(i == 24)
  {      
  blit((BITMAP*)pictures[ GINTRO ].dat, buffer, 0, 0, 0, 100, 640, 280);
  draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER90 ].dat, x, y);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  play_sample(FALL, 128, 128, 2000, FALSE);
  }
  options_counter = 0;
  while(options_counter < 20); 
  ;
  x += 15;
  }
 play_sample(SCREAM, 128, 128, 1500, FALSE);
 blit((BITMAP*)pictures[ GINTRO ].dat, buffer, 0, 0, 0, 100, 640, 280);   
 blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
 
 x = 0;
 y = 0;
 
 play_sample(GROAN, 128, 128, 2000, FALSE);
 options_counter = 0;
  while(options_counter < 60); 
  ;
 
 blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 100, 640, 280);  
 textout_centre(buffer, font,  "Oh no you have fallen in a dark cave!", 320, 210, makecol(200, 0, 0));
 textout(buffer, font,  "You only have a little bit of time before the light in the cave runs out", 5, 230, makecol(200, 0, 200));
 textout(buffer, font,  "The cave is going to be very hard to escape", 5, 240, makecol(200, 0, 200));
 textout(buffer, font,  "Press SPACE wherever you see a lot of light in each section of the cave", 5, 250, makecol(200, 0, 200));
 textout(buffer, font,  "The brighest spot in each cave section will be the exit of the section", 5, 260, makecol(200, 0, 200));
 textout(buffer, font,  "Continue to follow the light in the cave so you can escape in time", 5, 270, makecol(200, 0, 200));
 blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
 
 while(!key[KEY_ENTER] && !key[KEY_ESC]);
 ;
 stop_sample(MUSIC);
 }
  
void update_counter()
{
     if(rego != 2)
     {
     counter--;
     }
     else if(rego == 2)
     {
     counter++;
     }
     options_counter++;
     fps++;
     total_time++;
}
 
void create_map()
 {
 ifstream filemap;
 
  if(map == 1 && next == 1)
   {
   sprintf(printstr, "maps\\map1.txt");
   }
  else if(map == 2 && next == 1)
   {
   sprintf(printstr, "maps\\map2.txt");
   }
  else if(map == 3 && next == 1)
   {
   sprintf(printstr, "maps\\map3.txt");
   }
  else if(map == 4 && next == 1)
   {
   sprintf(printstr, "maps\\map4.txt");
   }
  else if(map == 5 && next == 1)
   {
  sprintf(printstr, "maps\\map5.txt");
   }
   filemap.open(printstr);
    for(int b = 0; b < MAX_Y; b++)
     {
     for(int z = 0; z < MAX_X; z++)
      {
       filemap >> test[z][b];
       if(test[z][b] == 'x')
        {
        grid[z][b] = 2;
        }
       else if(test[z][b] == 'o')
        {
        grid[z][b] = 1;
        }
       else if(test[z][b] == 'e')
        {
        grid[z][b] = 3;
        }
       else if(test[z][b] == 'u')
        {
        grid[z][b] = 5;
        }
       else if(test[z][b] == 'd')
        {
        grid[z][b] = 6;
        }
       else if(test[z][b] == 's')
        {
        grid[z][b] = 4;
        x = z;
        y = b;
        }
      }
     }
  next = 0;
   filemap.close();
     
  for(int j = 0; j < MAX_Y; j++)
   {
   for(int i = 0; i < MAX_X; i++)
    {
    if(grid[i][j] == 4 && (i == 0 && j == 0))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j+1].exists = 1;
     cave[i][j+1].exists = 1;     
     } 
    else if(grid[i][j] == 4 && (i > 0 && i < MAX_X && j == 0))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j+1].exists = 1;
     cave[i][j+1].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j+1].exists = 1;
     }
    else if(grid[i][j] == 4 && (i == MAX_X && j == 0))
     {
     cave[i][j].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j+1].exists = 1;
     cave[i][j+1].exists = 1;
     }
    else if(grid[i][j] == 4 && (i == 0 && j > 0 && j < MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j+1].exists = 1;
     cave[i][j+1].exists = 1;
     cave[i+1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     } 
    else if(grid[i][j] == 4 && (i > 0 && i < MAX_X && j > 0 && j < MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j+1].exists = 1;
     cave[i][j+1].exists = 1;
     cave[i+1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j+1].exists = 1;
     cave[i-1][j-1].exists = 1;
     }
    else if(grid[i][j] == 4 && (i == MAX_X && j > 0 && j < MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j+1].exists = 1;
     cave[i][j+1].exists = 1;
     cave[i-1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     }
    else if(grid[i][j] == 4 && (i == 0 && j == MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     } 
    else if(grid[i][j] == 4 && (i > 0 && i < MAX_X && j == MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i+1][j].exists = 1;
     cave[i+1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j-1].exists = 1;
     }
    else if(grid[i][j] == 4 && (i == MAX_X && j == MAX_Y))
     {
     cave[i][j].exists = 1;
     cave[i-1][j].exists = 1;
     cave[i-1][j-1].exists = 1;
     cave[i][j-1].exists = 1;
     }
    }
   }  
 
 }
   
void player_move()
 { 
   if(key[KEY_UP] && (y > 0 && x == 0) && grid[x][y-1] != 2) 
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5;  
   y -= 1;
   cave[x][y].exists = 1; 
   if(y != 0)
   {
   cave[x+1][y-1].exists = 1;
   cave[x][y-1].exists = 1;
   }
   cave[x+1][y].exists = 1;
   cave[x+1][y+1].exists = 1;
   
   cave[x][y+1].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )   
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
   else if(key[KEY_UP] && (y > 0 && x == MAX_X - 1) && grid[x][y-1] != 2) 
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   y -= 1;
   cave[x][y].exists = 1;
   if(y != 0)
   { 
   cave[x-1][y-1].exists = 1;
   cave[x][y-1].exists = 1;
   }
   cave[x-1][y].exists = 1;
   cave[x-1][y+1].exists = 1;   
   cave[x][y+1].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )    
   grid[x][y] = 0;
   ttc = total_time; 
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
   else if(key[KEY_UP] && (y > 0 && x != 0 && x != MAX_X - 1) && grid[x][y-1] != 2) 
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   y -= 1;
   cave[x][y].exists = 1;
   if(y != 0)
   { 
   cave[x-1][y-1].exists = 1;
   cave[x+1][y-1].exists = 1;
   cave[x][y-1].exists = 1;
   }
   cave[x-1][y].exists = 1;   
   cave[x-1][y+1].exists = 1;   
   cave[x][y+1].exists = 1;    
   cave[x+1][y].exists = 1;
   cave[x+1][y+1].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )    
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
   
   
  else if(key[KEY_DOWN] && (y < MAX_Y - 1 && x == 0) && grid[x][y+1] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   y += 1;
   cave[x][y].exists = 1;
   if(y != MAX_Y - 1)
   {
    cave[x+1][y+1].exists = 1;
    cave[x][y+1].exists = 1; 
   }
   cave[x+1][y-1].exists = 1;
   cave[x+1][y].exists = 1;
   cave[x][y-1].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )  
   grid[x][y] = 0; 
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
  else if(key[KEY_DOWN] && (y < MAX_Y - 1 && x == MAX_X - 1) && grid[x][y+1] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   y += 1;
   cave[x][y].exists = 1;
   cave[x-1][y-1].exists = 1;
   cave[x-1][y].exists = 1;
   if(y != MAX_Y - 1)
   {
    cave[x-1][y+1].exists = 1;
    cave[x][y+1].exists = 1; 
   }
   cave[x][y-1].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )    
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
  else if(key[KEY_DOWN] && (y < MAX_Y - 1 && x != 0 && x != MAX_X - 1) && grid[x][y+1] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5;  
   y += 1;
   cave[x][y].exists = 1;
   cave[x-1][y-1].exists = 1;
   cave[x-1][y].exists = 1;
   cave[x+1][y-1].exists = 1;
   cave[x+1][y].exists = 1;
   if(y != MAX_Y - 1)
   {
    cave[x-1][y+1].exists = 1;
    cave[x+1][y+1].exists = 1;
    cave[x][y+1].exists = 1; 
   }
   cave[x][y-1].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )   
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   }
   
   
  else if(key[KEY_RIGHT] && (x < MAX_X - 2 && y == 0) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;  
   cave[x+1][y].exists = 1;
   cave[x+1][y+1].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )         
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
  else if(key[KEY_RIGHT] && (x < MAX_X - 2 && y == MAX_Y - 1) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;
   cave[x+1][y].exists = 1;
   cave[x+1][y-1].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )      
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
  else if(key[KEY_RIGHT] && (x < MAX_X - 2 && y != 0 && y != MAX_Y - 1) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;
   cave[x+1][y].exists = 1;
   cave[x+1][y-1].exists = 1;
   cave[x+1][y+1].exists = 1;  
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )      
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
   else if(key[KEY_RIGHT] && (x == MAX_X - 2 && y == 0) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;  
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )         
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
  else if(key[KEY_RIGHT] && (x == MAX_X - 2 && y == MAX_Y - 1) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )      
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
  else if(key[KEY_RIGHT] && (x == MAX_X - 2 && y != 0 && y != MAX_Y - 1) && grid[x+1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x += 1;
   cave[x][y].exists = 1;
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )      
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = RIGHT;
   }
   
   
  else if(key[KEY_LEFT] && (x > 0 && y == 0) && grid[x-1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x -= 1;
   cave[x][y].exists = 1;
   cave[x-1][y+1].exists = 1; 
   cave[x-1][y].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )   
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = LEFT;
   }
   else if(key[KEY_LEFT] && (x > 0 && y == MAX_Y - 1) && grid[x-1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x -= 1;
   cave[x][y].exists = 1;
   cave[x-1][y-1].exists = 1; 
   cave[x-1][y].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )    
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = LEFT;
   }
   else if(key[KEY_LEFT] && (x > 0 && y != 0 && y != MAX_Y - 1) && grid[x-1][y] != 2)
   {
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 && grid[x][y+1] != 3 && grid[x][y-1] != 3)  
   grid[x][y] = 1;
   else if(grid[x][y] == 0 && grid[x][y-1] == 3)
   grid[x][y] = 6;
   else if(grid[x][y] == 0 && grid[x][y+1] == 3)
   grid[x][y] = 5; 
   x -= 1;
   cave[x][y].exists = 1;
   cave[x-1][y+1].exists = 1; 
   cave[x-1][y-1].exists = 1; 
   cave[x-1][y].exists = 1; 
   if(grid[x][y] != 3 && grid[x][y] != 5 && grid[x][y] != 6 )    
   grid[x][y] = 0;
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   play_sample(MOVE, 128, 128, 2000, FALSE);
   frame = LEFT;
   }
   
           
   else if(key[KEY_SPACE] && test[x][y] == 'e' && map != GAME_END)
   {
   play_sample(ENTER, 128, 128, 1500, FALSE);
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < 30); 
  ;
   counter = tcounter;
   total_time = ttc;
   level_intro();
   map++;
   reset_values();
   ttc = total_time;
   tcounter = counter;
   options_counter = 0;
  while(options_counter < gamespeed); 
  ;
   counter = tcounter;
   total_time = ttc;
   } 
   
   
  else if(key[KEY_SPACE] && test[x][y] == 'e' && map == GAME_END)
   {
    game_end();
   }
   
   
  else if(key[KEY_END])
   { 
    ttc = total_time;
    tcounter = counter;
    play_sample(GPAUSE, 128, 128, 2000, FALSE);
    draw_sprite(screen, (BITMAP*)pictures[ PAUSE ].dat, 276, 220);
    clear_keybuf();
    ttc = total_time;
   tcounter = counter;
   options_counter = 0;
   while(options_counter < 12); 
   ;
    while(!key[KEY_END]);
    ;
    play_sample(GPAUSE, 128, 128, 2000, FALSE);
    blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
    options_counter = 0;
    while(options_counter < 6); 
    ;
    counter = tcounter;
    total_time = ttc;
   }    
  }
  
  
void reset_values()
 {
    
  for(int j = 0; j < MAX_Y; j++)
  {
   for(int i = 0; i < MAX_X; i++)
    {
       cave[i][j].exists = 0; 
       show[i][j] = 0;      
    }
   }
  
  clear_bitmap(buffer);
  
  if(map == GAME_END + 1)
   {
   game_end();
   next = 0;
   }
  
  x = 0;
  y = 0;   
  next = 1;                      
  create_map();
  
  counter += difficulty; 
 
 } 

void redraw()
 {
 
 blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480); 
 draw_sprite(buffer, (BITMAP*)pictures[ BORDER ].dat, 15, 15);
 
 for(int j = 0; j < MAX_Y; j++)
  {
   for(int i = 0; i < MAX_X; i++)
    {
      if(cave[i][j].exists == 1)
       {
       show[i][j] = 1;       
       }
    }
   }
   
 for(int j = 0; j < MAX_Y; j++)
  {
   for(int i = 0; i < MAX_X; i++)
    {    
     if(show[i][j] == 1)
      { 
        if(grid[i][j] == 0)
         {
          blit((BITMAP*)pictures[ CFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
          if(frame == 0)
          draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b);
          else if(frame == 1)
          draw_sprite_h_flip(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b); //draws the sprite pointing to the left
         }
        else if(grid[i][j] == 1)
         {
          blit((BITMAP*)pictures[ CFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
         }
        else if(grid[i][j] == 2)
         {
          blit((BITMAP*)pictures[ CWALL ].dat, buffer, 0, 0, a, b, 640, 480);
         }
        else if(grid[i][j] == 3 && (x != i || y != j))
         {
         blit((BITMAP*)pictures[ LFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
         }
       else if(grid[i][j] == 3 && (x == i && y == j))
         {
         blit((BITMAP*)pictures[ LFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
         if(frame == 0)
          draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b);
          
          else if(frame == 1)
          draw_sprite_h_flip(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b); //draws the sprite pointing to the left
          
          textout_centre(buffer, font, "PRESS SPACE", 550, 150, makecol(200, 0, 200));
         } 
       else if(grid[i][j] == 4 && (x != i || y != j))
         {
          blit((BITMAP*)pictures[ CFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
         }
       else if(grid[i][j] == 4 && (x == i && y == j))
         {
          blit((BITMAP*)pictures[ CFLOOR ].dat, buffer, 0, 0, a, b, 640, 480);
          if(frame == 0)
          draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b);
          
          else if(frame == 1)
          draw_sprite_h_flip(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b); //draws the sprite pointing to the left
         }
       else if(grid[i][j] == 5 && (x != i || y != j))
        {
         blit((BITMAP*)pictures[ FLOOR_LU ].dat, buffer, 0, 0, a, b, 640, 480);
        }
       else if(grid[i][j] == 5 && (x == i && y == j))
        {
         blit((BITMAP*)pictures[ FLOOR_LU ].dat, buffer, 0, 0, a, b, 640, 480);
         if(frame == 0)
          draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b);
          
         else if(frame == 1)
         draw_sprite_h_flip(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b); //draws the sprite pointing to the left
        }
       else if(grid[i][j] == 6 && (x != i || y != j))
        {
         blit((BITMAP*)pictures[ FLOOR_LD ].dat, buffer, 0, 0, a, b, 640, 480);
        }
       else if(grid[i][j] == 6 && (x == i && y == j))
        {
         blit((BITMAP*)pictures[ FLOOR_LD ].dat, buffer, 0, 0, a, b, 640, 480);
         if(frame == 0)
          draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b);
          
          else if(frame == 1)
          draw_sprite_h_flip(buffer, (BITMAP*)pictures[ CHARACTER ].dat, a, b); //draws the sprite pointing to the left
        }
       } 
       a += 20;
      }
      b += 20;
      a = 20;        
    }                 
      b = 20;

  sprintf(printstr, "Time : %d", counter / 60);
  textout(buffer, font, printstr, 541, 370, makecol(200, 0, 0));

  if(counter > 3600)                                 
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCKB ].dat, 540, 380);
  else if(counter >= 3360 && counter <= 3600)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK60 ].dat, 540, 380);
  else if(counter < 3360 && counter >= 2940)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK49 ].dat, 540, 380);
   else if(counter < 2940 && counter >= 2520)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK42 ].dat, 540, 380);
   else if(counter < 2520 && counter >= 2100)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK35 ].dat, 540, 380);
   else if(counter < 2100 && counter >= 1680)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK28 ].dat, 540, 380);
   else if(counter < 1680 && counter >= 1260)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK21 ].dat, 540, 380);
   else if(counter < 1260 && counter >= 840)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK14 ].dat, 540, 380);
   else if(counter < 840 && counter >= 420)
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCK7 ].dat, 540, 380);
   else if(counter < 420 && counter >= 60)
   {
  play_sample(WARNING, 128, 128, 400, FALSE);
  draw_sprite(buffer, (BITMAP*)pictures[ CLOCKR ].dat, 540, 380);
    }
  else if(counter <= 50 && rego != 0)
   {
 ttc = total_time;
  tcounter = counter;
  options_counter = 0;
  while(options_counter < 30); 
  ;
  counter = tcounter;
  total_time = ttc;
  retry();
   } 

  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  
 // save_bitmap("screen.bmp", buffer, NULL);  
}

void level_intro()
 {
  level_counter = counter;
  ttc = total_time;
  tcounter = counter;
  clear_bitmap(buffer);
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  if(counter > 3600)
  {                                 
  textout_centre(buffer, font, "Seeing as how you made it through the cave with so much", 320, 150, makecol(200, 0, 200));
  textout_centre(buffer, font, "time to spare before all the light runs out, you should be", 320, 159, makecol(200, 0, 200));
  textout_centre(buffer, font, "able to escape the caves and reach a light source in time.", 320, 168, makecol(200, 0, 200));
  }
  else if(counter >= 2400 && counter <= 3600)
    {
  textout_centre(buffer, font, "With quite a bit of time left you breezed through the cave.", 320, 150, makecol(200, 0, 200));
  textout_centre(buffer, font, "If you keeping going at this pace you will be fleeing the", 320, 159, makecol(200, 0, 200)); 
  textout_centre(buffer, font, "cave before the bats come out.", 320, 168, makecol(200, 0, 200)); 
      }
  else if(counter < 2400 && counter >= 1200)
       {
  textout_centre(buffer, font, "Not as fast as you would like to be getting out of here,", 320, 150, makecol(200, 0, 200));
  textout_centre(buffer, font, "but it will be sufficient so as long as you don't make a", 320, 159, makecol(200, 0, 200)); 
  textout_centre(buffer, font, "deadly mistake.", 320, 168, makecol(200, 0, 200)); 
    }
   else if(counter < 1200 && counter >= 300) 
    {
  textout_centre(buffer, font, "For your sake you really need to speed things up.", 320, 150, makecol(200, 0, 200));
  textout_centre(buffer, font, "It doesn't look too promising for you at this point.", 320, 159, makecol(200, 0, 200));  
   }
   else if(counter < 300 && counter > 0)
      {
  textout_centre(buffer, font, "You are in serious danger of dying!  Hurry it up or else", 320, 150, makecol(200, 0, 200));
  textout_centre(buffer, font, "you'll be torn apart by the vicious animals in there!", 320, 159, makecol(200, 0, 200));
   }
  
  sprintf(printstr, "You gained %d", difficulty / 60);
  textout_centre(buffer, font, printstr, 320, 228, makecol(100, 100, 100));
  textout_centre(buffer, font,  "seconds of light source time", 320, 237, makecol(100, 100, 100));
  textout_centre(buffer, font,  "Press enter to continue", 320, 300, makecol(200, 0, 200));
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  
   while(!key[KEY_ENTER] && !key[KEY_ESC]);
  ;
  counter = tcounter;
  total_time = counter;
 } 
 
void retry()
 {
   clear_bitmap(buffer);
   blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480); 
   textout(buffer, font,  "Time has run out on you", 60, 209, makecol(200, 0, 200));
   textout(buffer, font,  "Enter L if you would like to retry that level", 60, 259, makecol(200, 200, 200));
   textout(buffer, font,  "Enter G if you would like to start a new game", 60, 268, makecol(200, 200, 200));
   textout(buffer, font,  "Enter Q if you would like to quit", 60, 277, makecol(200, 200, 200));
   blit(buffer, screen, 0, 0, 0, 0, 640, 480);
   
   while(!key[KEY_G] && !key[KEY_L] && !key[KEY_Q] && !key[KEY_ESC]);
   ;
   
   if(key[KEY_L] && map > 1)
    {
    counter = level_counter;
    reset_values();
    }
   else if(key[KEY_L] && map == 1)
    {
    map = 1;
    counter = 3600 - difficulty;
    reset_values();
    }
   else if(key[KEY_G])
    {
    map = 1;
    counter = 3600 - difficulty;
    reset_values();
    }
   else if(key[KEY_Q])
    {
    rego = 0;
    }
 }
 
void options()
 {
  rego = 2;
 
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout_centre(buffer, font,  "GAME CONTROLS", 320, 80, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Arrow Keys", 320, 120, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Used to move your character", 320, 130, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Space Bar", 320, 145, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Used to advance onto the next maze", 320, 155, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Enter Key", 320, 170, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Used to escape most sequences", 320, 180, makecol(200, 0, 200));
  textout_centre(buffer, font,  "End Key", 320, 195, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Used to pause the game and unpause the game", 320, 205, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Esc Key", 320, 220, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Used to quit the game", 320, 230, makecol(200, 0, 200));
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  while(!key[KEY_ENTER] && !key[KEY_ESC]);
  ;
  options_counter = 0;
  while(options_counter < 20);
  ; 
  x = 340;
  y = 200;
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, 340, 200); 
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0));  
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  while(!key[KEY_ENTER])
  {    
  if(key[KEY_LEFT] && x == 290)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "EASIEST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ;  
  }
  else if(key[KEY_LEFT] && x == 340)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "EASY", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_LEFT] && x == 390)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200));  
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_LEFT] && x == 440)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "HARD", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ;   
  }    
  else if(key[KEY_RIGHT] && x  == 240)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "EASY", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20);
  ;
  }
  else if(key[KEY_RIGHT] && x == 290)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50; 
  options_counter = 0;
  while(options_counter < 20); 
  ; 
  }
   else if(key[KEY_RIGHT] && x == 340)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "HARD", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_RIGHT] && x == 390)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a difficulty level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200));
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "HARDEST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20); 
  ;
  }
  
  if(x == 240)
  difficulty = LEV1;
  else if(x == 290)
  difficulty = LEV2;
  else if(x == 340)
  difficulty = LEV3;
  else if(x == 390)
  difficulty = LEV4;
  else if(x == 440)
  difficulty = LEV5;
  }
  
  options_counter = 0;
  while(options_counter < 10);
  ; 
  clear_keybuf();
  x = 340;
  y = 200;
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, 340, 200); 
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0));  
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  while(!key[KEY_ENTER])
  {    
  if(key[KEY_LEFT] && x == 290)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "SLOWEST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_LEFT] && x == 340)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "SLOW", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_LEFT] && x == 390)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200));  
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_LEFT] && x == 440)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x - 50, 200);
  textout(buffer, font,  "FAST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  x -= 50; 
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }    
  else if(key[KEY_RIGHT] && x  == 240)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "SLOW", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20);
  ;  
  }
  else if(key[KEY_RIGHT] && x == 290)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "NORMAL", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20); 
  ;  
  }
   else if(key[KEY_RIGHT] && x == 340)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200)); 
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205);  
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "FAST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20);
  ; 
  }
  else if(key[KEY_RIGHT] && x == 390)
  {
  blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout(buffer, font,  "Please choose a gamespeed level", 50, 100, makecol(200, 0, 200));
  textout(buffer, font,  "Arrow keys for changing selection", 50, 118, makecol(200, 0, 200));
  textout(buffer, font,  "Enter key for deciding selection", 50, 127, makecol(200, 0, 200));
  draw_sprite(buffer, (BITMAP*)pictures[ INTERVAL ].dat, 233, 205); 
  draw_sprite(buffer, (BITMAP*)pictures[ SLIDER ].dat, x + 50, 200);
  textout(buffer, font,  "FASTEST", 233, 225, makecol(200, 200, 0)); 
  blit(buffer, screen, 0, 0, 0, 0, 640, 480); 
  x += 50;
  options_counter = 0;
  while(options_counter < 20); 
  ;  
  }
  
  if(x == 240)
  gamespeed = SLOW1;
  else if(x == 290)
  gamespeed = MED2;
  else if(x == 340)
  gamespeed = MED3;
  else if(x == 390)
  gamespeed = MED4;
  else if(x == 440)
  gamespeed = FAST5;
  }
  
  rego = 1;

 }
 
void game_end()
 {
  rego = 2;
  ttc = total_time;
  y = 265;
  x = 43;
 
 blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);

 play_sample(HORN, 128, 128, 1000, 1);
 play_sample(MUSIC, 128, 128, 100, 1);   
for(int i = 0; i < 20; i++)
  {
  if(i < 20)
  {
  blit((BITMAP*)pictures[ GEND ].dat, buffer, 0, 0, 0, 100, 640, 280); 
  draw_sprite(buffer, (BITMAP*)pictures[ CHARACTER ].dat, x, y);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  }
  options_counter = 0; 
  while(options_counter < 20); 
  ; 
  x += 15;
  }
 
 x = 0;
 y = 0;
 
 stop_sample(MUSIC);
 options_counter = 0;
  while(options_counter < 180); 
  ;
 stop_sample(HORN);
 play_sample(MUSIC, 128, 128, 250, 1); 
// blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
 textout_centre(buffer, font,  "Hurray!  You made it out of the caves safely.", 320, 150, makecol(200, 0, 200));
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);
 options_counter = 0;
  while(options_counter < 120); 
  ; 
  
  checkfor_record();
 // record_scoreboard();
  clear_keybuf();
  options_counter = 0;
  while(options_counter < 6); 
  ; 
  
 blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
  textout_centre(buffer, font,  "GAME CREDITS", 320, 80, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Game Creator", 320, 120, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Kyle Clark", 320, 130, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Game Programmer", 320, 145, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Kyle Clark", 320, 155, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Game Artist", 320, 170, makecol(200, 0, 0));
  textout_centre(buffer, font,  "Kyle Clark", 320, 180, makecol(200, 0, 200));
  textout_centre(buffer, font,  "SPECIAL CREDITS", 320, 220, makecol(200, 0, 0));
  textout_centre(buffer, font,  "www.bloodshed.net", 320, 235, makecol(200, 0, 200));
  textout_centre(buffer, font,  "www.a1freesoundeffects.com", 320, 245, makecol(200, 0, 200));
  textout_centre(buffer, font,  "http://www.jrsoftware.org/isinfo.php", 320, 255, makecol(200, 0, 200));
  textout_centre(buffer, font,  "www.allegro.cc", 320, 265, makecol(200, 0, 200));
  textout_centre(buffer, font,  "23yrold3yrold at allegro.cc", 320, 275, makecol(200, 0, 200));
  textout_centre(buffer, font,  "Eric Love", 320, 285, makecol(200, 0, 200));
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);
  
  while(!key[KEY_ENTER] && !key[KEY_ESC]);
  ;
 stop_sample(MUSIC);
 rego = 0;
 } 
 
void record_scoreboard()
 {
   string  edittext;                         // an empty string for editting
   string::iterator iter = edittext.begin(); // string iterator
   int     caret  = 0;                       // tracks the text caret
   bool    insert = true;                    // true of should text be inserted
   
   // the game loop
   
   options_counter = 0;
  while(options_counter < 30); 
  ; 
   
   do
   {
      while(keypressed())
      {
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;

         // a character key was pressed; add it to the string
         if(ASCII >= 32 && ASCII <= 126)
         {
            // add the new char, inserting or replacing as need be
            if(insert || iter == edittext.end())
               iter = edittext.insert(iter, ASCII);
            else
               edittext.replace(caret, 1, 1, ASCII);

            // increment both the caret and the iterator
            caret++;
            iter++;
         }
         // some other, "special" key was pressed; handle it here
         else
            switch(scancode)
            {
               case KEY_DEL:
                  if(iter != edittext.end()) iter = edittext.erase(iter);
               break;

               case KEY_BACKSPACE:
                  if(iter != edittext.begin())
                  {
                     caret--;
                     iter--;
                     iter = edittext.erase(iter);
                  }
               break;
            
               case KEY_RIGHT:
                  if(iter != edittext.end())   caret++, iter++;
               break;
            
               case KEY_LEFT:
                  if(iter != edittext.begin()) caret--, iter--;
               break;
            
               case KEY_INSERT:
                  if(insert) insert = 0; else insert = 1;
               break;

               default:

               break;
            }
      }
      
      // clear screen
      clear(buffer);

      // output the string to the screen
      blit((BITMAP*)pictures[ MAZEBG ].dat, buffer, 0, 0, 0, 0, 640, 480);
      textout_centre(buffer, font, edittext.c_str(), 320, 140, WHITE);

      // output some stats using Allegro's printf functions
      textout_centre(buffer, font, "You have broken the record for the lowest time recorded in a whole game", 320, 100, makecol(200, 0, 200));
      textout_centre(buffer, font, "Please enter your name", 320, 115, makecol(200, 0, 0));
      sprintf(printstr, "%d", ttc / 60);
      textout_centre(buffer, font, printstr, 320, 150, WHITE);

      // draw the caret
      if(caret == 0)
      vline(buffer, 320, 138, 148, makecol(200, 200, 200));
      else
      vline(buffer, caret * 320, 138, 148, makecol(200, 200, 200));

      // blit to screen
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

   }while(!key[KEY_ESC] && !key[KEY_ENTER]); // end of game loop
 
 
 
 
 
    total_time = ttc;
    ofstream file_scoreboard("scoreboard.dat");
    file_scoreboard << total_time / 60;
    file_scoreboard << "\n";  
    file_scoreboard << difficulty; 
    file_scoreboard << "\n";  
    file_scoreboard <<  edittext.c_str();
    file_scoreboard.close();
 
}  

void checkfor_record()
 {
 int time;
 int level;
 string name;
    
 ifstream file_record("scoreboard.dat");
 file_record >> time >> difficulty >> name;
 file_record.close();
 if(ttc / 60 < time || time == 0)
 record_scoreboard(); 
 }
 
   
 
