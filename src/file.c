#include "file.h"
int cmpCourse(const void* node_data, const void* input_data){
    Course data01 = (Course)node_data;
    Course data02 = (Course)input_data;
    return strcmp(data01->courseID,data02->courseID);
}
bool File_LoadCourse(RBTree* root,char* path){
    FILE* fp = fopen(path,"r");
    if(fp == NULL){
        printf("FILE OPEN FAILE LoadCourse %s\n",path);
        return false;
    }
    Course temp = (Course)malloc(sizeof(struct Course));
    if(temp == NULL) return false;
    while(fscanf(fp,"%[^,],%[^,],%d,%[^\n]\n",temp->courseID,temp->courseName,&temp->credit,temp->teacherID) == 4){
        RBTree_INSERT(root,temp,cmpCourse);
        temp = (Course)malloc(sizeof(struct Course));
        if(temp == NULL) return false;
    }
    free(temp);
    fclose(fp);
    return true;
}
bool File_WriteCourse(RBTree* root,char* path){
    FILE* fp = fopen(path,"w");
    if(fp == NULL){
        printf("FILE OPEN FAILE WriteCourse %s\n",path);
        return false;
    }   
    RBTree stack[STACK_INDEX] = {NULL};
    int top = -1;
    RBTree cur = *root;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        Course temp = (Course)cur->data;
        fprintf(fp,"%s,%s,%d,%s\n",temp->courseID,temp->courseName,temp->credit,temp->teacherID);
        cur = cur->right;
    }
    fclose(fp);
    return true;
}
int cmpTeacher(const void* node_data, const void* input_data){
    Teacher data01 = (Teacher)node_data;
    Teacher data02 = (Teacher)input_data;
    return strcmp(data01->teacherID,data02->teacherID);    
}
bool File_LoadTeacher(RBTree* root,char* path){
    FILE* fp = fopen(path,"r");
    if(fp == NULL){
        printf("FILE OPEN FAILE Load %s\n",path);
        return false;
    }
    Teacher temp = (Teacher)malloc(sizeof(struct Teacher));
    if(temp == NULL) return false;
    while(fscanf(fp,"%[^,],%[^,],%[^\n]\n",temp->teacherID,temp->name,temp->password) == 3){
        RBTree_INSERT(root,temp,cmpTeacher);
        temp = (Teacher)malloc(sizeof(struct Teacher));
        if(temp == NULL) return false;
    }
    free(temp);
    fclose(fp);
    return true;
}
bool File_WriteTeacher(RBTree* root,char* path){
    FILE* fp = fopen(path,"w");
    if(fp == NULL){
        printf("FILE OPEN FAILE WriteTeacher %s\n",path);
        return false;
    }
    RBTree stack[STACK_INDEX] = {NULL};
    int top = -1;
    RBTree cur = *root;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        Teacher temp = (Teacher)cur->data;
        fprintf(fp,"%s,%s,%s\n",temp->teacherID,temp->name,temp->password);
        cur = cur->right;
    }
    fclose(fp);
    return true;
}

int cmpStudent(const void* node_data, const void* input_data){
    Student data01 = (Student)node_data;
    Student data02 = (Student)input_data;
    return strcmp(data01->studentID,data02->studentID);

}
bool File_LoadStudent(RBTree* root,char* path){
    FILE* fp = fopen(path,"r");
    if(fp == NULL){
        printf("FILE OPEN FAILED LoadStudent %s\n",path);
        return false;
    }
    Student temp = (Student)malloc(sizeof(struct Student));
    if(temp == NULL) return false;
    int count = 0;
    while(fscanf(fp,
    "%[^,],%[^,],%[^,],%[^,],%d,%[^,],%f,%[^,],%f,%[^,],%f,%d,%f,%d\n",
    temp->studentID,
    temp->name,
    temp->password,
    temp->major,
    &temp->semesterCount,
    temp->courseScores[0].courseID,
    &temp->courseScores[0].score,
    temp->courseScores[1].courseID,
    &temp->courseScores[1].score,
    temp->courseScores[2].courseID,
    &temp->courseScores[2].score,
    &temp->courseCount,
    &temp->totalScore,
    &temp->rankInMajor
) == 14){             
        RBTree_INSERT(root,temp,cmpStudent);
        temp = (Student)malloc(sizeof(struct Student));
        if(temp == NULL) return false;
        count++;
    }
    free(temp);
    fclose(fp);
    return true;
}
bool File_WriteStudent(RBTree* root,char* path){
    FILE* fp = fopen(path,"w");
    if(fp == NULL){
        printf("FILE OPEN FAILED WriteStudent %s",path);
        return false;
    }
    RBTree stack[STACK_INDEX] = {NULL};
    int top = -1;
    RBTree cur = *root;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        Student temp = (Student)cur->data;
        fprintf(fp,
    "%s,%s,%s,%s,%d,"
    "%s,%f,%s,%f,%s,%f,"
    "%d,%f,%d\n",
    temp->studentID,
    temp->name,
    temp->password,
    temp->major,
    temp->semesterCount,
    temp->courseScores[0].courseID,
    temp->courseScores[0].score,
    temp->courseScores[1].courseID,
    temp->courseScores[1].score,
    temp->courseScores[2].courseID,
    temp->courseScores[2].score,
    temp->courseCount,
    temp->totalScore,
    temp->rankInMajor);
        cur = cur->right;
    }
    fclose(fp);
    return true;
}

int cmpRoot(const void* node_data, const void* input_data){
    Root data01 = (Root)node_data;
    Root data02 = (Root)input_data;
    return strcmp(data01->adminID,data02->adminID);
}
bool File_LoadRoot(RBTree* root,char* path){
    FILE* fp = fopen(path,"r");
    if(fp == NULL){
        printf("FILE OPEN FAILE LoadRoot %s\n",path);
        return false;
    }
    Root temp = (Root)malloc(sizeof(struct Root));
    if(temp == NULL) return false;
    while(fscanf(fp,"%[^,],%[^,],%[^\n]\n",temp->adminID,temp->name,temp->password) == 3){
        RBTree_INSERT(root,temp,cmpRoot);
        temp = (Root)malloc(sizeof(struct Root));
        if(temp == NULL) return false;
    }
    free(temp);
    fclose(fp);
    return true;
}
bool File_WriteRoot(RBTree* root,char* path){
    FILE* fp = fopen(path,"w");
    if(fp == NULL){
        printf("FILE OPEN FAILE WriteRoot %s\n",path);
        return false;
    }   
    RBTree stack[STACK_INDEX] = {NULL};
    int top = -1;
    RBTree cur = *root;
    while(cur || ~top){
        while(cur){
            stack[++top] = cur;
            cur = cur->left;
        }
        cur = stack[top--];
        Root temp = (Root)cur->data;
        fprintf(fp,"%s,%s,%s\n",temp->adminID,temp->name,temp->password);
        cur = cur->right;
    }
    fclose(fp);
    return true;
}

// course teacher student root

bool File_Load(void){
    return  File_LoadCourse(&courseHead,dataPath[0]) && 
            File_LoadTeacher(&teacherHead,dataPath[1]) &&
            File_LoadStudent(&studentHead,dataPath[2]) &&
            File_LoadRoot(&rootHead,dataPath[3]);
}

bool File_Write(void){
    return  File_WriteCourse(&courseHead,dataPath[0]) &&
            File_WriteTeacher(&teacherHead,dataPath[1]) &&
            File_WriteStudent(&studentHead,dataPath[2]) &&
            File_WriteRoot(&rootHead,dataPath[3]);
}

bool File_FreeCourse(void* data){
    if(!data) return false;
    free((Course)data);
    return true;
}
bool File_FreeTeacher(void* data){
    if(!data) return false;
    free((Teacher)data);
    return  true;
}
bool File_FreeStudent(void* data){
    if(!data) return false;
    free((Student)data);
    return true;
}
bool File_FreeRoot(void* data){
    if(!data) return false;
    free((Root)data);
    return true;
}
bool File_RBTree_DESTROY(void){
    RBTree_DESTROY(courseHead,File_FreeCourse);
    RBTree_DESTROY(teacherHead,File_FreeTeacher);
    RBTree_DESTROY(studentHead,File_FreeStudent);
    RBTree_DESTROY(rootHead,File_FreeRoot);
}