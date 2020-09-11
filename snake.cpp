#include <graphics.h>
#include <conio.h>

#define WIDTH 40//横的有四十个格子
#define HEIGHT 30
#define SIZE 20

int Blocks[WIDTH][HEIGHT] = { 0 };
char moveDirection,oldMoveDirection;
int foodX, foodY;
int isFailure = 0;

void initGame();
void show();//每一次运行时重新绘制蛇和食物的图像
void updateWithInput();//与输入有关的更新
void updateWithoutInput();//与输入无关的更新
void moveSnake();//蛇运动

int main() {
	initGame();
	while (1) {
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
}

void initGame() {
	initgraph(WIDTH * SIZE, HEIGHT * SIZE);
	setbkcolor(LIGHTGRAY);
	BeginBatchDraw();
	cleardevice();
	setlinecolor(WHITE);
	int i;
	for (i = SIZE; i < WIDTH * SIZE; i += SIZE)
		line(i, 0, i, HEIGHT * SIZE);
	for (i = SIZE; i < HEIGHT * SIZE; i += SIZE)
		line(0, i, WIDTH * SIZE, i);
	moveDirection = oldMoveDirection = 'd';
	Blocks[WIDTH / 2][HEIGHT / 2] = 1;
	for (i = 1; i < 4; i++)
		Blocks[WIDTH / 2 - i][HEIGHT / 2] = i + 1;
	foodX = rand() % (WIDTH - 2) + 1;//1到38
	foodY = rand() % (HEIGHT - 2) + 1;
}

void show() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (Blocks[i][j] != 0)
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else
				setfillcolor(LIGHTGRAY);
			fillrectangle(i * SIZE, j * SIZE, (i + 1) * SIZE, (j + 1) * SIZE);
		}
	}
	setfillcolor(LIGHTGREEN);
	fillrectangle(foodX * SIZE, foodY * SIZE, (foodX + 1) * SIZE, (foodY + 1) * SIZE);

	if (isFailure) {
		settextcolor(LIGHTRED);
		settextstyle(80, 0, _T("宋体"));
		setbkmode(TRANSPARENT);
		outtextxy(240, 220, _T("游戏失败"));
	}

	FlushBatchDraw();
}

void updateWithInput() {
	if (_kbhit() && isFailure == 0) {
		char input = _getch();
		if (input == 'a' || input == 's' || input == 'd' || input == 'w') {
			moveDirection = input;
			if (moveDirection == 'a' && oldMoveDirection == 'd')
				moveDirection = 'd';
			else if (moveDirection == 's' && oldMoveDirection == 'w')
				moveDirection = 'w';
			else if (moveDirection == 'd' && oldMoveDirection == 'a')
				moveDirection = 'a';
			else if (moveDirection == 'w' && oldMoveDirection == 's')
				moveDirection = 's';
			else oldMoveDirection = input;
			moveSnake();
		}
	}
}

void updateWithoutInput() {
	if (isFailure)
		return;
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 6) {
		moveSnake();
		waitIndex = 1;
	}
}

void moveSnake() {
	int i, j;
	//日常对蛇身更新
	for (i = 0; i < WIDTH; i++)
		for (j = 0; j < HEIGHT; j++)
			if (Blocks[i][j] != 0)
				Blocks[i][j]++;
	//寻找旧蛇头和旧蛇尾的xy坐标
	int oldHeadX, oldHeadY, oldTailX, oldTailY;
	int tailBlocks = 0;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			if (tailBlocks < Blocks[i][j]) {
				tailBlocks = Blocks[i][j];
				oldTailX = i;
				oldTailY = j;
			}
			if (Blocks[i][j] == 2) {
				oldHeadX = i;
				oldHeadY = j;
			}
		}
	}
	//根据蛇的移动方向来赋值新蛇头的xy坐标
	int newHeadX = oldHeadX, newHeadY = oldHeadY;
	switch (moveDirection) {
	case'a':
		newHeadX -= 1;
		break;
	case 's':
		newHeadY += 1;
		break;
	case 'd':
		newHeadX += 1;
		break;
	case 'w':
		newHeadY -= 1;
		break;
	}
	//判断游戏是否失败
	if (newHeadX >= WIDTH || newHeadX < 0 || newHeadY >= HEIGHT || newHeadY < 0 || Blocks[newHeadX][newHeadY] != 0) {
		isFailure = 1;
		return;
	}
	//对蛇头的处理
	Blocks[newHeadX][newHeadY] = 1;
	//对吃到食物以及蛇尾的处理
	if (newHeadX == foodX && newHeadY == foodY) {
		foodX = rand() % (WIDTH - 2) + 1;
		foodY = rand() % (HEIGHT - 2) + 1;
	}
	else
		Blocks[oldTailX][oldTailY] = 0;
}
