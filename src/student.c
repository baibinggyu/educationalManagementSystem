#include "student.h"

bool Student_Login(void){
    SDL_Texture* tex_Student_Login = IMG_LoadTexture(render,Loggin_Path);
    if(tex_Student_Login == NULL){
        SDL_Log("tex_Student_Login ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Texture* quit_button_click = IMG_LoadTexture(render,"./resources/quit_button_click.png");
    SDL_Texture* quit_button_hover = IMG_LoadTexture(render,"./resources/quit_button_hover.png");
    if(!quit_button_click || !quit_button_hover){
        SDL_Log("quit_button_click || quit_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Color fg = {0,0,0,255};
    char* thisClass = "学生端";
    SDL_Texture* tex_thisClass = SDL_CreateTextureFromTTF(font_bigger,thisClass,fg,2000);
    if(tex_thisClass == NULL){
        SDL_Log("学生端 tex_thisClass ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Rect src_thisClass = {0,0,0,0};
    TTF_SizeUTF8(font_bigger,thisClass,&src_thisClass.w,&src_thisClass.h);
    SDL_Rect dst_thisClass = {(WINDOW_WIDTH >> 1) - (src_thisClass.w >> 1),(WINDOW_HEIGHT >> 1) - (src_thisClass.h >> 1) - 430,src_thisClass.w,src_thisClass.h};
    SDL_Rect dst_quit = {1743,33,BUTTON_WEIGHT,BUTTON_HEIGHT};

    bool is_dst_quit_click = false;
    bool is_dst_quit_hover = false;
    SDL_Rect dst_account = {668,620,640,70};
    SDL_Rect dst_passwd = {668,723,447,70};
    SDL_Rect dst_input_account = {776,632,0,0};
    SDL_Rect dst_input_passwd = {776,745,0,0};
    SDL_Rect dst_loggin_button  = {1148,745,165,70};
    bool is_input_account = false;
    bool is_input_passwd = false;
    bool is_loggin_button_hover = false;
    bool is_loggin_button_click = false;
    char text_account[21] = "";
    char text_passwd[21] = "";
    char text_passwd_fake[21] = "";
    SDL_Texture* tex_account = NULL;
    SDL_Texture* tex_passwd_fake = NULL;
    SDL_Texture* tex_LiaoNing = IMG_LoadTexture(render,"./resources/liaoNing.png");
    if(tex_LiaoNing ==  NULL){
        SDL_Log("tex_LiaoNing Load ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_LiaoNing = {0,0,261,256};
    int x,y;
    SDL_Event e;
    while(true){
        if(tryCount == 3) {Is_Whole_Quit = true;}
        if(Is_Whole_Quit) break;

        TTF_SizeUTF8(font_miao,text_account,&dst_input_account.w,&dst_input_account.h);
        TTF_SizeUTF8(font_miao,text_passwd_fake,&dst_input_passwd.w,&dst_input_passwd.h);
        SDL_GetMouseState(&x,&y);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_Student_Login,NULL,NULL);
        SDL_RenderCopy(render,tex_thisClass,&src_thisClass,&dst_thisClass);
        SDL_RenderCopy(render,tex_LiaoNing,NULL,&dst_LiaoNing);

        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
                break;
            }else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }else if (e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_input_account && strlen(text_account) > 0){
                        text_account[strlen(text_account) - 1] = '\0';
                    }else if (is_input_passwd && strlen(text_passwd) > 0){
                        text_passwd[strlen(text_passwd) - 1] = '\0';
                        text_passwd_fake[strlen(text_passwd_fake) - 1] = '\0';
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN ){
                    SDL_StartTextInput();
                    is_input_account = false;
                    is_input_passwd = false;
                    if(e.button.button == SDL_BUTTON_LEFT){
                        if(SDL_IsInRange(x,y,&dst_quit)) {
                            Mix_PlayMusic(buttonPlay,1);
                            is_dst_quit_click = true;
                        }
                    if(SDL_IsInRange(x,y,&dst_account)){
                        is_input_account = true;
                        SDL_StartTextInput();
                        SDL_SetTextInputRect(&dst_input_account);
                    }
                    if(SDL_IsInRange(x,y,&dst_passwd)){
                        is_input_passwd = true;
                        SDL_StartTextInput();
                        SDL_SetTextInputRect(&dst_input_passwd);
                    }
                    if(SDL_IsInRange(x,y,&dst_loggin_button)){
                        Mix_PlayMusic(buttonPlay,1);
                        is_loggin_button_click = true;
                    }
                }
            }else if (e.type == SDL_MOUSEBUTTONUP){
                if(is_dst_quit_click) break;
                if(is_loggin_button_click && strlen(text_account) && strlen(text_passwd)){
// 测试!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    if(Student_IsCountMatch(text_account,text_passwd)){
                        Student_Ui(text_account);
                        strcpy(text_account,"");
                        strcpy(text_passwd,"");
                        strcpy(text_passwd_fake,"");
                        tryCount = 0;
                    }else{
                        tryCount++;
                        if(tryCount != 3){
                            Edu_ShowAccountPasswdErrorRender();
                        }else{
                            Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes();
                        }
                        strcpy(text_passwd,"");
                        strcpy(text_passwd_fake,"");
                    }
                }
                is_loggin_button_click = false;
            }else if (e.type == SDL_TEXTINPUT){
                if(is_input_account && strlen(text_account) < ACCOUNT_LEN){
                    strcat(text_account,e.text.text);
                }else if (is_input_passwd && strlen(text_passwd) < PASSWD_MAX_LEN){
                    strcat(text_passwd,e.text.text);
                    strcat(text_passwd_fake,"*");
                }
                    
            }


        }
        if(SDL_IsInRange(x,y,&dst_loggin_button)){
           is_loggin_button_hover = true;
        }else is_loggin_button_hover = false;

        if(SDL_IsInRange(x,y,&dst_quit)){
            is_dst_quit_hover = true;
        }else {
            is_dst_quit_hover = false;
            is_dst_quit_click = false;
        }

        if (is_dst_quit_click) SDL_RenderCopy(render,quit_button_click,NULL,&dst_quit);
        else if(is_dst_quit_hover) SDL_RenderCopy(render,quit_button_hover,NULL,&dst_quit);

        tex_account = SDL_CreateTextureFromTTF(font_miao,text_account,fg,2000);
        tex_passwd_fake = SDL_CreateTextureFromTTF(font_miao,text_passwd_fake,fg,2000);
        SDL_RenderCopy(render,tex_account,NULL,&dst_input_account);
        SDL_RenderCopy(render,tex_passwd_fake,NULL,&dst_input_passwd);
        SDL_DestroyTexture(tex_account);
        SDL_DestroyTexture(tex_passwd_fake);
        SDL_RenderPresent(render);
    
    }
    
    SDL_DestroyTexture(tex_LiaoNing);
    SDL_DestroyTexture(tex_account);
    SDL_DestroyTexture(tex_passwd_fake);
    SDL_DestroyTexture(tex_passwd_fake);
    SDL_DestroyTexture(quit_button_click);
    SDL_DestroyTexture(quit_button_hover);
    SDL_DestroyTexture(tex_Student_Login);
    return true;
}

static int Student_CmpCountAccountMatch(const void* node_data, const void* input_data){
    Student data01 = (Student)node_data;
    Student data02 = (Student)input_data;
    return strcmp(data01->studentID,data02->studentID);
}
static int Student_CmpCountPasswdMatch(const void* node_data, const void* input_data){
    Student data01 = (Student)node_data;
    Student data02 = (Student)input_data;
    return strcmp(data01->password,data02->password);
}
bool Student_IsCountMatch(char* studentID,char* passwd){
    struct Student temp;
    strcpy(temp.studentID,studentID);
    RBTree res = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(!res) return false;
    strcpy(temp.password,passwd);
    return Student_CmpCountPasswdMatch(&temp,res->data) == 0 ? true : false;
}

bool Student_Ui(char* studentID){
    SDL_Texture* tex_StudentUi = IMG_LoadTexture(render,"./resources/student_UI.png");
    if(!tex_StudentUi){
        SDL_Log("tex_StudentUi Load Error:%s",SDL_GetError());
        return false;
    }
    
    // 长宽高 80
    int x = 80;
    SDL_Rect dst_CheckPersonalData = {870,200,x,x};
    SDL_Rect dst_SearchProfessionalRank = {870,310,x,x};
    SDL_Rect dst_QueryCoursePerformanceAnalysis = {870,420,x,x};
    SDL_Rect dst_AcademicWarningReminder = {870,528,x,x};
    SDL_Rect dst_ChangePersonalPassword = {870,635,x,x};
    SDL_Rect dst_Exit = {870,735,x,x};

    bool is_CheckPersonalData_pause = false;
    bool is_CheckPersonalData_click = false;
    bool is_SearchProfessionalRank_pause = false;
    bool is_SearchProfessionalRank_click = false;
    bool is_QueryCoursePerformanceAnalysis_pause = false;
    bool is_QueryCoursePerformanceAnalysis_click = false;
    bool is_AcademicWarningReminder_pause = false;
    bool is_AcademicWarningReminder_click = false;
    bool is_ChangePersonalPassword_pause = false;
    bool is_ChangePersonalPassword_click = false;
    bool is_Exit_pause = false;
    bool is_Exit_click = false;

    SDL_Texture* tex_CheckPersonalData_pause = IMG_LoadTexture(render,"./resources/button_num01_hover.png");
    SDL_Texture* tex_CheckPersonalData_click = IMG_LoadTexture(render,"./resources/button_num01_click.png");
    SDL_Texture* tex_SearchProfessionalRank_pause = IMG_LoadTexture(render,"./resources/button_num02_hover.png");
    SDL_Texture* tex_SearchProfessionalRank_click = IMG_LoadTexture(render,"./resources/button_num02_click.png");
    SDL_Texture* tex_QueryCoursePerformanceAnalysis_pause = IMG_LoadTexture(render,"./resources/button_num03_hover.png");
    SDL_Texture* tex_QueryCoursePerformanceAnalysis_click = IMG_LoadTexture(render,"./resources/button_num03_click.png");
    SDL_Texture* tex_AcademicWarningReminder_pause = IMG_LoadTexture(render,"./resources/button_num04_hover.png");
    SDL_Texture* tex_AcademicWarningReminder_click = IMG_LoadTexture(render,"./resources/button_num04_click.png");
    SDL_Texture* tex_ChangePersonalPassword_pause = IMG_LoadTexture(render,"./resources/button_num05_hover.png");
    SDL_Texture* tex_ChangePersonalPassword_click = IMG_LoadTexture(render,"./resources/button_num05_click.png");
    SDL_Texture* tex_Exit_pause = IMG_LoadTexture(render,"./resources/button_num06_hover.png");
    SDL_Texture* tex_Exit_click = IMG_LoadTexture(render,"./resources/button_num06_click.png");
    if(!(tex_CheckPersonalData_pause && tex_CheckPersonalData_click &&
        tex_SearchProfessionalRank_pause && tex_SearchProfessionalRank_click &&
        tex_QueryCoursePerformanceAnalysis_pause && tex_QueryCoursePerformanceAnalysis_click &&
        tex_AcademicWarningReminder_pause && tex_AcademicWarningReminder_click &&
        tex_ChangePersonalPassword_pause && tex_ChangePersonalPassword_click &&
        tex_Exit_pause && tex_Exit_click)){
            SDL_Log("Student_Ui Load ERROR:%s",SDL_GetError());
            return false;
        }

    SDL_Event e;
    int mx,my;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_StudentUi,NULL,NULL);
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }

            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_CheckPersonalData_pause) {Mix_PlayMusic(buttonPlay,1);is_CheckPersonalData_click = true;}
                if(is_SearchProfessionalRank_pause) {Mix_PlayMusic(buttonPlay,1);is_SearchProfessionalRank_click = true;}
                if(is_QueryCoursePerformanceAnalysis_pause) {Mix_PlayMusic(buttonPlay,1);is_QueryCoursePerformanceAnalysis_click = true;}
                if(is_AcademicWarningReminder_pause) {Mix_PlayMusic(buttonPlay,1);;is_AcademicWarningReminder_click = true;}
                if(is_ChangePersonalPassword_pause){Mix_PlayMusic(buttonPlay,1);is_ChangePersonalPassword_click = true;}
                if(is_Exit_pause) {Mix_PlayMusic(buttonPlay,1);is_Exit_click = true;}
            }
//-----------------------------------------------------------------------
//                      功能实现!!!!!!
//-----------------------------------------------------------------------
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_CheckPersonalData_click){
                    Student_CheckPersonalData(studentID);
                    is_CheckPersonalData_click = false;
                }
                if(is_SearchProfessionalRank_click){
                    Student_SearchProfessionalRank(studentID);
                    is_SearchProfessionalRank_click = false;
                }
                if(is_QueryCoursePerformanceAnalysis_click){
                    Student_QueryCoursePerformanceAnalysis(studentID);
                    is_QueryCoursePerformanceAnalysis_click = false;
                }
                if(is_AcademicWarningReminder_click){
                    Student_AcademicWarningReminder(studentID);
                    is_AcademicWarningReminder_click = false;
                }
                if(is_ChangePersonalPassword_click){
                    Student_ChangePersonalPassword(studentID);
                    is_ChangePersonalPassword_click = false;
                }
                if(is_Exit_click){
                    break;
                    is_Exit_click = false;
                }
            }
            
        }

        if(SDL_IsInRange(mx,my,&dst_CheckPersonalData)) is_CheckPersonalData_pause = true;
        else is_CheckPersonalData_pause = false;
        if(SDL_IsInRange(mx,my,&dst_SearchProfessionalRank)) is_SearchProfessionalRank_pause = true;
        else is_SearchProfessionalRank_pause = false;
        if(SDL_IsInRange(mx,my,&dst_QueryCoursePerformanceAnalysis)) is_QueryCoursePerformanceAnalysis_pause = true;
        else is_QueryCoursePerformanceAnalysis_pause = false;
        if(SDL_IsInRange(mx,my,&dst_AcademicWarningReminder)) is_AcademicWarningReminder_pause = true;
        else is_AcademicWarningReminder_pause = false;
        if(SDL_IsInRange(mx,my,&dst_ChangePersonalPassword)) is_ChangePersonalPassword_pause = true;
        else is_ChangePersonalPassword_pause = false;
        if(SDL_IsInRange(mx,my,&dst_Exit)) is_Exit_pause = true;
        else is_Exit_pause = false;

        if(is_CheckPersonalData_click) SDL_RenderCopy(render,tex_CheckPersonalData_click,NULL,&dst_CheckPersonalData);
        else if  (is_CheckPersonalData_pause) SDL_RenderCopy(render,tex_CheckPersonalData_pause,NULL,&dst_CheckPersonalData);
        if(is_SearchProfessionalRank_click) SDL_RenderCopy(render,tex_SearchProfessionalRank_click,NULL,&dst_SearchProfessionalRank);
        else if (is_SearchProfessionalRank_pause) SDL_RenderCopy(render,tex_SearchProfessionalRank_pause,NULL,&dst_SearchProfessionalRank);
        if(is_QueryCoursePerformanceAnalysis_click) SDL_RenderCopy(render,tex_QueryCoursePerformanceAnalysis_click,NULL,&dst_QueryCoursePerformanceAnalysis);
        else if (is_QueryCoursePerformanceAnalysis_pause) SDL_RenderCopy(render,tex_QueryCoursePerformanceAnalysis_pause,NULL,&dst_QueryCoursePerformanceAnalysis);
        if(is_AcademicWarningReminder_click) SDL_RenderCopy(render,tex_AcademicWarningReminder_click,NULL,&dst_AcademicWarningReminder);
        else if (is_AcademicWarningReminder_pause) SDL_RenderCopy(render,tex_AcademicWarningReminder_pause,NULL,&dst_AcademicWarningReminder);
        if (is_ChangePersonalPassword_click) SDL_RenderCopy(render,tex_ChangePersonalPassword_click,NULL,&dst_ChangePersonalPassword);
        else if (is_ChangePersonalPassword_pause) SDL_RenderCopy(render,tex_ChangePersonalPassword_pause,NULL,&dst_ChangePersonalPassword);
        if(is_Exit_click) SDL_RenderCopy(render,tex_Exit_click,NULL,&dst_Exit);
        else if (is_Exit_pause) SDL_RenderCopy(render,tex_Exit_pause,NULL,&dst_Exit);


        SDL_RenderPresent(render);  
    }

    SDL_DestroyTexture(tex_CheckPersonalData_pause);
    SDL_DestroyTexture(tex_CheckPersonalData_click);
    SDL_DestroyTexture(tex_SearchProfessionalRank_pause);
    SDL_DestroyTexture(tex_SearchProfessionalRank_click);
    SDL_DestroyTexture(tex_QueryCoursePerformanceAnalysis_pause);
    SDL_DestroyTexture(tex_QueryCoursePerformanceAnalysis_click);
    SDL_DestroyTexture(tex_AcademicWarningReminder_pause);
    SDL_DestroyTexture(tex_AcademicWarningReminder_click);
    SDL_DestroyTexture(tex_ChangePersonalPassword_pause);
    SDL_DestroyTexture(tex_ChangePersonalPassword_click);
    SDL_DestroyTexture(tex_Exit_pause);
    SDL_DestroyTexture(tex_Exit_click);
    SDL_DestroyTexture(tex_StudentUi);
    return true;
}




bool Student_ChangePersonalPassword(char* account){
    struct Student temp;
    strcpy(temp.studentID,account);
    RBTree cur = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(cur == NULL){
        printf("未知错误\n");
        return false;
    }
    TTF_Font* font_temp = TTF_OpenFont(FONT_MIAO_PATH,20);

    char ch_first_input[21] = "";
    char ch_second_input[21] = "";
    char ch_third_input[21] = "";
    char ch_second_fake[21] = "";
    char ch_third_fake[21] = "";
    SDL_Rect dst_change_password = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect dst_first_box = {dst_change_password.x + 240,dst_change_password.y + 76,220,70};
    SDL_Rect dst_second_box = {dst_first_box.x,dst_change_password.y + 173,dst_first_box.w,dst_first_box.h};
    SDL_Rect dst_third_box = {dst_first_box.x,dst_change_password.y +  259,dst_first_box.w,dst_first_box.h};
    int dx = 10;
    int dh = 20;
    SDL_Rect dst_confirm = {dst_change_password.x + 180,dst_change_password.y + 370 ,150,76};
    SDL_Rect dst_first_box_input = {dst_first_box.x + dx,dst_first_box.y + dh,dst_first_box.w - dx,dst_first_box.h};
    SDL_Rect dst_second_box_input = {dst_second_box.x + dx,dst_second_box.y + dh,dst_second_box.w - dx,dst_second_box.h};
    SDL_Rect dst_third_box_input = {dst_third_box.x + dx,dst_third_box.y + dh,dst_third_box.w - dx,dst_third_box.h};
    SDL_Rect dst_quit = {1940,0,40,40};
    // printf("dst_first_box %d %d %d %d\n",dst_first_box.x,dst_first_box.y,dst_first_box.w,dst_first_box.h);
    // printf("dst_first_input %d %d %d %d \n",dst_first_box_input.x,dst_first_box_input.y,dst_first_box_input.w,dst_first_box_input.h);
    char* ch_confirm = "确认";
    SDL_Texture* tex_confirm_black = SDL_CreateTextureFromTTF(font,ch_confirm,black,2000);
    SDL_Texture* tex_confirm_blue = SDL_CreateTextureFromTTF(font,ch_confirm,blue,2000);
    SDL_Texture* tex_confirm_red = SDL_CreateTextureFromTTF(font,ch_confirm,red,2000);
   
    if(!(tex_confirm_black && tex_confirm_blue && tex_confirm_red)){
        SDL_Log("Teacher_ChangePersonalPassword Error:%s",SDL_GetError());
        return false;
    }

    SDL_Texture* tex_change_password  = IMG_LoadTexture(render,"./resources/teacher_change_password.png");
    if(tex_change_password == NULL){
        SDL_Log("teacher_ChangePersonalPassword : %s",SDL_GetError());
        return false;
    }

    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    bool is_first_input = false;
    bool is_second_input = false;
    bool is_third_input = false;
    bool is_quit_hover = false;
    bool is_quit_click = false;

    int mx,my;
    SDL_Event e;    
    while(true){
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_GetMouseState(&mx,&my);
        SDL_RenderCopy(render,tex_change_password,NULL,&dst_change_password);
        SDL_RenderCopy(render,tex_confirm_black,NULL,&dst_confirm);
        SDL_ShowQuitX(&dst_quit,black,black);
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_first_input && strlen(ch_first_input) > 0){
                        ch_first_input[strlen(ch_first_input) - 1] = '\0';
                    }
                    if(is_second_input && strlen(ch_second_input) > 0){
                        ch_second_input[strlen(ch_second_input) - 1] = '\0';
                        ch_second_fake[strlen(ch_second_fake) - 1] = '\0';
                    }
                    if(is_third_input && strlen(ch_third_input) > 0){
                        ch_third_input[strlen(ch_third_input) - 1] = '\0';
                        ch_third_fake[strlen(ch_third_fake) - 1] = '\0';
                    }
                    
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
                if(SDL_IsInRange(mx,my,&dst_first_box)){
                    // printf("1\n");
                    is_first_input = true;
                    SDL_StartTextInput();
                    is_second_input = false;
                    is_third_input = false;
                }else if(SDL_IsInRange(mx,my,&dst_second_box)){
                    // printf("2\n");
                    is_second_input = true;
                    SDL_StartTextInput();
                    is_first_input =false;
                    is_third_input = false;
                }else if(SDL_IsInRange(mx,my,&dst_third_box)){
                    // printf("3\n");
                    is_third_input = true;
                    SDL_StartTextInput();
                    is_first_input = false;
                    is_second_input = false;
                }else{
                    SDL_StopTextInput();
                    is_first_input = false;
                    is_second_input = false;
                    is_third_input = false;
                }
                if(is_confirm_hover){
                    is_confirm_click = true;
                }else{
                    is_confirm_click = false;
                }
            }
        
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_quit_click){
                    break;
                    is_quit_click = false;   
                }
        //-------------------------------------------这里
                if(is_confirm_click){
                    if(strlen(ch_first_input) && strlen(ch_second_input) && strlen(ch_third_input)){
                         if(strcmp(((Student)cur->data)->password,ch_first_input) != 0){
                        Edu_Warn("您输入的原密码与真实密码不相同，请重新输入!",40);
                    }else{
                        if(strcmp(ch_second_input,ch_third_input) != 0){
                            Edu_Warn("您两次输入的密码不同，请重新输入!",40);
                            strcpy(ch_second_input,"");
                            strcpy(ch_second_fake,"");
                            strcpy(ch_third_fake,"");
                            strcpy(ch_third_input,"");
                        }else{
                            strcpy(((Student)cur->data)->password,ch_second_input);
                            Edu_Success("恭喜修改密码成功!",40);
                            break;
                        }
                    }
                    }
                    is_confirm_click = false;
                }
            }   
            if(e.type == SDL_TEXTINPUT){
                if(is_first_input && strlen(ch_first_input) < 20){
                    strcat(ch_first_input,e.text.text);
                }
                if(is_second_input && strlen(ch_second_input) < 20){
                    strcat(ch_second_input,e.text.text);
                    strcat(ch_second_fake,"*");
                }
                if(is_third_input && strlen(ch_third_input) < 20){
                    strcat(ch_third_input,e.text.text);
                    strcat(ch_third_fake,"*");

                }
            }

        }
        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover = true;
        }else{
                 is_confirm_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_quit)){
            is_quit_hover = true;
        }else{
            is_quit_hover = false;
        }

        if(is_confirm_click){
            SDL_RenderCopy(render,tex_confirm_red,NULL,&dst_confirm);
        }else if(is_confirm_hover){
            SDL_RenderCopy(render,tex_confirm_blue,NULL,&dst_confirm);
        }

        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }

        SDL_Texture* tex_first = SDL_CreateTextureFromTTF(font_temp,ch_first_input,black,2000);
        TTF_SizeUTF8(font_temp,ch_first_input,&dst_first_box_input.w,&dst_first_box_input.h);
        SDL_Texture* tex_second = SDL_CreateTextureFromTTF(font_temp,ch_second_fake,black,2000);
        TTF_SizeUTF8(font_temp,ch_second_fake,&dst_second_box_input.w,&dst_second_box_input.h);
        SDL_Texture* tex_third = SDL_CreateTextureFromTTF(font_temp,ch_third_fake,black,2000);
        TTF_SizeUTF8(font_temp,ch_third_fake,&dst_third_box_input.w,&dst_third_box_input.h);
        
        SDL_RenderCopy(render,tex_first,NULL,&dst_first_box_input);
        SDL_RenderCopy(render,tex_second,NULL,&dst_second_box_input);
        SDL_RenderCopy(render,tex_third,NULL,&dst_third_box_input);

        SDL_DestroyTexture(tex_first);
        SDL_DestroyTexture(tex_second);
        SDL_DestroyTexture(tex_third);


        SDL_RenderPresent(render);


    }


    SDL_DestroyTexture(tex_confirm_red);
    SDL_DestroyTexture(tex_confirm_blue);
    SDL_DestroyTexture(tex_confirm_black);
    SDL_DestroyTexture(tex_change_password);
    TTF_CloseFont(font_temp);
    return false;
}

bool Student_AcademicWarningReminder(char* studentID){
    struct Student temp;
    strcpy(temp.studentID,studentID);
    RBTree find = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(find == NULL) return false;
    Student data = (Student)find->data;
    int score = 0;
    SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        for(int i = 0;i < MAX_COURSES;i++)
        score += Edu_GetScore(data->courseScores[i].score);
    if(score < 7){
        Edu_Warn("您的学分偏低，还请多多注意",40);
    }else{
        Edu_Success("您没有学业危机",40);
    }
    return true;
}

bool Student_QueryCoursePerformanceAnalysis(char* studentID){
    if(studentID == NULL || strlen(studentID) < 1) return false;
    struct Student temp;
    strcpy(temp.studentID,studentID);
    RBTree find = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(find == NULL){
        printf("未知错误!\n");
        return false;
    }
    Student data = (Student)find->data;
    char ch_admath_score[10] = "";
    char ch_logic_score[10] = "";
    char ch_discrete_score[10] = "";
    char ch_total_score[10] = "";
    sprintf(ch_admath_score,"%.2f",data->courseScores[0].score);
    sprintf(ch_logic_score,"%.2f",data->courseScores[1].score);
    sprintf(ch_discrete_score,"%.2f",data->courseScores[2].score);
    sprintf(ch_total_score,"%.2f",data->courseScores[0].score + data->courseScores[1].score + data->courseScores[2].score);
    SDL_Texture* tex_score_analysis = IMG_LoadTexture(render,"./resources/student_score_analysis.png");
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    SDL_Texture* tex_admath_score = SDL_CreateTextureFromTTF(font_temp,ch_admath_score,black,2000);
    SDL_Texture* tex_logic_score = SDL_CreateTextureFromTTF(font_temp,ch_logic_score,black,2000);
    SDL_Texture* tex_discrete_score = SDL_CreateTextureFromTTF(font_temp,ch_discrete_score,black,2000);
    SDL_Texture* tex_total_score = SDL_CreateTextureFromTTF(font_temp,ch_total_score,black,2000);
    if(!(tex_admath_score && tex_discrete_score && tex_logic_score && tex_score_analysis && tex_total_score)){
        SDL_Log("Student_QueryCoursePerformanceAnalysis Error:%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_score_analysis = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    
    SDL_Rect dst_admath_score = {dst_score_analysis.x + 196,dst_score_analysis.y + 116};
    SDL_Rect dst_logic_score = {dst_admath_score.x,dst_admath_score.y + 60};
    SDL_Rect dst_discrete_score = {dst_logic_score.x,dst_logic_score.y + 60};
    SDL_Rect dst_total_score = {dst_discrete_score.x, dst_discrete_score.y + 60};
    TTF_SizeUTF8(font_temp,ch_admath_score,&dst_admath_score.w,&dst_admath_score.h);
    TTF_SizeUTF8(font_temp,ch_logic_score,&dst_logic_score.w,&dst_logic_score.h);
    TTF_SizeUTF8(font_temp,ch_discrete_score,&dst_discrete_score.w,&dst_discrete_score.h);
    TTF_SizeUTF8(font_temp,ch_total_score,&dst_total_score.w,&dst_total_score.h);
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - 100,dst_score_analysis.y + 390,150,75};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_score_analysis,NULL,&dst_score_analysis);
        SDL_RenderCopy(render,tex_admath_score,NULL,&dst_admath_score);
        SDL_RenderCopy(render,tex_discrete_score,NULL,&dst_discrete_score);
        SDL_RenderCopy(render,tex_logic_score,NULL,&dst_logic_score);
        SDL_RenderCopy(render,tex_total_score,NULL,&dst_total_score);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click =true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click){
                    break;
                    is_confirm_click = false;
                }
            }
        }

        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover = true;
        }else{
            is_confirm_hover = false;
        }
        if(is_confirm_click){
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }
        SDL_RenderPresent(render);
    }


    SDL_DestroyTexture(tex_total_score);
    SDL_DestroyTexture(tex_logic_score);
    SDL_DestroyTexture(tex_discrete_score);
    SDL_DestroyTexture(tex_admath_score);
    TTF_CloseFont(font_temp);
    SDL_DestroyTexture(tex_score_analysis);
    return true;
}

int Student_CmpRank(const void* node01,const void* node02){
    Student data01 = (Student)((*((RBTree*)node01))->data);
    Student data02 = (Student)((*((RBTree*)node02))->data);
    return data01->totalScore < data02->totalScore;
}

bool Student_SearchProfessionalRank(char* studentID){
    if(studentID == NULL || strlen(studentID) < 1) return false;
    struct Student temp;
    strcpy(temp.studentID,studentID);
    RBTree find = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(find == NULL){
        printf("未知错误!\n");
        return false;
    }
    RBTree arr[PERSON_MAX_LIMIT] = {NULL};
    RBTree stack[STACK_INDEX] = {NULL};
    int top = -1;
    RBTree cur = studentHead;
    int index = 0;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        arr[index++] =  cur;
        cur = cur->right;
    }   
    qsort(arr,index,sizeof(RBTree),Student_CmpRank);
    int rank = -1;
    for(int i = 0;i < index;i++){
        if(find == arr[i]){
            rank = i + 1;
            break;
        }
    }
    if(rank == -1){
        printf("Student_SearchProfessionalRank Error\n");
        return false;
    }
    char ch_rank[5] = "";
    sprintf(ch_rank,"%d",rank);
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    SDL_Texture* tex_backGround = IMG_LoadTexture(render,"./resources/student_rank.png");
    SDL_Texture* tex_rank = SDL_CreateTextureFromTTF(font_temp,ch_rank,black,2000);
    SDL_Rect dst_backGround = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect dst_rank = {WINDOW_CENTER_X,WINDOW_CENTER_Y - 45};
    TTF_SizeUTF8(font_temp,ch_rank,&dst_rank.w,&dst_rank.h);
    int w = 150,h = 75;
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - (w >> 1),WINDOW_CENTER_Y + 125,w,h};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_backGround,NULL,&dst_backGround);
        SDL_RenderCopy(render,tex_rank,NULL,&dst_rank);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE) break;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click){
                    break;
                }
            }
        }

        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover = true;
        }else{
            is_confirm_hover = false;
        }
        if(is_confirm_click){
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if (is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }

        SDL_RenderPresent(render);
    }
    SDL_DestroyTexture(tex_backGround);
    SDL_DestroyTexture(tex_rank);
    TTF_CloseFont(font_temp);
    return true;
}
bool Student_CheckPersonalData(char* studentID){
    if(studentID == NULL || strlen(studentID) < 1) return false;
    struct Student temp;
    strcpy(temp.studentID,studentID);
    RBTree find = RBTree_SEARCH(&studentHead,&temp,Student_CmpCountAccountMatch);
    if(find == NULL){
        printf("未知错误!\n");
        return false;
    }
    Student data = (Student)find->data;
    char ch_semesterCount[5] ="";
    char ch_courseCount[5] = "";
    sprintf(ch_semesterCount,"%d",data->semesterCount);
    sprintf(ch_courseCount,"%d",data->courseCount);
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,30);

    SDL_Texture* tex_studentID = SDL_CreateTextureFromTTF(font_temp,data->studentID,black,2000);
    SDL_Texture* tex_name = SDL_CreateTextureFromTTF(font_temp,data->name,black,2000);
    SDL_Texture* tex_major = SDL_CreateTextureFromTTF(font_temp,data->major,black,2000);
    SDL_Texture* tex_semesterCount = SDL_CreateTextureFromTTF(font_temp,ch_semesterCount,black,20000);
    SDL_Texture* tex_courseCount = SDL_CreateTextureFromTTF(font_temp,ch_courseCount,black,2000);
    SDL_Texture* tex_backGround = IMG_LoadTexture(render,"./resources/student_CheckPersonalData.png");
    if(!(tex_studentID && tex_name && tex_major && tex_semesterCount && tex_courseCount && tex_backGround)){
        SDL_Log("Student_CheckPersonalData Error:%s",SDL_GetError());
        return false;
    }

    SDL_Rect dst_backGround = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect dst_studentID = {dst_backGround.x + 216,dst_backGround.y + 112};
    SDL_Rect dst_name = {dst_studentID.x,dst_backGround.y + 166};
    SDL_Rect dst_major = {dst_studentID.x,dst_backGround.y + 220};
    SDL_Rect dst_semesterCount = {dst_backGround.x + 238,dst_backGround.y + 260};
    SDL_Rect dst_courseCount = {dst_backGround.x + 285,dst_backGround.y + 310};
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - 75,WINDOW_CENTER_Y + 100,150,75};
    TTF_SizeUTF8(font_temp,data->studentID,&dst_studentID.w,&dst_studentID.h);
    TTF_SizeUTF8(font_temp,data->name,&dst_name.w,&dst_name.h);
    TTF_SizeUTF8(font_temp,data->major,&dst_major.w,&dst_major.h);
    TTF_SizeUTF8(font_temp,ch_semesterCount,&dst_semesterCount.w,&dst_semesterCount.h);
    TTF_SizeUTF8(font_temp,ch_courseCount,&dst_courseCount.w,&dst_courseCount.h);
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    int mx,my;
    SDL_Event e;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_backGround,NULL,&dst_backGround);
        SDL_RenderCopy(render,tex_studentID,NULL,&dst_studentID);
        SDL_RenderCopy(render,tex_name,NULL,&dst_name);
        SDL_RenderCopy(render,tex_major,NULL,&dst_major);
        SDL_RenderCopy(render,tex_semesterCount,NULL,&dst_semesterCount);
        SDL_RenderCopy(render,tex_courseCount,NULL,&dst_courseCount);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        if(Is_Whole_Quit)break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click){
                    is_confirm_click = false;
                    break;
                }
            }

        }
        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover = true;
        }else{
            is_confirm_hover = false;
        }


        if(is_confirm_click){
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }
    SDL_RenderPresent(render);

    }

   

    TTF_CloseFont(font_temp);
    SDL_DestroyTexture(tex_studentID);
    SDL_DestroyTexture(tex_name);
    SDL_DestroyTexture(tex_major);
    SDL_DestroyTexture(tex_semesterCount);
    SDL_DestroyTexture(tex_courseCount);
    SDL_DestroyTexture(tex_backGround);

    return true;
}