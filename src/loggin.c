#include <loggin.h>
#include "root.h"
#include "student.h"
#include "teacher.h"

bool Power_on(void){
    Mix_Music* openStartMusic = Mix_LoadMUS("./resources/school_song.mp3");
    if(openStartMusic == NULL) {
        SDL_Log("openStartMusic ERROR:%s",SDL_GetError());
        return false;
    }
    Mix_PlayMusic(openStartMusic,0);
    return 1;
}

int menu(void){
    Edu_Init();
    if(!Power_on()){
        SDL_Log("开机声音问题:");
        return -1;
    }
    SDL_Event e;
    Uint64 cur = SDL_GetTicks64() ;
    Uint64 last = cur + 150000;
    SDL_Texture* Tex_menu = IMG_LoadTexture(render,"./resources/liao_ning_shi_fang_open.png");
    if(Tex_menu == NULL){
        SDL_Log("Tex_menu Load Error:%s",SDL_GetError());
        return -1;
    }
    char* tip01 = "欢迎来到辽宁师范大学教务管理系统";
    char* tip02 = "按任意键进入系统";
    SDL_Color color = {0,0,0,255};
    SDL_Texture* Tex_ttf01 = SDL_CreateTextureFromTTF(font,tip01,color,2000);
    if( Tex_ttf01 == NULL){
        SDL_Log("SDL_CreateTextureFromTTF ERROR:%s-->%s",SDL_GetError(),__LINE__);
        return -1;
    }
    int ttf_w,ttf_h;
    TTF_SizeUTF8(font,tip01,&ttf_w,&ttf_h);
    SDL_Rect src_ttf_rect01 = {0,0,ttf_w,ttf_h};
    SDL_Rect dst_ttf_rect01 = {(WINDOW_WIDTH >> 1) - (ttf_w >> 1),(WINDOW_HEIGHT >> 1) - (ttf_h >> 1),ttf_w ,ttf_h };
    
    SDL_Texture* Tex_ttf02 = SDL_CreateTextureFromTTF(font,tip02,color,2000);
    if(Tex_ttf02 == NULL){
        SDL_Log("SDL_CreateTextureFromTTF ERROR:%s-->%s",SDL_GetError(),__LINE__);
        return -1;
    }
    TTF_SizeUTF8(font,tip02,&ttf_w,&ttf_h);
    SDL_Rect src_ttf_rect02 = {0,0,ttf_w,ttf_h};
    SDL_Rect dst_ttf_rect02 = {(WINDOW_WIDTH >> 1) - (ttf_w >> 1),(WINDOW_HEIGHT >> 1) + (ttf_h >> 1),ttf_w ,ttf_h };

    
    Mix_VolumeMusic(MIX_MAX_VOLUME >> 2);
    while(cur <= last){
        cur = SDL_GetTicks64();
        SDL_RenderClear(render);
        SDL_RenderCopy(render,Tex_menu,NULL,NULL);
        SDL_RenderCopy(render,Tex_ttf01,&src_ttf_rect01,&dst_ttf_rect01);
        if((cur %  750) > 375)
            SDL_RenderCopy(render,Tex_ttf02,&src_ttf_rect02,&dst_ttf_rect02);
        if(SDL_PollEvent(&e))
            if(e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN){
                Mix_HaltMusic();
                break;
            }else if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
                Mix_HaltMusic();
                    break;                
            }
        SDL_RenderPresent(render);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    // Mix_FadeOutMusic(1500);
    SDL_DestroyTexture(Tex_menu);
    Tex_menu = IMG_LoadTexture(render,"./resources/loggin.png");
    if(Tex_menu == NULL){
        SDL_Log("Tex_menu2 Load Error:%s",SDL_GetError());
        return -1;
    }
    SDL_Texture* teacher_button_click = IMG_LoadTexture(render,"./resources/teacher_button_click.png");
    SDL_Texture* teacher_button_hover = IMG_LoadTexture(render,"./resources/teacher_button_hover.png");
    if(!teacher_button_hover || !teacher_button_click ){
        SDL_Log("teacher_button_click ||teacher_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Texture* quit_button_click = IMG_LoadTexture(render,"./resources/quit_button_click.png");
    SDL_Texture* quit_button_hover = IMG_LoadTexture(render,"./resources/quit_button_hover.png");
    if(!quit_button_click || !quit_button_hover){
        SDL_Log("quit_button_click || quit_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Texture* student_button_click = IMG_LoadTexture(render,"./resources/student_button_click.png");
    SDL_Texture* student_button_hover = IMG_LoadTexture(render,"./resources/student_button_hover.png");
    if(!student_button_click || !student_button_hover){
        SDL_Log("student_button_click || student_button_hover ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Texture* root_button_click = IMG_LoadTexture(render,"./resources/root_button_click.png");
    SDL_Texture* root_button_hover = IMG_LoadTexture(render,"./resources/root_button_hover.png");
    if(!root_button_click || !root_button_hover){
        SDL_Log("root_button_click || root_button_click ERROR:%s",SDL_GetError());
        return -1;
    }
    SDL_Texture* tex_LiaoNing = IMG_LoadTexture(render,"./resources/liaoNing.png");
    if(tex_LiaoNing ==  NULL){
        SDL_Log("tex_LiaoNing Load ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_LiaoNing = {0,0,261,256};

    bool is_teacher_button_click = false;
    bool is_teacher_button_hover = false;
    bool is_quit_button_click = false;
    bool is_quit_button_hover = false;
    bool is_student_button_click = false;
    bool is_student_button_hover = false;
    bool is_root_button_click = false;
    bool is_root_button_hover = false;
    int mx,my;
    int width = BUTTON_WEIGHT;
    int height = BUTTON_HEIGHT;
    int y = 684;
    int quitY = 33;
    SDL_Rect dst_teacher = {237,y,width,height};
    SDL_Rect dst_student = {890,y,width,height};
    SDL_Rect dst_root = {1544,y,width,height};
    SDL_Rect dts_quit = {1743,quitY,width,height};
    
    SDL_Rect dst_version = {};
    TTF_SizeUTF8(font_miao,"西山湖教务管理系统1.0 作者:白玉",&dst_version.w,&dst_version.h);
    dst_version.x = WINDOW_WIDTH - dst_version.w;
    dst_version.y = WINDOW_HEIGHT - dst_version.h;
    SDL_Texture* tex_version = SDL_ShowWord( FONT_ZHENGKAI_PATH,"西山湖教务管理系统1.0 作者:白玉",&dst_version,black,2000);

    while(true){
        SDL_RenderClear(render);
        SDL_RenderCopy(render,Tex_menu,NULL,NULL);
        SDL_RenderCopy(render,tex_version,NULL,&dst_version);
        SDL_GetMouseState(&mx,&my);
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT)
                break;
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT && SDL_IsInRange(mx,my,&dst_teacher)){
                    Mix_PlayMusic(buttonPlay,1);
                    is_teacher_button_click  = true;
                }else if(e.button.button == SDL_BUTTON_LEFT && SDL_IsInRange(mx,my,&dts_quit)){
                    Mix_PlayMusic(buttonPlay,1);
                    is_quit_button_click = true;
                }else if (e.button.button == SDL_BUTTON_LEFT && SDL_IsInRange(mx,my,&dst_student)){
                    Mix_PlayMusic(buttonPlay,1);
                    is_student_button_click = true;
                }else if (e.button.button == SDL_BUTTON_LEFT && SDL_IsInRange(mx,my,&dst_root)){
                    Mix_PlayMusic(buttonPlay,1);
                    is_root_button_click = true;
                }
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    break;
                }
            }

            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_teacher_button_click){
                    Teacher_Login();
                }else if (is_root_button_click){
                    Root_Login();
                }else if(is_student_button_click){
                    Student_Login();
                }
                is_teacher_button_click = false;
                is_quit_button_click =  false;
                is_root_button_click = false;
                is_student_button_click = false;
            }
            if(SDL_IsInRange(mx,my,&dst_teacher)){
                is_teacher_button_hover = true;
            }else is_teacher_button_hover = false;

            if(SDL_IsInRange(mx,my,&dts_quit)){
                is_quit_button_hover = true;
            }else is_quit_button_hover = false;

            if(SDL_IsInRange(mx,my,&dst_student)){
                is_student_button_hover = true;
            }else is_student_button_hover = false;

            if(SDL_IsInRange(mx,my,&dst_root)){
                is_root_button_hover = true;
            }else is_root_button_hover = false;

        }
        if(Is_Whole_Quit) break;
        SDL_RenderCopy(render,tex_LiaoNing,NULL,&dst_LiaoNing);
        if(is_teacher_button_click) SDL_RenderCopy(render,teacher_button_click,NULL,&dst_teacher);
        else if(is_teacher_button_hover) SDL_RenderCopy(render,teacher_button_hover,NULL,&dst_teacher);
        else if(is_quit_button_click) {SDL_RenderCopy(render,quit_button_click,NULL,&dts_quit);break;}
        else if(is_quit_button_hover) SDL_RenderCopy(render,quit_button_hover,NULL,&dts_quit);
        else if (is_student_button_click) SDL_RenderCopy(render,student_button_click,NULL,&dst_student);
        else if (is_student_button_hover) SDL_RenderCopy(render,student_button_hover,NULL,&dst_student);
        else if(is_root_button_click) SDL_RenderCopy(render,root_button_click,NULL,&dst_root);
        else if (is_root_button_hover) SDL_RenderCopy(render,root_button_hover,NULL,&dst_root);
        SDL_RenderPresent(render); 
    // SDL_Delay(16.6);
    }       

    SDL_DestroyTexture(tex_version);
    SDL_DestroyTexture(tex_LiaoNing);
    SDL_DestroyTexture(Tex_ttf02);
    SDL_DestroyTexture(Tex_ttf01);
    SDL_DestroyTexture(root_button_hover);
    SDL_DestroyTexture(root_button_click);
    SDL_DestroyTexture(student_button_hover);
    SDL_DestroyTexture(student_button_click);
    SDL_DestroyTexture(quit_button_hover);
    SDL_DestroyTexture(quit_button_click);
    SDL_DestroyTexture(teacher_button_click);
    SDL_DestroyTexture(teacher_button_hover);
    SDL_DestroyTexture(Tex_menu);
    Edu_Quit();
    return 0;
}

bool Edu_ShowAccountPasswdErrorRender(){
    SDL_Surface* sur_error = IMG_Load("./resources/account_passwd_error.png");
    if(!sur_error){
        SDL_Log("sur_error Load ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_error = {(WINDOW_WIDTH >> 1) - (sur_error->w >> 1),(WINDOW_HEIGHT >> 1) - (sur_error->h >> 1),sur_error->w,sur_error->h};
    SDL_Rect dst_button_confirm = {dst_error.x + 215,dst_error.y + 355,150,76};
    SDL_Rect dst_fp_cover = {dst_error.x + 193,dst_error.y + 349,120,65};
    TTF_Font* font_temp = TTF_OpenFont("./resources/slideyouran-Regular.ttf",50);
    if(font_temp == NULL){
        SDL_Log("Edu_ShowAccountPasswdErrorRender font_temp ERROR:%s",SDL_GetError());
        return  false;
    }
    char* word = "确认";
    
    SDL_Texture* tex_button_confirm_black = SDL_CreateTextureFromTTF(font_temp,word,black,2000);
    SDL_Texture* tex_button_confirm_red = SDL_CreateTextureFromTTF(font_temp,word,red,2000);
    SDL_Texture* tex_button_confirm_blue = SDL_CreateTextureFromTTF(font_temp,word,blue,2000);
    SDL_Texture* tex_error = SDL_CreateTextureFromSurface(render,sur_error);
    TTF_SizeUTF8(font_temp,word,&dst_button_confirm.w,&dst_button_confirm.h);

    SDL_FreeSurface(sur_error);
    if(!tex_error){
        SDL_Log("tex_error ERROR:%s",SDL_GetError());
        return false;
    }
    bool is_button_confirm_hover = false;
    bool is_button_confirm_click = false;


    SDL_Event e;
    int mx,my;
    while(true){
        SDL_GetMouseState(&mx,&my);
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
                if(is_button_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_button_confirm_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_button_confirm_click){
                    break;
                }
            }
            
        }
        if(SDL_IsInRange(mx,my,&dst_button_confirm)){
            is_button_confirm_hover = true;
        }else{
            is_button_confirm_hover = false;
            is_button_confirm_click = false;
        }

        SDL_RenderCopy(render,tex_error,NULL,&dst_error);
        SDL_SetRenderDrawColor(render,250,186,167,255);
        SDL_RenderFillRect(render,&dst_fp_cover);
        SDL_RenderCopy(render,tex_button_confirm_black,NULL,&dst_button_confirm);
        if(is_button_confirm_click){
            SDL_RenderCopy(render,tex_button_confirm_red,NULL,&dst_button_confirm);
        }else if(is_button_confirm_hover){
            SDL_RenderCopy(render,tex_button_confirm_blue,NULL,&dst_button_confirm);
        }

        SDL_RenderSetClipRect(render,&dst_error);
        SDL_RenderPresent(render);
    }

    
    SDL_DestroyTexture(tex_button_confirm_black);
    SDL_DestroyTexture(tex_button_confirm_red);
    SDL_DestroyTexture(tex_button_confirm_blue);
    TTF_CloseFont(font_temp);
    SDL_RenderSetClipRect(render,NULL);
    SDL_DestroyTexture(tex_error);
    return true;
}   

bool Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes(void){
    SDL_Surface* sur_error = IMG_Load("./resources/account_passwd_error_without_content.png");
    if(!sur_error){
        SDL_Log("sur_error Load ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_error = {(WINDOW_WIDTH >> 1) - (sur_error->w >> 1),(WINDOW_HEIGHT >> 1) - (sur_error->h >> 1),sur_error->w,sur_error->h};
    SDL_Rect dst_button_confirm = {dst_error.x + 215,dst_error.y + 358,150,76};
    SDL_Rect dst_fp_cover = {dst_error.x + 194,dst_error.y + 358,120,65};
    SDL_Texture* tex_error = SDL_CreateTextureFromSurface(render,sur_error);
    SDL_FreeSurface(sur_error);
    if(!tex_error){
        SDL_Log("tex_error ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Color fg = {0,0,0,255};
    char* tip01 = "您输入错误的次数太多";
    char* tip02 = "本程序拒接您的登陆请求";
    char* word = "确认";
    // ---------------------------------------------------------------
    //                      字体大小!!!!
    // ---------------------------------------------------------------
    int ttf_size = 50;
    TTF_Font* font_temp = TTF_OpenFont("./resources/slideyouran-Regular.ttf",ttf_size);
    if(font_temp == NULL){
        SDL_Log("Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes ttf ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Surface* sur_tip01 = TTF_RenderUTF8_Blended(font_temp,tip01,fg);
    SDL_Surface* sur_tip02 = TTF_RenderUTF8_Blended(font_temp,tip02,fg);
    if(!(sur_tip01 && sur_tip02)){
        SDL_Log("Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes Surface ERROR:%s",SDL_GetError());
        return false;
    }
    
    SDL_Texture* tex_tip01 = SDL_CreateTextureFromSurface(render,sur_tip01);
    SDL_Texture* tex_tip02 = SDL_CreateTextureFromSurface(render,sur_tip02);
    if(!(tex_tip01 && tex_tip02)){
        SDL_Log("Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes Texture ERROR:%s",SDL_GetError());
        return false;
    }

    SDL_FreeSurface(sur_tip01);
    SDL_FreeSurface(sur_tip02);

    if(!(tex_tip01 && tex_tip02)){
        SDL_Log("Edu_ShowAccountPasswdErrorRenderMoreThanThreeTimes Error :%s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_tip01;
    SDL_Rect dst_tip02;
    TTF_SizeUTF8(font_temp,tip01,&dst_tip01.w,&dst_tip01.h);
    TTF_SizeUTF8(font_temp,tip02,&dst_tip02.w,&dst_tip02.h);
    dst_tip01.x = (WINDOW_WIDTH >> 1) - (dst_tip01.w >> 1);
    dst_tip01.y = (WINDOW_HEIGHT >> 1) - (dst_tip01.h >> 1) - (TTF_SIZE >> 1);
    dst_tip02.x = (WINDOW_WIDTH >> 1) - (dst_tip02.w >> 1);
    dst_tip02.y = (WINDOW_HEIGHT >> 1) - (dst_tip02.h >> 1) + (TTF_SIZE >> 1);

    SDL_Color black = {0,0,0,255};
    SDL_Color red = {255,49,49,255};
    SDL_Color blue = {82,113,255,255};
    SDL_Texture* tex_button_confirm_black = SDL_CreateTextureFromTTF(font_temp,word,black,2000);
    SDL_Texture* tex_button_confirm_red = SDL_CreateTextureFromTTF(font_temp,word,red,2000);
    SDL_Texture* tex_button_confirm_blue = SDL_CreateTextureFromTTF(font_temp,word,blue,2000);
    TTF_SizeUTF8(font_temp,word,&dst_button_confirm.w,&dst_button_confirm.h);

    bool is_button_confirm_hover = false;
    bool is_button_confirm_click = false;

    SDL_Event e;
    int mx,my;
    while(true){
        SDL_GetMouseState(&mx,&my);
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
                if(is_button_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_button_confirm_click = true;
                }
            }
            if(e.type == SDL_MOUSEBUTTONUP){
                if(is_button_confirm_click){
                    break;
                }
            }
        }
        if(SDL_IsInRange(mx,my,&dst_button_confirm)){
            is_button_confirm_hover = true;
        }else{
            is_button_confirm_hover = false;
            is_button_confirm_click = false;
        }

        SDL_RenderCopy(render,tex_error,NULL,&dst_error);
        SDL_RenderCopy(render,tex_tip01,NULL,&dst_tip01);
        SDL_RenderCopy(render,tex_tip02,NULL,&dst_tip02);
        SDL_SetRenderDrawColor(render,250,186,167,255);
        SDL_RenderFillRect(render,&dst_fp_cover);
        SDL_RenderCopy(render,tex_button_confirm_black,NULL,&dst_button_confirm);
        if(is_button_confirm_click){
            SDL_RenderCopy(render,tex_button_confirm_red,NULL,&dst_button_confirm);
        }else if(is_button_confirm_hover){
            SDL_RenderCopy(render,tex_button_confirm_blue,NULL,&dst_button_confirm);
        }
        SDL_RenderSetClipRect(render,&dst_error);
        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_button_confirm_black);
    SDL_DestroyTexture(tex_button_confirm_red);
    SDL_DestroyTexture(tex_button_confirm_blue);
    SDL_DestroyTexture(tex_tip01);
    SDL_DestroyTexture(tex_tip02);
    TTF_CloseFont(font_temp);
    SDL_RenderSetClipRect(render,NULL);
    SDL_DestroyTexture(tex_error);
    return true;
}