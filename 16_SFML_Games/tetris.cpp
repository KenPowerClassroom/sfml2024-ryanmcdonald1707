#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int GRIDHEIGHT = 20;
const int GRIDWIDTH = 10;

int field[GRIDHEIGHT][GRIDWIDTH] = {0};

struct Point
{int x,y;} tetronimoPos[60], backupPos[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check()
{
   for (int i=0;i<4;i++)
      if (tetronimoPos[i].x<0 || tetronimoPos[i].x>= GRIDWIDTH || tetronimoPos[i].y>= GRIDHEIGHT) return 0;
      else if (field[tetronimoPos[i].y][tetronimoPos[i].x]) return 0;

   return 1;
};


int tetris()
{
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture tilesTexture, backgroundTexture, frameTexture;
    tilesTexture.loadFromFile("images/tetris/tiles.png");
    backgroundTexture.loadFromFile("images/tetris/background.png");
    frameTexture.loadFromFile("images/tetris/frame.png");

    Sprite tiles(tilesTexture), background(backgroundTexture), frame(frameTexture);

    int dx=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; 

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
              if (e.key.code==Keyboard::Up) rotate=true;
              else if (e.key.code==Keyboard::Left) dx=-1;
              else if (e.key.code==Keyboard::Right) dx=1;
        }

    if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

    //// <- Move -> ///
    for (int i=0;i<4;i++)  { backupPos[i]=tetronimoPos[i]; tetronimoPos[i].x+=dx; }
    if (!check()) for (int i=0;i<4;i++) tetronimoPos[i]=backupPos[i];

    //////Rotate//////
    if (rotate)
      {
        Point p = tetronimoPos[1]; //center of rotation
        for (int i=0;i<4;i++)
          {
            int x = tetronimoPos[i].y-p.y;
            int y = tetronimoPos[i].x-p.x;
            tetronimoPos[i].x = p.x - x;
            tetronimoPos[i].y = p.y + y;
           }
           if (!check()) for (int i=0;i<4;i++) tetronimoPos[i]=backupPos[i];
      }

    ///////Tick//////
    if (timer>delay)
      {
        for (int i=0;i<4;i++) { backupPos[i]=tetronimoPos[i]; tetronimoPos[i].y+=1; }

        if (!check())
        {
         for (int i=0;i<4;i++) field[backupPos[i].y][backupPos[i].x]=colorNum;

         colorNum=1+rand()%7;
         int n=rand()%7;
         for (int i=0;i<4;i++)
           {
            tetronimoPos[i].x = figures[n][i] % 2;
            tetronimoPos[i].y = figures[n][i] / 2;
           }
        }

         timer=0;
      }

    ///////check lines//////////
    int k = GRIDHEIGHT -1;
    for (int i = GRIDHEIGHT -1;i>0;i--)
    {
        int count=0;
        for (int j=0;j< GRIDWIDTH;j++)
        {
            if (field[i][j]) count++;
            field[k][j]=field[i][j];
        }
        if (count< GRIDWIDTH) k--;
    }

    dx=0; rotate=0; delay=0.3;

    /////////draw//////////
    window.clear(Color::White);    
    window.draw(background);
          
    for (int i=0;i< GRIDHEIGHT ;i++)
     for (int j=0;j< GRIDWIDTH;j++)
       {
         if (field[i][j]==0) continue;
         tiles.setTextureRect(IntRect(field[i][j]*18,0,18,18));
         tiles.setPosition(j*18,i*18);
         tiles.move(28,31); //offset
         window.draw(tiles);
       }

    for (int i=0;i<4;i++)
      {
        tiles.setTextureRect(IntRect(colorNum*18,0,18,18));
        tiles.setPosition(tetronimoPos[i].x*18,tetronimoPos[i].y*18);
        tiles.move(28,31); //offset
        window.draw(tiles);
      }

    window.draw(frame);
    window.display();
    }

    return 0;
}
