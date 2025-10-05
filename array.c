#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static inline ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
  if (*lineptr == NULL || *n == 0) {
    *n = 128;
    *lineptr = malloc(*n);
    if (*lineptr == NULL) return -1;
  }

  size_t i = 0;
  int c;
  while ((c = fgetc(stream)) != EOF) {
    if (i + 1 >= *n) {
      *n *= 2;
      char *new_ptr = realloc(*lineptr, *n);
      if (!new_ptr) return -1;
      *lineptr = new_ptr;
    }
    (*lineptr)[i++] = c;
    if (c == '\n') break;
  }

  if (i == 0 && c == EOF) return -1;

  (*lineptr)[i] = '\0';
  return i;
}


/* 
  =====================
   var Type System
  ===================== 
*/

typedef enum VarType {
  TYPE_INT,
  TYPE_DOUBLE,
  TYPE_CHAR,
  TYPE_STRING
} VarType;

typedef struct var {
  VarType type;
  union {
    int    i;
    double d;
    char ch;
    char  *s;
  } data;
} var;

/* var constructors */

var $int(int x) {
  var v; v.type = TYPE_INT; v.data.i = x; return v;
}

var $char(char ch){
  var v; v.type = TYPE_CHAR; v.data.ch = ch; return v;
}

var $double(double d) {
  var v; v.type = TYPE_DOUBLE; v.data.d = d; return v;
}

var $str(const char *s) {
  var v; v.type = TYPE_STRING;
  v.data.s = strdup(s);
  return v;
}

static inline VarType __var_type(const char *str) {
    if (!str || !*str) return TYPE_STRING;

    int len = strlen(str);

    if (len == 1 && !isdigit(str[0])) {
      return TYPE_CHAR;
    }

    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    if (errno == 0 && *endptr == '\0') {
      return TYPE_INT;
    }

    errno = 0;
    double dval = strtod(str, &endptr);
    if (errno == 0 && *endptr == '\0') {
      return TYPE_DOUBLE;
    }

    return TYPE_STRING;
}

static inline char* __input(){
  char *input = NULL;
  size_t len = 0;

  ssize_t read = getline(&input, &len, stdin);

  if (read == -1) {
    free(input);
    return NULL;
  }

  input[strcspn(input, "\n")] = '\0';
  return input;
}

void var_get(var *v) {
  char *s = __input();
  if (!s) return;

  VarType type = __var_type(s);

  switch (type) {
    case TYPE_INT:
      *v = $int((int)strtol(s, NULL, 10));
      break;

    case TYPE_DOUBLE:
      *v = $double(strtod(s, NULL));
      break;

    case TYPE_CHAR:
      *v = $char(s[0]);
      break;

    case TYPE_STRING:
    default:
      *v = $str(s);
      break;
  }

  free(s);
}

void var_free(var *v) {
  if (v->type == TYPE_STRING && v->data.s) {
    free(v->data.s);
    v->data.s = NULL;
  }
}

void var_print(const var *v) {
  switch (v->type) {
    case TYPE_INT:    printf("%d", v->data.i); break;
    case TYPE_DOUBLE: printf("%f", v->data.d); break;
    case TYPE_CHAR:   printf("\'%c\'", v->data.ch); break;
    case TYPE_STRING: printf("\"%s\"", v->data.s); break;
    default:          printf("<unknown>"); break;
  }
}

void var_println(const var *v){
  var_print(v);
  printf("\n");
}

int _int(const var* v){
  if (!v) return 0;
  if (v->type == TYPE_INT) return v->data.i;
  if (v->type == TYPE_CHAR) return (int) v->data.ch;
  if (v->type == TYPE_STRING){
    char *s = v->data.s;
    VarType type = __var_type(s);
    if (type == TYPE_INT) return (int)strtol(s, NULL, 10);
    if (type == TYPE_DOUBLE) return (int)strtod(s, NULL);
  }
  return 0;
}

char _char(const var* v){
  return v && v->type == TYPE_CHAR ? v->data.ch : '\0';
}

double _double(const var* v){
  if (!v) return 0.0;
  if (v->type == TYPE_DOUBLE) return v->data.i;
  if (v->type == TYPE_CHAR) return (double) v->data.ch;
  if (v->type == TYPE_STRING){
    char *s = v->data.s;
    VarType type = __var_type(s);
    if (type == TYPE_INT) return (double)strtol(s, NULL, 10);
    if (type == TYPE_DOUBLE) return strtod(s, NULL);
  }
  return 0.0;
}

const char* _str(const var* v){
  return (v && v->type == TYPE_STRING) ? v->data.s : NULL;
}

/* 
  =====================
   Dynamic Array
  ===================== 
*/

typedef struct Array {
  var *values;
  size_t size;
  size_t capacity;
} Array;


/* Functions Definitions */
/* Constructors, Destructors, Utils */

Array* new_array(void);
Array* new_array_size(size_t size);
Array* new_array_size_fill(size_t size, var fill);
// Array* new_array_copy_from_int(int arr[], size_t size);1
/**
 *  @brief  creates a new array by copying all the elements from the source array 
 *  @param  __array Pointer to source array
 *  @return  Copy of source array
*/
Array* array_slice(Array* array, int start, int end);
Array* array_copy(const Array* array);
void array_free_memory(Array* array);

/* internal helpers */

static bool __is_index_out_of_bounds(const Array* array, int index);
static bool __is_index_valid_for_insert(const Array* array, int index);
static bool __increase_array_capacity(Array *array, int factor);
static bool __double_array_capacity(Array *array);
static bool __is_array_empty(const Array *array);
static bool __is_array_full(const Array* array);

/* core API */

bool array_get(const Array* array, int index, var* out);
bool array_put(Array* array, int index, var value);
int array_size(const Array* array);
void array_clear(Array* array);
bool array_push(Array* array, var value);
bool array_insert(Array* array, int index, var value);
bool array_pop(Array* array); 
bool array_delete(Array* array, int index);
bool array_back(const Array* array, var* out);
bool array_front(const Array* array, var* out);
int array_find(const Array* array, var target);

/* print Helper */

void array_print(const Array* array);
void array_println(const Array* array);



/* IMPLEMENTATIONS */

/* Internal Helpers */

static inline bool __is_index_out_of_bounds(const Array* array, int index){
  return index < 0 || index >= (int) array -> size;
}

static inline bool __is_index_valid_for_insert(const Array* array, int index){
  return index < 0 || index > (int) array -> size;
}

static inline bool __is_array_full(const Array* array){
  return array->size >= array->capacity;
}

static inline bool __increase_array_capacity(Array *array, int factor){
  array->capacity *= factor;
  var* new_values = realloc(array->values, array->capacity * sizeof *array->values);
  if (!new_values) return false;
  array->values = new_values;
  return true;
}

static inline bool __double_array_capacity(Array *array){
  return __increase_array_capacity(array, 2);
}

static inline bool __is_array_empty(const Array* array){
  return array->size == 0;
}

/* Core API */

Array* new_array(){
  Array* array = (Array *) malloc(sizeof(Array));
  if (!array) return NULL;
  array->values = (var *) malloc(sizeof *array->values);
  if (!array->values) {
    free(array);
    return NULL;
  }
  array->capacity = 1;
  array->size = 0;
  return array;
}

Array* new_array_size(size_t size){
  Array* array = new_array();
  if (!array) return NULL;

  array->values = (var *) calloc(size, sizeof *array->values);
  if (!array->values) {
    free(array);
    return NULL;
  }
  array -> capacity = size;
  array->size = size;
  return array;
}

Array* new_array_size_fill(size_t size, var fill){
  Array* array = new_array_size(size);
  if (!array) return NULL;
  for (int i = 0; i < size; i++) array->values[i] = fill;
  return array;
}

Array* array_copy(const Array* array){
  Array* copy = new_array_size(array->size);
    if (!copy) return NULL;
    for (size_t i = 0; i < array->size; i++){
      copy->values[i] = array->values[i];
      if (array->values[i].type == TYPE_STRING)
          copy->values[i].data.s = strdup(array->values[i].data.s);
    }
    return copy;
}

void array_free_memory(Array* array){
  for (size_t i = 0; i < array->size; i++){
    var_free(&array->values[i]);
  }
  free(array->values);
  free(array);
}

void array_clear(Array* array){
  for (size_t i = 0; i < array->size; i++)
    var_free(&array->values[i]);
  array->size = 0;
}

int array_size(const Array* array){
  return (int) array->size;
}

bool array_get(const Array* array, int index, var* out){
  if (__is_index_out_of_bounds(array, index)) return false;
  *out = array->values[index];
  return true;
}

bool array_put(Array* array, int index, var value){
  if (__is_index_out_of_bounds(array, index)) return false;
  var_free(&array->values[index]);
  array->values[index] = value;
  return true;
}

bool array_push(Array *array, var value) {
  if (__is_array_full(array)) if (!__double_array_capacity(array)) return false;
  array->values[array->size++] = value;
  return true;
}

bool array_pop(Array* array){
  if (__is_array_empty(array)) return false;
  var_free(&array->values[array->size-1]);
  array->size--;
  return true;
}

bool array_delete(Array* array, int index){
  if (__is_index_out_of_bounds(array, index)) return false;
  var_free(&array->values[index]);
  for (int i = index; i < array->size - 1; i++) 
    array->values[i] = array->values[i + 1];
  array->size--;
  return true;
}

int array_find(const Array* array, var target){
  for (size_t i = 0; i < array->size; i++){
    if (array->values[i].type == target.type) {
      switch (target.type) {
        case TYPE_INT:    
          if (array->values[i].data.i == target.data.i) 
            return i; break;
        case TYPE_DOUBLE: 
          if (array->values[i].data.d == target.data.d) 
            return i; break;
        case TYPE_CHAR:   
          if (array->values[i].data.ch == target.data.ch) 
            return i; break;
        case TYPE_STRING: 
          if (strcmp(array->values[i].data.s, target.data.s) == 0) 
            return i; break;
      }
    }
  }
  return -1;
}

void array_print(const Array* array){
  printf("[");
  for (int i = 0; i < array->size; i++){
    var_print(&array->values[i]);
    if (i != array->size - 1) printf(", ");
  }
  printf("]");
}

void array_println(const Array* array){
  array_print(array);
  printf("\n");
}

Array* array_slice(Array* array, int start, int end){
  if (
    !array
    || __is_index_out_of_bounds(array, start) 
    || __is_index_out_of_bounds(array, end)
    || end < start
  ) return NULL;

  size_t slice_size = end - start + 1;
  Array* slice = new_array_size(slice_size);
  if (!slice) return NULL;

  for (int i = 0; i < slice_size; i++){
    slice->values[i] = array->values[start + i];
  }

  return slice;
}

bool array_front(const Array* array, var* out){
  if (__is_array_empty(array)) return false;
  *out = array->values[0];
  return true;
}

bool array_back(const Array* array, var* out){
  if (__is_array_empty(array)) return false;
  int size = array->size;
  *out = array->values[size-1];
  return true;
}

bool array_insert(Array* array, int index, var value){
  if (!array) return false;
  if (__is_index_valid_for_insert(array, index)) return false;
  if (__is_array_full(array)) if (!__double_array_capacity(array)) return false;
  for (int i = array->size - 1; i >= index; i--) 
    array->values[i+1] = array->values[i];
  array->values[index] = value;
  array->size++;
  return true;
}



int main(){
  Array* arr = new_array();
  int size;
  scanf("%d", &size);
  getchar();
  var v;
  for (int i = 0; i < size; i++){
    var_get(&v);
    array_push(arr, v);
  }
  array_println(arr);

  array_push(arr, $str("3.14"));
  array_println(arr);

  var out; 
  if (array_get(arr, array_size(arr) - 1, &out)){
    int last = _int(&out);
    printf("%d\n", last);
  }

  var_free(&out);

  arr = new_array();
  array_push(arr, $int(10));
  array_push(arr, $double(3.14));
  array_push(arr, $char('A'));
  array_push(arr, $str("Hello"));

  printf("Array contents: ");
  array_println(arr);

  var v;
  if (array_get(arr, 1, &v)) {
    printf("Index 1 is: ");
    var_print(&v);
    printf("\n");
  }

  if (array_get(arr, 0, &v)){
    int num = _int(&v);
    printf("%d\n", num);
  }

  array_free_memory(arr);

  Array* week_days = new_array_size(7);
  array_println(week_days);
  array_put(week_days, 0, $str("monday"));
  array_put(week_days, 1, $str("tuesday"));
  array_put(week_days, 2, $str("wednesday"));
  array_put(week_days, 3, $str("thursday"));
  array_put(week_days, 4, $str("friday"));
  array_put(week_days, 5, $str("saturday"));
  array_put(week_days, 6, $str("sunday"));
  array_println(week_days);

  array_put(week_days, 5, $char('A'));
  array_println(week_days);

  Array* week_days_slice = array_slice(week_days, 0, 4);
  array_println(week_days_slice);

  array_push(week_days_slice, $str("saturday"));
  array_push(week_days_slice, $str("sunday"));
  array_println(week_days_slice);
  array_pop(week_days_slice);
  array_pop(week_days_slice);
  array_insert(week_days_slice, 0, $str("sunday"));
  array_delete(week_days_slice, 1);
  array_println(week_days_slice);

  int idx = array_find(week_days, $str("sunday"));
  printf("%d ", idx);
  var found; array_get(week_days, idx, &found);
  var_println(&found);

  array_free_memory(week_days_slice);
  array_free_memory(week_days);

  int day = 0;
  scanf("%d", &day);
  var week_day; array_get(week_days, day - 1, &week_day);
  var_println(&week_day);
}

