#ifndef _FILE_H
#define _FILE_H
#include "EducationalManagementSystem.h"
// 传入可读指针
// course teacher student root
int cmpCourse(const void* node_data, const void* input_data);
bool File_LoadCourse(RBTree* root,char* path);
bool File_WriteCourse(RBTree* root,char* path);

int cmpTeacher(const void* node_data, const void* input_data);
bool File_LoadTeacher(RBTree* root,char* path);
bool File_WriteTeacher(RBTree* root,char* path);

int cmpStudent(const void* node_data, const void* input_data);
bool File_LoadStudent(RBTree* root,char* path);
bool File_WriteStudent(RBTree* root,char* path);

int cmpRoot(const void* node_data, const void* input_data);
bool File_LoadRoot(RBTree* root,char* path);
bool File_WriteRoot(RBTree* root,char* path);


bool File_Load(void);
// 传入可写指针
bool File_Write(void);

bool File_FreeCourse(void* data);
bool File_FreeTeacher(void* data);
bool File_FreeStudent(void* data);
bool File_FreeRoot(void* data);
bool File_RBTree_DESTROY(void);



#endif