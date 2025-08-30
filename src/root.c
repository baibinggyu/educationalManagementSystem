#include "root.h"
bool Root_Login(void){
    SDL_Texture* tex_Root_Login = IMG_LoadTexture(render,Root_Login_Path);
    if(tex_Root_Login == NULL){
        SDL_Log("tex_Root_Login ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Texture* quit_button_pause = IMG_LoadTexture(render,"./resources/quit_button_pause.png");
    SDL_Texture* quit_button_click = IMG_LoadTexture(render,"./resources/quit_button_click.png");
    SDL_Texture* quit_button_hover = IMG_LoadTexture(render,"./resources/quit_button_hover.png");
    if(!(quit_button_click && quit_button_hover && quit_button_pause)){
        SDL_Log("quit_button_click || quit_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Color fg = {0,0,0,255};
    char* thisClass = "管理员端";
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
    int w = 597,h = 149;
    SDL_Rect dst_account = {692,384,w,h};
    SDL_Rect dst_passwd = {692,533,w,h};
    SDL_Rect dst_input_account = {843,430,0,0};
    SDL_Rect dst_input_passwd = {843,590,0,0};
    SDL_Rect dst_loggin_button  = {1127,683,160,78};
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
        SDL_RenderCopy(render,tex_Root_Login,NULL,NULL);
        SDL_RenderCopy(render,tex_thisClass,&src_thisClass,&dst_thisClass);
        SDL_RenderCopy(render,quit_button_pause,NULL,&dst_quit);
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
                    if(Root_IsCountMatch(text_account,text_passwd)){
                        Root_Ui(text_account);
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
    SDL_DestroyTexture(quit_button_pause);
    SDL_DestroyTexture(quit_button_hover);
    SDL_DestroyTexture(quit_button_click);
    SDL_DestroyTexture(tex_thisClass);
    SDL_DestroyTexture(tex_Root_Login);
    return true;
}

static int Root_CmpCountAccountMatch(const void* node_data, const void* input_data){
    Root data01 = (Root)node_data;
    Root data02 = (Root)input_data;
    return strcmp(data01->adminID,data02->adminID);
}
static int Root_CmpCountPasswdMatch(const void* node_data, const void* input_data){
    Root data01 = (Root)node_data;
    Root data02 = (Root)input_data;
    return strcmp(data01->password,data02->password);    

}
bool Root_IsCountMatch(char* adminID,char* passwd){
    struct Root temp;
    strcpy(temp.adminID,adminID);
    RBTree res = RBTree_SEARCH(&rootHead,&temp,Root_CmpCountAccountMatch);
    if(!res) return false;
    strcpy(temp.password,passwd);
    return Root_CmpCountPasswdMatch(&temp,res->data) == 0 ? true : false;
}
bool Root_Ui(char* adminID){
SDL_Texture* tex_StudentUi = IMG_LoadTexture(render,"./resources/root_UI.png");
    if(!tex_StudentUi){
        SDL_Log("tex_StudentUi Load Error:%s",SDL_GetError());
        return false;
    }

    // 长宽高 80
    int x = 80;
    SDL_Rect dst_ManageStudentInformation = {870,228,x,x};
    SDL_Rect dst_SetUpAccountAndInitialPassword = {870,339,x,x};
    SDL_Rect dst_QueryTheRankingOfStudentsInAllMajors = {870,446,x,x};
    SDL_Rect dst_CheckTheRankingOfIndividualSubjects = {870,555,x,x};
    SDL_Rect dst_Exit = {870,664,x,x};

    bool is_ManageStudentInformation_pause = false;
    bool is_ManageStudentInformation_click = false;
    bool is_SetUpAccountAndInitialPassword_pause = false;
    bool is_SetUpAccountAndInitialPassword_click = false;
    bool is_QueryTheRankingOfStudentsInAllMajors_pause = false;
    bool is_QueryTheRankingOfStudentsInAllMajors_click = false;
    bool is_CheckTheRankingOfIndividualSubjects_pause = false;
    bool is_CheckTheRankingOfIndividualSubjects_click = false;
    bool is_Exit_pause = false;
    bool is_Exit_click = false;

    SDL_Texture* tex_ManageStudentInformation_pause = IMG_LoadTexture(render,"./resources/button_num01_hover.png");
    SDL_Texture* tex_ManageStudentInformation_click = IMG_LoadTexture(render,"./resources/button_num01_click.png");
    SDL_Texture* tex_SetUpAccountAndInitialPassword_pause = IMG_LoadTexture(render,"./resources/button_num02_hover.png");
    SDL_Texture* tex_SetUpAccountAndInitialPassword_click = IMG_LoadTexture(render,"./resources/button_num02_click.png");
    SDL_Texture* tex_QueryTheRankingOfStudentsInAllMajors_pause = IMG_LoadTexture(render,"./resources/button_num03_hover.png");
    SDL_Texture* tex_QueryTheRankingOfStudentsInAllMajors_click = IMG_LoadTexture(render,"./resources/button_num03_click.png");
    SDL_Texture* tex_CheckTheRankingOfIndividualSubjects_pause = IMG_LoadTexture(render,"./resources/button_num04_hover.png");
    SDL_Texture* tex_CheckTheRankingOfIndividualSubjects_click = IMG_LoadTexture(render,"./resources/button_num04_click.png");
    SDL_Texture* tex_Exit_pause = IMG_LoadTexture(render,"./resources/button_num05_hover.png");
    SDL_Texture* tex_Exit_click = IMG_LoadTexture(render,"./resources/button_num05_click.png");

    if(!(tex_ManageStudentInformation_pause && tex_ManageStudentInformation_click &&
        tex_SetUpAccountAndInitialPassword_pause && tex_SetUpAccountAndInitialPassword_click &&
        tex_QueryTheRankingOfStudentsInAllMajors_pause && tex_QueryTheRankingOfStudentsInAllMajors_click &&
        tex_CheckTheRankingOfIndividualSubjects_pause && tex_CheckTheRankingOfIndividualSubjects_click &&
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
                if(is_ManageStudentInformation_pause) {Mix_PlayMusic(buttonPlay,1);is_ManageStudentInformation_click = true;}
                if(is_SetUpAccountAndInitialPassword_pause) {Mix_PlayMusic(buttonPlay,1);is_SetUpAccountAndInitialPassword_click = true;}
                if(is_QueryTheRankingOfStudentsInAllMajors_pause) {Mix_PlayMusic(buttonPlay,1);;is_QueryTheRankingOfStudentsInAllMajors_click = true;}
                if(is_CheckTheRankingOfIndividualSubjects_pause){Mix_PlayMusic(buttonPlay,1);is_CheckTheRankingOfIndividualSubjects_click = true;}
                if(is_Exit_pause) {Mix_PlayMusic(buttonPlay,1);is_Exit_click = true;}
            }
//-----------------------------------------------------------------------
//                      功能实现!!!!!!
//-----------------------------------------------------------------------
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_ManageStudentInformation_click){
                     Root_ManageStudentInformation();
                    is_ManageStudentInformation_click = false;
                }
                if(is_SetUpAccountAndInitialPassword_click){
                    Root_SetUpAccountAndInitialPassword();
                    is_SetUpAccountAndInitialPassword_click = false;
                }
                if(is_QueryTheRankingOfStudentsInAllMajors_click){
                    Root_QueryTheRankingOfStudentsInAllMajors();
                    is_QueryTheRankingOfStudentsInAllMajors_click = false;
                }
                if(is_CheckTheRankingOfIndividualSubjects_click){
                    Root_CheckTheRankingOfIndividualSubjects();
                    is_CheckTheRankingOfIndividualSubjects_click = false;
                }
                if(is_Exit_click){
                    break;
                    is_Exit_click = false;
                }
            }
            
        }

        if(SDL_IsInRange(mx,my,&dst_ManageStudentInformation)) is_ManageStudentInformation_pause = true;
        else is_ManageStudentInformation_pause = false;
        if(SDL_IsInRange(mx,my,&dst_SetUpAccountAndInitialPassword)) is_SetUpAccountAndInitialPassword_pause = true;
        else is_SetUpAccountAndInitialPassword_pause = false;
        if(SDL_IsInRange(mx,my,&dst_QueryTheRankingOfStudentsInAllMajors)) is_QueryTheRankingOfStudentsInAllMajors_pause = true;
        else is_QueryTheRankingOfStudentsInAllMajors_pause = false;
        if(SDL_IsInRange(mx,my,&dst_CheckTheRankingOfIndividualSubjects)) is_CheckTheRankingOfIndividualSubjects_pause = true;
        else is_CheckTheRankingOfIndividualSubjects_pause = false;
        if(SDL_IsInRange(mx,my,&dst_Exit)) is_Exit_pause = true;
        else is_Exit_pause = false;

        if(is_ManageStudentInformation_click) SDL_RenderCopy(render,tex_ManageStudentInformation_click,NULL,&dst_ManageStudentInformation);
        else if  (is_ManageStudentInformation_pause) SDL_RenderCopy(render,tex_ManageStudentInformation_pause,NULL,&dst_ManageStudentInformation);
        if(is_SetUpAccountAndInitialPassword_click) SDL_RenderCopy(render,tex_SetUpAccountAndInitialPassword_click,NULL,&dst_SetUpAccountAndInitialPassword);
        else if (is_SetUpAccountAndInitialPassword_pause) SDL_RenderCopy(render,tex_SetUpAccountAndInitialPassword_pause,NULL,&dst_SetUpAccountAndInitialPassword);
        if(is_QueryTheRankingOfStudentsInAllMajors_click) SDL_RenderCopy(render,tex_QueryTheRankingOfStudentsInAllMajors_click,NULL,&dst_QueryTheRankingOfStudentsInAllMajors);
        else if (is_QueryTheRankingOfStudentsInAllMajors_pause) SDL_RenderCopy(render,tex_QueryTheRankingOfStudentsInAllMajors_pause,NULL,&dst_QueryTheRankingOfStudentsInAllMajors);
        if (is_CheckTheRankingOfIndividualSubjects_click) SDL_RenderCopy(render,tex_CheckTheRankingOfIndividualSubjects_click,NULL,&dst_CheckTheRankingOfIndividualSubjects);
        else if (is_CheckTheRankingOfIndividualSubjects_pause) SDL_RenderCopy(render,tex_CheckTheRankingOfIndividualSubjects_pause,NULL,&dst_CheckTheRankingOfIndividualSubjects);
        if(is_Exit_click) SDL_RenderCopy(render,tex_Exit_click,NULL,&dst_Exit);
        else if (is_Exit_pause) SDL_RenderCopy(render,tex_Exit_pause,NULL,&dst_Exit);
        SDL_RenderPresent(render);  
    }

    SDL_DestroyTexture(tex_ManageStudentInformation_pause);
    SDL_DestroyTexture(tex_ManageStudentInformation_click);
    SDL_DestroyTexture(tex_SetUpAccountAndInitialPassword_pause);
    SDL_DestroyTexture(tex_SetUpAccountAndInitialPassword_click);
    SDL_DestroyTexture(tex_QueryTheRankingOfStudentsInAllMajors_pause);
    SDL_DestroyTexture(tex_QueryTheRankingOfStudentsInAllMajors_click);
    SDL_DestroyTexture(tex_CheckTheRankingOfIndividualSubjects_pause);
    SDL_DestroyTexture(tex_CheckTheRankingOfIndividualSubjects_click);
    SDL_DestroyTexture(tex_Exit_pause);
    SDL_DestroyTexture(tex_Exit_click);
    SDL_DestroyTexture(tex_StudentUi);
    return true;
}
static bool Root_StudentInformationAdd(RBTree node,char* buffer,size_t size){
    if(!node || !buffer) return false;
    Student data = (Student)node->data;
    snprintf(buffer,size,"%-12s %-15s %-20s %-35s %s:%.2f %s:%.2f %s:%.2f 总分:%.2f",
            data->studentID,data->name,data->password,data->major,"高数",
            data->courseScores[0].score,"数字逻辑",data->courseScores[1].score,
            "离散数学",data->courseScores[2].score,data->totalScore);
    return true;
}

bool Root_ManageStudentInformation(void){
    RBTree arr[PERSON_MAX_LIMIT] = {NULL};
    RBTree stack[STACK_INDEX] = {NULL};
    int index = 0;
    int top = -1;
    RBTree cur = studentHead;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        arr[index++] =  cur;
        cur = cur->right;
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
    SDL_Texture* tex_select_black = SDL_CreateTextureFromTTF(font_temp,"选择",black,2000);
    SDL_Texture* tex_select_blue = SDL_CreateTextureFromTTF(font_temp,"选择",blue,2000);
    SDL_Texture* tex_select_red = SDL_CreateTextureFromTTF(font_temp,"选择",red,2000);

    SDL_Rect dst_next = {1425,1040,120,40};
    SDL_Rect dst_last = {435,1040,120,40};
    SDL_Rect dst_select = {930,1040,120,40};
    SDL_Rect dst_quit = {1940,0,40,40};
    SDL_Color fg = {0,0,0,255};
    char buffer[1024] = "";
        
    bool is_next_blue = false;
    bool is_last_blue = false;
    bool is_next_red = false;
    bool is_last_red = false;
    bool is_select_blue = false;
    bool is_select_red = false;
    bool is_quit_blue = false;
    bool is_quit_red = false;
    bool is_update = true;

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
        SDL_RenderDrawRect(render,&dst_select);
        SDL_RenderCopy(render,tex_next_black,NULL,&dst_next);
        SDL_RenderCopy(render,tex_last_black,NULL,&dst_last);
        SDL_RenderCopy(render,tex_select_black,NULL,&dst_select);
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
                if(is_select_blue){
                    Mix_PlayMusic(buttonPlay,1);
                    is_select_red = true;
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
                if(is_select_red){
                    Root_SelectAndUpdate();
                    is_update  = true;
                    is_select_red = false;

                }
                if(is_quit_red){
                    break;
                }
            }

        }
        if(is_update){
            index = 0;
            int top = -1;
            RBTree cur = studentHead;
            while(cur || ~top){
                while(cur){
                    stack[++top] = cur;
                    cur = cur->left;
                }
                cur = stack[top--];
                arr[index++] =  cur;
                cur = cur->right;
            }   
            is_update = false;
        }
        int x = 0, y = 0;
        for(int i = curPage; i < curPage + 30 && i < index && i >= 0;i++){
            if(arr[i]){
            Root_StudentInformationAdd(arr[i],buffer,sizeof(buffer));
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
        if(SDL_IsInRange(mx,my,&dst_select)){
            is_select_blue = true;
        }else{
            is_select_blue = false;
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
        if(is_select_red){
            SDL_RenderCopy(render,tex_select_red,NULL,&dst_select);
        }else if(is_select_blue){
            SDL_RenderCopy(render,tex_select_red,NULL,&dst_select);
        }
        if(is_quit_red){
            SDL_ShowQuitX(&dst_quit,red,red);
        }else if (is_quit_blue){
            SDL_ShowQuitX(&dst_quit,blue,blue);
        }

        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_select_black);
    SDL_DestroyTexture(tex_select_blue);
    SDL_DestroyTexture(tex_select_red);
    SDL_DestroyTexture(tex_next_red);
    SDL_DestroyTexture(tex_last_red);
    SDL_DestroyTexture(tex_next_blue);
    SDL_DestroyTexture(tex_last_blue);
    SDL_DestroyTexture(tex_last_black);
    SDL_DestroyTexture(tex_next_black);
    TTF_CloseFont(font_temp);
    return true;
}

bool Root_SelectAndUpdate(void){
    int buttonW = 150,buttonH = 75;
    SDL_Texture* tex_root_button_CheckStudentInformation_black = IMG_LoadTexture(render,"./resources/root_button_CheckStudentInformation_black.png");
    SDL_Texture* tex_root_button_CheckStudentInformation_blue = IMG_LoadTexture(render,"./resources/root_button_CheckStudentInformation_blue.png");
    SDL_Texture* tex_root_button_CheckStudentInformation_red = IMG_LoadTexture(render,"./resources/root_button_CheckStudentInformation_red.png");
    SDL_Texture* tex_root_button_UpdataStudentInformation_black = IMG_LoadTexture(render,"./resources/root_button_UpdataStudentInformation_black.png");
    SDL_Texture* tex_root_button_UpdataStudentInformation_blue = IMG_LoadTexture(render,"./resources/root_button_UpdataStudentInformation_blue.png");
    SDL_Texture* tex_root_button_UpdataStudentInformation_red = IMG_LoadTexture(render,"./resources/root_button_UpdataStudentInformation_red.png");
    SDL_Texture* tex_root_button_RemoveStudentInformation_black = IMG_LoadTexture(render,"./resources/root_button_RemoveStudentInformation_black.png");
    SDL_Texture* tex_root_button_RemoveStudentInformation_blue = IMG_LoadTexture(render,"./resources/root_button_RemoveStudentInformation_blue.png");
    SDL_Texture* tex_root_button_RemoveStudentInformation_red = IMG_LoadTexture(render,"./resources/root_button_RemoveStudentInformation_red.png");
    SDL_Texture* tex_root_button_quit_black = IMG_LoadTexture(render,"./resources/root_button_quit_black.png");
    SDL_Texture* tex_root_button_quit_blue = IMG_LoadTexture(render,"./resources/root_button_quit_blue.png");
    SDL_Texture* tex_root_button_quit_red = IMG_LoadTexture(render,"./resources/root_button_quit_red.png");
    SDL_Texture* tex_back_ground = IMG_LoadTexture(render,"./resources/beige_background.png");
    if(!(tex_root_button_CheckStudentInformation_black && tex_root_button_CheckStudentInformation_blue &&
         tex_root_button_CheckStudentInformation_red && tex_root_button_UpdataStudentInformation_black &&
         tex_root_button_UpdataStudentInformation_blue && tex_root_button_UpdataStudentInformation_red &&
         tex_root_button_RemoveStudentInformation_black && tex_root_button_RemoveStudentInformation_blue &&
         tex_root_button_RemoveStudentInformation_red && tex_root_button_quit_black &&
         tex_root_button_quit_blue && tex_root_button_quit_red && tex_back_ground)){
            SDL_Log("Root_SelectAndUpdate Error:%s",SDL_GetError());
            return false;
         }

    SDL_Rect dst_back_groumd = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    int dh = 100;
    SDL_Rect dst_button_CheckStudentInformation = {dst_back_groumd.x + ((TIPS_WIDTH - buttonW) >> 1),dst_back_groumd.y + dh,buttonW,buttonH};
    SDL_Rect dst_button_UpdataStudentInformation = {dst_button_CheckStudentInformation.x,dst_button_CheckStudentInformation.y + dh,buttonW,buttonH};
    SDL_Rect dst_button_RemoveStudentInformation = {dst_button_UpdataStudentInformation.x,dst_button_UpdataStudentInformation.y + dh,buttonW,buttonH};
    SDL_Rect dst_quit = {dst_button_RemoveStudentInformation.x,dst_button_RemoveStudentInformation.y + dh,buttonW,buttonH};
    
    bool is_button_CheckStudentInformation_hover = false;
    bool is_button_CheckStudentInformation_click = false;
    bool is_button_UpdataStudentInformation_hover = false;
    bool is_button_UpdataStudentInformation_click = false;
    bool is_button_RemoveStudentInformation_hover = false;
    bool is_button_RemoveStudentInformation_click = false;
    bool is_quit_hover = false;
    bool is_quit_click =false;

    int mx,my;
    SDL_Event e;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_back_ground,NULL,&dst_back_groumd);
        SDL_RenderCopy(render,tex_root_button_CheckStudentInformation_black,NULL,&dst_button_CheckStudentInformation);
        SDL_RenderCopy(render,tex_root_button_UpdataStudentInformation_black,NULL,&dst_button_UpdataStudentInformation);
        SDL_RenderCopy(render,tex_root_button_RemoveStudentInformation_black,NULL,&dst_button_RemoveStudentInformation);
        SDL_RenderCopy(render,tex_root_button_quit_black,NULL,&dst_quit);

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
                if(is_button_CheckStudentInformation_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_button_CheckStudentInformation_click = true;
                }
                if(is_button_UpdataStudentInformation_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_button_UpdataStudentInformation_click = true;
                }
                if(is_button_RemoveStudentInformation_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_button_RemoveStudentInformation_click = true;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_button_CheckStudentInformation_click){
                    Root_SelectAndUpdate_Sub_QueryStudentInformation();
                    is_button_CheckStudentInformation_click = false;
                    
                }
                if(is_button_UpdataStudentInformation_click){
                    Root_SelectAndUpdate_Sub_ModifyStudentInformation();
                    is_button_UpdataStudentInformation_click = false;
                }
                if(is_button_RemoveStudentInformation_click){
                    Root_SelectAndUpdate_Sub_DeleteStudentInformation();
                    is_button_RemoveStudentInformation_click = false;
                }
                if(is_quit_click){
                    is_quit_click = false;
                    break;
                }
            }


        }

        if(SDL_IsInRange(mx,my,&dst_button_CheckStudentInformation)){
            is_button_CheckStudentInformation_hover = true;
        }else{
            is_button_CheckStudentInformation_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_button_UpdataStudentInformation)){
            is_button_UpdataStudentInformation_hover = true;
        }else{
            is_button_UpdataStudentInformation_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_button_RemoveStudentInformation)){
            is_button_RemoveStudentInformation_hover =true;
        }else{
            is_button_RemoveStudentInformation_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_quit)){
            is_quit_hover = true;
        }else{
            is_quit_hover = false;
        }
        if(is_button_CheckStudentInformation_click){
            SDL_RenderCopy(render,tex_root_button_CheckStudentInformation_red,NULL,&dst_button_CheckStudentInformation);
        }else if(is_button_CheckStudentInformation_hover){
            SDL_RenderCopy(render,tex_root_button_CheckStudentInformation_blue,NULL,&dst_button_CheckStudentInformation);
        }
        if(is_button_UpdataStudentInformation_click){
            SDL_RenderCopy(render,tex_root_button_UpdataStudentInformation_red,NULL,&dst_button_UpdataStudentInformation);
        }else if(is_button_UpdataStudentInformation_hover){
            SDL_RenderCopy(render,tex_root_button_UpdataStudentInformation_blue,NULL,&dst_button_UpdataStudentInformation);
        }
        if(is_button_RemoveStudentInformation_click){
            SDL_RenderCopy(render,tex_root_button_RemoveStudentInformation_red,NULL,&dst_button_RemoveStudentInformation);
        }else if(is_button_RemoveStudentInformation_hover){
            SDL_RenderCopy(render,tex_root_button_RemoveStudentInformation_blue,NULL,&dst_button_RemoveStudentInformation);
        }
        if(is_quit_click){
            SDL_RenderCopy(render,tex_root_button_quit_red,NULL,&dst_quit);
        }else if(is_quit_hover){
            SDL_RenderCopy(render,tex_root_button_quit_blue,NULL,&dst_quit);
        }
        SDL_RenderPresent(render);
    }



    SDL_DestroyTexture(tex_root_button_CheckStudentInformation_black);
    SDL_DestroyTexture(tex_root_button_CheckStudentInformation_blue);
    SDL_DestroyTexture(tex_root_button_CheckStudentInformation_red);
    SDL_DestroyTexture(tex_root_button_UpdataStudentInformation_black);
    SDL_DestroyTexture(tex_root_button_UpdataStudentInformation_blue);
    SDL_DestroyTexture(tex_root_button_UpdataStudentInformation_red);
    SDL_DestroyTexture(tex_root_button_RemoveStudentInformation_black);
    SDL_DestroyTexture(tex_root_button_RemoveStudentInformation_blue);
    SDL_DestroyTexture(tex_root_button_RemoveStudentInformation_red);
    SDL_DestroyTexture(tex_root_button_quit_black);
    SDL_DestroyTexture(tex_root_button_quit_blue);
    SDL_DestroyTexture(tex_root_button_quit_red);
    SDL_DestroyTexture(tex_back_ground);
    return true;    
}
static int Root_CmpStudent(const void* node_data,const void* input_data){
    return strcmp(((Student)node_data)->studentID,((Student)input_data)->studentID);
}
static int Root_RBTree_CmpStudent(const void* node_data, const void* input_data){
    Student data01 = (Student)node_data;
    Student data02 = (Student)input_data;
    return strcmp(data01->studentID,data02->studentID);
}

// 能查询 ./resources/root_SelectAndUpdate_Sub_QueryStudentInformation.png

// 输入查询学号 ./resources/student_CheckStudentNumber.png
bool Root_SelectAndUpdate_Sub_QueryStudentInformation(void){
    SDL_Texture* tex_CheckStudentNumber = IMG_LoadTexture(render,"./resources/student_CheckStudentNumber.png");
    if(tex_CheckStudentNumber == NULL){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation Load Error :%s",SDL_GetError());
        return false;
    }
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    if(!font_temp){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation font_temp Load Error:%s",SDL_GetError);
        return false;
    }
    char ch_account[13] = "";
    SDL_Rect dst_CheckStudentNumbe = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    bool is_quit_hover = false;
    bool is_quit_click =false;
    bool is_texting = false;
    int w = 150,h = 75;
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - (w >> 1),WINDOW_CENTER_Y + 150,w,h};
    SDL_Rect dst_quit = {WINDOW_WIDTH - 40,0,40,40};
    SDL_Rect dst_account_box = {dst_CheckStudentNumbe.x + 40,dst_CheckStudentNumbe.y +  162,450,52};
    int dx = 110;
    SDL_Rect dst_text_input = {dst_account_box.x + dx,dst_account_box.y + 10};
    
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_CheckStudentNumber,NULL,&dst_CheckStudentNumbe);
        SDL_Texture* tex_account = SDL_CreateTextureFromTTF(font_temp,ch_account,black,2000);
        TTF_SizeUTF8(font_temp,ch_account,&dst_text_input.w,&dst_text_input.h);
        SDL_RenderCopy(render,tex_account,NULL,&dst_text_input);
        SDL_DestroyTexture(tex_account);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        SDL_ShowQuitX(&dst_quit,black,black);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_texting && strlen(ch_account) > 0){
                        ch_account[strlen(ch_account) - 1] ='\0';
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }
                if(SDL_IsInRange(mx,my,&dst_account_box)){
                    is_texting = true;
                }else{
                    is_texting = false;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click && strlen(ch_account)){
                    struct Student temp;
                    strcpy(temp.studentID,ch_account);
                    RBTree find = RBTree_SEARCH(&studentHead,&temp,Root_CmpStudent);
                    if(find == NULL){
                        Edu_Error("您输入的学号有误，请重新输入!",40);
                        strcpy(ch_account,"");
                    }else{
                        Student data = (Student)find->data;
                        Root_SelectAndUpdate_Sub_QueryStudentInformation_Show(data);
                        break;
                    }
                    is_confirm_click = false;
                }else{
                    is_confirm_click = false;
                }
                if(is_quit_click){
                    break;
                }
            }
            if(e.type == SDL_TEXTINPUT){
                if(is_texting && strlen(ch_account) < 12){
                    strcat(ch_account,e.text.text);
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
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }
        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }else if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }


        SDL_RenderPresent(render);
    }

    TTF_CloseFont(font_temp);
    SDL_DestroyTexture(tex_CheckStudentNumber);
    return true;
}
bool Root_SelectAndUpdate_Sub_QueryStudentInformation_Show(Student stu){
    SDL_Texture* tex_SelectAndUpdate_Sub_QueryStudentInformation = IMG_LoadTexture(render,"./resources/root_SelectAndUpdate_Sub_QueryStudentInformation.png");
    if(tex_SelectAndUpdate_Sub_QueryStudentInformation == NULL){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation_Show tex_SelectAndUpdate_Sub_QueryStudentInformation Load Error :%s",SDL_GetError());
        return false;
    }
    TTF_Font* font_temp  = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    // char ch_account[13] = "";
    // char ch_name[50]  = "";
    // char ch_major[50]  =  "";
    char ch_semesterCount[5] = "";
    char ch_courseCount[5] =  "";
    char ch_admath[10] = "";
    char ch_logic[10] = "";
    char ch_discrete[10] = "";
    sprintf(ch_semesterCount,"%d",stu->semesterCount);
    sprintf(ch_courseCount,"%d",stu->courseCount);
    sprintf(ch_admath,"%.2f",stu->courseScores[0].score);
    sprintf(ch_logic,"%.2f",stu->courseScores[1].score);
    sprintf(ch_discrete,"%.2f",stu->courseScores[2].score);
    SDL_Texture* tex_account = SDL_CreateTextureFromTTF(font_temp,stu->studentID,black,2000);
    SDL_Texture* tex_name = SDL_CreateTextureFromTTF(font_temp,stu->name,black,2000);
    SDL_Texture* tex_major = SDL_CreateTextureFromTTF(font_temp,stu->major,black,2000);
    SDL_Texture* tex_semesterCount = SDL_CreateTextureFromTTF(font_temp,ch_semesterCount,black,2000);
    SDL_Texture* tex_courseCount = SDL_CreateTextureFromTTF(font_temp,ch_courseCount,black,2000);
    SDL_Texture* tex_admath = SDL_CreateTextureFromTTF(font_temp,ch_admath,black,2000);
    SDL_Texture* tex_logic = SDL_CreateTextureFromTTF(font_temp,ch_logic,black,2000);
    SDL_Texture* tex_discrete = SDL_CreateTextureFromTTF(font_temp,ch_discrete,black,2000);
    if(!(tex_account && tex_name && tex_major && tex_semesterCount && tex_courseCount && tex_admath && tex_logic && tex_discrete)){
        SDL_Log("Texture Load Error Root_SelectAndUpdate_Sub_QueryStudentInformation_Show :  %s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_SelectAndUpdate_Sub_QueryStudentInformation = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    int dx = 10,dh = -10;
    SDL_Rect dst_account = {dst_SelectAndUpdate_Sub_QueryStudentInformation.x + dx + 200,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 70 + dh};
    SDL_Rect dst_name = {dst_account.x,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 108  + dh};
    SDL_Rect dst_major = {dst_name.x,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 158  + dh};
    SDL_Rect dst_semesterCount = {dst_SelectAndUpdate_Sub_QueryStudentInformation.x + 222,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 200  + dh};
    SDL_Rect dst_coureseCount = {dst_SelectAndUpdate_Sub_QueryStudentInformation.x + 265,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 240  + dh};
    SDL_Rect dst_admath = {dst_SelectAndUpdate_Sub_QueryStudentInformation.x + 236,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 286  + dh};
    SDL_Rect dst_logic = {dst_admath.x,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 327  + dh};
    SDL_Rect dst_discrete = {dst_admath.x,dst_SelectAndUpdate_Sub_QueryStudentInformation.y + 370 +dh};
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - 75,WINDOW_CENTER_Y  + 150,150,75};
    TTF_SizeUTF8(font_temp,stu->studentID,&dst_account.w,&dst_account.h);
    TTF_SizeUTF8(font_temp,stu->name,&dst_name.w,&dst_name.h);
    TTF_SizeUTF8(font_temp,stu->major,&dst_major.w,&dst_major.h);
    TTF_SizeUTF8(font_temp,ch_semesterCount,&dst_semesterCount.w,&dst_semesterCount.h);
    TTF_SizeUTF8(font_temp,ch_courseCount,&dst_coureseCount.w,&dst_coureseCount.h);
    TTF_SizeUTF8(font_temp,ch_admath,&dst_admath.w,&dst_admath.h);
    TTF_SizeUTF8(font_temp,ch_logic,&dst_logic.w,&dst_logic.h);
    TTF_SizeUTF8(font_temp,ch_discrete,&dst_discrete.w,&dst_discrete.h);
    
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_SelectAndUpdate_Sub_QueryStudentInformation,NULL,&dst_SelectAndUpdate_Sub_QueryStudentInformation);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        SDL_RenderCopy(render,tex_account,NULL,&dst_account);
        SDL_RenderCopy(render,tex_name,NULL,&dst_name);
        SDL_RenderCopy(render,tex_major,NULL,&dst_major);
        SDL_RenderCopy(render,tex_admath,NULL,&dst_admath);
        SDL_RenderCopy(render,tex_logic,NULL,&dst_logic);
        SDL_RenderCopy(render,tex_discrete,NULL,&dst_discrete);
        SDL_RenderCopy(render,tex_semesterCount,NULL,&dst_semesterCount);
        SDL_RenderCopy(render,tex_courseCount,NULL,&dst_coureseCount);
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
                    break;
                }
            }


        }
        if(SDL_IsInRange(mx,my,&dst_confirm)){
            is_confirm_hover  = true;
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


    SDL_DestroyTexture(tex_account);
    SDL_DestroyTexture(tex_name);
    SDL_DestroyTexture(tex_major);
    SDL_DestroyTexture(tex_semesterCount);
    SDL_DestroyTexture(tex_courseCount);
    SDL_DestroyTexture(tex_admath);
    SDL_DestroyTexture(tex_logic);
    SDL_DestroyTexture(tex_discrete);
    SDL_DestroyTexture(tex_SelectAndUpdate_Sub_QueryStudentInformation);
    TTF_CloseFont(font_temp);
    return true;
}



bool Root_SelectAndUpdate_Sub_ModifyStudentInformation(void){
SDL_Texture* tex_CheckStudentNumber = IMG_LoadTexture(render,"./resources/root_SelectAndUpdate_Sub_ModifyStudentInformation.png");
    if(tex_CheckStudentNumber == NULL){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation Load Error :%s",SDL_GetError());
        return false;
    }
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    if(!font_temp){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation font_temp Load Error:%s",SDL_GetError);
        return false;
    }
    char ch_account[13] = "";
    SDL_Rect dst_CheckStudentNumbe = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    bool is_quit_hover = false;
    bool is_quit_click =false;
    bool is_texting = false;
    int w = 150,h = 75;
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - (w >> 1),WINDOW_CENTER_Y + 150,w,h};
    SDL_Rect dst_quit = {WINDOW_WIDTH - 40,0,40,40};
    SDL_Rect dst_account_box = {dst_CheckStudentNumbe.x + 40,dst_CheckStudentNumbe.y +  162,450,52};
    int dx = 110;
    SDL_Rect dst_text_input = {dst_account_box.x + dx,dst_account_box.y + 10};
    
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_CheckStudentNumber,NULL,&dst_CheckStudentNumbe);
        SDL_Texture* tex_account = SDL_CreateTextureFromTTF(font_temp,ch_account,black,2000);
        TTF_SizeUTF8(font_temp,ch_account,&dst_text_input.w,&dst_text_input.h);
        SDL_RenderCopy(render,tex_account,NULL,&dst_text_input);
        SDL_DestroyTexture(tex_account);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        SDL_ShowQuitX(&dst_quit,black,black);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_texting && strlen(ch_account) > 0){
                        ch_account[strlen(ch_account) - 1] ='\0';
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }
                if(SDL_IsInRange(mx,my,&dst_account_box)){
                    is_texting = true;
                }else{
                    is_texting = false;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click && strlen(ch_account)){
                    struct Student temp;
                    strcpy(temp.studentID,ch_account);
                    RBTree find = RBTree_SEARCH(&studentHead,&temp,Root_CmpStudent);
                    if(find == NULL){
                        Edu_Error("您输入的学号有误，请重新输入!",40);
                        strcpy(ch_account,"");
                    }else{
                        Student data = (Student)find->data;
                        Root_SelectAndUpdate_Sub_ModifyStudentInformation_Show(data);
                        break;
                    }
                    is_confirm_click = false;
                }else{
                    is_confirm_click = false;
                }
                if(is_quit_click){
                    break;
                }
            }
            if(e.type == SDL_TEXTINPUT){
                if(is_texting && strlen(ch_account) < 12){
                    strcat(ch_account,e.text.text);
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
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }
        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }else if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }


        SDL_RenderPresent(render);
    }

    TTF_CloseFont(font_temp);
    SDL_DestroyTexture(tex_CheckStudentNumber);
    return true;
}

bool Root_SelectAndUpdate_Sub_ModifyStudentInformation_Show(Student stu){
    if(!stu){
        printf("未知错误!\n");
        return false;
    }
    SDL_Texture* tex_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show = IMG_LoadTexture(render,"./resources/root_SelectAndUpdate_Sub_ModifyStudentInformation_Show.png");
    if(tex_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show == NULL){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show Error :%s",SDL_GetError());
        return false;
    }

    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    SDL_Rect dst_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect dst_quit = {WINDOW_WIDTH - 40,0,40,40};
    int w = 100,h = 50;
    SDL_Rect dst_modify = {WINDOW_CENTER_X - (w >> 1),WINDOW_CENTER_Y + 200,w,h};
    SDL_Rect dst_temp = dst_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show;
    
    SDL_Rect dst_name = {dst_temp.x + 87,dst_temp.y + 89,317,41};
    SDL_Rect dst_passwd = {dst_temp.x + 87,dst_temp.y + 144,320,27};
    SDL_Rect dst_major = {dst_temp.x + 84,dst_temp.y + 182,326,26};
    SDL_Rect dst_semesterCount = {dst_temp.x + 113,dst_temp.y + 212,323,35};
    SDL_Rect dst_courseCount = {dst_temp.x + 200,dst_temp.y + 250,286,45};
    SDL_Rect dst_admath = {dst_temp.x + 200,dst_temp.y + 299,286,38};
    SDL_Rect dst_logic = {dst_temp.x + 200,dst_temp.y + 339,286,50};
    SDL_Rect dst_discrete = {dst_temp.x + 200,dst_temp.y + 390,286,40};
    char ch_name[50] = "";
    char ch_passwd[21] = "";
    char ch_major[50] = "";
    char ch_semesterCount[5] = "";
    char ch_courseCount[5] = "";
    char ch_admath[10] = "";
    char ch_logic[10] = "";
    char ch_discrete[10] = "";
    SDL_Rect dst_name_input = {dst_temp.x + 87,dst_temp.y + 92};
    SDL_Rect dst_passwd_input = {dst_temp.x + 87,dst_temp.y + 134};
    SDL_Rect dst_major_input = {dst_temp.x + 84,dst_temp.y - 12 + 182};
    SDL_Rect dst_semesterCount_input = {dst_temp.x + 113,dst_temp.y + 212};
    SDL_Rect dst_courseCount_input = {dst_temp.x + 200,dst_temp.y + 250};
    SDL_Rect dst_admath_input = {dst_temp.x + 200,dst_temp.y + 299};
    SDL_Rect dst_logic_input = {dst_temp.x + 200,dst_temp.y + 339};
    SDL_Rect dst_discrete_input = {dst_temp.x + 200,dst_temp.y + 390};
    
    bool is_name = false;
    bool is_passwd = false;
    bool is_major = false;
    bool is_semesterCount = false;
    bool is_courseCount = false;
    bool is_admath = false;
    bool is_logic = false;
    bool is_discrete = false;

    bool is_quit_click = false;
    bool is_quit_hover = false;
    bool is_modify_click = false;
    bool is_modify_hover = false;
    
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show,NULL,&dst_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show);
        SDL_ShowButtonWithContent("修改",&dst_modify,black);
        SDL_ShowQuitX(&dst_quit,black,black);
        
        SDL_Texture* tex_name = SDL_CreateTextureFromTTF(font_temp,ch_name,black,2000);
        SDL_Texture* tex_passwd = SDL_CreateTextureFromTTF(font_temp,ch_passwd,black,2000);
        SDL_Texture* tex_major = SDL_CreateTextureFromTTF(font_temp,ch_major,black,2000);
        SDL_Texture* tex_semesterCount = SDL_CreateTextureFromTTF(font_temp,ch_semesterCount,black,2000);
        SDL_Texture* tex_courseCount = SDL_CreateTextureFromTTF(font_temp,ch_courseCount,black,2000);
        SDL_Texture* tex_admath = SDL_CreateTextureFromTTF(font_temp,ch_admath,black,2000);
        SDL_Texture* tex_logic = SDL_CreateTextureFromTTF(font_temp,ch_logic,black,2000);
        SDL_Texture* tex_discrete = SDL_CreateTextureFromTTF(font_temp,ch_discrete,black,2000);
        TTF_SizeUTF8(font_temp,ch_name,&dst_name_input.w,&dst_name_input.h);
        TTF_SizeUTF8(font_temp,ch_passwd,&dst_passwd_input.w,&dst_passwd_input.h);
        TTF_SizeUTF8(font_temp,ch_major,&dst_major_input.w,&dst_major_input.h);
        TTF_SizeUTF8(font_temp,ch_semesterCount,&dst_semesterCount_input.w,&dst_semesterCount_input.h);
        TTF_SizeUTF8(font_temp,ch_courseCount,&dst_courseCount_input.w,&dst_courseCount_input.h);
        TTF_SizeUTF8(font_temp,ch_admath,&dst_admath_input.w,&dst_admath_input.h);
        TTF_SizeUTF8(font_temp,ch_logic,&dst_logic_input.w,&dst_logic_input.h);
        TTF_SizeUTF8(font_temp,ch_discrete,&dst_discrete_input.w,&dst_discrete_input.h);
        SDL_RenderCopy(render,tex_name,NULL,&dst_name_input);
        SDL_RenderCopy(render,tex_passwd,NULL,&dst_passwd_input);
        SDL_RenderCopy(render,tex_major,NULL,&dst_major_input);
        SDL_RenderCopy(render,tex_semesterCount,NULL,&dst_semesterCount_input);
        SDL_RenderCopy(render,tex_courseCount,NULL,&dst_courseCount_input);
        SDL_RenderCopy(render,tex_admath,NULL,&dst_admath_input);
        SDL_RenderCopy(render,tex_logic,NULL,&dst_logic_input);
        SDL_RenderCopy(render,tex_discrete,NULL,&dst_discrete_input);
        SDL_DestroyTexture(tex_name);
        SDL_DestroyTexture(tex_passwd);
        SDL_DestroyTexture(tex_major);
        SDL_DestroyTexture(tex_semesterCount);
        SDL_DestroyTexture(tex_courseCount);
        SDL_DestroyTexture(tex_admath);
        SDL_DestroyTexture(tex_logic);
        SDL_DestroyTexture(tex_discrete);

        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
                continue;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE) {
                if (is_name && strlen(ch_name) > 0) {
                    ch_name[strlen(ch_name) - 1] = '\0';
                }
                if (is_passwd && strlen(ch_passwd) > 0) {
                    ch_passwd[strlen(ch_passwd) - 1] = '\0';
                }
                if (is_major && strlen(ch_major) > 0) {
                    ch_major[strlen(ch_major) - 1] = '\0';
                }
                if (is_semesterCount && strlen(ch_semesterCount) > 0) {
                    ch_semesterCount[strlen(ch_semesterCount) - 1] = '\0';
                }
                if (is_courseCount && strlen(ch_courseCount) > 0) {
                    ch_courseCount[strlen(ch_courseCount) - 1] = '\0';
                }
                if (is_admath && strlen(ch_admath) > 0) {
                    ch_admath[strlen(ch_admath) - 1] = '\0';
                }
                if (is_logic && strlen(ch_logic) > 0) {
                    ch_logic[strlen(ch_logic) - 1] = '\0';
                }
                if (is_discrete && strlen(ch_discrete) > 0) {
                    ch_discrete[strlen(ch_discrete) - 1] = '\0';
                }
            }

            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_modify_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_modify_click = true;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }

                if (SDL_IsInRange(mx, my, &dst_name)) {
                    SDL_StartTextInput();
                    is_name = true;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_passwd)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = true;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_major)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = true;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_semesterCount)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = true;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_courseCount)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = true;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_admath)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = true;
                    is_logic = false;
                    is_discrete = false;
                } else if (SDL_IsInRange(mx, my, &dst_logic)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = true;
                    is_discrete = false;
                }else if (SDL_IsInRange(mx, my, &dst_discrete)) {
                    SDL_StartTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = true;
                } else {
                    SDL_StopTextInput();
                    is_name = false;
                    is_passwd = false;
                    is_major = false;
                    is_semesterCount = false;
                    is_courseCount = false;
                    is_admath = false;
                    is_logic = false;
                    is_discrete = false;
                }

                
            }
            
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_modify_click){
                    if(strlen(ch_name)){
                        strcpy(stu->name,ch_name);
                    }
                    if(strlen(ch_passwd)){
                        strcpy(stu->password,ch_passwd);
                    }
                    if(strlen(ch_major)){
                        strcpy(stu->major,ch_major);
                    }
                    if(strlen(ch_semesterCount)){
                        stu->semesterCount = atoi(ch_semesterCount);
                    }
                    if(strlen(ch_courseCount)){
                        stu->courseCount = atoi(ch_courseCount);
                    }
                    if(strlen(ch_admath)){
                        stu->courseScores[0].score = atof(ch_admath);
                    }
                    if(strlen(ch_logic)){
                        stu->courseScores[1].score = atof(ch_logic);
                    }
                    if(strlen(ch_discrete)){
                        stu->courseScores[2].score = atof(ch_discrete);
                    }
                    stu->totalScore = stu->courseScores[0].score + stu->courseScores[1].score + stu->courseScores[2].score;
                    Edu_Success("修改成功!",40);
                    is_modify_click = false;
                    break;
                }
                if(is_quit_click){
                    break;
                }
            }
            if (e.type == SDL_TEXTINPUT) {
            const char* input = e.text.text;
            char c = input[0]; // SDL_TEXTINPUT 通常一次输入一个字符

            // 一般字段（无特殊限制）
            if (is_name && strlen(ch_name) < sizeof(ch_name) - 1) {
                strcat(ch_name, input);
            }
            if (is_passwd && strlen(ch_passwd) < sizeof(ch_passwd) - 1) {
                strcat(ch_passwd, input);
            }
            if (is_major && strlen(ch_major) < sizeof(ch_major) - 1) {
                strcat(ch_major, input);
            }

            // 仅整数（不接受小数点）
            if (is_semesterCount && strlen(ch_semesterCount) < sizeof(ch_semesterCount) - 1 && isdigit(c)) {
                strcat(ch_semesterCount, input);
            }
            if (is_courseCount && strlen(ch_courseCount) < sizeof(ch_courseCount) - 1 && isdigit(c)) {
                strcat(ch_courseCount, input);
            }

            // 数字（允许一个小数点，且不允许以 '.' 开头）
            if (is_admath && strlen(ch_admath) < sizeof(ch_admath) - 1) {
                if ((isdigit(c)) || (c == '.' && strlen(ch_admath) > 0 && strchr(ch_admath, '.') == NULL)) {
                    strcat(ch_admath, input);
                }
            }
            if (is_logic && strlen(ch_logic) < sizeof(ch_logic) - 1) {
                if ((isdigit(c)) || (c == '.' && strlen(ch_logic) > 0 && strchr(ch_logic, '.') == NULL)) {
                    strcat(ch_logic, input);
                }
            }
            if (is_discrete && strlen(ch_discrete) < sizeof(ch_discrete) - 1) {
                if ((isdigit(c)) || (c == '.' && strlen(ch_discrete) > 0 && strchr(ch_discrete, '.') == NULL)) {
                    strcat(ch_discrete, input);
                }
            }
        }
        }
        if(SDL_IsInRange(mx,my,&dst_modify)){
            is_modify_hover = true;
        }else{
            is_modify_hover = false;
        }

        if(is_modify_click){
            SDL_ShowButtonWithContent("修改",&dst_modify,red);
        }else if(is_modify_hover){
            SDL_ShowButtonWithContent("修改",&dst_modify,blue);
        }
        
        if(SDL_IsInRange(mx,my,&dst_quit)){
            is_quit_hover = true;
        }else{
            is_quit_hover =false;
        }

        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }else if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }
        
        SDL_RenderPresent(render);
    }
    SDL_DestroyTexture(tex_SelectAndUpdate_Sub_QueryStudentInformation_Sub_ModifyStudentInformation_Show);
    TTF_CloseFont(font_temp);
    return true;
}

static bool Root_FreeStudent(void* data){
    if(data == NULL) return false;
    Student temp = (Student)data;
    free(temp);
    return true;
}

bool Root_SelectAndUpdate_Sub_DeleteStudentInformation(void){
    SDL_Texture* tex_CheckStudentNumber = IMG_LoadTexture(render,"./resources/student_RemoveStudentNumber.png");
    if(tex_CheckStudentNumber == NULL){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation Load Error :%s",SDL_GetError());
        return false;
    }
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,40);
    if(!font_temp){
        SDL_Log("Root_SelectAndUpdate_Sub_QueryStudentInformation font_temp Load Error:%s",SDL_GetError);
        return false;
    }
    char ch_account[13] = "";
    SDL_Rect dst_CheckStudentNumbe = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    bool is_quit_hover = false;
    bool is_quit_click =false;
    bool is_texting = false;
    int w = 150,h = 75;
    SDL_Rect dst_confirm = {WINDOW_CENTER_X - (w >> 1),WINDOW_CENTER_Y + 150,w,h};
    SDL_Rect dst_quit = {WINDOW_WIDTH - 40,0,40,40};
    SDL_Rect dst_account_box = {dst_CheckStudentNumbe.x + 40,dst_CheckStudentNumbe.y +  162,450,52};
    int dx = 110;
    SDL_Rect dst_text_input = {dst_account_box.x + dx,dst_account_box.y + 10};
    
    int mx,my;
    SDL_Event e;
    while(true){
        if(Is_Whole_Quit) break;
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_CheckStudentNumber,NULL,&dst_CheckStudentNumbe);
        SDL_Texture* tex_account = SDL_CreateTextureFromTTF(font_temp,ch_account,black,2000);
        TTF_SizeUTF8(font_temp,ch_account,&dst_text_input.w,&dst_text_input.h);
        SDL_RenderCopy(render,tex_account,NULL,&dst_text_input);
        SDL_DestroyTexture(tex_account);
        SDL_ShowButtonWithContent("确认",&dst_confirm,black);
        SDL_ShowQuitX(&dst_quit,black,black);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
                if(e.key.keysym.sym == SDLK_BACKSPACE){
                    if(is_texting && strlen(ch_account) > 0){
                        ch_account[strlen(ch_account) - 1] ='\0';
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }
                if(SDL_IsInRange(mx,my,&dst_account_box)){
                    is_texting = true;
                }else{
                    is_texting = false;
                }
                if(is_quit_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_confirm_click && strlen(ch_account)){
                    struct Student temp;
                    strcpy(temp.studentID,ch_account);
                    RBTree find = RBTree_SEARCH(&studentHead,&temp,Root_CmpStudent);
                    if(find == NULL){
                        Edu_Error("您输入的学号有误，请重新输入!",40);
                        strcpy(ch_account,"");
                    }else{
                        Student data = (Student)find->data;
                        RBTree_DELETE(&studentHead,data,Root_CmpStudent,Root_FreeStudent);
                        Edu_Success("成功删除!",40);
                        break;
                    }
                    is_confirm_click = false;
                }else{
                    is_confirm_click = false;
                }
                if(is_quit_click){
                    break;
                }
            }
            if(e.type == SDL_TEXTINPUT){
                if(is_texting && strlen(ch_account) < 12){
                    strcat(ch_account,e.text.text);
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
            SDL_ShowButtonWithContent("确认",&dst_confirm,red);
        }else if(is_confirm_hover){
            SDL_ShowButtonWithContent("确认",&dst_confirm,blue);
        }
        if(is_quit_click){
            SDL_ShowQuitX(&dst_quit,black,red);
        }else if(is_quit_hover){
            SDL_ShowQuitX(&dst_quit,black,blue);
        }


        SDL_RenderPresent(render);
    }

    TTF_CloseFont(font_temp);
    SDL_DestroyTexture(tex_CheckStudentNumber);
    return true;
}

























































bool Root_SetUpAccountAndInitialPassword(void){
    
    char ch_account[13] = "";
    char ch_passwd[21] = "";
    char ch_passwd_fake[21] = "";
    SDL_Texture* tex_set_up_a_user_account_and_initial_password = IMG_LoadTexture(render,"./resources/root_set_up_a_user_account_and_initial_password.png");
    if(tex_set_up_a_user_account_and_initial_password == NULL){
        SDL_Log("SetUpAccountAndInitialPassword Error :%s",SDL_GetError());
        return false;
    }
    int box_w = 347,box_h = 63;
    int dx = 20;
    SDL_Rect dst_set_up_a_user_account_and_initial_password = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect  dst_account_box = {dst_set_up_a_user_account_and_initial_password.x + 113,dst_set_up_a_user_account_and_initial_password.y + 173,box_w,box_h};
    SDL_Rect dst_passwd_box =  {dst_set_up_a_user_account_and_initial_password.x + 113,dst_set_up_a_user_account_and_initial_password.y +  260,box_w,box_h};
    SDL_Rect  dst_account_input = {dst_set_up_a_user_account_and_initial_password.x + 113 + dx,dst_set_up_a_user_account_and_initial_password.y + 173,box_w - dx,box_h};
    SDL_Rect  dst_passwd_input = {dst_set_up_a_user_account_and_initial_password.x + 113 + dx,dst_set_up_a_user_account_and_initial_password.y +  260 + 10,box_w - dx,box_h};
    SDL_Rect dst_confirm_button = {dst_set_up_a_user_account_and_initial_password.x +  178,dst_set_up_a_user_account_and_initial_password.y  + 370,153,76};
    int ab = 40;
    SDL_Rect dst_quit_button = {WINDOW_WIDTH - ab,0,ab,ab};
    // SDL_Rect dst_confirm_ttf = {dst_confirm_button.x , dst_confirm_button.y};    
    SDL_Texture* tex_confirm_red = SDL_CreateTextureFromTTF(font,"确认",red,2000);
    SDL_Texture* tex_confirm_blue = SDL_CreateTextureFromTTF(font,"确认",blue,2000); 
    SDL_Texture* tex_confirm_black = SDL_CreateTextureFromTTF(font,"确认",black,2000);
    // TTF_SizeUTF8(font,"确认",&dst_confirm_ttf.w,&dst_confirm_ttf.h);
    bool is_confirm_button_hover = false;
    bool is_confirm_button_click = false;
    bool is_quit_button_hover = false;
    bool is_quit_button_click = false;
    bool is_account_input = false;
    bool is_passwd_input =false;

    int mx,my;
    SDL_Event e;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_set_up_a_user_account_and_initial_password,NULL,&dst_set_up_a_user_account_and_initial_password);
        SDL_RenderCopy(render,tex_confirm_black,NULL,&dst_confirm_button);
        SDL_ShowQuitX(&dst_quit_button,black,black);
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
                    if(is_account_input && strlen(ch_account) > 0){
                        ch_account[strlen(ch_account) - 1] = '\0';
                    }
                    if(is_passwd_input && strlen(ch_passwd) > 0){
                        ch_passwd[strlen(ch_passwd) - 1] = '\0';
                        ch_passwd_fake[strlen(ch_passwd_fake) - 1] = '\0';
                    }
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN ){
                if(e.button.button == SDL_BUTTON_LEFT){
                    if(is_confirm_button_hover){
                        Mix_PlayMusic(buttonPlay,1);
                        is_confirm_button_click = true;
                    }
                    if(is_quit_button_hover){
                        Mix_PlayMusic(buttonPlay,1);
                        is_quit_button_click = true;
                    }
                    if(SDL_IsInRange(mx,my,&dst_account_box)){
                        is_account_input = true;
                        is_passwd_input = false; 
                        SDL_StartTextInput();
                    }
                    else if(SDL_IsInRange(mx,my,&dst_passwd_box)){
                        is_passwd_input = true;
                        is_account_input = false;  
                        SDL_StartTextInput();
                    }
                    else {
                        SDL_StopTextInput();
                        is_account_input = false;
                        is_passwd_input = false;
                    }
                }
                

            }
            if(e.type == SDL_MOUSEBUTTONUP){
                    
                if(is_confirm_button_click && strlen(ch_passwd) > 0){
                    if(strlen(ch_account) != 12){
                        Edu_Error("您输入的账号不为十二位，请重新输入账号",40);
                        strcpy(ch_account,"");
                    }else{
                        struct Student data;
                    strcpy(data.studentID,ch_account);
                    RBTree temp = RBTree_SEARCH(&studentHead,&data,Root_CmpStudent);
                    if(temp){
                        Edu_Warn("该学生已存在,请误重复设置",40);
                    }else{
                        Student node = (Student)malloc(sizeof(struct Student));
                        if(node == NULL){
                            printf("无法分配内存\n");
                            return false;
                        }
                        strcpy(node->studentID,data.studentID);
                        strcpy(node->password,ch_passwd);
                        strcpy(node->major,"未录入");
                        strcpy(node->name,"未录入");
                        node->courseCount = -1;
                        strcpy(node->courseScores[0].courseID,"暂无");
                        strcpy(node->courseScores[1].courseID,"暂无");
                        strcpy(node->courseScores[2].courseID,"暂无");
                        node->courseScores[0].score = -1;
                        node->courseScores[1].score = -1;
                        node->courseScores[2].score = -1;
                        node->totalScore = -1;
                        node->rankInMajor = INT_MAX;
                        node->semesterCount = -1;
                        RBTree_INSERT(&studentHead,node,Root_RBTree_CmpStudent);
                        Edu_Success("设置完成",40);
                        strcpy(ch_account,"");
                      }
                    }
                    
                    strcpy(ch_passwd,"");
                    strcpy(ch_passwd_fake,"");
                    is_confirm_button_click = false;
                }else is_confirm_button_click = false;
                if(is_quit_button_click){
                    break;
                }
            }
            if(e.type == SDL_TEXTINPUT){
                if(is_account_input && strlen(ch_account) < 12){
                    strcat(ch_account,e.text.text);
                }
                if(is_passwd_input && strlen(ch_passwd) < 20){
                    strcat(ch_passwd,e.text.text);
                    strcat(ch_passwd_fake,"*");
                }

            }
            
        }

        if(SDL_IsInRange(mx,my,&dst_confirm_button)){
            is_confirm_button_hover = true;
        }else{
            is_confirm_button_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_quit_button)){
            is_quit_button_hover = true;
        }else{
            is_quit_button_hover = false;
        }
                
        SDL_Texture* tex_temp_account = SDL_CreateTextureFromTTF(font_miao,ch_account,black,2000);
        SDL_Texture* tex_temp_passwd = SDL_CreateTextureFromTTF(font_miao,ch_passwd_fake,black,2000);
        TTF_SizeUTF8(font_miao,ch_account,&dst_account_input.w,&dst_account_input.h);
        TTF_SizeUTF8(font_miao,ch_passwd_fake,&dst_passwd_input.w,&dst_passwd_input.h);

        if(is_confirm_button_click){
            SDL_RenderCopy(render,tex_confirm_red,NULL,&dst_confirm_button);
        }else if(is_confirm_button_hover){
            SDL_RenderCopy(render,tex_confirm_blue,NULL,&dst_confirm_button);
        }
        if(is_quit_button_click){
            SDL_ShowQuitX(&dst_quit_button,black,red);
        }else if (is_quit_button_hover){
            SDL_ShowQuitX(&dst_quit_button,black,blue);
        }
        SDL_RenderCopy(render,tex_temp_account,NULL,&dst_account_input);
        SDL_RenderCopy(render,tex_temp_passwd,NULL,&dst_passwd_input);
        SDL_DestroyTexture(tex_temp_account);
        SDL_DestroyTexture(tex_temp_passwd);

        SDL_RenderPresent(render);
    }
    
    SDL_DestroyTexture(tex_confirm_black);
    SDL_DestroyTexture(tex_confirm_red);
    SDL_DestroyTexture(tex_confirm_blue);
    SDL_DestroyTexture(tex_set_up_a_user_account_and_initial_password);
    return true;
}


static int Root_CmpStudentToTalScore(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->totalScore < data02->totalScore;
}

static bool Root_StudentInformationAddRankByTotalScore(int index,RBTree node,char* buffer,size_t size){
    if(!node || !buffer) return false;
    Student data = (Student)node->data;
    snprintf(buffer,size,"%3d %-12s %-15s %-35s %s:%.2f %s:%.2f %s:%.2f 总分:%.2f",index + 1,
            data->studentID,data->name,data->major,"高数",
            data->courseScores[0].score,"数字逻辑",data->courseScores[1].score,
            "离散数学",data->courseScores[2].score,data->totalScore);
    return true;
}
bool Root_QueryTheRankingOfStudentsInAllMajors(void){
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
    qsort(arr,index,sizeof(RBTree),Root_CmpStudentToTalScore);
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
    SDL_Texture* tex_select_black = SDL_CreateTextureFromTTF(font_temp,"退出",black,2000);
    SDL_Texture* tex_select_blue = SDL_CreateTextureFromTTF(font_temp,"退出",blue,2000);
    SDL_Texture* tex_select_red = SDL_CreateTextureFromTTF(font_temp,"退出",red,2000);

    SDL_Rect dst_next = {1425,1040,120,40};
    SDL_Rect dst_last = {435,1040,120,40};
    SDL_Rect dst_select = {930,1040,120,40};
    SDL_Rect dst_quit = {1940,0,40,40};
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
        SDL_RenderDrawRect(render,&dst_select);
        SDL_RenderCopy(render,tex_next_black,NULL,&dst_next);
        SDL_RenderCopy(render,tex_last_black,NULL,&dst_last);
        SDL_RenderCopy(render,tex_select_black,NULL,&dst_select);
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

                }
                
            }

        }
        int x = 0, y = 0;
        for(int i = curPage; i < curPage + 30 && i < index && i >= 0;i++){
            if(arr[i]){
            Root_StudentInformationAddRankByTotalScore(i,arr[i],buffer,sizeof(buffer));
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
        if(SDL_IsInRange(mx,my,&dst_select)){
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
            SDL_RenderCopy(render,tex_select_red,NULL,&dst_select);
        }else if(is_quit_blue){
            SDL_RenderCopy(render,tex_select_red,NULL,&dst_select);
        }
        

        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_select_black);
    SDL_DestroyTexture(tex_select_blue);
    SDL_DestroyTexture(tex_select_red);
    SDL_DestroyTexture(tex_next_red);
    SDL_DestroyTexture(tex_last_red);
    SDL_DestroyTexture(tex_next_blue);
    SDL_DestroyTexture(tex_last_blue);
    SDL_DestroyTexture(tex_last_black);
    SDL_DestroyTexture(tex_next_black);
    TTF_CloseFont(font_temp);
    return true;
}

    // Teacher_QueryCourseGradeInformation("100001");// 高数
    // Teacher_QueryCourseGradeInformation("100002"); // 数字逻辑
    // Teacher_QueryCourseGradeInformation("100002"); // 离散数学
static bool Root_StudentInformationAdd_QueryCourseGradeInformation(RBTree node,char* major,char* buffer,size_t size,int rank){
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
static int Root_CmpGradeMATH001(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[0].score < data02->courseScores[0].score ;
}
static int Root_CmpGradeCS001(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[1].score < data02->courseScores[1].score;
}
static int Root_CmpGradeCS002(const void* a,const void* b){
    Student data01 = (Student)(*(RBTree*)a)->data;
    Student data02 = (Student)(*(RBTree*)b)->data;
    return data01->courseScores[2].score < data02->courseScores[2].score;
}
bool Root_QueryCourseGradeInformation(char* courseID){
    if(!courseID|| strlen(courseID) < 1) return false;
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

    // 对成绩进行排序
    if(!strcmp(courseID,"MATH001")){
        qsort(arr,index,sizeof(RBTree),Root_CmpGradeMATH001);
    }
    if(!strcmp(courseID,"CS001")){
        qsort(arr,index,sizeof(RBTree),Root_CmpGradeCS001);
    }
    if(!(strcmp(courseID,"CS002"))){
        qsort(arr,index,sizeof(RBTree),Root_CmpGradeCS002);
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
            Root_StudentInformationAdd_QueryCourseGradeInformation(arr[i],courseID,buffer,sizeof(buffer),i);
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

bool Root_CheckTheRankingOfIndividualSubjects(void){
    SDL_Texture* tex_backGround = IMG_LoadTexture(render,"./resources/beige_background.png");
    if(tex_backGround == NULL){
        SDL_Log("Root_CheckTheRankingOfIndividualSubjects Error :%s",SDL_GetError());
        return  false;
    }
    int w = 150,h = 75,dh = 100;
    SDL_Rect dst_backGround = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_HEIGHT};
    SDL_Rect dst_advanced_mathematics = {dst_backGround.x + 181,dst_backGround.y + 75,w,h};
    SDL_Rect dst_digital_logic = {dst_advanced_mathematics.x,dst_advanced_mathematics.y  + dh,w,h};
    SDL_Rect dst_discrete_mathematics = {dst_digital_logic.x,dst_digital_logic.y + dh,w,h};
    SDL_Rect dst_quit = {dst_discrete_mathematics.x,dst_discrete_mathematics.y + dh,w,h};

    bool is_advanced_mathematics_hover = false;
    bool is_advanced_mathematics_click = false;
    bool is_digital_logic_hover = false;
    bool is_digital_logic_click = false;
    bool is_discrete_mathematics_hover = false;
    bool is_discrete_mathematics_click = false;
    bool is_quit_hover = false;
    bool is_quit_click = false;
    int mx,my;
    SDL_Event e;
    while(true){
        SDL_GetMouseState(&mx,&my);
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_RenderCopy(render,tex_backGround,NULL,&dst_backGround);
        // if(!(is_advanced_mathematics_click || is_advanced_mathematics_hover))
            SDL_ShowButtonWithContent("高等数学",&dst_advanced_mathematics,black);
        // if(!(is_digital_logic_click || is_digital_logic_hover))
            SDL_ShowButtonWithContent("数字逻辑",&dst_digital_logic,black);
        // if(!(is_discrete_mathematics_click ||  is_discrete_mathematics_hover))
            SDL_ShowButtonWithContent("离散数学",&dst_discrete_mathematics,black);
        // if(!(is_quit_click ||  is_quit_hover))
            SDL_ShowButtonWithContent("退出",&dst_quit,black);
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT){
                if(is_quit_hover){  
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_click = true;
                }
                if(is_advanced_mathematics_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_advanced_mathematics_click = true;
                }
                if(is_digital_logic_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_digital_logic_click = true;
                }
                if(is_discrete_mathematics_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_discrete_mathematics_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_quit_click){
                    break;
                }
                if(is_advanced_mathematics_click){
                    Root_QueryCourseGradeInformation("MATH001");
                    is_advanced_mathematics_click = false;
                }
                if(is_digital_logic_click){
                    Root_QueryCourseGradeInformation("CS001");
                    is_digital_logic_click = false;
                }
                if(is_discrete_mathematics_click){
                    Root_QueryCourseGradeInformation("CS002");
                    is_discrete_mathematics_click =false;
                }
            }

        }

        if(SDL_IsInRange(mx,my,&dst_quit)){
            is_quit_hover = true;
        }else{
            is_quit_hover = false;
        }   
        if(SDL_IsInRange(mx,my,&dst_advanced_mathematics)){
            is_advanced_mathematics_hover = true;
        }else{
            is_advanced_mathematics_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_digital_logic)){
            is_digital_logic_hover = true;
        }else{
            is_digital_logic_hover = false;
        }
        if(SDL_IsInRange(mx,my,&dst_discrete_mathematics)){
            is_discrete_mathematics_hover = true;
        }else{
            is_discrete_mathematics_hover = false;
        }

        if(is_quit_click){
            SDL_ShowButtonWithContent("退出",&dst_quit,red);
        }else if(is_quit_hover){
            SDL_ShowButtonWithContent("退出",&dst_quit,blue);
        }
        if(is_advanced_mathematics_click){
            SDL_ShowButtonWithContent("高等数学",&dst_advanced_mathematics,red);
        }else if(is_advanced_mathematics_hover){
            SDL_ShowButtonWithContent("高等数学",&dst_advanced_mathematics,blue);
        }
        if(is_digital_logic_click){
            SDL_ShowButtonWithContent("数字逻辑",&dst_digital_logic,red);
        }else if(is_digital_logic_hover){
            SDL_ShowButtonWithContent("数字逻辑",&dst_digital_logic,blue);
        }
        if(is_discrete_mathematics_click){
            SDL_ShowButtonWithContent("离散数学",&dst_discrete_mathematics,red);
        }else if(is_discrete_mathematics_hover){
            SDL_ShowButtonWithContent("离散数学",&dst_discrete_mathematics,blue);
        }

        SDL_RenderPresent(render);
    }
    SDL_DestroyTexture(tex_backGround);
    return true;
}
