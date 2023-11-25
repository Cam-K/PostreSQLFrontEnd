#include <stdlib.h>
#include <stdio.h>
#include <libpq-fe.h>
#include <string.h>

#define MAX_STR 64



int getAllStudents(PGconn*);

int addStudent(PGconn*, char*, char*, char*, int, int, int);
int updateStudentEmail(PGconn*, int, char*);
int deleteStudent(PGconn*, int);

void do_exit(PGconn *conn){
    PQfinish(conn);
    exit(1);
}


int main(int argc, char* argv[]){
 	

  PGconn *conn = PQconnectdb("user=postgres password=postgres dbname=assignment4");


  if(PQstatus(conn) == CONNECTION_BAD){
    printf("Unable to connect: %s \n", PQerrorMessage(conn));
    do_exit(conn);
  }
  char ch[MAX_STR];
  char fname[MAX_STR];
  char lname[MAX_STR];
  char email[MAX_STR];
  char sid[MAX_STR];
  int sidi;
  int month;
  int day;
  int year;
  char smonth[MAX_STR];
  char sday[MAX_STR];
  char syear[MAX_STR];
  printf("\nPostgreSQL Interface \n");
  PGresult *res = PQexec(conn, "SELECT VERSION()");
  printf("%s \n", PQgetvalue(res, 0,0));
  while(1){
  printf("PostgreSQL Interface \n");
  printf("Select One of Three options:\n");
  printf("1. Print students Database\n");
  printf("2. Add New Entry\n");
  printf("3. Delete Student from Database\n");
  printf("4. Update Student Email\n");
  printf("q to quit\n");
  printf(">>");
  fgets(ch, MAX_STR, stdin);
  if(ch[0] == '1'){
    getAllStudents(conn);
  } else if(ch[0] == '2'){
    printf("First Name? \n");
    fgets(fname, MAX_STR, stdin); 
    fname[strcspn(fname, "\n")] = '\0';
    printf("Last Name? \n");
    fgets(lname, MAX_STR, stdin); 
    lname[strcspn(lname, "\n")] = '\0';
    printf("Email? \n");
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = '\0';
    printf("DOB (as number): Month? \n");
    fgets(smonth, MAX_STR, stdin);
    printf("DOB (as number): Day? \n");
    fgets(sday, MAX_STR, stdin);
    printf("DOB (as number): Year? \n");
    fgets(syear, MAX_STR, stdin);
    month = atoi(smonth);
    day = atoi(sday);
    year = atoi(syear);
    addStudent(conn, fname, lname, email, month, day, year);
  } else if(ch[0] == '3'){
    printf("Student Id to Delete:");
    fgets(sid, MAX_STR, stdin);
    sidi = atoi(sid);
    deleteStudent(conn, sidi);

  } else if(ch[0] == '4'){
    printf("Student ID to update: \n");
    fgets(sid, MAX_STR, stdin);
    sidi = atoi(sid);
    printf("Updated Student email? \n");
    fgets(email, MAX_STR, stdin);
    email[strcspn(email, "\n")] = '\0';
    updateStudentEmail(conn, sidi, email);
  } else if(ch[0] == 'q'){
    exit(1);
  }
  }

  return 0;

}

/*getAllStudents()
gets all the students from the database and prints them to the terminal
*/
int getAllStudents(PGconn* conn){
  PGresult *res = PQexec(conn, "SELECT * FROM students");
  int rows = PQntuples(res);
  for(int i = 0; i < rows; ++i){
    for(int j = 0; j < 5; ++j){
      printf("%s ", PQgetvalue(res, i, j));
    }
    printf("\n");
  }
  return 0;
}

/*addStudent()
adds a new student to the database
*/
int addStudent(PGconn* conn, char* fname, char* lname, char* email, int month, int day, int year){
  char query[256] = "";
  char smonth[MAX_STR];
  char sday[MAX_STR];
  char syear[MAX_STR];
  strcat(query, "INSERT INTO students(first_name, last_name, email, enrollment_date)");
  strcat(query, " VALUES ('");
  strcat(query, fname);
  strcat(query, "', '");
  strcat(query, lname);
  strcat(query, "', '");
  strcat(query, email);
  strcat(query, "', '");
  snprintf(smonth, 10, "%d", month);
  snprintf(sday, 10, "%d", day);
  snprintf(syear, 10, "%d", year);
  strcat(query, syear);
  strcat(query, "-");
  strcat(query, smonth);
  strcat(query, "-");
  strcat(query, sday);
  strcat(query, "')");
  printf("Query: %s \n", query);
  PGresult *res = PQexec(conn, query);
  //if(PQresultStatus(res) != PGRES_TUPLES_OK)
   // printf("Failed to add query \n");
  return 0;
} 

/*deleteStudent()
Deletes a student from the database with the corresponding student id
*/
int deleteStudent(PGconn* conn, int s_id){
  char query[256] = "";
  char ssid[MAX_STR];
  strcat(query, "DELETE FROM students WHERE student_id = ");
  snprintf(ssid, 10, "%d", s_id);
  strcat(query, ssid);
  PGresult *res = PQexec(conn, query);
  return 0;
}


/*updateStudentEmail()
Changes a student email associated with the student id given
*/
int updateStudentEmail(PGconn* conn, int s_id, char* email){
  char query[256] = "";
  char ssid[MAX_STR];
  strcat(query, "UPDATE students SET ");
  strcat(query, "email='");
  strcat(query, email);
  strcat(query, "' WHERE student_id = ");
  snprintf(ssid, 10, "%d", s_id);
  strcat(query, ssid);
  printf("%s \n", query);
  PGresult *res = PQexec(conn, query);
  return 0;
}


