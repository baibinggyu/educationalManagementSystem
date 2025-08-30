#ifndef _STUDENT_H
#define _STUDENT_H
#include "EducationalManagementSystem.h"
#include "loggin.h"
bool Student_Login(void);
bool Student_IsCountMatch(char* studentID,char* passwd);
bool Student_Ui(char* studentID);
bool Student_ChangePersonalPassword(char* account);
bool Student_AcademicWarningReminder(char* studentID);
bool Student_QueryCoursePerformanceAnalysis(char* studentID);
bool Student_SearchProfessionalRank(char* studentID);
bool Student_CheckPersonalData(char* studentID);
// 绩点：0 - 5。成绩低于 60 分的绩点为 0，60 分对应绩点 1.0，
// 61 分对应绩点 1.1，......，99 分对应绩点 4.9，100 分对应绩点 5.0。
float Edu_GetScore(float score);


#endif