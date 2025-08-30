#include "EducationalManagementSystem.h"
bool Edu_Init(void){
    if(!File_Load()){
        perror("FILE LAOD ERROR:");
        return false;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        SDL_Log("SDL_Init ERROR:%s",SDL_GetError());
        return false;
    }
    win = SDL_CreateWindow("教务管理系统",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if(win == NULL){
        SDL_Log("SDL_CreateWIndow ERROR:%s",SDL_GetError());
        return false;
    }
    render = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    if(render == NULL){
        SDL_Log("SDL_CreateRender ERROR:%s",SDL_GetError());
        return false;
    }
    int imgFlag = IMG_INIT_JPG | IMG_INIT_PNG;
    if(!(IMG_Init(imgFlag) & imgFlag)){
        SDL_Log("IMG_Init ERROR:%s",SDL_GetError());
        return false;
    }
    if(TTF_Init() == -1){
        SDL_Log("TTF_Init ERROR:%s",SDL_GetError());
        return false;
    }
    int mixFlag = MIX_INIT_MP3;
    if((Mix_Init(mixFlag) & mixFlag) != mixFlag){
        SDL_Log("MIX_Init ERROR:%s",SDL_GetError());
        return false;
    }
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0){
        SDL_Log("SDL_OpenAudio ERROR:%s",SDL_GetError());
        return false;
    }
    buttonPlay = Mix_LoadMUS("./resources/button.mp3");
    if(buttonPlay == NULL){
        SDL_Log("buttonPlay ERROR:%s",SDL_GetError());
        return false;
    }
    font = TTF_OpenFont("./resources/slideyouran-Regular.ttf",TTF_SIZE);
    if(font == NULL){
        SDL_Log("FONT Load ERROR:%s",SDL_GetError());
        return false;
    }
    font_miao = TTF_OpenFont("./resources/miao.ttf",INPUT_SIZE);
    if(font_miao == NULL){
        SDL_Log("FONT_MIAO Load ERROR:%s",SDL_GetError());
        return false;
    }
    font_bigger = TTF_OpenFont("./resources/slideyouran-Regular.ttf",TTF_SIZE_BIIGER);
    if(font == NULL){
        SDL_Log("FONT Load ERROR:%s",SDL_GetError());
        return false;
    }
        
    return true;
}
bool Edu_Quit(void){
    File_Write();
    File_RBTree_DESTROY();
    TTF_CloseFont(font_bigger);
    TTF_CloseFont(font_miao);
    TTF_CloseFont(font);
    Mix_FreeMusic(buttonPlay);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return true;
}

bool SDL_IsInRange(int x,int y,SDL_Rect* dest){
    return x >= dest->x && x <= (dest->x + dest->w)
            && y >= dest->y && (y <= dest->y + dest->h); 
}

SDL_Texture* SDL_CreateTextureFromTTF(TTF_Font* ttf,char* word,SDL_Color color,int size){
    SDL_Surface* temp = TTF_RenderUTF8_Blended_Wrapped(ttf,word,color,size);
    if(!temp) return NULL;
    SDL_Texture* res = SDL_CreateTextureFromSurface(render,temp);
    SDL_FreeSurface(temp);
    if(!res) return NULL;
    return res;
}
bool SDL_ShowQuitX(SDL_Rect* dst,SDL_Color fg,SDL_Color fgX){
    SDL_SetRenderDrawColor(render,fg.r,fg.g,fg.b,fg.a);
    SDL_RenderDrawRect(render,dst);
    SDL_SetRenderDrawColor(render,fgX.r,fgX.g,fgX.b,fgX.a);
    SDL_RenderDrawLine(render,dst->x,dst->y,dst->x + dst->w,dst->y + dst->h);
    SDL_RenderDrawLine(render,dst->x + dst->w,dst->y,dst->x,dst->y + dst->h);
    SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
    return true;
}
// 暴力特别耗性能 二分
int SDL_GetAdaptiveFontSize(const char* fontPath, const char* word, int maxWidth, int maxHeight) {
    if (!(fontPath && word)) return -1;

    int low = 2, high = 100;
    int bestSize = -1;

    while (low <= high) {
        int mid = (low + high) / 2;
        TTF_Font* font = TTF_OpenFont(fontPath, mid);
        if (!font) break;

        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, word, (SDL_Color){0, 0, 0, 255});
        TTF_CloseFont(font);
        if (!surface) break;

        int w = surface->w;
        int h = surface->h;
        SDL_FreeSurface(surface);

        if (w <= maxWidth && h <= maxHeight) {
            bestSize = mid;
            low = mid + 1;  
        } else {
            high = mid - 1; 
        }
    }

    return bestSize;
}


bool SDL_ShowButton(SDL_Rect* dst,char* word,SDL_Color fg){
    if(!(dst && word))  return false;
    SDL_SetRenderDrawColor(render,black.r,black.g,black.b,black.a);
    SDL_RenderDrawRect(render,dst);
    TTF_Font* font_zhengkai = TTF_OpenFont(FONT_ZHENGKAI_PATH,SDL_GetAdaptiveFontSize(FONT_ZHENGKAI_PATH,word,dst->w,dst->h));
    SDL_Surface* sur_word = TTF_RenderUTF8_Blended(font_zhengkai,word,fg);
    SDL_Texture* tex_word = SDL_CreateTextureFromSurface(render,sur_word);
    SDL_Rect dst_word = {dst->x + (dst->w - sur_word->w) / 2 ,dst->y + (dst->h - sur_word->h) / 2,sur_word->w,sur_word->h};
    SDL_RenderCopy(render,tex_word,NULL,&dst_word);
    SDL_DestroyTexture(tex_word);
    SDL_FreeSurface(sur_word);
    TTF_CloseFont(font_zhengkai);
    SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
    return true;
}

bool SDL_ShowInputBox(SDL_Rect* dst,char* title,char* tip,int titleSize,int tipSize,SDL_Color fg){
    SDL_SetRenderDrawColor(render,black.r,black.g,black.b,black.a);
    SDL_RenderDrawRect(render,dst);
    TTF_Font* font_title = TTF_OpenFont(FONT_ZHENGKAI_PATH,titleSize);
    TTF_Font* font_tip = TTF_OpenFont(FONT_ZHENGKAI_PATH,tipSize);
     if(!(font_title && font_tip)){
        SDL_Log("SDL_ShowInputBox ERROR:%s",SDL_GetError());
        return false;
     }

    SDL_Surface* sur_title = TTF_RenderUTF8_Blended_Wrapped(font_title,title,fg,dst->w);
    SDL_Surface* sur_tip = TTF_RenderUTF8_Blended_Wrapped(font_tip,tip,fg,dst->w);
    if(!(sur_title && sur_tip)){
        SDL_Log("SDL_ShowInputBox ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_Texture* tex_title = SDL_CreateTextureFromSurface(render,sur_title);
    SDL_Texture* tex_tip = SDL_CreateTextureFromSurface(render,sur_tip);
    if(!(tex_title && tex_tip)){
        SDL_Log("SDL_ShowInputBox ERROR:%s",SDL_GetError());
        return false;
    }
    SDL_RenderCopy(render,tex_title,NULL,&(SDL_Rect){dst->x + (dst->w - sur_title->w) / 2,dst->y + 5,sur_title->w,sur_title->h});
    SDL_RenderCopy(render,tex_tip,NULL,&(SDL_Rect){dst->x + (dst->w - sur_tip->w) ,dst->y + (dst->h -  sur_tip->h) /2,sur_tip->w,sur_tip->h});
    SDL_FreeSurface(sur_tip);
    SDL_FreeSurface(sur_title);
    TTF_CloseFont(font_tip);
    TTF_CloseFont(font_title);

    SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
}


bool Edu_Warn(char* word, int size) {
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH, size);
    if (!font_temp) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        return false;
    }

    SDL_Surface* sur_word = TTF_RenderUTF8_Blended_Wrapped(font_temp, word, TIPS_COLOR, NEWLINE);
    if (!sur_word) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        TTF_CloseFont(font_temp);
        return false;
    }
    SDL_Texture* tex_word = SDL_CreateTextureFromSurface(render, sur_word);
    SDL_Rect dst_word = {
        WINDOW_CENTER_X - (sur_word->w >> 1),
        WINDOW_CENTER_Y - (sur_word->h >> 1) - 80,
        sur_word->w,
        sur_word->h
    };
    SDL_FreeSurface(sur_word);

    SDL_Texture* tex_warn = IMG_LoadTexture(render, "./resources/warn.png");
    if (!tex_warn) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        return false;
    }
    SDL_Rect dst_warn = {
        WINDOW_CENTER_X - (TIPS_WIDTH >> 1),
        WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),
        TIPS_WIDTH,
        TIPS_HEIGHT
    };

    SDL_Rect dst_button_confirm = {dst_warn.x + 200, dst_warn.y + 360, 120, 60};

    SDL_Rect dst_fill_confirm = {
        dst_button_confirm.x + 5,
        dst_button_confirm.y + 10,
        dst_button_confirm.w - 30,
        dst_button_confirm.h - 7
    };

    TTF_Font* font_button = TTF_OpenFont("./resources/slideyouran-Regular.ttf", 50);
    if (!font_button) {
        SDL_Log("Edu_Warn ERROR button font %s", SDL_GetError());
        SDL_DestroyTexture(tex_warn);
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        return false;
    }

    SDL_Texture* tex_button_confirm_black = SDL_CreateTextureFromTTF(font_button, "确认", black, 2000);
    SDL_Texture* tex_button_confirm_blue = SDL_CreateTextureFromTTF(font_button, "确认", blue, 2000);
    SDL_Texture* tex_button_confirm_red = SDL_CreateTextureFromTTF(font_button, "确认", red, 2000);
    if (!tex_button_confirm_black || !tex_button_confirm_blue || !tex_button_confirm_red) {
        SDL_Log("Edu_Warn ERROR 创建确认按钮纹理失败");
        SDL_DestroyTexture(tex_warn);
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        TTF_CloseFont(font_button);
        return false;
    }

    bool is_hover = false;
    bool is_click = false;

    SDL_Event e;
    int mx, my;
    while (true) {
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_GetMouseState(&mx, &my);
        if (Is_Whole_Quit) break;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                Is_Whole_Quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (SDL_IsInRange(mx, my, &dst_button_confirm)) {
                    Mix_PlayMusic(buttonPlay, 1); 
                    is_click = true;
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                if (is_click) break;
            }
        }

        is_hover = SDL_IsInRange(mx, my, &dst_button_confirm);

        SDL_RenderCopy(render, tex_warn, NULL, &dst_warn);
        SDL_RenderCopy(render, tex_word, NULL, &dst_word);
        SDL_SetRenderDrawColor(render, beige.r, beige.g, beige.b, beige.a);
        SDL_RenderFillRect(render, &dst_fill_confirm);
        SDL_RenderCopy(render, tex_button_confirm_black, NULL, &dst_button_confirm);
        if (is_click) {
            SDL_RenderCopy(render, tex_button_confirm_red, NULL, &dst_button_confirm);
        } else if (is_hover) {
            SDL_RenderCopy(render, tex_button_confirm_blue, NULL, &dst_button_confirm);
        }

        SDL_RenderPresent(render);
    }
    SDL_DestroyTexture(tex_button_confirm_black);
    SDL_DestroyTexture(tex_button_confirm_red);
    SDL_DestroyTexture(tex_button_confirm_blue);
    SDL_DestroyTexture(tex_warn);
    SDL_DestroyTexture(tex_word);
    TTF_CloseFont(font_button);
    TTF_CloseFont(font_temp);
    return true;
}


bool Edu_Error(char* word,int size){
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH,size);
    if(!font_temp){
        SDL_Log("Edu_Warn ERROR %s",SDL_GetError());
        return false;
    }
    SDL_Surface* sur_word = TTF_RenderUTF8_Blended_Wrapped(font_temp,word,TIPS_COLOR,NEWLINE);
    if(!sur_word){
        SDL_Log("Edu_Warn ERROR %s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_word = {WINDOW_CENTER_X - (sur_word->w >> 1),WINDOW_CENTER_Y - (sur_word->h >> 1) - 50 ,sur_word->w,sur_word->h};
    SDL_Texture* tex_word = SDL_CreateTextureFromSurface(render,sur_word);
    SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
    SDL_RenderFillRect(render,&dst_word); 
    SDL_Texture* tex_warn = IMG_LoadTexture(render,"./resources/error.png");
    if(!tex_warn){
        SDL_Log("Edu_Warn ERROR %s",SDL_GetError());
        return false;
    }
    SDL_Rect dst_warn = {WINDOW_CENTER_X - (TIPS_WIDTH >> 1),WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),TIPS_WIDTH,TIPS_WIDTH};
    SDL_Rect dst_confirm  = {dst_warn.x +  180 + 10,dst_warn.y + 352 ,140,70};
    SDL_Texture* tex_confirm_hover = SDL_CreateTextureFromTTF(font,"确认",blue,2000);
    SDL_Texture* tex_confirm_click = SDL_CreateTextureFromTTF(font,"确认",red,2000);
    if(!(tex_confirm_click && tex_confirm_hover)){
        SDL_Log("Edu_Warn ERROR %s",SDL_GetError());
        return false;
    }
    int dx = 10;
    SDL_Rect dst_fill_confirm = {dst_confirm.x + dx,dst_confirm.y + dx,dst_confirm.w - 2 * dx,dst_confirm.h  - 2 *dx};
    bool is_confirm_hover = false;
    bool is_confirm_click = false;
    SDL_Event e;
    int mx,my;
    while(true){
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_GetMouseState(&mx,&my);
        if(Is_Whole_Quit) break;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                Is_Whole_Quit = true;
            }
            if(e.type  == SDL_KEYDOWN){
                if(e.key.keysym.sym  == SDLK_ESCAPE){
                    break;
                }
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(is_confirm_hover){
                    Mix_PlayMusic(buttonPlay,1);
                    is_confirm_click = true;
                }else{
                    is_confirm_click = false;
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
        SDL_RenderCopy(render,tex_warn,NULL,&dst_warn);
        SDL_RenderCopy(render,tex_word,NULL,&dst_word);
        if(is_confirm_click){
            SDL_SetRenderDrawColor(render,beige.r,beige.g,beige.b,beige.a);
            SDL_RenderFillRect(render,&dst_fill_confirm);
            SDL_RenderCopy(render,tex_confirm_click,NULL,&dst_confirm);
        }else if (is_confirm_hover){
            SDL_SetRenderDrawColor(render,beige.r,beige.g,beige.b,beige.a);
            SDL_RenderFillRect(render,&dst_fill_confirm);
            SDL_RenderCopy(render,tex_confirm_hover,NULL,&dst_confirm);
        }
          SDL_RenderPresent(render);

    }

    SDL_DestroyTexture(tex_confirm_click);
    SDL_DestroyTexture(tex_confirm_hover);
    SDL_DestroyTexture(tex_warn);
    SDL_DestroyTexture(tex_word);
    SDL_FreeSurface(sur_word);
    TTF_CloseFont(font_temp);
    return true;
    return true;
}

bool Edu_Success(char* word,int size){
    TTF_Font* font_temp = TTF_OpenFont(FONT_ZHENGKAI_PATH, size);
    if (!font_temp) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        return false;
    }

    SDL_Surface* sur_word = TTF_RenderUTF8_Blended_Wrapped(font_temp, word, TIPS_COLOR, NEWLINE);
    if (!sur_word) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        TTF_CloseFont(font_temp);
        return false;
    }
    SDL_Texture* tex_word = SDL_CreateTextureFromSurface(render, sur_word);
    SDL_Rect dst_word = {
        WINDOW_CENTER_X - (sur_word->w >> 1),
        WINDOW_CENTER_Y - (sur_word->h >> 1) - 80,
        sur_word->w,
        sur_word->h
    };
    SDL_FreeSurface(sur_word);

    SDL_Texture* tex_warn = IMG_LoadTexture(render, "./resources/success.png");
    if (!tex_warn) {
        SDL_Log("Edu_Warn ERROR %s", SDL_GetError());
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        return false;
    }
    SDL_Rect dst_warn = {
        WINDOW_CENTER_X - (TIPS_WIDTH >> 1),
        WINDOW_CENTER_Y - (TIPS_HEIGHT >> 1),
        TIPS_WIDTH,
        TIPS_HEIGHT
    };

    SDL_Rect dst_button_confirm = {dst_warn.x + 200, dst_warn.y + 360, 120, 60};

    SDL_Rect dst_fill_confirm = {
        dst_button_confirm.x + 5,
        dst_button_confirm.y + 10,
        dst_button_confirm.w - 30,
        dst_button_confirm.h - 7
    };

    TTF_Font* font_button = TTF_OpenFont("./resources/slideyouran-Regular.ttf", 50);
    if (!font_button) {
        SDL_Log("Edu_Warn ERROR button font %s", SDL_GetError());
        SDL_DestroyTexture(tex_warn);
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        return false;
    }

    SDL_Texture* tex_button_confirm_black = SDL_CreateTextureFromTTF(font_button, "确认", black, 2000);
    SDL_Texture* tex_button_confirm_blue = SDL_CreateTextureFromTTF(font_button, "确认", blue, 2000);
    SDL_Texture* tex_button_confirm_red = SDL_CreateTextureFromTTF(font_button, "确认", red, 2000);
    if (!tex_button_confirm_black || !tex_button_confirm_blue || !tex_button_confirm_red) {
        SDL_Log("Edu_Warn ERROR 创建确认按钮纹理失败");
        SDL_DestroyTexture(tex_warn);
        SDL_DestroyTexture(tex_word);
        TTF_CloseFont(font_temp);
        TTF_CloseFont(font_button);
        return false;
    }

    bool is_hover = false;
    bool is_click = false;

    SDL_Event e;
    int mx, my;

    while (true) {
        SDL_SetRenderDrawColor(render,white.r,white.g,white.b,white.a);
        SDL_RenderClear(render);
        SDL_GetMouseState(&mx, &my);
        if (Is_Whole_Quit) break;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                Is_Whole_Quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                break;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                if (SDL_IsInRange(mx, my, &dst_button_confirm)) {
                    Mix_PlayMusic(buttonPlay, 1); 
                    is_click = true;
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                if (is_click) break;
            }
        }

        is_hover = SDL_IsInRange(mx, my, &dst_button_confirm);

        SDL_RenderCopy(render, tex_warn, NULL, &dst_warn);
        SDL_RenderCopy(render, tex_word, NULL, &dst_word);
        SDL_SetRenderDrawColor(render, beige.r, beige.g, beige.b, beige.a);
        SDL_RenderFillRect(render, &dst_fill_confirm);
        SDL_RenderCopy(render, tex_button_confirm_black, NULL, &dst_button_confirm);
        if (is_click) {
            SDL_RenderCopy(render, tex_button_confirm_red, NULL, &dst_button_confirm);
        } else if (is_hover) {
            SDL_RenderCopy(render, tex_button_confirm_blue, NULL, &dst_button_confirm);
        }

        SDL_RenderPresent(render);
    }

    SDL_DestroyTexture(tex_button_confirm_black);
    SDL_DestroyTexture(tex_button_confirm_red);
    SDL_DestroyTexture(tex_button_confirm_blue);
    SDL_DestroyTexture(tex_warn);
    SDL_DestroyTexture(tex_word);
    TTF_CloseFont(font_button);
    TTF_CloseFont(font_temp);
    return true;
}
float Edu_GetScore(float score){
    if(score < 0 ) return INT_MIN;
    if(score < 60) return 0;
    return 1.0 * (score - 60) / 10 + 1;
}

// 使用static 避免性能浪费
bool SDL_ShowButtonWithContent(char* word,SDL_Rect* dst,SDL_Color word_fg){
    static SDL_Texture* tex_button = NULL;
    static int last_size = -1;
    static TTF_Font* font_ShowButtonWithContent = NULL;

    if (!tex_button) {
        tex_button = IMG_LoadTexture(render, "./resources/beige_button.png");
        if (!tex_button) {
            SDL_Log("SDL_ShowButtonWithContent Error: %s", SDL_GetError());
            return false;
        }
    }

    int size = SDL_GetAdaptiveFontSize(FONT_PATH, word, dst->w, dst->h);
    if (size <= 0) return false;

    if (size != last_size) {
        if (font_ShowButtonWithContent) TTF_CloseFont(font_ShowButtonWithContent);
        font_ShowButtonWithContent = TTF_OpenFont(FONT_PATH, size);
        last_size = size;
    }

    if (!font_ShowButtonWithContent) return false;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font_ShowButtonWithContent, word, word_fg);
    if (!surface) return false;

    SDL_Texture* tex_word = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
    if (!tex_word) return false;

    SDL_RenderCopy(render, tex_button, NULL, dst);
    SDL_RenderCopy(render, tex_word, NULL, dst);

    SDL_DestroyTexture(tex_word);
    return true;
}

SDL_Texture* SDL_ShowWord(char* font_path,char* word,SDL_Rect* dst,SDL_Color fg,int swap){
    SDL_Texture* this = NULL;
    int size = SDL_GetAdaptiveFontSize(font_path,word,dst->w,dst->h);
    TTF_Font* font_temp =  TTF_OpenFont(font_path,size);
    if(font_temp == NULL){
        SDL_Log("SDL_ShowWord Error :%s",SDL_GetError());
        return false;
    }
    this  =  SDL_CreateTextureFromTTF(font_temp,word,fg,swap);
    if(this == NULL){
        SDL_Log("SDL_ShowWord Error :%s",SDL_GetError());
        return false;
    }
    TTF_CloseFont(font_temp);
    return this;
}