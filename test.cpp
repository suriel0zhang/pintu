#include<conio.h>
#include<stdio.h>
#include<easyx.h>
#include<time.h>
#include<Windows.h>
#include<mmsystem.h>
#include<vector>
#include<iostream>
#pragma comment(lib,"Winmm.lib") 
using namespace std;
constexpr auto N = 3;
IMAGE img[4], imgs[9];
int aim_c, aim_r;
int map[3][3] = { 0 };
int NUM = 0;

class pack_auto {
private:
	const int ans[4][4] = { {0,0,0,0}, {0,0,1,2}, {0,3,4,5}, {0,6,7,8} };
	int judge = 0;
	const int nxtx[4] = { 0,1,-1,0 }, nxty[4] = { 1,0,0,-1 };
	vector<vector<int>> a;
	vector<vector<int>> succed, steparr;
public:
	int k = 0;
	int get_judge() { return judge; }
	vector<vector<int>> get_ans() { return steparr; }
	void set_a(const vector<vector<int>>& x) { a = x; }
	pack_auto() {
		for (int i = 1; i <= 4; ++i) {
			a.push_back(vector<int>(4));
		}
	}
	int check() {
		for (int i = 1; i <= 3; ++i)
			for (int j = 1; j <= 3; ++j)
				if (ans[i][j] != a[i][j])return 0;
		return 1;
	}
	int test(int step) {
		int cnt = 0;
		for (int i = 1; i <= 3; ++i)
			for (int j = 1; j <= 3; ++j)
				if (ans[i][j] != a[i][j]) { if (++cnt + step > k) return 0; }
		return 1;
	}
	void show() {
		for (int i = 1; i <= 3; i++) {
			for (int j = 1; j <= 3; j++) {
				cout << a[i][j] << ' ';
			}
			cout << endl;
		}
		cout << "-------------" << endl;
	}
	void add() {
		vector<int> tmp;
		for (int i = 1; i <= 3; i++) {
			for (int j = 1; j <= 3; j++) {
				tmp.push_back(a[i][j]);
			}
		}
		succed.push_back(tmp);
	}
	void del() {
		succed.pop_back();
	}
	void show_step() {
		//		for(auto k : succed) {
		//			cout << k[0] << ' ' << k[1] << ' ' << k[2] << endl;
		//			cout << k[3] << ' ' << k[4] << ' ' << k[5] << endl;
		//			cout << k[6] << ' ' << k[7] << ' ' << k[8] << endl;
		//			cout << "-------------" << endl;
		//		}
		steparr = succed;
	}
	void A_star(int step, int x, int y, int pre) {
		if (step == k) { if (check())judge = 1, show_step(); return; }
		if (judge) { return; }
		for (int i = 0; i < 4; ++i) {
			int nx = x + nxtx[i], ny = y + nxty[i];
			if (nx < 1 || nx > 3 || ny < 1 || ny > 3 || pre + i == 3) continue;//加入了上述最优性剪枝
			swap(a[x][y], a[nx][ny]); add();
			if (test(step) && !judge) A_star(step + 1, nx, ny, i);//A*估价合法再向下搜索
			swap(a[x][y], a[nx][ny]); del();
		}
	}
};
vector<vector<int>> work() {
	int x, y;
	pack_auto sol;
	vector<vector<int>> a(4, vector<int>(4));
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			a[i + 1][j + 1] = map[i][j];
			if (a[i + 1][j + 1] == 8) x = i + 1, y = j + 1;
		}
	sol.set_a(a);
	while (++sol.k) {
		sol.A_star(0, x, y, -1);
		if (sol.get_judge()) { break; }
	}
	return sol.get_ans();
}
void start();//游戏规则，开始界面设计
int rules();//封面按钮
void init();//加载资源
void GameInit();//游戏数据初始化
void DrawMap();//游戏渲染
void BGM();//播放音乐
void play();//玩家操作
void Judge();//判断输赢
int main() {
	//设置窗口大小
	initgraph(6 * 100, 6 * 100);
	//设置图片
	start();
	init();
	GameInit();
	DrawMap();
	while (1) {
		play();
		Judge();
	}
	system("pause");//等待用户按键
	closegraph();
	return 0;
}
//开始界面
void start() {
	loadimage(NULL, L"cover.png", 600, 600);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(60, 0, _T("楷体"), 0, 0, 4, false, false, false);
	outtextxy(180, 120, L"拼图游戏");                    //游戏名称
	settextstyle(30, 0, _T("微软雅黑"));
	setfillcolor(BROWN);
	setlinestyle(BS_SOLID, 5);
	setlinecolor(RED);
	fillroundrect(220, 220, 370, 270, 10, 10);
	settextstyle(30, 0, _T("宋体"), 0, 0, 6, false, false, false);  //开始按钮
	outtextxy(270, 230, L"开始");
	fillroundrect(220, 300, 370, 350, 10, 10);
	outtextxy(240, 310, L"游戏规则");
	setfillcolor(BROWN);
	setlinestyle(BS_SOLID, 5);
	setlinecolor(BLACK);
	fillcircle(490, 440, 30);  //音乐控制按钮:开
	fillcircle(560, 440, 30);  //音乐控制按钮:关
	outtextxy(380, 430, L"音乐:");
	setfillcolor(BLACK);
	POINT pts[] = { {481,425},{481,455},{507,440} };
	fillpolygon(pts, 3);
	fillrectangle(546, 425, 554, 455);
	fillrectangle(566, 425, 574, 455);
	rules();
}
//游戏初始化
void init()
{
	//加载资源图片，4张图4个关卡
	loadimage(&img[0], L"picture1.jpg", 600, 600);
	loadimage(&img[1], L"picture2.png", 600, 600);
	loadimage(&img[2], L"picture3.jpg", 600, 600);
	loadimage(&img[3], L"picture4.png", 600, 600);
	//设置最后一张图片为空白图片，作为目标图片
	loadimage(&imgs[8], L"white.jpg", 200, 200);
	//设置随机种子
	srand((unsigned)time(NULL));
}
//封面选项函数
int rules()
{
	ExMessage Mou;    //鼠标消息
	while (1)
	{
		Mou = getmessage(EX_MOUSE);
		switch (Mou.message)    //对鼠标信息进行匹配
		{
		case WM_LBUTTONDOWN:            //按下左键
			if (Mou.x >= 220 && Mou.x <= 370 && Mou.y >= 300 && Mou.y <= 350)
			{
				HWND hwnd = GetHWnd();
				MessageBox(NULL, L"1.鼠标左键点击空白图处周围图片交换位置\n2.鼠标右键任意处按下显示参照图片\n3.鼠标中键更换背景图片\n4.按Esc键返回封面", L"游戏规则", MB_OKCANCEL);
				break;                     //规则按钮
			}
			if (Mou.x >= 220 && Mou.x <= 370 && Mou.y >= 220 && Mou.y <= 270)
			{
				return 0;                  //开始按钮
			}
			if (Mou.x >= 460 && Mou.x <= 520 && Mou.y >= 410 && Mou.y <= 470)
			{
				BGM();                     //音乐播放按钮
				break;
			}
			if (Mou.x >= 530 && Mou.x <= 590 && Mou.y >= 410 && Mou.y <= 470)
			{
				mciSendString(L"close back", 0, 0, 0);     //音乐关闭按钮
				break;
			}
		}
	}
}
//拼图构造函数
void GameInit()
{
	//把拼图贴上去
	putimage(0, 0, &img[NUM]);
	//设置绘图目标为img对象   对拼图图片进行切割
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (n == 8)	break;
			//获取100*100像素图片，存储在img中；
			getimage(&imgs[n++], x * 200, y * 200, (x + 1) * 200, (y + 1) * 200);
		}
	}
	//设置绘图目标为绘图窗口
	SetWorkingImage();

	//初始化地图0~15
	for (int i = 0, k = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			map[i][j] = k++;
		}
	}
	//打乱地图
	for (int k = 0; k <= 1000; k++)
	{
		//得到目标所在的行和列
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 8)  //空白图片作为交换目标
				{
					aim_r = i;
					aim_c = j;
					break;
				}
			}
		}
		//一千次打乱顺序之后需要将空白图片转移到右下角
		//可以封装成函数下面这个代码
		if (k == 1000)
		{
			//将空白图片循环转移到右下角
			while (aim_r < 2)
			{
				//保证空白目标在最下
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 8;
				aim_r++;
			}
			while (aim_c < 2)
			{
				//保证空白目标在最右
				map[aim_r][aim_c] = map[aim_r][aim_c + 1];
				map[aim_r][aim_c + 1] = 8;
				aim_c++;
			}
			return;
		}

		int dir = rand() % 4;   //随机一个方向
		switch (dir)
		{
		case 0:  //向上交换
			if (aim_r >= 1)
			{
				//空白图片和空白处上面的图片交换
				map[aim_r][aim_c] = map[aim_r - 1][aim_c];
				map[aim_r - 1][aim_c] = 8;
				break;
			}
		case 1:  //向下交换
			if (aim_r < 2)
			{
				//空白图片和空白处下面的图片交换
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 8;
				break;
			}
		case 2:  //向左交换
			if (aim_c >= 1)
			{
				//空白图片和空白处左边的图片交换
				map[aim_r][aim_c] = map[aim_r][aim_c - 1];
				map[aim_r][aim_c - 1] = 8;
				break;
			}
		case 3:  //向右交换
			if (aim_c < 2)
			{
				//空白图片和空白处右边的图片交换 
				map[aim_r][aim_c] = map[aim_r][aim_c + 1];
				map[aim_r][aim_c + 1] = 8;
				break;
			}
		}
	}
}
//绘图函数
void DrawMap()
{
	FlushBatchDraw();  //开始渲染图片
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			putimage(x * 200, y * 200, &imgs[map[y][x]]);
		}
	}
	EndBatchDraw();
}
//背景音乐函数
void BGM()
{
	//打开音乐，播放音乐
	mciSendStringW(L"open m1.mp3 alias back", NULL, 0, NULL);
	mciSendStringW(_T("play back repeat"), 0, 0, 0);
}
//数据更新函数
void play() {
	int col, row;  //鼠标点击的位置
	ExMessage msg;    //鼠标消息
	msg = getmessage(EX_MOUSE | EX_KEY);   //获取鼠标消息
	switch (msg.message)      //对鼠标消息进行匹配
	{
	case WM_LBUTTONDOWN:  //当鼠标消息是左键按下时
		//获取鼠标按下所在列
		col = msg.x / 200;
		if (msg.x == 600)
			col = 2;
		//获取鼠标按下所在行
		row = msg.y / 200;
		if (msg.y == 600)
			row = 2;
		//得到目标所在行和列
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 8)    //空白处为交换目标
				{
					aim_r = i;
					aim_c = j;
				}
			}
		}
		//判断鼠标点击位置和目标是否相邻，相邻交换数据
		if (row == aim_r && col == aim_c + 1 ||
			row == aim_r && col == aim_c - 1 ||
			row == aim_r + 1 && col == aim_c ||
			row == aim_r - 1 && col == aim_c)
		{
			//鼠标点击图片和空白目标图片交换
			map[aim_r][aim_c] = map[row][col];
			map[row][col] = 8;
		}
		DrawMap();
		break;
	case WM_RBUTTONDOWN: //当鼠标消息是右键按下时
		putimage(0, 0, &img[NUM]);   //将关卡图片贴到窗口上
		break;
	case WM_RBUTTONUP:  //当鼠标消息是右键抬起时
		DrawMap();
		break;
	case WM_MBUTTONDOWN:
		NUM++;
		if (NUM == 4)
			NUM = 0;   //返回第一张图
		//重新开始游戏
		GameInit(); //游戏初始化
		DrawMap();  //渲染地图
		break;
	case WM_KEYDOWN:
		if (msg.vkcode == VK_ESCAPE)    //按Esc键返回封面
		{
			start();
			break;
		}
		if (msg.vkcode == VK_SPACE) {
			vector<vector<int>> ans = work();
			for (auto& k : ans) {
				map[0][0] = k[0]; map[0][1] = k[1]; map[0][2] = k[2];
				map[1][0] = k[3]; map[1][1] = k[4]; map[1][2] = k[5];
				map[2][0] = k[6]; map[2][1] = k[7]; map[2][2] = k[8];
				DrawMap();
				Sleep(200);
			}
			break;
		}
	}
}
//通关判断函数
void Judge()
{
	//判断当前每张图片是否在对应位置
	if (map[0][0] == 0 && map[0][1] == 1 && map[0][2] == 2 &&
		map[1][0] == 3 && map[1][1] == 4 && map[1][2] == 5 &&
		map[2][0] == 6 && map[2][1] == 7 && map[2][2] == 8)
	{
		//挑战成功之后将全图贴上
		putimage(0, 0, &img[NUM++]);
		//四个关卡都胜利之后退出程序
		if (NUM == 4)
		{
			MessageBox(GetHWnd(), L"挑战成功", L"Vectory", MB_OK);
			exit(0);
			return;
		}
		//每过一个关卡判断是否进入下一个关卡
		if (MessageBox(GetHWnd(), L"是否进入下一关", L"Vectory", MB_YESNO) == IDYES)
		{
			//重新开始游戏
			GameInit(); //游戏初始化
			DrawMap();  //渲染地图
		}
		//退出游戏
		else exit(0);
	}
}