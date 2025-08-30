#ifndef _ROOT_H
#define _ROOT_H
#include "EducationalManagementSystem.h"
#include "loggin.h"
bool Root_Login(void);
bool Root_IsCountMatch(char* adminID,char* passwd);
bool Root_Ui(char* adminID);
bool Root_ManageStudentInformation(void);
bool Root_SelectAndUpdate(void);
bool Root_SelectAndUpdate_Sub_QueryStudentInformation(void);
bool Root_SelectAndUpdate_Sub_QueryStudentInformation_Show(Student stu);
bool Root_SelectAndUpdate_Sub_ModifyStudentInformation(void);
bool Root_SelectAndUpdate_Sub_ModifyStudentInformation_Show(Student stu);
bool Root_SelectAndUpdate_Sub_DeleteStudentInformation(void);
bool Root_SetUpAccountAndInitialPassword(void);
bool Root_QueryTheRankingOfStudentsInAllMajors(void);
bool Root_QueryCourseGradeInformation(char* courseID);
bool Root_CheckTheRankingOfIndividualSubjects(void);
#endif