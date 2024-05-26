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
			if (nx < 1 || nx > 3 || ny < 1 || ny > 3 || pre + i == 3) continue;//���������������Լ�֦
			swap(a[x][y], a[nx][ny]); add();
			if (test(step) && !judge) A_star(step + 1, nx, ny, i);//A*���ۺϷ�����������
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
void start();//��Ϸ���򣬿�ʼ�������
int rules();//���水ť
void init();//������Դ
void GameInit();//��Ϸ���ݳ�ʼ��
void DrawMap();//��Ϸ��Ⱦ
void BGM();//��������
void play();//��Ҳ���
void Judge();//�ж���Ӯ
int main() {
	//���ô��ڴ�С
	initgraph(6 * 100, 6 * 100);
	//����ͼƬ
	start();
	init();
	GameInit();
	DrawMap();
	while (1) {
		play();
		Judge();
	}
	system("pause");//�ȴ��û�����
	closegraph();
	return 0;
}
//��ʼ����
void start() {
	loadimage(NULL, L"cover.png", 600, 600);
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(60, 0, _T("����"), 0, 0, 4, false, false, false);
	outtextxy(180, 120, L"ƴͼ��Ϸ");                    //��Ϸ����
	settextstyle(30, 0, _T("΢���ź�"));
	setfillcolor(BROWN);
	setlinestyle(BS_SOLID, 5);
	setlinecolor(RED);
	fillroundrect(220, 220, 370, 270, 10, 10);
	settextstyle(30, 0, _T("����"), 0, 0, 6, false, false, false);  //��ʼ��ť
	outtextxy(270, 230, L"��ʼ");
	fillroundrect(220, 300, 370, 350, 10, 10);
	outtextxy(240, 310, L"��Ϸ����");
	setfillcolor(BROWN);
	setlinestyle(BS_SOLID, 5);
	setlinecolor(BLACK);
	fillcircle(490, 440, 30);  //���ֿ��ư�ť:��
	fillcircle(560, 440, 30);  //���ֿ��ư�ť:��
	outtextxy(380, 430, L"����:");
	setfillcolor(BLACK);
	POINT pts[] = { {481,425},{481,455},{507,440} };
	fillpolygon(pts, 3);
	fillrectangle(546, 425, 554, 455);
	fillrectangle(566, 425, 574, 455);
	rules();
}
//��Ϸ��ʼ��
void init()
{
	//������ԴͼƬ��4��ͼ4���ؿ�
	loadimage(&img[0], L"picture1.jpg", 600, 600);
	loadimage(&img[1], L"picture2.png", 600, 600);
	loadimage(&img[2], L"picture3.jpg", 600, 600);
	loadimage(&img[3], L"picture4.png", 600, 600);
	//�������һ��ͼƬΪ�հ�ͼƬ����ΪĿ��ͼƬ
	loadimage(&imgs[8], L"white.jpg", 200, 200);
	//�����������
	srand((unsigned)time(NULL));
}
//����ѡ���
int rules()
{
	ExMessage Mou;    //�����Ϣ
	while (1)
	{
		Mou = getmessage(EX_MOUSE);
		switch (Mou.message)    //�������Ϣ����ƥ��
		{
		case WM_LBUTTONDOWN:            //�������
			if (Mou.x >= 220 && Mou.x <= 370 && Mou.y >= 300 && Mou.y <= 350)
			{
				HWND hwnd = GetHWnd();
				MessageBox(NULL, L"1.����������հ�ͼ����ΧͼƬ����λ��\n2.����Ҽ����⴦������ʾ����ͼƬ\n3.����м���������ͼƬ\n4.��Esc�����ط���", L"��Ϸ����", MB_OKCANCEL);
				break;                     //����ť
			}
			if (Mou.x >= 220 && Mou.x <= 370 && Mou.y >= 220 && Mou.y <= 270)
			{
				return 0;                  //��ʼ��ť
			}
			if (Mou.x >= 460 && Mou.x <= 520 && Mou.y >= 410 && Mou.y <= 470)
			{
				BGM();                     //���ֲ��Ű�ť
				break;
			}
			if (Mou.x >= 530 && Mou.x <= 590 && Mou.y >= 410 && Mou.y <= 470)
			{
				mciSendString(L"close back", 0, 0, 0);     //���ֹرհ�ť
				break;
			}
		}
	}
}
//ƴͼ���캯��
void GameInit()
{
	//��ƴͼ����ȥ
	putimage(0, 0, &img[NUM]);
	//���û�ͼĿ��Ϊimg����   ��ƴͼͼƬ�����и�
	SetWorkingImage(&img[NUM]);
	for (int y = 0, n = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (n == 8)	break;
			//��ȡ100*100����ͼƬ���洢��img�У�
			getimage(&imgs[n++], x * 200, y * 200, (x + 1) * 200, (y + 1) * 200);
		}
	}
	//���û�ͼĿ��Ϊ��ͼ����
	SetWorkingImage();

	//��ʼ����ͼ0~15
	for (int i = 0, k = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			map[i][j] = k++;
		}
	}
	//���ҵ�ͼ
	for (int k = 0; k <= 1000; k++)
	{
		//�õ�Ŀ�����ڵ��к���
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 8)  //�հ�ͼƬ��Ϊ����Ŀ��
				{
					aim_r = i;
					aim_c = j;
					break;
				}
			}
		}
		//һǧ�δ���˳��֮����Ҫ���հ�ͼƬת�Ƶ����½�
		//���Է�װ�ɺ��������������
		if (k == 1000)
		{
			//���հ�ͼƬѭ��ת�Ƶ����½�
			while (aim_r < 2)
			{
				//��֤�հ�Ŀ��������
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 8;
				aim_r++;
			}
			while (aim_c < 2)
			{
				//��֤�հ�Ŀ��������
				map[aim_r][aim_c] = map[aim_r][aim_c + 1];
				map[aim_r][aim_c + 1] = 8;
				aim_c++;
			}
			return;
		}

		int dir = rand() % 4;   //���һ������
		switch (dir)
		{
		case 0:  //���Ͻ���
			if (aim_r >= 1)
			{
				//�հ�ͼƬ�Ϳհ״������ͼƬ����
				map[aim_r][aim_c] = map[aim_r - 1][aim_c];
				map[aim_r - 1][aim_c] = 8;
				break;
			}
		case 1:  //���½���
			if (aim_r < 2)
			{
				//�հ�ͼƬ�Ϳհ״������ͼƬ����
				map[aim_r][aim_c] = map[aim_r + 1][aim_c];
				map[aim_r + 1][aim_c] = 8;
				break;
			}
		case 2:  //���󽻻�
			if (aim_c >= 1)
			{
				//�հ�ͼƬ�Ϳհ״���ߵ�ͼƬ����
				map[aim_r][aim_c] = map[aim_r][aim_c - 1];
				map[aim_r][aim_c - 1] = 8;
				break;
			}
		case 3:  //���ҽ���
			if (aim_c < 2)
			{
				//�հ�ͼƬ�Ϳհ״��ұߵ�ͼƬ���� 
				map[aim_r][aim_c] = map[aim_r][aim_c + 1];
				map[aim_r][aim_c + 1] = 8;
				break;
			}
		}
	}
}
//��ͼ����
void DrawMap()
{
	FlushBatchDraw();  //��ʼ��ȾͼƬ
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			putimage(x * 200, y * 200, &imgs[map[y][x]]);
		}
	}
	EndBatchDraw();
}
//�������ֺ���
void BGM()
{
	//�����֣���������
	mciSendStringW(L"open m1.mp3 alias back", NULL, 0, NULL);
	mciSendStringW(_T("play back repeat"), 0, 0, 0);
}
//���ݸ��º���
void play() {
	int col, row;  //�������λ��
	ExMessage msg;    //�����Ϣ
	msg = getmessage(EX_MOUSE | EX_KEY);   //��ȡ�����Ϣ
	switch (msg.message)      //�������Ϣ����ƥ��
	{
	case WM_LBUTTONDOWN:  //�������Ϣ���������ʱ
		//��ȡ��갴��������
		col = msg.x / 200;
		if (msg.x == 600)
			col = 2;
		//��ȡ��갴��������
		row = msg.y / 200;
		if (msg.y == 600)
			row = 2;
		//�õ�Ŀ�������к���
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (map[i][j] == 8)    //�հ״�Ϊ����Ŀ��
				{
					aim_r = i;
					aim_c = j;
				}
			}
		}
		//�ж������λ�ú�Ŀ���Ƿ����ڣ����ڽ�������
		if (row == aim_r && col == aim_c + 1 ||
			row == aim_r && col == aim_c - 1 ||
			row == aim_r + 1 && col == aim_c ||
			row == aim_r - 1 && col == aim_c)
		{
			//�����ͼƬ�Ϳհ�Ŀ��ͼƬ����
			map[aim_r][aim_c] = map[row][col];
			map[row][col] = 8;
		}
		DrawMap();
		break;
	case WM_RBUTTONDOWN: //�������Ϣ���Ҽ�����ʱ
		putimage(0, 0, &img[NUM]);   //���ؿ�ͼƬ����������
		break;
	case WM_RBUTTONUP:  //�������Ϣ���Ҽ�̧��ʱ
		DrawMap();
		break;
	case WM_MBUTTONDOWN:
		NUM++;
		if (NUM == 4)
			NUM = 0;   //���ص�һ��ͼ
		//���¿�ʼ��Ϸ
		GameInit(); //��Ϸ��ʼ��
		DrawMap();  //��Ⱦ��ͼ
		break;
	case WM_KEYDOWN:
		if (msg.vkcode == VK_ESCAPE)    //��Esc�����ط���
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
//ͨ���жϺ���
void Judge()
{
	//�жϵ�ǰÿ��ͼƬ�Ƿ��ڶ�Ӧλ��
	if (map[0][0] == 0 && map[0][1] == 1 && map[0][2] == 2 &&
		map[1][0] == 3 && map[1][1] == 4 && map[1][2] == 5 &&
		map[2][0] == 6 && map[2][1] == 7 && map[2][2] == 8)
	{
		//��ս�ɹ�֮��ȫͼ����
		putimage(0, 0, &img[NUM++]);
		//�ĸ��ؿ���ʤ��֮���˳�����
		if (NUM == 4)
		{
			MessageBox(GetHWnd(), L"��ս�ɹ�", L"Vectory", MB_OK);
			exit(0);
			return;
		}
		//ÿ��һ���ؿ��ж��Ƿ������һ���ؿ�
		if (MessageBox(GetHWnd(), L"�Ƿ������һ��", L"Vectory", MB_YESNO) == IDYES)
		{
			//���¿�ʼ��Ϸ
			GameInit(); //��Ϸ��ʼ��
			DrawMap();  //��Ⱦ��ͼ
		}
		//�˳���Ϸ
		else exit(0);
	}
}