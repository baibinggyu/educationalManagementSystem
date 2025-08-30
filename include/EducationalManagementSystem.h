#ifndef _EDUCATIONALMANAGEMENTSYSTEM_H_
#define _EDUCATIONALMANAGEMENTSYSTEM_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <wchar.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <locale.h>
#include "RBTree.h"

// Forward declarations
bool File_Load(void);
bool File_Write(void);
bool File_RBTree_DESTROY(void);
int menu(void);
bool Edu_ShowAccountPasswdErrorRender(void);
bool Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes(void);

// 四张表
#define TABLE_COUNT 4


extern int tryCount; //登陆次数

// extern enum Usr usr;
extern RBTree courseHead;
extern RBTree teacherHead;
extern RBTree studentHead;
extern RBTree rootHead;
extern char* dataPath[TABLE_COUNT];
extern FILE* fp[TABLE_COUNT];


extern SDL_Window* win;
extern SDL_Renderer* render;
extern Mix_Music* buttonPlay;
extern TTF_Font* font;
extern TTF_Font* font_miao;
extern TTF_Font* font_bigger;
extern SDL_Color black;
extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color white;
extern SDL_Color beige;

// 全局控制退出
extern bool Is_Whole_Quit;
#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080
#define WINDOW_CENTER_X (WINDOW_WIDTH / 2) 
#define WINDOW_CENTER_Y (WINDOW_HEIGHT / 2)
#define STACK_INDEX 100 // 树高不超过STACK_INDEX
#define PERSON_MAX_LIMIT 1000 // 假设不超过1000人
#define BUTTON_WEIGHT 200
#define BUTTON_HEIGHT 100
#define TEXT_LENGTH 20
#define ACCOUNT_LEN 12
#define PASSWD_MAX_LEN 20
#define TTF_SIZE 100
#define INPUT_SIZE 40
#define TTF_SIZE_BIIGER 200
#define FONT_PATH "./resources/slideyouran-Regular.ttf"
#define FONT_MIAO_PATH "./resources/miao.ttf"
#define FONT_ZHENGKAI_PATH "./resources/zhengkai.ttf"
#define FONT_HACK_PATH "./resources/Hack-Regular.ttf"
#define DATE_CALSS_NUM 3
// credit 学分
#define HIGH_MATH_CREDIT 3
#define DIGITAL_LOGIC_CREDIT 2
#define DISCRET_EMATH_CREDIT 4

#define TIPS_WIDTH 512
#define TIPS_HEIGHT 512
#define NEWLINE 384
#define TIPS_COLOR black


// 课程表
typedef struct Course{
    char courseID[20];      // 课程编号（唯一）
    char courseName[50];    // 课程名称
    int credit;             // 学分
    char teacherID[20];     // 授课教师ID
}* Course;
// 教师表
typedef struct Teacher{
    char teacherID[20];     // 教师编号（唯一）
    char name[50];          // 姓名
    char password[20];      // 登录密码
    // 可扩展：联系方式、所属部门等
}* Teacher;

#define MAX_COURSES 3

typedef struct {
    char courseID[5];      // 课程编号
    float score;            // 成绩
} StudentCourseScore;

//查询某课程成绩为不及格[0～60) 、及格[60～70) 、中等[70～80) 、良好 [80～90)、优秀[90～100]的人数和所占百分比
// enum
// 学生表
typedef struct Student{
    char studentID[20];      // 学号（唯一）
    char name[50];           // 姓名
    char password[20];       // 登录密码
    char major[50];          // 专业
    int semesterCount;       // 学期数
    StudentCourseScore courseScores[MAX_COURSES];  // 课程成绩列表  
    // 高数 3 学分、数字逻辑 2 学分、离散数学 4 学分
    int courseCount;         // 已修课程数量
    float totalScore;        // 总成绩（可预先计算）
    int rankInMajor;         // 专业排名（可预先计算）
}* Student;

// root表
typedef struct Root{
    char adminID[20];       // 管理员编号（唯一）
    char name[50];          // 姓名
    char password[20];      // 登录密码
}* Root;




bool Edu_Init(void);
bool Edu_Quit(void);
bool SDL_IsInRange(int x,int y,SDL_Rect* dest);
SDL_Texture* SDL_CreateTextureFromTTF(TTF_Font* ttf,char* word,SDL_Color color,int size);
int SDL_GetAdaptiveFontSize(const char* fontPath, const char* word, int maxWidth, int maxHeight);
bool SDL_ShowQuitX(SDL_Rect* dst,SDL_Color fg,SDL_Color fgX);
bool SDL_ShowButton(SDL_Rect* dst,char* word,SDL_Color fg); 
bool SDL_ShowButtonWithContent(char* word, SDL_Rect* dst, SDL_Color fg);
bool SDL_ShowInputBox(SDL_Rect* dst,char* title,char* tip,int titleSize,int tipSize,SDL_Color fg);
bool Edu_Warn(char* wrod,int size);
bool Edu_Error(char* wrod,int size);
bool Edu_Success(char* word,int size);
SDL_Texture* SDL_ShowWord(char* font_path,char* word,SDL_Rect* dst,SDL_Color fg,int swap);
#endif 