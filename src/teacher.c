#include "teacher.h"

bool Teacher_Login(void){
    SDL_Texture* tex_Teacher_Login = IMG_LoadTexture(render,Loggin_Path);
    if(tex_Teacher_Login == NULL){
        SDL_Log("tex_Teacher_Login ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Texture* quit_button_click = IMG_LoadTexture(render,"./resources/quit_button_click.png");
    SDL_Texture* quit_button_hover = IMG_LoadTexture(render,"./resources/quit_button_hover.png");
    if(!quit_button_click || !quit_button_hover ){
        SDL_Log("quit_button_click || quit_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Color fg = {0,0,0,255};
    char* thisClass = "教师端";
    SDL_Texture* tex_thisClass = SDL_CreateTextureFromTTF(font_bigger,thisClass,fg,2000);
    if(tex_thisClass == NULL){
        SDL_Log("教师端 tex_thisClass ERROR:%s",SDL_GetError());
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
        SDL_RenderCopy(render,tex_Teacher_Login,NULL,NULL);
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
                    if(Teacher_IsCountMatch(text_account,text_passwd)){
                        Teacher_Ui(text_account);
                        strcpy(text_account,"");
                        strcpy(text_passwd,"");
                        strcpy(text_passwd_fake,"");
                        tryCount = 0;
                    }else{
                        tryCount++;
                        if(tryCount < 3){
                            Edu_ShowAccountPasswdErrorRender();
                        }else{
                            Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes();
                            Is_Whole_Quit = true;
                            break;
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
    SDL_DestroyTexture(tex_thisClass);
    SDL_DestroyTexture(tex_Teacher_Login);
    
    return true;
}


static int Teacher_CmpCountAccountMatch(const void* node_data, const void* input_data){
    Teacher data01 = (Teacher)node_data;
    Teacher data02 = (Teacher)input_data;
    return strcmp(data01->teacherID,data02->teacherID);
}
static int Teacher_CmpCountPasswdMatch(const void* node_data, const void* input_data){
    Teacher data01 = (Teacher)node_data;
    Teacher data02 = (Teacher)input_data;
    return strcmp(data01->password,data02->password);
}
bool Teacher_IsCountMatch(char* teacherID,char* passwd){
    struct Teacher temp;
    strcpy(temp.teacherID,teacherID);
    RBTree res = RBTree_SEARCH(&teacherHead,&temp,Teacher_CmpCountAccountMatch);
    if(!res) return false;
    strcpy(temp.password,passwd);
    return Teacher_CmpCountPasswdMatch(&temp,res->data) == 0 ? true : false;
}


bool Teacher_Ui(char* teacherID){
    SDL_Texture* tex_StudentUi = IMG_LoadTexture(render,"./resources/teacher_UI.png");
    if(!tex_StudentUi){
        SDL_Log("tex_StudentUi Load Error:%s",SDL_GetError());
        return false;
    }
    
    // 长宽高 80
    int x = 80;
    SDL_Rect dst_CheckPersonalData = {871,230,x,x};
    SDL_Rect dst_QueryCourseGradeInformation = {871,338,x,x};
    SDL_Rect dst_EnterOrModifyGrades = {871,446,x,x};
    SDL_Rect dst_CheckTheClassSchedule = {871,555,x,x};
    SDL_Rect dst_ChangePersonalPassword = {871,661,x,x};
    SDL_Rect dst_Exit = {870,763,x,x};

    bool is_CheckPersonalData_pause = false;
    bool is_CheckPersonalData_click = false;
    bool is_QueryCourseGradeInformation_pause = false;
    bool is_QueryCourseGradeInformation_click = false;
    bool is_EnterOrModifyGrades_pause = false;
    bool is_EnterOrModifyGrades_click = false;
    bool is_CheckTheClassSchedule_pause = false;
    bool is_CheckTheClassSchedule_click = false;
    bool is_ChangePersonalPassword_pause = false;
    bool is_ChangePersonalPassword_click = false;
    bool is_Exit_pause = false;
    bool is_Exit_click = false;

    SDL_Texture* tex_CheckPersonalData_pause = IMG_LoadTexture(render,"./resources/button_num01_hover.png");
    SDL_Texture* tex_CheckPersonalData_click = IMG_LoadTexture(render,"./resources/button_num01_click.png");
    SDL_Texture* tex_QueryCourseGradeInformation_pause = IMG_LoadTexture(render,"./resources/button_num02_hover.png");
    SDL_Texture* tex_QueryCourseGradeInformation_click = IMG_LoadTexture(render,"./resources/button_num02_click.png");
    SDL_Texture* tex_EnterOrModifyGrades_pause = IMG_LoadTexture(render,"./resources/button_num03_hover.png");
    SDL_Texture* tex_EnterOrModifyGrades_click = IMG_LoadTexture(render,"./resources/button_num03_click.png");
    SDL_Texture* tex_CheckTheClassSchedule_pause = IMG_LoadTexture(render,"./resources/button_num04_hover.png");
    SDL_Texture* tex_CheckTheClassSchedule_click = IMG_LoadTexture(render,"./resources/button_num04_click.png");
    SDL_Texture* tex_ChangePersonalPassword_pause = IMG_LoadTexture(render,"./resources/button_num05_hover.png");
    SDL_Texture* tex_ChangePersonalPassword_click = IMG_LoadTexture(render,"./resources/button_num05_click.png");
    SDL_Texture* tex_Exit_pause = IMG_LoadTexture(render,"./resources/button_num06_hover.png");
    SDL_Texture* tex_Exit_click = IMG_LoadTexture(render,"./resources/button_num06_click.png");
    if(!(tex_CheckPersonalData_pause && tex_CheckPersonalData_click &&
        tex_QueryCourseGradeInformation_pause && tex_QueryCourseGradeInformation_click &&
        tex_EnterOrModifyGrades_pause && tex_EnterOrModifyGrades_click &&
        tex_CheckTheClassSchedule_pause && tex_CheckTheClassSchedule_click &&
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
                if(is_QueryCourseGradeInformation_pause) {Mix_PlayMusic(buttonPlay,1);is_QueryCourseGradeInformation_click = true;}
                if(is_EnterOrModifyGrades_pause) {Mix_PlayMusic(buttonPlay,1);is_EnterOrModifyGrades_click = true;}
                if(is_CheckTheClassSchedule_pause) {Mix_PlayMusic(buttonPlay,1);;is_CheckTheClassSchedule_click = true;}
                if(is_ChangePersonalPassword_pause){Mix_PlayMusic(buttonPlay,1);is_ChangePersonalPassword_click = true;}
                if(is_Exit_pause) {Mix_PlayMusic(buttonPlay,1);is_Exit_click = true;}
            }
//-----------------------------------------------------------------------
//                      功能实现!!!!!!
//-----------------------------------------------------------------------
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_CheckPersonalData_click){
                    Teacher_CheckPersonalData(teacherID);
                    is_CheckPersonalData_click = false;
                }
                if(is_QueryCourseGradeInformation_click){
                    Teacher_QueryCourseGradeInformation(teacherID);
                    is_QueryCourseGradeInformation_click = false;
                }
                if(is_EnterOrModifyGrades_click){
                    Teacher_EnterOrModifyGrades(teacherID);
                    is_EnterOrModifyGrades_click = false;
                }
                if(is_CheckTheClassSchedule_click){
                    Teacher_CheckTheClassSchedule(teacherID);
                    is_CheckTheClassSchedule_click = false;
                }
                if(is_ChangePersonalPassword_click){
                    Teacher_ChangePersonalPassword(teacherID);       
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
        if(SDL_IsInRange(mx,my,&dst_QueryCourseGradeInformation)) is_QueryCourseGradeInformation_pause = true;
        else is_QueryCourseGradeInformation_pause = false;
        if(SDL_IsInRange(mx,my,&dst_EnterOrModifyGrades)) is_EnterOrModifyGrades_pause = true;
        else is_EnterOrModifyGrades_pause = false;
        if(SDL_IsInRange(mx,my,&dst_CheckTheClassSchedule)) is_CheckTheClassSchedule_pause = true;
        else is_CheckTheClassSchedule_pause = false;
        if(SDL_IsInRange(mx,my,&dst_ChangePersonalPassword)) is_ChangePersonalPassword_pause = true;
        else is_ChangePersonalPassword_pause = false;
        if(SDL_IsInRange(mx,my,&dst_Exit)) is_Exit_pause = true;
        else is_Exit_pause = false;

        if(is_CheckPersonalData_click) SDL_RenderCopy(render,tex_CheckPersonalData_click,NULL,&dst_CheckPersonalData);
        else if  (is_CheckPersonalData_pause) SDL_RenderCopy(render,tex_CheckPersonalData_pause,NULL,&dst_CheckPersonalData);
        if(is_QueryCourseGradeInformation_click) SDL_RenderCopy(render,tex_QueryCourseGradeInformation_click,NULL,&dst_QueryCourseGradeInformation);
        else if (is_QueryCourseGradeInformation_pause) SDL_RenderCopy(render,tex_QueryCourseGradeInformation_pause,NULL,&dst_QueryCourseGradeInformation);
        if(is_EnterOrModifyGrades_click) SDL_RenderCopy(render,tex_EnterOrModifyGrades_click,NULL,&dst_EnterOrModifyGrades);
        else if (is_EnterOrModifyGrades_pause) SDL_RenderCopy(render,tex_EnterOrModifyGrades_pause,NULL,&dst_EnterOrModifyGrades);
        if(is_CheckTheClassSchedule_click) SDL_RenderCopy(render,tex_CheckTheClassSchedule_click,NULL,&dst_CheckTheClassSchedule);
        else if (is_CheckTheClassSchedule_pause) SDL_RenderCopy(render,tex_CheckTheClassSchedule_pause,NULL,&dst_CheckTheClassSchedule);
        if (is_ChangePersonalPassword_click) SDL_RenderCopy(render,tex_ChangePersonalPassword_click,NULL,&dst_ChangePersonalPassword);
        else if (is_ChangePersonalPassword_pause) SDL_RenderCopy(render,tex_ChangePersonalPassword_pause,NULL,&dst_ChangePersonalPassword);
        if(is_Exit_click) SDL_RenderCopy(render,tex_Exit_click,NULL,&dst_Exit);
        else if (is_Exit_pause) SDL_RenderCopy(render,tex_Exit_pause,NULL,&dst_Exit);


        SDL_RenderPresent(render);  
    }

    SDL_DestroyTexture(tex_CheckPersonalData_pause);
    SDL_DestroyTexture(tex_CheckPersonalData_click);
    SDL_DestroyTexture(tex_QueryCourseGradeInformation_pause);
    SDL_DestroyTexture(tex_QueryCourseGradeInformation_click);
    SDL_DestroyTexture(tex_EnterOrModifyGrades_pause);
    SDL_DestroyTexture(tex_EnterOrModifyGrades_click);
    SDL_DestroyTexture(tex_CheckTheClassSchedule_pause);
    SDL_DestroyTexture(tex_CheckTheClassSchedule_click);
    SDL_DestroyTexture(tex_ChangePersonalPassword_pause);
    SDL_DestroyTexture(tex_ChangePersonalPassword_click);
    SDL_DestroyTexture(tex_Exit_pause);
    SDL_DestroyTexture(tex_Exit_click);
    SDL_DestroyTexture(tex_StudentUi);
    return true;
}
static Course Teacher_GetCourseInformationFromTeacher(char* teacherID){
    if(!teacherID || strlen(teacherID) < 1) return NULL;
    RBTree stack[100] = {NULL};
    int top = -1;
    RBTree cur = courseHead;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        Course data = (Course)stack[top]->data;
        if(strcmp(data->teacherID,teacherID) == 0) return data;
        cur = stack[top--]->right;
    }
    return NULL;
}

// 查看个人信息
bool Teacher_CheckPersonalData(char* teacherID){
    struct Teacher temp;
    strcpy(temp.teacherID,teacherID);
    RBTree node = RBTree_SEARCH(&teacherHead,&temp,Teacher_CmpCountAccountMatch);
    if(!node){
        printf("未知错误!\n");
        return false;
    }
    Teacher data = (Teacher)node->data;
    int pixel = 40;
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,pixel);
    // 教师编号 课程名称 教师姓名
    
    int w,h;
    SDL_Texture* tex_teacherID = SDL_CreateTextureFromTTF(font_temp,data->teacherID,black,2000);
    SDL_Texture* tex_name = SDL_CreateTextureFromTTF(font_temp,data->name,black,2000);
    if(!(tex_teacherID && tex_name)){
        SDL_Log("Teacher_CheckPersonalData ERROR:%s",SDL_GetError());
        return false;
    }
    // 连表查询
    SDL_Texture* tex_course = NULL;
    Course course = Teacher_GetCourseInformationFromTeacher(data->teacherID);
    if(course){
        tex_course = SDL_CreateTextureFromTTF(font_temp,course->courseName,black,2000);
        if(tex_course == NULL){
            SDL_Log("Teacher_CheckPersonalData ERROR:%s",SDL_GetError());
            return false;
        }
    }


    SDL_Texture* tex_CheckPersonalData = IMG_LoadTexture(render,"./resources/teacher_CheckPersonalData.png");
    SDL_Texture* tex_success = IMG_LoadTexture(render,"./resources/success.png");
    if(!(tex_CheckPersonalData && tex_success)){
        SDL_Log("Teacher_CheckPersonalData Error:%s",SDL_GetError());
        return false;
    }
    
    SDL_Rect dst_CheckPersonalData = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    int confirmW = 144,confirmH = confirmW / 2;
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - (confirmW >> 1) - 4,WINDOW_CENTER_Y - (confirmH >> 1) + 202,confirmW - 12,confirmH - 12};
    
    int tempX = 166,tempY = 10;
    SDL_Rect dst_teacherID = {dst_CheckPersonalData.x + tempX,dst_CheckPersonalData.y + 176 + tempY};
    TTF_SizeUTF8(font_temp,data->teacherID,&dst_teacherID.w,&dst_teacherID.h);

    SDL_Rect dst_name = {dst_CheckPersonalData.x + tempX,dst_CheckPersonalData.y + 343 + tempY};
    TTF_SizeUTF8(font_temp,data->name,&dst_name.w,&dst_name.h);

    SDL_Rect dst_course = {dst_CheckPersonalData.x + tempX,dst_CheckPersonalData.y + 260 + tempY};
    if(course){
        TTF_SizeUTF8(font_temp,course->courseName,&dst_course.w,&dst_course.h);
    }

    SDL_Event e;
    
    SDL_Texture* tex_confirm_pause = SDL_CreateTextureFromTTF(font_temp,"确认",black,2000);
    SDL_Texture* tex_confirm_click = SDL_CreateTextureFromTTF(font_temp,"确认",red,2000);
    SDL_Texture* tex_confirm_hover = SDL_CreateTextureFromTTF(font_temp,"确认",blue,2000);
    if(!(tex_confirm_click && tex_confirm_hover && tex_confirm_pause)){
        SDL_Log("Teacher_CheckPersonalData Error:%s",SDL_GetError());
        return false;
    }
    

    bool is_confirm_hover = false;
    bool is_confirm_click = false;


    int mx,my;

    while(true){
        SDL_GetMouseState(&mx,&my);
        if(Is_Whole_Quit) break;
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
                continue;
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
                    break;
                }
            }

        }
        
        SDL_RenderCopy(render,tex_CheckPersonalData,NULL,&dst_CheckPersonalData);

        SDL_RenderCopy(render,tex_teacherID,NULL,&dst_teacherID);
        SDL_RenderCopy(render,tex_name,NULL,&dst_name);
        SDL_RenderCopy(render,tex_course,NULL,&dst_course);

        SDL_SetRenderDrawColor(render,beige.r,beige.g,beige.b,beige.a);
        SDL_RenderFillRect(render,&dst_confirm);
        SDL_RenderCopy(render,tex_confirm_pause,NULL,&dst_confirm);
        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover = true;
        }else{
            is_confirm_hover = false;
            is_confirm_click = false;
        }

        if(is_confirm_click){
            SDL_RenderCopy(render,tex_confirm_click,NULL,&dst_confirm);
        }else if(is_confirm_hover){
            SDL_RenderCopy(render,tex_confirm_hover,NULL,&dst_confirm);
        }
        SDL_RenderPresent(render);
    }


    SDL_DestroyTexture(tex_confirm_pause);
    SDL_DestroyTexture(tex_confirm_click);
    SDL_DestroyTexture(tex_confirm_hover);
    SDL_RenderSetClipRect(render,NULL);
    SDL_DestroyTexture(tex_success);
    SDL_DestroyTexture(tex_CheckPersonalData);
    if(course){
        SDL_DestroyTexture(tex_course);
    }
    SDL_DestroyTexture(tex_name);
    SDL_DestroyTexture(tex_teacherID);
    TTF_CloseFont(font_temp);
    return true;
}
static bool Teacher_StudentInformationAdd(RBTree node,char* major,char* buffer,size_t size,int rank){
    if(!node || !buffer || strlen(major) < 1){
        strcpy(buffer,"");
        return false;
    }
    char ch[20] = "";
    if(!strcmp(major,"MATH001")){
        strcpy(ch,"高等数学");
    }
    if(!strcmp(major,"CS001")){
        strcpy(ch,"数字逻辑");
    }
    if(!strcmp(major,"CS002")){
        strcpy(ch,"离散数学");
    }
    int index = 0;
    float score = -1;
    Student data = (Student)node->data;
    for(index;index < MAX_COURSES;index++){
        if(!strcmp(data->courseScores[index].courseID,major)){
            score = data->courseScores[index].score;
            break;
        }
    }
    snprintf(buffer,size,"%4d %12s %20s %s:%.2f",
            rank + 1,data->studentID,data->name,ch,score);
    return true;
}


// qsort中传入的是RBTree* 不是RBTree  模板和c++的sort传入的回调函数不同
static int Teacher_CmpGradeMATH001(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[0].score < data02->courseScores[0].score ;
}
static int Teacher_CmpGradeCS001(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[1].score < data02->courseScores[1].score;
}
static int Teacher_CmpGradeCS002(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[2].score < data02->courseScores[2].score;
}

bool Teacher_QueryCourseGradeInformation(char* teacherID){
    if(!teacherID || strlen(teacherID) < 1) return false;
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
   

    RBTree arr02[PERSON_MAX_LIMIT] = {NULL};
    RBTree stack02[STACK_INDEX] = {NULL};
    int index02 = 0;
    top = -1;
    cur = courseHead;
    Course find = NULL;
    while(cur || ~top){
        while(cur){
            if(!strcmp(((Course)cur->data)->teacherID,teacherID)){
                find = (Course)cur->data;
                goto Teacher_QueryCourseGradeInformation_lables;
            }
            stack02[++top] = cur;
            cur = cur->left;
        }
        cur = stack02[top--];
        arr02[index02++] =  cur;
        cur = cur->right;
    }   
Teacher_QueryCourseGradeInformation_lables:

    if(!find){
        Edu_Warn("未找到您的授课",40);
        return false;
    }
    // 对成绩进行排序
    if(!strcmp(find->courseID,"MATH001")){
        qsort(arr,index,sizeof(RBTree),Teacher_CmpGradeMATH001);
    }
    if(!strcmp(find->courseID,"CS001")){
        qsort(arr,index,sizeof(RBTree),Teacher_CmpGradeCS001);
    }
    if(!(strcmp(find->courseID,"CS002"))){
        qsort(arr,index,sizeof(RBTree),Teacher_CmpGradeCS002);
    }

    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,30);
    SDL_Surface* sur_StudentInformation = NULL;
    SDL_Texture* tex_StudentInformation = NULL;
    SDL_Rect dst_StudentInformation = {0,0,0,0};
    SDL_Texture* tex_next_black = SDL_CreateTextureFromTTF(font_temp,"下一页",black,2000);
    SDL_Texture* tex_last_black = SDL_CreateTextureFromTTF(font_temp,"上一页",black,2000);
    SDL_Texture* tex_next_blue = SDL_CreateTextureFromTTF(font_temp,"下一页",blue,2000);
    SDL_Texture* tex_last_blue = SDL_CreateTextureFromTTF(font_temp,"上一页",blue,2000);
    SDL_Texture* tex_next_red = SDL_CreateTextureFromTTF(font_temp,"下一页",red,2000);
    SDL_Texture* tex_last_red = SDL_CreateTextureFromTTF(font_temp,"上一页",red,2000);
    SDL_Texture* tex_quit_black = SDL_CreateTextureFromTTF(font_temp,"退出",black,2000);
    SDL_Texture* tex_quit_blue = SDL_CreateTextureFromTTF(font_temp,"退出",blue,2000);
    SDL_Texture* tex_quit_red = SDL_CreateTextureFromTTF(font_temp,"退出",red,2000);

    SDL_Rect dst_next = {1425,1040,120,40};
    SDL_Rect dst_last = {435,1040,120,40};
    SDL_Rect dst_quit = {930,1040,120,40};
    SDL_Color fg = {0,0,0,255};
    char buffer[1024] = "";
        
    bool is_next_blue = false;
    bool is_last_blue = false;
    bool is_next_red = false;
    bool is_last_red = false;
    bool is_quit_blue = false;
    bool is_quit_red = false;


    int mx,my;
    SDL_Event e;
    int curPage = 0;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,255,255,255,255);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render,0,0,0,255);
        SDL_RenderDrawRect(render,&dst_last);
        SDL_RenderDrawRect(render,&dst_next);
        SDL_RenderDrawRect(render,&dst_quit);
        SDL_RenderCopy(render,tex_next_black,NULL,&dst_next);
        SDL_RenderCopy(render,tex_last_black,NULL,&dst_last);
        SDL_RenderCopy(render,tex_quit_black,NULL,&dst_quit);
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
                if(is_next_blue){
                    Mix_PlayMusic(buttonPlay,1);
                    is_next_red = true;
                }
                if(is_last_blue){
                    Mix_PlayMusic(buttonPlay,1);
                    is_last_red = true;
                }
                if(is_quit_blue){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_red = true;
                }

            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_next_red){
                    if(curPage + 30 < index){
                        curPage += 30;
                    }
                    is_next_red = false;
                }             
                if(is_last_red){
                    if((curPage -30) >= 0){
                        curPage -= 30;
                    }
                    is_last_red = false;
                }   
                if(is_quit_red){
                    break;
                    is_quit_red = false;

                }

            }

        }
        int x = 600, y = 0;
        for(int i = curPage; i < curPage + 30 && i < index && i >= 0;i++){
            if(arr[i]){
            Teacher_StudentInformationAdd(arr[i],find->courseID,buffer,sizeof(buffer),i);
            sur_StudentInformation = TTF_RenderUTF8_Blended_Wrapped(font_temp,buffer,fg,1980);
            if(!sur_StudentInformation){
                SDL_Log("sur_StudentInformation Load ERROR:%s",SDL_GetError());
                return false;
            }
            tex_StudentInformation = SDL_CreateTextureFromSurface(render,sur_StudentInformation);
            if(!tex_StudentInformation){
                SDL_Log("tex_StudentInformation Load ERROR:%s",SDL_GetError());
                return false;
            }
            dst_StudentInformation.w = sur_StudentInformation->w;
            dst_StudentInformation.h = sur_StudentInformation->h;
            dst_StudentInformation.x = x;
            dst_StudentInformation.y = (i % 30) * dst_StudentInformation.h ;
            SDL_RenderCopy(render,tex_StudentInformation,NULL,&dst_StudentInformation);
            SDL_DestroyTexture(tex_StudentInformation);
            SDL_FreeSurface(sur_StudentInformation);
            }else{
                break;
            }
        }
        if(SDL_IsInRange(mx,my,&dst_last)){
            is_last_blue = true;
        }else{
            is_last_blue = false;
        }
        if(SDL_IsInRange(mx,my,&dst_next)){
            is_next_blue = true;
        }else{
            is_next_blue = false;
        }
        if(SDL_IsInRange(mx,my,&dst_quit)){
            is_quit_blue = true;
        }else{
            is_quit_blue = false;
        }


        if(is_next_red){
            SDL_RenderCopy(render,tex_next_red,NULL,&dst_next);
        }else if(is_next_blue){
            SDL_RenderCopy(render,tex_next_blue,NULL,&dst_next);
        }
        if(is_last_red){
            SDL_RenderCopy(render,tex_last_red,NULL,&dst_last);
        }else if(is_last_blue){
            SDL_RenderCopy(render,tex_last_blue,NULL,&dst_last);
        }
        if(is_quit_red){
            SDL_RenderCopy(render,tex_quit_red,NULL,&dst_quit);
        }else if(is_quit_blue){
            SDL_RenderCopy(render,tex_quit_red,NULL,&dst_quit);
        }

        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_quit_black);
    SDL_DestroyTexture(tex_quit_blue);
    SDL_DestroyTexture(tex_quit_red);
    SDL_DestroyTexture(tex_next_red);
    SDL_DestroyTexture(tex_last_red);
    SDL_DestroyTexture(tex_next_blue);
    SDL_DestroyTexture(tex_last_blue);
    SDL_DestroyTexture(tex_last_black);
    SDL_DestroyTexture(tex_next_black);
    TTF_CloseFont(font_temp);
    return true;
}
                    
bool Teacher_ChangePersonalPassword(char* account){
    struct Teacher temp;
    strcpy(temp.teacherID,account);
    RBTree cur = RBTree_SEARCH(&teacherHead,&temp,Teacher_CmpCountAccountMatch);
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
                    Mix_PlayMusic(buttonPlay,1);
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
                         if(strcmp(((Teacher)cur->data)->password,ch_first_input) != 0){
                        Edu_Warn("您输入的原密码与真实密码不相同，请重新输入!",40);
                    }else{
                        if(strcmp(ch_second_input,ch_third_input) != 0){
                            Edu_Warn("您两次输入的密码不同，请重新输入!",40);
                            strcpy(ch_second_input,"");
                            strcpy(ch_second_fake,"");
                            strcpy(ch_third_fake,"");
                            strcpy(ch_third_input,"");
                        }else{
                            strcpy(((Teacher)cur->data)->password,ch_second_input);
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

static int Teacher_CmpStudent(const void* node_data, const void* input_data){
    Student data01 = (Student)node_data;
    Student data02 = (Student)input_data;
    return strcmp(data01->studentID,data02->studentID);
}
bool Teacher_EnterOrModifyGrades(char* teacherID){
    Course course = NULL;
    RBTree stack[PERSON_MAX_LIMIT] = {NULL};
    int top = -1;
    RBTree cur = courseHead;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            course = (Course)cur->data;
            if(!strcmp(course->teacherID,teacherID)){
                goto Teacher_EnterOrModifyGrades_lables;
            }
            cur = cur->left;
        }
        cur = stack[top--]->right;
    }
Teacher_EnterOrModifyGrades_lables:
    if(cur == NULL || strcmp(((Course)cur->data)->teacherID,teacherID)){
        Edu_Error("您暂无授课记录",40);
        return false;
    }
    course = (Course)cur->data;

    TTF_Font* font_temp = TTF_OpenFont(FONT_MIAO_PATH,40);
    if(font_temp == NULL){
        SDL_Log("Teacher_EnterOrModifyGrades Error: Failed to load font");
        return false;
    }

    SDL_Texture* tex_backGround = IMG_LoadTexture(render,"./resources/teacher_EnterOrModifyGrades.png");
    if(tex_backGround == NULL){
        SDL_Log("Teacher_EnterOrModifyGrades Error 1 :%s ",SDL_GetError());
        TTF_CloseFont(font_temp);
        return false;
    }
    SDL_Rect dst_backGround = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT };
    char ch_studentId[13] = "";
    char ch_score[5] = "";
    char ch_semesterCount[3] ="";
    int dx = 10,dh = 10;
    int w = 347,h = 65;
    SDL_Rect dst_studenID_box = {dst_backGround.x +  113,dst_backGround.y + 172,w,h};
    SDL_Rect dst_score_box = {dst_studenID_box.x,dst_backGround.y + 255,w,h};
    SDL_Rect dst_semesterCount_box = {dst_studenID_box.x,dst_backGround.y + 341,w,h};
    
    SDL_Rect dst_studentID_input = {dst_backGround.x +  113 + dx ,dst_backGround.y + 172 + dh};
    SDL_Rect dst_score_input = {dst_studenID_box.x + dx,dst_backGround.y + 255 + dh};
    SDL_Rect dst_semesterCount_input = {dst_studenID_box.x + dx,dst_backGround.y + 341 + dh};
    SDL_Rect dst_confirm = {dst_backGround.x +  190,dst_backGround.y + 446,100,50};

    bool is_studentID_input = false;
    bool is_score_input = false;
    bool is_semesterCount = false;
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    bool is_quit_hover = false;
    bool is_quit_click = false;
    SDL_Rect dst_quit = {WINDOW_WIDTH - 40,0,40,40};

    int mx,my;
    SDL_Event e;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_backGround,NULL,&dst_backGround);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        SDL_ShowQuitX(&dst_quit,black,black);
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){ 
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(SDL_IsInRange(mx,my,&dst_studenID_box)){
                    is_studentID_input = true;
                }else{
                    is_studentID_input = false;
                }
                if(SDL_IsInRange(mx,my,&dst_score_box)){
                    is_score_input = true;
                }else{
                    is_score_input = false;
                }
                if(SDL_IsInRange(mx,my,&dst_semesterCount_box)){
                    is_semesterCount = true;
                }else{
                    is_semesterCount = false;
                }
                if(is_confirm_hover){
                    is_confirm_click = true;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }

            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click){
                    if(!(strlen(ch_score) && strlen(ch_semesterCount) && strlen(ch_studentId))) {
                        is_confirm_click = false;
                        continue;
                    }
                    struct Student temp;
                    strcpy(temp.studentID,ch_studentId);
                    RBTree student_data = RBTree_SEARCH(&studentHead,&temp,Teacher_CmpStudent);
                    if(student_data == NULL){
                        Edu_Warn("该学生不存在，请重新输入",40);
                        strcpy(ch_score,"");
                        strcpy(ch_semesterCount,"");
                        strcpy(ch_studentId,"");
                    }else{
                        Student stu = (Student)student_data->data;
                    // printf("%s\n  course:%s\n courseID:%s %f %d\n",stu->name,course->courseName, course->courseID,stu->courseScores[0].score,stu->semesterCount);
                        
                        if(!strcmp("MATH001",course->courseID)){
                            stu->courseScores[0].score = atof(ch_score);
                        }else if(!strcmp("CS001",course->courseID)){
                            stu->courseScores[1].score = atof(ch_score);
                        }else if(!strcmp("CS002",course->courseID)){
                            stu->courseScores[2].score = atof(ch_score);
                        }
                        stu->semesterCount = atoi(ch_semesterCount);
                        stu->totalScore = stu->courseScores[0].score + stu->courseScores[1].score + stu->courseScores[2].score;
                        Edu_Success("修改成功!",40);
                        // printf("%f  %d \n",stu->courseScores[0].score,stu->semesterCount);
                        break;
                    }
                    
                }
                if(is_quit_click){
                    is_quit_click = false;
                    break;
                }
            }

            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_studentID_input && strlen(ch_studentId) > 0){
                        ch_studentId[strlen(ch_studentId) - 1] = '\0';
                    }
                    if(is_score_input && strlen(ch_score) > 0){
                        ch_score[strlen(ch_score) - 1] = '\0';
                    }
                    if(is_semesterCount && strlen(ch_semesterCount) > 0){
                        ch_semesterCount[strlen(ch_semesterCount) - 1] = '\0';
                    }
                }
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
            }

            if(e.type == SDL_TEXTINPUT){
                if(is_studentID_input && strlen(ch_studentId) < 12){
                    strcat(ch_studentId,e.text.text);
                }
                if(is_score_input && strlen(ch_score) < 4 ){
                    char c = e.text.text[0];
                    if (isdigit(c)) {
                        // 是数字，直接追加
                        strcat(ch_score, e.text.text);
                    } else if (c == '.') {
                        // 如果是小数点，检查之前是否已有小数点
                        if (strchr(ch_score, '.') == NULL) {
                            strcat(ch_score, e.text.text);
                        }
                        // 如果已有小数点，忽略本次输入
                    }
                }
                if(is_semesterCount && strlen(ch_semesterCount) < 2 && isdigit(e.text.text[0])){
                    strcat(ch_semesterCount,e.text.text);
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

        SDL_Texture* tex_studentID = SDL_CreateTextureFromTTF(font_temp,ch_studentId,black,2000);
        SDL_Texture* tex_score = SDL_CreateTextureFromTTF(font_temp,ch_score,black,2000);
        SDL_Texture* tex_semesterCount = SDL_CreateTextureFromTTF(font_temp,ch_semesterCount,black,2000);
        
        TTF_SizeUTF8(font_temp,ch_semesterCount,&dst_semesterCount_input.w,&dst_semesterCount_input.h);
        TTF_SizeUTF8(font_temp,ch_studentId,&dst_studentID_input.w,&dst_studentID_input.h);
        TTF_SizeUTF8(font_temp,ch_score,&dst_score_input.w,&dst_score_input.h);

        if(is_confirm_click){
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue)
;        }
        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }else if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }
        if(tex_studentID){
            SDL_RenderCopy(render,tex_studentID,NULL,&dst_studentID_input);
            SDL_DestroyTexture(tex_studentID);
        }
        if(tex_score){
            SDL_RenderCopy(render,tex_score,NULL,&dst_score_input);
            SDL_DestroyTexture(tex_score);
        }
        if(tex_semesterCount){
            SDL_RenderCopy(render,tex_semesterCount,NULL,&dst_semesterCount_input);
            SDL_DestroyTexture(tex_semesterCount);
        }
        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_backGround);
    TTF_CloseFont(font_temp);
    return true;
}
bool Teacher_CheckTheClassSchedule(char* teacherID){
    SDL_Texture* tex_table = NULL;
    // 高数
    if(!(strcmp(teacherID,"100001"))){
        tex_table = IMG_LoadTexture(render,"./resources/table_math001.png");
        if(tex_table == NULL){
            SDL_Log("Teacher_CheckTheClassSchedule : %s",SDL_GetError());
            return false;
        }
    }
    // 数字逻辑
    if(!(strcmp(teacherID,"100002"))){
        tex_table = IMG_LoadTexture(render,"./resources/table_cs001.png");
        if(tex_table == NULL){
            SDL_Log("Teacher_CheckTheClassSchedule : %s",SDL_GetError());
            return false;
        }
    }
    if(!(strcmp(teacherID,"100003"))){
        tex_table = IMG_LoadTexture(render,"./resources/table_cs002.png");
        if(tex_table == NULL){
            SDL_Log("Teacher_CheckTheClassSchedule : %s",SDL_GetError());
            return false;
        }
    }
    if(tex_table == NULL){
        Edu_Error("暂未查询到您的授课信息",40);
        return false;
    }
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_MOUSEBUTTONDOWN) {Mix_PlayMusic(buttonPlay,1);break;}
            if(e.type == SDL_KEYDOWN) {Mix_PlayMusic(buttonPlay,1);break;}
             if(e.type == SDL_QUIT) Is_Whole_Quit = true;
        }
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_table,NULL,NULL);
        SDL_RenderPresent(render);
    }
    SDL_DestroyTexture(tex_table);
    return true;
}