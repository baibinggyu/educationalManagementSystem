#ifndef _LOGGIN_H
#define _LOGGIN_H
#include <SDL.h>    
#include <SDL_mixer.h>
#include <stdbool.h>
#include "EducationalManagementSystem.h"
#define Loggin_Path "./resources/loggin_backGround.png"
#define Root_Login_Path "./resources/root_loggin_backGround.png"
bool Power_on(void);
int menu(void);
bool Edu_ShowAccountPasswdErrorRender(void);
bool Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes(void);
#endif