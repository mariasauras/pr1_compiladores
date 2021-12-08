#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern int yyparse();
extern FILE *yyin;
extern FILE *yyout;
extern int yylineno;
#include "exemple_funcions.h" 


/*********************************************************************/
/*                   Funciones FLEX                                  */
/*********************************************************************/

int init_analisi_lexica(char *filename)
{
  int error;
  yyin = fopen(filename,"r");
  if(yyin == NULL) {
    error = EXIT_FAILURE;
  } else {
    error = EXIT_SUCCESS;
  }
  return error;
}

int end_analisi_lexica()
{
  int error;
  error = fclose(yyin);
  if (error == 0) {
    error = EXIT_SUCCESS;
  } else {
    error = EXIT_FAILURE;
  }
  return error;
}

int init_analisi_sintactica(char* filename)
{
  int error = EXIT_SUCCESS;
  yyout = fopen(filename,"w");
  if (yyout == NULL) {
    error = EXIT_FAILURE;
  }
  return error;
}

int end_analisi_sintactica(void)
{
  int error;

  error = fclose(yyout);

  if(error == 0) {
    error = EXIT_SUCCESS;
  } else {
    error = EXIT_FAILURE;
  }
  return error;
}

int analisi_semantica(void)
{
  int error;

  if (yyparse() == 0) {
    error =  EXIT_SUCCESS;
  } else {
    error =  EXIT_FAILURE;
  }
  return error;
}


void yyerror(char *explanation)
{
  fprintf(stderr, "Error: %s , in line %d\n", explanation, yylineno);
}

/**********************************************************************/
/*                  ARITHMETICAL FUNCTIONS                            */
/**********************************************************************/

void sum_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE  || v1.value_type == STRING_TYPE || v2.value_type == STRING_TYPE){
    yyerror("Can't operate with these value type");
  } else {
    if (v1.value_type == INT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = INT_TYPE;
      (*val).value_data.enter = v1.value_data.enter + v2.value_data.enter;
    } else if(v1.value_type == INT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.enter + v2.value_data.real;
    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real + v2.value_data.enter;
    } else if (v1.value_type == FLOAT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real + v2.value_data.real;
    } else if(v1.value_type == MATRIX_TYPE && v2.value_type == MATRIX_TYPE){
      (*val).value_type = MATRIX_TYPE;
      
      if (v1.value_data.row != v2.value_data.row)
        yyerror("ERROR! Incompatible dimension.");
      
      if(v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == INT_TYPE){
        (*val).value_data.matrix_type = INT_TYPE;
        int pos;
        (*val).value_data.integer_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(long));
        if((*val).value_data.integer_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.integer_matrix[pos] = v1.value_data.integer_matrix[pos] + v2.value_data.integer_matrix[pos]; 
          }

      } else if(v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v1.value_data.float_matrix[pos] + v2.value_data.float_matrix[pos]; 
          }  
      } else if(v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == INT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v1.value_data.float_matrix[pos] + (float)v2.value_data.integer_matrix[pos]; 
          }  
      } else if(v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v2.value_data.float_matrix[pos] + (float)v1.value_data.integer_matrix[pos]; 
          }  
      } else yyerror("Can't exectue sum operation with vectors or matrices if value are different than FLOAT or INTEGER");
    }
  }
}

void rest_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE  || v1.value_type == STRING_TYPE || v2.value_type == STRING_TYPE){
    yyerror("Can't operate with these value type");
  } else {
    if (v1.value_type == INT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = INT_TYPE;
      (*val).value_data.enter = v1.value_data.enter - v2.value_data.enter;
    } else if(v1.value_type == INT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.enter - v2.value_data.real;
    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == INT_TYPE){
       (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real - v2.value_data.enter;
    } else if (v1.value_type == FLOAT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real - v2.value_data.real;
      
    }else if(v1.value_type == MATRIX_TYPE && v2.value_type == MATRIX_TYPE){
      (*val).value_type = MATRIX_TYPE;
      
      if (v1.value_data.row != v2.value_data.row)
        yyerror("ERROR! Incompatible dimension.");
      
      if(v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == INT_TYPE){
        (*val).value_data.matrix_type = INT_TYPE;
        int pos;
        (*val).value_data.integer_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(long));
        if((*val).value_data.integer_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.integer_matrix[pos] = v1.value_data.integer_matrix[pos] - v2.value_data.integer_matrix[pos]; 
          }

      } else if(v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v1.value_data.float_matrix[pos] - v2.value_data.float_matrix[pos]; 
          }  
      } else if(v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == INT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v1.value_data.float_matrix[pos] - (float)v2.value_data.integer_matrix[pos]; 
          }  
      } else if(v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE){
        (*val).value_data.matrix_type = FLOAT_TYPE;
        int pos;
        (*val).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
        if((*val).value_data.float_matrix == NULL) yyerror("Error. Can't inicialize heap memory");

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v1.value_data.column; j++){
            pos = v1.value_data.column*i+j;
            (*val).value_data.float_matrix[pos] = v2.value_data.float_matrix[pos] - (float)v1.value_data.integer_matrix[pos]; 
          }  
      } else yyerror("Can't exectue sum operation with vectors or matrices if value are different than FLOAT or INTEGER");
    }
  }
}

void mul_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE ){
    yyerror("Can't operate with these value type");
  } else {
    if (v1.value_type == INT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = INT_TYPE;
      (*val).value_data.enter = v1.value_data.enter * v2.value_data.enter;

    } else if(v1.value_type == INT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.enter * v2.value_data.real;

    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == INT_TYPE){
       (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real * v2.value_data.enter;

    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == FLOAT_TYPE) {
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real * v2.value_data.real;

    } else if(v1.value_type == MATRIX_TYPE && v2.value_type == MATRIX_TYPE) {
      (*val).value_type = MATRIX_TYPE;
      
      if(v1.value_data.column != v2.value_data.row){
         yyerror("BAD DECLARATION: The num. of rows of matrix A should be equal to the number of column of matrix B");
      } else {

        if( v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == INT_TYPE){
        (*val).value_data.matrix_type = INT_TYPE;
        int pos,p,p1,sum=0;

        (*val).value_data.integer_matrix = calloc(v1.value_data.row*v2.value_data.column, sizeof(long));
        if((*val).value_data.integer_matrix == NULL) yyerror("Can't inicialize heap memory");

        (*val).value_data.row = v1.value_data.row;
        (*val).value_data.column = v2.value_data.column;
        (*val).value_data.num_elems = v1.value_data.row*v2.value_data.column;

        for(int i = 0; i<v1.value_data.row; i++)
          for(int j = 0; j<v2.value_data.column; j++){
            pos = v2.value_data.column*i+j;
            for(int k = 0; k< v2.value_data.row; k++){
              p = v1.value_data.column*i + k;
              p1 = v2.value_data.column*k + j;
              sum = sum +  v1.value_data.integer_matrix[p] * v2.value_data.integer_matrix[p1];
            } 
            (*val).value_data.integer_matrix[pos] = sum;
            sum = 0;
          }
      }

      }
       
    } else{
      /* CONCAT*/
      (*val).value_type = STRING_TYPE;
      (*val).value_data.ident.lexema = malloc(strlen(v1.value_data.ident.lexema) + strlen(v2.value_data.ident.lexema) + 1);
      strcpy((*val).value_data.ident.lexema, v1.value_data.ident.lexema);
      strcat((*val).value_data.ident.lexema, v2.value_data.ident.lexema);
    }
  }
}

void div_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE  || v1.value_type == STRING_TYPE || v2.value_type == STRING_TYPE){
    yyerror("Can't operate with these value type");
  } else {
    if (v1.value_type == INT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = (float)v1.value_data.enter / v2.value_data.enter;
    } else if(v1.value_type == INT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.enter / v2.value_data.real;
    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == INT_TYPE){
       (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real / v2.value_data.enter;
    } else {
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = v1.value_data.real / v2.value_data.real;
      
    }
  }
}

void mod_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE  || v1.value_type == STRING_TYPE || 
  v2.value_type == STRING_TYPE || v1.value_type == FLOAT_TYPE || v2.value_type == FLOAT_TYPE ){
    yyerror("Can't operate with these value type");
  } else {
    (*val).value_type = INT_TYPE;
    (*val).value_data.enter = v1.value_data.enter % v2.value_data.enter;
  }
}

void pow_op(sym_value_type * val, sym_value_type v1, sym_value_type v2){

  if(v1.value_type == BOOL_TYPE || v2.value_type == BOOL_TYPE  || v1.value_type == STRING_TYPE || v2.value_type == STRING_TYPE){
    yyerror("Can't operate with these value type");
  } else {
    if (v1.value_type == INT_TYPE && v2.value_type == INT_TYPE){
      (*val).value_type = INT_TYPE;
      (*val).value_data.enter = pow(v1.value_data.enter,v2.value_data.enter);  
    } else if(v1.value_type == INT_TYPE && v2.value_type == FLOAT_TYPE){
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = pow(v1.value_data.enter,v2.value_data.real);
    } else if(v1.value_type == FLOAT_TYPE && v2.value_type == INT_TYPE){
       (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = pow(v1.value_data.real,v2.value_data.enter);
    } else {
      (*val).value_type = FLOAT_TYPE;
      (*val).value_data.real = pow(v1.value_data.real,v2.value_data.real);
      
    }
  }
}

/*********************************************************************/
/*                 MATRIX&VECTOR FUNCTIONS                           */
/*********************************************************************/

/* Function to inicialize columns */
void col_ini(sym_value_type * matrix, sym_value_type matrix_value){

  (*matrix).value_type = MATRIX_TYPE;
  /* Indico que tipo de valor tendra */
  if(matrix_value.value_type == INT_TYPE){

    (*matrix).value_data.matrix_type = INT_TYPE;

    (*matrix).value_data.integer_matrix = calloc(1, sizeof(long));
    if((*matrix).value_data.integer_matrix == NULL) yyerror("Error. Can't inicialize heap memory");
    (*matrix).value_data.integer_matrix[0] = matrix_value.value_data.enter;

  } else if(matrix_value.value_type == FLOAT_TYPE){
   
    (*matrix).value_data.matrix_type = FLOAT_TYPE;

    (*matrix).value_data.float_matrix = calloc(1, sizeof(float));
     if((*matrix).value_data.float_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");
    (*matrix).value_data.float_matrix[0] = matrix_value.value_data.real;

  } else yyerror(" Matrix only accept Integer or Float.");
  
  (*matrix).value_data.num_elems = 1;
  (*matrix).value_data.column = 1;
  
}

/* Function to inicialice more than one columns */
void col_value(sym_value_type * matrix, sym_value_type v1, sym_value_type v2){
  
  (*matrix).value_type = MATRIX_TYPE;

  if(v2.value_type == INT_TYPE){

    (*matrix).value_data.matrix_type = INT_TYPE;

    (*matrix).value_data.integer_matrix = calloc(v1.value_data.column+1, sizeof(long));
    if((*matrix).value_data.integer_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");

    for(int i = 0; i < v1.value_data.column; i++) 
      (*matrix).value_data.integer_matrix[i] = v1.value_data.integer_matrix[i];
    
    free(v1.value_data.integer_matrix);

    (*matrix).value_data.integer_matrix[v1.value_data.column] = v2.value_data.enter;

  } else if(v2.value_type == FLOAT_TYPE){
    /* Indico que tipo de valor tendra */
    (*matrix).value_data.matrix_type = FLOAT_TYPE;
    (*matrix).value_data.float_matrix = calloc(v1.value_data.column+1, sizeof(float));
    if((*matrix).value_data.float_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");

    for(int i = 0; i < v1.value_data.column; i++) 
      (*matrix).value_data.float_matrix[i] = v1.value_data.float_matrix[i];
    
    free(v1.value_data.float_matrix);

    (*matrix).value_data.float_matrix[v1.value_data.column] = v2.value_data.real;

  } else yyerror(" Matrix only accept Integer or Float.");
  
  (*matrix).value_data.column=v1.value_data.column+1;
  (*matrix).value_data.num_elems++;
}

/* Function for using vectors and matrices with more than one line */
void row_value(sym_value_type *matrix, sym_value_type v1, sym_value_type v2){

  (*matrix).value_type = MATRIX_TYPE;
  
  
  if(v1.value_data.column != v2.value_data.column) yyerror("Error. Incompatibel dimension: Diferent size of columns in matrix");

  /* Type of vectors and matrices: 
   1. INT TYPE
   2. FLOAT TYPE
   3. INT TYPE AND FLOAT TYPE
   4. FLOAT TYPE AND INT TYPE */

  if(v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == INT_TYPE){
    (*matrix).value_data.matrix_type = INT_TYPE;

    (*matrix).value_data.integer_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(long));
    if((*matrix).value_data.integer_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");

    /* Copy the content of first row (V1) in the final matrix */
    for(int i=0; i<v1.value_data.num_elems ; i++) (*matrix).value_data.integer_matrix[i] = v1.value_data.integer_matrix[i];

    /* Copy the content of second row (V2) in the final matrix.
        In ths part of the code we treat this if V2 was the second row but not it's not necessarily 
        the second line it may be the fourth (for example).  */
    int j = 0;
    for(int i = v1.value_data.num_elems ; i<v2.value_data.column+v1.value_data.num_elems; i++){
      (*matrix).value_data.integer_matrix[i] = v2.value_data.integer_matrix[j];
      j++;
    }

    /* free the heap memory of the V1 and v2*/
    free(v1.value_data.integer_matrix);
    free(v2.value_data.integer_matrix);
   
  } else if ((v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE)){
    (*matrix).value_data.matrix_type = FLOAT_TYPE;

    (*matrix).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
    if((*matrix).value_data.float_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");
  
    for(int i=0; i<v1.value_data.num_elems ; i++) (*matrix).value_data.float_matrix[i] = v1.value_data.float_matrix[i];

    int j = 0;
    for(int i = v1.value_data.num_elems ; i<v2.value_data.column+v1.value_data.num_elems; i++){
      (*matrix).value_data.float_matrix[i] = v2.value_data.float_matrix[j];
      j++;
    }

    free(v1.value_data.float_matrix);
    free(v2.value_data.float_matrix);
  } else if ((v1.value_data.matrix_type == INT_TYPE && v2.value_data.matrix_type == FLOAT_TYPE)){
    (*matrix).value_data.matrix_type = FLOAT_TYPE;

    (*matrix).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
    if((*matrix).value_data.float_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");
  
    for(int i=0; i<v1.value_data.num_elems ; i++) (*matrix).value_data.float_matrix[i] = (float)v1.value_data.integer_matrix[i];

    int j = 0;
    for(int i = v1.value_data.num_elems ; i<v2.value_data.column+v1.value_data.num_elems; i++){
      (*matrix).value_data.float_matrix[i] = v2.value_data.float_matrix[j];
      j++;
    }

    free(v1.value_data.integer_matrix);
    free(v2.value_data.float_matrix);
  }else if ((v1.value_data.matrix_type == FLOAT_TYPE && v2.value_data.matrix_type == INT_TYPE)){
    (*matrix).value_data.matrix_type = FLOAT_TYPE;

    (*matrix).value_data.float_matrix = calloc(v1.value_data.num_elems+v2.value_data.num_elems, sizeof(float));
    if((*matrix).value_data.float_matrix  == NULL) yyerror("Error. Can't inicialize heap memory");
  
    for(int i=0; i<v1.value_data.num_elems ; i++) (*matrix).value_data.float_matrix[i] = v1.value_data.float_matrix[i];

    int j = 0;
    for(int i = v1.value_data.num_elems ; i<v2.value_data.column+v1.value_data.num_elems; i++){
      (*matrix).value_data.float_matrix[i] = (float)v2.value_data.integer_matrix[j];
      j++;
    }

    free(v2.value_data.integer_matrix);
    free(v1.value_data.float_matrix);
  }else yyerror("INCORRECT VALUE OF MATRIX & VECTOR");

  (*matrix).value_data.row = (*matrix).value_data.row + 1;
  (*matrix).value_data.column = v2.value_data.column;
  (*matrix).value_data.num_elems = (*matrix).value_data.row * (*matrix).value_data.column;

}

/**********************************************************************/
/*                  BOOLEAN FUNCTIONS                            */
/**********************************************************************/
