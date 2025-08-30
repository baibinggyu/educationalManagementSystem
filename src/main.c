#include "EducationalManagementSystem.h"
#include "RBTree.h"
bool Is_Whole_Quit = false;
SDL_Window* win = NULL;
SDL_Renderer* render = NULL;
Mix_Music* buttonPlay = NULL;   
TTF_Font* font = NULL;
TTF_Font* font_miao = NULL;
TTF_Font* font_bigger = NULL;
// course teacher student root
char* dataPath[TABLE_COUNT] = {"./data/course.csv","./data/teacher.csv","./data/student.csv","./data/root.csv"};
FILE* fp[TABLE_COUNT] = {NULL};
// enum Usr usr;
RBTree courseHead = NULL;
RBTree teacherHead = NULL;
RBTree studentHead = NULL;
RBTree rootHead = NULL;
int tryCount = 0;

SDL_Color black = {0,0,0,255};
SDL_Color red = {255,49,49,255};
SDL_Color blue = {82,113,255,255};
SDL_Color white = {255,255,255,255};
SDL_Color beige = {250,186,167,255};

int main(void){
    setlocale(LC_ALL, "");
    menu();
    return 0;
}
