#include "init.h"
#include "Rabbit.h"
#include "Turtle.h"


bool IsStartStory = false;
bool IsBossStory = false;
bool IsEndStory = false;


bool GetStartStory()
{
	return IsStartStory;
}

void SetStartStory(bool src)
{
	IsStartStory = src;
}

bool GetBossStory()
{
	return IsBossStory;
}

void SetBossStory(bool src)
{
	IsBossStory = src;
}

bool GetEndStory()
{
	return IsEndStory;
}

void SetEndStory(bool src)
{
	IsEndStory = src;
}


char g_RabbitColor = E_White;
char g_TurtleColor = E_White;

//스토리 화면 틀
char StoryFrame[SCREEN_HEIGHT][SCREEN_WIDTH] = 
{
"∥============================================================================∥",
"∥                                                                 skip(q) >> ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥____________________________________________________________________________∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥                                                                            ∥",
"∥============================================================================∥"
};

//스토리 토끼
char StoryRabbit[RabbitY][8] =
{
	" (\\(\\",
	" ('-')",
	"o(   )"
};

//스토리 자라
char StoryTurtle[TURTLE_HEIGHT][20] =
{
	"       ______       ",
	" ___ _/ \__/ \_   /|",
	"(_o / \ /  \ / \_/ |",
	" \__ -----------__/ ",
	"     \_|_|_|_|_/    ",
	"    /_|_|  /_|_|    ",
};		


typedef enum StoryStatus
{
	E_Story = 0,  // 해설
	E_Rabbit, // 토끼
	E_Turtle, // 자라
} StoryStatus;

typedef struct Story
{
	StoryStatus status; //enum (토끼인지 자라인지 해설인지)
	char ment[STR_MAX];
}Story;


Story StartStory[] = {
	{E_Story, "[아아아주 옛날 호랭이가 담배피던 시절...]"},
	{E_Story, "[용왕에게 관심받고 싶던 자라는 토끼의 간이 건강에 좋다는 소문을 들었다.]"},
	{E_Story, "[용왕의 관심을 받기 위해 토끼를 꼬드기기 시작하는데...]"},
	{E_Turtle ,"안녕! 우리 용궁에서 당근 1+1 이벤트 진행중인데 너도 올래?"},
	{E_Rabbit, "정말로? 길이 어디야!" },
	{E_Turtle, "나를 따라와 내가 안내해줄게!"},
	{E_Story, "[그리고 용궁]"},
	{E_Rabbit, "으응? 여기는 어디야? 당근은 어디에 있어?"},
	{E_Turtle, "...으하하하!!! 드디어 토끼의 간을 얻게 되었구만!!"},
	{E_Rabbit, "?!"},
	{E_Turtle, "간은 숙성시키는게 좋다 했으니까.. 감옥에 가둬놓는게 좋겠지?"},
	{E_Rabbit, "풀어줘!!!"},
	{E_Turtle, "숙성 후에 보자고. 크하하!"}
};

Story BossStory[] = {
	{E_Turtle, "?! 뭐야!! 네가 왜 거기서 나와!!"},
	{E_Rabbit, "너에게 줄 간은 없어!!" },
	{E_Turtle, "그렇게 나오시겠다? 좋아 승부야!" },
	{E_Turtle, "네가 이기면 특별히 간은 내어주지." },
	{E_Rabbit, "원래부터 내 간이었어! 덤벼!" }
};

Story EndStory[] = {
	{E_Turtle, "으악! 안돼 내 간!!"},
	{E_Rabbit, "뭐가 네 간이야! 잘 있어라 멍청한 자라!"},
	{E_Story, "[그렇게 토끼는 간을 뺏기지 않고 행복하게 살았답니다~!]"}
};


void CharacterDraw()
{
	// 토끼 그리기
	for (int y = 0; y < RabbitY; y++)
	{
		_DrawTextColor(6, 14 + y, StoryRabbit[y], g_RabbitColor);
	}

	// 자라 그리기
	for (int y = 0; y < TURTLE_HEIGHT; y++)
	{
		_DrawTextColor(55, 11 + y, StoryTurtle[y], g_TurtleColor);
	}
}

void PlayStartStory()
{
	int StartStoryCount = sizeof(StartStory) / sizeof(StartStory[0]);

	for (int i = 0; i < StartStoryCount; i++)
	{
		switch (StartStory[i].status) {
		case E_Story:
			// 멘트 출력
			_DrawTextColor(4, 19, StartStory[i].ment, E_White);

			// 스토리 틀 그리기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		case E_Rabbit:
			// 캐릭터 이름 출력
			_DrawTextColor(4, 19, "토끼", E_White);

			// 멘트 출력
			_DrawTextColor(4, 21, StartStory[i].ment, E_White);

			// 토끼, 자라 색상 변경
			g_RabbitColor = E_White;
			g_TurtleColor = E_Gray;

			CharacterDraw();

			// 스토리 틀 그리기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		case E_Turtle:
			// 캐릭터 이름 출력
			_DrawTextColor(4, 19, "자라", E_White);

			// 멘트 출력
			_DrawTextColor(4, 21, StartStory[i].ment, E_White);

			// 토끼, 자라 색상 변경
			g_RabbitColor = E_Gray;
			g_TurtleColor = E_White;

			CharacterDraw();

			// 스토리 틀 그리기
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		}

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q') 
			{
				IsStartStory = true; // 게임 시작 스토리 완료
				return;
			}
			if (g_Key == ' ') break;

			//_Invalidate();
		}
	}

	IsStartStory = true; // 게임 시작 스토리 완료
}

void PlayBossStory()
{
	int BossStoryCount = sizeof(BossStory) / sizeof(BossStory[0]);

	for (int i = 0; i < BossStoryCount; i++)
	{

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q')
			{
				IsBossStory = true; // 게임 시작 스토리 완료
				return;
			}
			if (g_Key == ' ') break;
		}
	}
}

void PlayEndStory()
{
	int EndStoryCount = sizeof(EndStory) / sizeof(EndStory[0]);

	for (int i = 0; i < EndStoryCount; i++)
	{

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q')
			{
				IsEndStory = true; // 게임 시작 스토리 완료
				return;
			}
			if (g_Key == ' ') break;
		}
	}
}