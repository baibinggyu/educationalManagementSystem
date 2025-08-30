#ifndef _TEACHER_H
#define _TEACHER_H
#include "EducationalManagementSystem.h"
#include "loggin.h"

bool Teacher_Login(void);
bool Teacher_IsCountMatch(char* teacherID,char* passwd);
bool Teacher_Ui(char* teacherID);
bool Teacher_CheckPersonalData(char* teacherID);
bool Teacher_QueryCourseGradeInformation(char* teacherID);
bool Teacher_ChangePersonalPassword(char* account);
bool Teacher_EnterOrModifyGrades(char* teacherID);
bool Teacher_CheckTheClassSchedule(char* teacherID);
#endif