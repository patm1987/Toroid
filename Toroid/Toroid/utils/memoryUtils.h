
#define ALLOCATE(type) (type*)malloc(sizeof(type))
#define ALLOCATE_ARRAY(type,count) (type*)malloc(sizeof(type)*(count))
#define SAFE_FREE(obj){if(obj){free(obj);(obj) = NULL;}}

#define ZERO(obj,type) memset((obj),0,sizeof(type))
#define ZERO_ARRAY(obj,type,count) memset((obj),0,sizeof(type)*(count))
