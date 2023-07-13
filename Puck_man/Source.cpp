#include <SDL.h>
#include <SDL_image.h>
#include <iostream>	//这东西主要用于调试
using namespace std;
#define Max 64
#define ScreenW 1024
#define ScreenH 1024

// 渲染器
SDL_Renderer* renderer = nullptr;

Uint32 startTime;	//上一帧的时间
Uint32 frameDelay = 15;	//两帧间隔的时间。单位毫秒。15毫秒大概66帧
//Uint32 endTime;		//这一帧的时间
//Uint32 elapsedTime;	//经过的时间

//吃豆人
Uint32 movestartTime;	//上次移动的时间
Uint32 moveDelay = 75;//移动的基础间隔


//鬼
Uint32 gui1movestartTime;	//上次移动的时间
Uint32 gui1moveDelay = 85;	//移动的基础间隔//减速后245
Uint32 gui2movestartTime;	//
Uint32 gui2moveDelay = 90;//减速后270
Uint32 gui3movestartTime;	//
Uint32 gui3moveDelay = 90;//减速后270
Uint32 gui4movestartTime;	//
Uint32 gui4moveDelay = 90;//减速后270
//行动模式
Uint32 escapestartTime;	//
Uint32 escapeDelay = 5000;
Uint32 killstartTime;	//
Uint32 killDelay = 5000;


//定义向量
struct Vector
{
	int x;
	int y;
};
//定义四个方向
Vector UP = { 0,-1 };
Vector LEFT = { -1,0 };
Vector DOWN = { 1,0 };
Vector RIGHT = { 0,1 };

Vector mandrawVector = { 0,0 };
Vector gui1drawVector = {};
Vector gui2drawVector = {};
Vector gui3drawVector = {};
Vector gui4drawVector = {};
//定义角色
struct Character
{
	struct Vector Position;
	int Direction;
};
//定义格子
struct Check
{
	struct Vector Position;
	bool wall;
	bool douzi;
	bool bigdou;
};

int sumdou = 0;
int eatendou = 0;


//地图
Check Map[Max][Max];
//吃豆人
Character man =
{
	20,38,1
};
//鬼
Character gui1 =
{
	20,23,1
};
Character gui2 =
{
	19,25,1
};
Character gui3 =
{
	21,25,1
};
Character gui4 =
{
	22,25,1
};
//分散模式
bool escape = false;
//杀鬼模式
bool kill = false;

//地图中的墙1和豆子2和大豆子3
int Wall[Max][Max] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,3,0,1,0,0,0,1,0,2,0,1,0,0,0,1,0,2,0,1,0,2,0,1,0,0,0,1,0,2,0,1,0,0,0,1,0,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,2,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,0,2,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,2,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,0,2,0,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,3,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,0,2,0,1,0,2,0,1,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,1,0,2,0,1,0,2,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,0,2,0,1,0,2,0,1,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,1,0,2,0,1,0,2,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,2,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,0,2,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

//判断前方是不是墙。返回false为有墙，true为无墙
bool DetermineWall(Vector position,int Direction)
{
		int x = position.x;
		int y = position.y;
		
	switch (Direction)
	{
	case 1:
	{
		if (Map[x - 1][y - 2].wall == true)
		{
			return false;
		}
		else if (Map[x][y - 2].wall == true)
		{
			return false;
		}
		else if (Map[x + 1][y - 2].wall == true)
		{
			return false;
		}
		return true;
		break;
	}
	case 2:
	{
		
		if (Map[x - 2][y - 1].wall == true)
		{
			return false;
		}
		else if (Map[x - 2][y].wall == true)
		{
			return false;
		}
		else if (Map[x - 2][y + 1].wall == true)
		{
			return false;
		}
		return true;
		break;
	}
	case 3:
	{
		if (Map[x - 1][y + 2].wall == true)
		{
			return false;
		}
		else if (Map[x][y + 2].wall == true)
		{
			return false;
		}
		else if (Map[x + 1][y + 2].wall == true)
		{
			return false;
		}
		return true;
		break;
	}
	case 4:
	{
		if (Map[x + 2][y - 1].wall == true)
		{
			return false;
		}
		else if (Map[x + 2][y].wall == true)
		{
			return false;
		}
		else if (Map[x + 2][y + 1].wall == true)
		{
			return false;
		}
		return true;
		break;
	}
	default:
		return true;
		break;
	}
}
//边界问题。根据当前位置和方向判断是否撞上了边界返回true
bool DetermineEdge(Vector position, int Direction)
{
	int x = position.x;
	int y = position.y;
	switch (Direction)
	{
	case 1:
	{
		if (position.y == -2)
			return true;
		break;
	}
	case 2:
	{
		if (position.x == -2)
			return true;
		break;
	}
	case 3:
	{
		if (position.y == 53)
			return true;
		break;
	}
	case 4:
	{
		if (position.x == 43)
			return true;
		break;
	}
	default:
		break;
	}
	return false;

}

//转化0，1，2为布尔值
bool GetBool(int a,int mode)
{
	switch (mode)
	{
	case 1:
	{
		if (a == 1)
			return true;
		else
			return false;
		break;
	}
	case 2:
	{
		if (a == 2)
			return true;
		else
			return false;
		break;
	}
	case 3:
	{
		if (a == 3)
			return true;
		else
			return false;
		break;
	}
	default:
	{
		cout << "未知布尔参数转化模式" << endl;
		break;
	}
	}
	
}
//加载图片为surface
SDL_Surface* GetSurface(const char* filePath)
{
	//surface
	SDL_Surface* surface = NULL;
	surface = IMG_Load(filePath);
	if (surface == NULL)
	{
		cout << "加载图片  "<<filePath<<"  失败" << endl;
	}
	if (surface != NULL)
	{
		cout << "加载图片  " << filePath<< "  成功" << endl;
	}
	
	//返回surface
	return surface;
}
//绘制
void Draw(int x, int y, SDL_Surface* surface)
{
	//将输入的surface转为纹理
	SDL_Texture* texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_Rect
	SDL_Rect destinationRect; // 目标位置和大小
	destinationRect.x = x; // x是目标位置的横坐标
	destinationRect.y = y; // y是目标位置的纵坐标
	destinationRect.w = surface->w; // 使用图像宽度作为目标宽度
	destinationRect.h = surface->h; // 使用图像高度作为目标高度
	SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
	//将渲染器的内容呈现到窗口上 此处放到每帧的最后执行
	//SDL_RenderPresent(renderer);
}
void Draw(int x, int y, SDL_Texture* texture)
{
	//SDL_Rect
	SDL_Rect destinationRect; // 目标位置和大小
	destinationRect.x = x; // x是目标位置的横坐标
	destinationRect.y = y; // y是目标位置的纵坐标
	destinationRect.w = 48; // 使用图像宽度作为目标宽度
	destinationRect.h = 48; // 使用图像高度作为目标高度
	SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
}

void Pinghua(Vector* drawVector, Vector valueVector, Uint32 themoveDelay, Uint32 theframeDelay)
{
	int x = valueVector.x;
	int y = valueVector.y;
	x = (x - 1) * 16;
	y = (y - 1) * 16;
	int srawmove = 16.0 / themoveDelay * theframeDelay + 1;

	if (((drawVector->x - x) * (drawVector->x - x) > 1) && ((drawVector->x - x) * (drawVector->x - x) < 2304))
	{
		if ((x - drawVector->x) > 0)
		{
			drawVector->x = srawmove + drawVector->x;
		}
		else if ((x - drawVector->x) < 0)
		{
			drawVector->x = -srawmove + drawVector->x;
		}
	}
	else if (((drawVector->x - x) * (drawVector->x - x) >= 2304))
	{
		drawVector->x = x;
	}
	else
	{
		//drawVector->x = x;
	}
	if (((drawVector->y - y) * (drawVector->y - y) > 1) && ((drawVector->y - y) * (drawVector->y - y) < 2304))
	{
		if ((y - drawVector->y) > 0)
		{
			drawVector->y = srawmove + drawVector->y;
		}
		else if ((y - drawVector->y) < 0)
		{
			drawVector->y = -srawmove + drawVector->y;
		}
	}
	else if (((drawVector->y - y) * (drawVector->y - y) >= 2304))
	{
		drawVector->y = y;
	}
	else
	{
		//drawVector->y = y;
	}
}

//被鬼抓到
void Catch(Character* theman, Vector* gui_Position)
{
	if (!kill)
	{
		if ((theman->Position.x - gui_Position->x) * (theman->Position.x - gui_Position->x) + (theman->Position.y - gui_Position->y) * (theman->Position.y - gui_Position->y) < 4)
		{
			theman->Position = { 20,38 };
			theman->Direction = 1;
			cout << "被鬼抓到了" << endl;
		}
	}
	else if (kill)
	{
		if ((theman->Position.x - gui_Position->x) * (theman->Position.x - gui_Position->x) + (theman->Position.y - gui_Position->y) * (theman->Position.y - gui_Position->y) < 4)
		{
			gui_Position->x = 20;
			gui_Position->y = 24;
			cout << "杀鬼了啊啊啊啊" << endl;
		}
	}
}
int findMin(int a, int b, int c, int d)
{
	//取第一个有效值为最小值
	int min;
	if (a != -1)
	{
		min = a;
	}
	else if (b != -1)
	{
		min = b;
	}
	else if (c != -1)
	{
		min = c;
	}
	else
	{
		min = d;
	}

	if ((b < min) && b != -1) {
		min = b;
	}

	if ((c < min) && c != -1) {
		min = c;
	}

	if ((d < min) && d != -1) {
		min = d;
	}

	return min;
}
int findMax(int a, int b, int c, int d)
{
	//取第一个有效值为最大值
	int max;
	if (a != -1)
	{
		max = a;
	}
	else if (b != -1)
	{
		max = b;
	}
	else if (c != -1)
	{
		max = c;
	}
	else
	{
		max = d;
	}

	if ((b > max) && b != -1) {
		max = b;
	}

	if ((c > max) && c != -1) {
		max = c;
	}

	if ((d > max) && d != -1) {
		max = d;
	}

	return max;
}//计算追踪的方向，应用方向及追踪
void Track(Character* gui, Vector destination, bool mode)
{
	//计算四个方向的距离
	int a;
	int b;
	int c;
	int d;
	if (DetermineWall(gui->Position, 1) && gui->Direction != 3)
	{
		a = (gui->Position.x - destination.x) * (gui->Position.x - destination.x) + (gui->Position.y - 1 - destination.y) * (gui->Position.y - 1 - destination.y);
	}
	else
	{
		a = -1;
	}
	if (DetermineWall(gui->Position, 2) && gui->Direction != 4)
	{
		b = (gui->Position.x - 1 - destination.x) * (gui->Position.x - 1 - destination.x) + (gui->Position.y - destination.y) * (gui->Position.y - destination.y);
	}
	else
	{
		b = -1;
	}
	if (DetermineWall(gui->Position, 3) && gui->Direction != 1)
	{
		c = (gui->Position.x - destination.x) * (gui->Position.x - destination.x) + (gui->Position.y + 1 - destination.y) * (gui->Position.y + 1 - destination.y);
	}
	else
	{
		c = -1;
	}
	if (DetermineWall(gui->Position, 4) && gui->Direction != 2)
	{
		d = (gui->Position.x + 1 - destination.x) * (gui->Position.x + 1 - destination.x) + (gui->Position.y - destination.y) * (gui->Position.y - destination.y);
	}
	else
	{
		d = -1;
	}
	//使用不同的模式
	if (!mode)
	{
		//根据abcd中的最小值，赋予方向，并移动
		int min = findMin(a, b, c, d);
		//cout << "a" << a << "  b" << b << "  c" << c << "  d" << d << "  Min" << min << endl;
		if (min == a)
		{
			gui->Direction = 1;
			gui->Position = { gui->Position.x, gui->Position.y - 1 };
			//cout << "a" << endl;
		}
		else if (min == b)
		{
			gui->Direction = 2;
			gui->Position = { gui->Position.x - 1, gui->Position.y };
			//cout << "b" << endl;
		}
		else if (min == c)
		{
			gui->Direction = 3;
			gui->Position = { gui->Position.x, gui->Position.y + 1 };
			//cout << "c" << endl;
		}
		else if (min == d)
		{
			gui->Direction = 4;
			gui->Position = { gui->Position.x + 1, gui->Position.y };
			//cout << "d" << endl;
		}
		if ((gui->Position.x == -1) && (gui->Position.y == 25))
		{
			gui->Position.x = 41;
			gui->Position.y = 25;
		}
		else if ((gui->Position.x == 41) && (gui->Position.y == 25))
		{
			gui->Position.x = -1;
			gui->Position.y = 25;
		}
	}
	else if (mode)
	{
		//根据abcd中的最大值，赋予方向，并移动
		int max = findMax(a, b, c, d);
		//cout << "a" << a << "  b" << b << "  c" << c << "  d" << d << "  Min" << max << endl;
		if (max == a)
		{
			gui->Direction = 1;
			gui->Position = { gui->Position.x, gui->Position.y - 1 };
			//cout << "a" << endl;
		}
		else if (max == b)
		{
			gui->Direction = 2;
			gui->Position = { gui->Position.x - 1, gui->Position.y };
			//cout << "b" << endl;
		}
		else if (max == c)
		{
			gui->Direction = 3;
			gui->Position = { gui->Position.x, gui->Position.y + 1 };
			//cout << "c" << endl;
		}
		else if (max == d)
		{
			gui->Direction = 4;
			gui->Position = { gui->Position.x + 1, gui->Position.y };
			//cout << "d" << endl;
		}
		if ((gui->Position.x == -1) && (gui->Position.y == 25))
		{
			gui->Position.x = 41;
			gui->Position.y = 25;
		}
		else if ((gui->Position.x == 41) && (gui->Position.y == 25))
		{
			gui->Position.x = -1;
			gui->Position.y = 25;
		}
	}
}

int main(int argc, char* argv[])
{
	//键盘输入的布尔值
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;

	
#pragma region 初始化

	//初始化SDL各种东西
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

	}
	//我也不知道是什么，总是和IMAGE有关
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		cout << "和IMAGE有关的初始化失败" << endl;
	}
	//创建窗口
	SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenW, ScreenH, SDL_WINDOW_SHOWN);
	if (window == nullptr) {

	}
	//创建渲染器
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr) {

	}

	//初始化地图数据
	for (int i = 0; i < Max; i++)
	{
		for (int j = 0; j < Max; j++)
		{
			//初始化墙的数据
			Map[i][j] =
			{
				{i ,j },
				GetBool(Wall[j][i],1)
			};
			//初始化豆子的数据
			Map[i][j].douzi = GetBool(Wall[j][i], 2);
			if (GetBool(Wall[j][i], 2))
			{
				sumdou += 1;
			}
			Map[i][j].bigdou = GetBool(Wall[j][i], 3);
			if (GetBool(Wall[j][i], 3))
			{
				sumdou += 1;
			}
			cout << Map[i][j].Position.x << Map[i][j].Position.y << endl;
		}
	}
	cout << "共有豆子：" << sumdou << endl;

	//初始化事件队列
	SDL_Event event;
	//初始化运行布尔值
	bool running = true;

	//底层墙的目标矩形区域
	SDL_Rect destinationRect_Walls; // 目标位置和大小
	destinationRect_Walls.x = 0; // x是目标位置的横坐标
	destinationRect_Walls.y = 0; // y是目标位置的纵坐标
	destinationRect_Walls.w = ScreenW; // 使用图像宽度作为目标宽度
	destinationRect_Walls.h = ScreenH; // 使用图像高度作为目标高度

#pragma region 加载图片surface和texture
	//墙
	SDL_Surface* surface_Wall = GetSurface("./images/wall.png");
	//吃豆人
	SDL_Surface* surface_Man1 = GetSurface("./images/man1.png");
	SDL_Surface* surface_Man2 = GetSurface("./images/man2.png");
	SDL_Surface* surface_Man3 = GetSurface("./images/man3.png");
	SDL_Surface* surface_Man4 = GetSurface("./images/man4.png");
	SDL_Texture* texture_Man1 = SDL_CreateTextureFromSurface(renderer, surface_Man1);
	SDL_Texture* texture_Man2 = SDL_CreateTextureFromSurface(renderer, surface_Man2);
	SDL_Texture* texture_Man3 = SDL_CreateTextureFromSurface(renderer, surface_Man3);
	SDL_Texture* texture_Man4 = SDL_CreateTextureFromSurface(renderer, surface_Man4);
	int mantexturenumber = 0;
	SDL_Texture* texture_Mans[4] = { texture_Man1 ,texture_Man2, texture_Man3, texture_Man4 };
	//豆子
	SDL_Surface* surface_Douzi = GetSurface("./images/douzi.png"); 
	//大豆子
	SDL_Surface* surface_BigDou = GetSurface("./images/bigdou.png");
	//鬼1
	SDL_Surface* surface_Gui11 = GetSurface("./images/gui11.png");
	SDL_Surface* surface_Gui12 = GetSurface("./images/gui12.png");
	SDL_Surface* surface_Gui13 = GetSurface("./images/gui13.png");
	SDL_Surface* surface_Gui14 = GetSurface("./images/gui14.png");
	SDL_Texture* texture_Gui11 = SDL_CreateTextureFromSurface(renderer, surface_Gui11);
	SDL_Texture* texture_Gui12 = SDL_CreateTextureFromSurface(renderer, surface_Gui12);
	SDL_Texture* texture_Gui13 = SDL_CreateTextureFromSurface(renderer, surface_Gui13);
	SDL_Texture* texture_Gui14 = SDL_CreateTextureFromSurface(renderer, surface_Gui14);
	int gui1texturenumber = 0;
	SDL_Texture* texture_Gui1s[4] = { texture_Gui11 ,texture_Gui12, texture_Gui13, texture_Gui14 };
	//鬼2
	SDL_Surface* surface_Gui21 = GetSurface("./images/gui21.png");
	SDL_Surface* surface_Gui22 = GetSurface("./images/gui22.png");
	SDL_Surface* surface_Gui23 = GetSurface("./images/gui23.png");
	SDL_Surface* surface_Gui24 = GetSurface("./images/gui24.png");
	SDL_Texture* texture_Gui21 = SDL_CreateTextureFromSurface(renderer, surface_Gui21);
	SDL_Texture* texture_Gui22 = SDL_CreateTextureFromSurface(renderer, surface_Gui22);
	SDL_Texture* texture_Gui23 = SDL_CreateTextureFromSurface(renderer, surface_Gui23);
	SDL_Texture* texture_Gui24 = SDL_CreateTextureFromSurface(renderer, surface_Gui24);
	int gui2texturenumber = 0;
	SDL_Texture* texture_Gui2s[4] = { texture_Gui21 ,texture_Gui22, texture_Gui23, texture_Gui24 };
	//鬼3
	SDL_Surface* surface_Gui31 = GetSurface("./images/gui31.png");
	SDL_Surface* surface_Gui32 = GetSurface("./images/gui32.png");
	SDL_Surface* surface_Gui33 = GetSurface("./images/gui33.png");
	SDL_Surface* surface_Gui34 = GetSurface("./images/gui34.png");
	SDL_Texture* texture_Gui31 = SDL_CreateTextureFromSurface(renderer, surface_Gui31);
	SDL_Texture* texture_Gui32 = SDL_CreateTextureFromSurface(renderer, surface_Gui32);
	SDL_Texture* texture_Gui33 = SDL_CreateTextureFromSurface(renderer, surface_Gui33);
	SDL_Texture* texture_Gui34 = SDL_CreateTextureFromSurface(renderer, surface_Gui34);
	int gui3texturenumber = 0;
	SDL_Texture* texture_Gui3s[4] = { texture_Gui31 ,texture_Gui32, texture_Gui33, texture_Gui34 };
	//鬼3
	//鬼4
	SDL_Surface* surface_Gui41 = GetSurface("./images/gui41.png");
	SDL_Surface* surface_Gui42 = GetSurface("./images/gui42.png");
	SDL_Surface* surface_Gui43 = GetSurface("./images/gui43.png");
	SDL_Surface* surface_Gui44 = GetSurface("./images/gui44.png");
	SDL_Texture* texture_Gui41 = SDL_CreateTextureFromSurface(renderer, surface_Gui41);
	SDL_Texture* texture_Gui42 = SDL_CreateTextureFromSurface(renderer, surface_Gui42);
	SDL_Texture* texture_Gui43 = SDL_CreateTextureFromSurface(renderer, surface_Gui43);
	SDL_Texture* texture_Gui44 = SDL_CreateTextureFromSurface(renderer, surface_Gui44);
	int gui4texturenumber = 0;
	SDL_Texture* texture_Gui4s[4] = { texture_Gui41 ,texture_Gui42, texture_Gui43, texture_Gui44 };
	//胜利
	SDL_Surface* surface_Win = GetSurface("./images/win.png");

	for (int i = 0; i < Max; i++)
	{
		for (int j = 0; j < Max; j++)
		{
			if (Map[i][j].wall == true)
			{
				Draw(i*16, j*16, surface_Wall);

			}
		}
	}

	//纹理全部墙
	SDL_Texture* Texture_Walls = NULL;
	SDL_Surface* surface_Walls = GetSurface("./images/walls.png");
	//表面全部墙
	//SDL_Surface* surface_Walls = SDL_CreateRGBSurface(0, ScreenW, ScreenH, 32, 0, 0, 0, 0);
	// 读取渲染器上的像素数据到表面全部墙
	//SDL_RenderReadPixels(renderer, NULL, surface_Walls->format->format, surface_Walls->pixels, surface_Walls->pitch);
	// 将表面全部墙转换为纹理全部墙
	Texture_Walls = SDL_CreateTextureFromSurface(renderer, surface_Walls);
	if (Texture_Walls != NULL)
	{
		cout << "表面墙加载成功" << endl;
	}
	else if (Texture_Walls == NULL)
	{
		cout << "表面墙加载失败" << endl;
	}
	//释放表面全部墙
	SDL_FreeSurface(surface_Walls);



#pragma endregion




	// 初始化上一帧的时间
	startTime = SDL_GetTicks();
#pragma endregion



	while (running) {
		//下面是每一个循环

			//清除渲染器
			//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
			//SDL_RenderClear(renderer);

#pragma region 处理事件
		//处理各种事件
		while (SDL_PollEvent(&event))
		{

			//退出程序事件
			if (event.type == SDL_QUIT)
			{
				running = false;
			}

#pragma region 键盘输入


			else if (event.type == SDL_KEYDOWN)
			{
				// 处理键盘按下事件
				SDL_Keycode keycode = event.key.keysym.sym;
				switch (keycode)
				{
				case SDLK_UP:
				{
					upPressed = true;
					break;
				}
				case SDLK_DOWN:
				{
					downPressed = true;
					break;
				}
				case SDLK_LEFT:
				{
					leftPressed = true;
					break;
				}
				case SDLK_RIGHT:
				{
					rightPressed = true;
					break;
				}
				// 处理其他键盘按键...
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				SDL_Keycode keycode = event.key.keysym.sym;
				switch (keycode)
				{
				case SDLK_UP:
				{
					upPressed = false;
					break;
				}
				case SDLK_DOWN:
				{
					downPressed = false;
					break;
				}
				case SDLK_LEFT:
				{
					leftPressed = false;
					break;
				}
				case SDLK_RIGHT:
				{
					rightPressed = false;
					break;
				}
				// 处理其他键盘按键...
				}
			}
			// 处理其他事件

		}
		if (upPressed) {
			if (DetermineWall(man.Position, 1))
			{
				man.Direction = 1;
			}
		}
		if (downPressed) {
			if (DetermineWall(man.Position, 3))
			{
				man.Direction = 3;
			}
		}
		if (leftPressed) {
			if (DetermineWall(man.Position, 2))
			{
				man.Direction = 2;
			}
		}
		if (rightPressed) {
			if (DetermineWall(man.Position, 4))
			{
				man.Direction = 4;
			}
		}
#pragma endregion

#pragma endregion




#pragma region 处理逻辑
		//吃豆人移动
		if ((SDL_GetTicks() - movestartTime) > moveDelay)
		{
			mantexturenumber = (mantexturenumber + 1) % 4;
			movestartTime = SDL_GetTicks();
			//移动
			switch (man.Direction)
			{
			case 1:
			{
				if (DetermineWall(man.Position, 1) && !DetermineEdge(man.Position,man.Direction))
				{

					man.Position = { man.Position.x + 0,man.Position.y - 1 };
				}
				else
				{
					mantexturenumber = mantexturenumber - 1;
					if (mantexturenumber == -1)
					{
						mantexturenumber = 3;
					}
				}
				break;
			}
			case 2:
			{
				if (DetermineWall(man.Position, 2) && !DetermineEdge(man.Position, man.Direction))
				{

					man.Position = { man.Position.x - 1,man.Position.y + 0 };
				}
				else
				{
					mantexturenumber = mantexturenumber - 1;
					if (mantexturenumber == -1)
					{
						mantexturenumber = 3;
					}
				}
				break;
			}
			case 3:
			{
				if (DetermineWall(man.Position, 3) && !DetermineEdge(man.Position, man.Direction))
				{

					man.Position = { man.Position.x + 0,man.Position.y + 1 };
				}
				else
				{
					mantexturenumber = mantexturenumber - 1;
					if (mantexturenumber == -1)
					{
						mantexturenumber = 3;
					}
				}
				break;
			}
			case 4:
			{
				if (DetermineWall(man.Position, 4) && !DetermineEdge(man.Position, man.Direction))
				{

					man.Position = { man.Position.x + 1,man.Position.y + 0 };
				}
				else
				{
					mantexturenumber = mantexturenumber - 1;
					if (mantexturenumber == -1)
					{
						mantexturenumber = 3;
					}
				}
				break;
			}
			default:
				break;

			}
			if ((man.Position.x == -1) && (man.Position.y == 25))
			{
				man.Position.x = 41;
				man.Position.y = 25;
			}
			else if ((man.Position.x == 41) && (man.Position.y == 25))
			{
				man.Position.x = -1;
				man.Position.y = 25;
			}

#pragma region 吃豆子和大豆子
			if (Map[man.Position.x - 1][man.Position.y - 1].douzi == true)
			{
				Map[man.Position.x - 1][man.Position.y - 1].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x - 1][man.Position.y].douzi == true)
			{
				Map[man.Position.x - 1][man.Position.y].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x - 1][man.Position.y + 1].douzi == true)
			{
				Map[man.Position.x - 1][man.Position.y + 1].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x][man.Position.y - 1].douzi == true)
			{
				Map[man.Position.x][man.Position.y - 1].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x][man.Position.y].douzi == true)
			{
				Map[man.Position.x][man.Position.y].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x][man.Position.y + 1].douzi == true)
			{
				Map[man.Position.x][man.Position.y + 1].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x + 1][man.Position.y - 1].douzi == true)
			{
				Map[man.Position.x + 1][man.Position.y - 1].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x + 1][man.Position.y].douzi == true)
			{
				Map[man.Position.x + 1][man.Position.y].douzi = false;
				eatendou += 1;
			}
			if (Map[man.Position.x + 1][man.Position.y + 1].douzi == true)
			{
				Map[man.Position.x + 1][man.Position.y + 1].douzi = false;
				eatendou += 1;
			}
			

			if (Map[man.Position.x - 1][man.Position.y - 1].bigdou == true)
			{
				Map[man.Position.x - 1][man.Position.y - 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x - 1][man.Position.y].bigdou == true)
			{
				Map[man.Position.x - 1][man.Position.y].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x - 1][man.Position.y + 1].bigdou == true)
			{
				Map[man.Position.x - 1][man.Position.y + 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x][man.Position.y - 1].bigdou == true)
			{
				Map[man.Position.x][man.Position.y - 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x][man.Position.y].bigdou == true)
			{
				Map[man.Position.x][man.Position.y].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x][man.Position.y + 1].bigdou == true)
			{
				Map[man.Position.x][man.Position.y + 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x + 1][man.Position.y - 1].bigdou == true)
			{
				Map[man.Position.x + 1][man.Position.y - 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x + 1][man.Position.y].bigdou == true)
			{
				Map[man.Position.x + 1][man.Position.y].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			if (Map[man.Position.x + 1][man.Position.y + 1].bigdou == true)
			{
				Map[man.Position.x + 1][man.Position.y + 1].bigdou = false;
				escape = true;
				kill = true;
				escapestartTime = SDL_GetTicks();
				killstartTime = SDL_GetTicks();
				eatendou += 1;
				gui1moveDelay = 245;
				gui2moveDelay = 270;
				gui3moveDelay = 270;
				gui4moveDelay = 270;
			}
			cout << "共吃豆子：" << eatendou << endl;
			Catch(&man, &gui1.Position);
			Catch(&man, &gui2.Position);
			Catch(&man, &gui3.Position);
			Catch(&man, &gui4.Position);
#pragma endregion
		}

		if ((SDL_GetTicks() - gui1movestartTime) > gui1moveDelay)
		{
			gui1texturenumber = (gui1texturenumber + 1) % 4;
			gui1movestartTime = SDL_GetTicks();
			Vector gui1destination = man.Position;
			Track(&gui1, gui1destination,escape);
			Catch(&man, &gui1.Position);
		}
		if ((SDL_GetTicks() - gui2movestartTime) > gui2moveDelay)
		{
			gui2texturenumber = (gui2texturenumber + 1) % 4;
			gui2movestartTime = SDL_GetTicks();
			Vector gui2destination = man.Position;
			switch (man.Direction)
			{
			case 1:
			{
				gui2destination = { gui2destination.x,gui2destination.y - 9 };
				break;
			}
			case 2:
			{
				gui2destination = { gui2destination.x - 9,gui2destination.y };
				break;
			}
			case 3:
			{
				gui2destination = { gui2destination.x,gui2destination.y + 9 };
				break;
			}
			case 4:
			{
				gui2destination = { gui2destination.x + 9,gui2destination.y };
				break;
			}
			default:
				break;
			}
			Track(&gui2, gui2destination, escape);
			Catch(&man, &gui2.Position);
		}
		if ((SDL_GetTicks() - gui3movestartTime) > gui3moveDelay)
		{
			gui3texturenumber = (gui3texturenumber + 1) % 4;
			gui3movestartTime = SDL_GetTicks();
			Vector gui3destination = man.Position;
			switch (man.Direction)
			{
			case 1:
			{
				gui3destination = { gui3destination.x,gui3destination.y - 6 };
				break;
			}
			case 2:
			{
				gui3destination = { gui3destination.x - 6,gui3destination.y };
				break;
			}
			case 3:
			{
				gui3destination = { gui3destination.x,gui3destination.y + 6 };
				break;
			}
			case 4:
			{
				gui3destination = { gui3destination.x + 6,gui3destination.y };
				break;
			}
			default:
				break;
			}
			gui3destination = { (2 * gui3destination.x - gui1.Position.x),(2 * gui3destination.y - gui1.Position.y) };
			Track(&gui3, gui3destination, escape);
			Catch(&man, &gui3.Position);
		}
		if ((SDL_GetTicks() - gui4movestartTime) > gui4moveDelay)
		{
			gui4texturenumber = (gui4texturenumber + 1) % 4;
			gui4movestartTime = SDL_GetTicks();
			Vector gui4destination = man.Position;
			int distance = (gui4.Position.x - man.Position.x) * (gui4.Position.x - man.Position.x) + (gui4.Position.y - man.Position.y) * (gui4.Position.y - man.Position.y);
			if (distance <= 12*12)
			{
				Track(&gui4, gui4destination, true);
			}
			else
			{
			Track(&gui4, gui4destination, escape);
			}
			Catch(&man, &gui4.Position);
		}
	
		//计时器，进入escape模式 或 kill模式 后启用，用于关闭escape模式 和kill模式 。
		if (SDL_GetTicks()-escapestartTime>escapeDelay)
		{
			escape = false;
		}
		if (SDL_GetTicks() - killstartTime > killDelay)
		{
			kill = false;
			gui1moveDelay = 85;
			gui2moveDelay = 90;
			gui3moveDelay = 90;
			gui4moveDelay = 90;
		}
		if (SDL_GetTicks() > 10000)
		{
			escape = true;
			escapestartTime = SDL_GetTicks();
		}
		if (SDL_GetTicks() > 11500)
		{
			escape = false;
			
		}
		if (SDL_GetTicks() > 22000)
		{
			escape = true;
			escapestartTime = SDL_GetTicks();
		}
		if (SDL_GetTicks() > 23000)
		{
			escape = false;
			
		}
		if (SDL_GetTicks() > 38000)
		{
			escape = true;
			escapestartTime = SDL_GetTicks();
		}
		if (SDL_GetTicks() > 38500)
		{
			escape = false;
			
		}
		if (SDL_GetTicks() > 54000)
		{
			escape = true;
			escapestartTime = SDL_GetTicks();
		}
		if (SDL_GetTicks() > 54100)
		{
			escape = false;
			
		}
#pragma endregion


#pragma region 渲染画面

		//endTime = SDL_GetTicks();
		//如果到下一帧的时间了，就开始绘制工作

		if ((SDL_GetTicks() - startTime) > frameDelay)
		{
			//cout << "绘制这一帧" << endl;

			// 清空渲染器
			SDL_RenderClear(renderer);
			//绘制底层——地图
			SDL_RenderCopy(renderer, Texture_Walls, NULL, &destinationRect_Walls);
			//绘制豆子
			SDL_Texture* Texture_Douzi = SDL_CreateTextureFromSurface(renderer, surface_Douzi);
			SDL_Rect destinationRect_Douzi; // 目标位置和大小
			destinationRect_Douzi.w = 48; // 使用图像宽度作为目标宽度
			destinationRect_Douzi.h = 48; // 使用图像高度作为目标高度
			//绘制大豆子
			SDL_Texture* Texture_BigDou = SDL_CreateTextureFromSurface(renderer, surface_BigDou);
			SDL_Rect destinationRect_BigDou; // 目标位置和大小
			destinationRect_BigDou.w = 48; // 使用图像宽度作为目标宽度
			destinationRect_BigDou.h = 48; // 使用图像高度作为目标高度
			
			
			for (int a = 0; a < Max; a++)
			{
				for (int b = 0; b < Max; b++)
				{
					if (Map[a][b].douzi == true)
					{
						//cout << "画豆子" << endl;
						destinationRect_Douzi.x = (a - 1) * 16;
						destinationRect_Douzi.y = (b - 1) * 16;
						SDL_RenderCopy(renderer, Texture_Douzi, NULL, &destinationRect_Douzi);

					}
					if (Map[a][b].bigdou == true)
					{
						//cout << "画大豆子" << endl;
						destinationRect_BigDou.x = (a - 1) * 16;
						destinationRect_BigDou.y = (b - 1) * 16;
						SDL_RenderCopy(renderer, Texture_BigDou, NULL, &destinationRect_BigDou);

					}
				}
			}
			
			
			//绘制吃豆人
			Pinghua(&mandrawVector, man.Position, moveDelay,frameDelay);
			Draw(mandrawVector.x,mandrawVector.y, texture_Mans[mantexturenumber]);
			//绘制鬼
			Pinghua(&gui1drawVector, gui1.Position, gui1moveDelay, frameDelay);
			Pinghua(&gui2drawVector, gui2.Position, gui2moveDelay, frameDelay);
			Pinghua(&gui3drawVector, gui3.Position, gui3moveDelay, frameDelay);
			Pinghua(&gui4drawVector, gui4.Position, gui4moveDelay, frameDelay);
			Draw(gui1drawVector.x, gui1drawVector.y, texture_Gui1s[gui1texturenumber]);
			Draw(gui2drawVector.x, gui2drawVector.y, texture_Gui2s[gui2texturenumber]);
			Draw(gui3drawVector.x, gui3drawVector.y, texture_Gui3s[gui3texturenumber]);
			Draw(gui4drawVector.x, gui4drawVector.y, texture_Gui4s[gui4texturenumber]);


			/*
			cout << "man的x为" << man.Position.x << endl;
			cout << "man的y为" << man.Position.y << endl;
			cout << "man的Direction为" << man.Direction << endl;
			*/

			//更新时间
			startTime = SDL_GetTicks();

			if (sumdou == eatendou)
			{
				frameDelay = 60000;
				if (SDL_GetTicks() > startTime + 1500)
				{
				Draw(0, 0, surface_Win);
				}
			}

			// 在每一帧的末尾，更新窗口显示
			SDL_RenderPresent(renderer);
		}
		if (sumdou == eatendou)
		{
			frameDelay = 60000;
			if (SDL_GetTicks() > startTime + 200)
			{
				Draw(0, 0, surface_Win);
			}
		SDL_RenderPresent(renderer);
		}

		




#pragma endregion

	}

	// 销毁窗口、渲染器和退出SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	IMG_Quit();  //清理SDL_Image资源

	return 0;
}
