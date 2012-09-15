
//! \brief	allocates an element of the given \a type
#define ALLOCATE(type) (type*)malloc(sizeof(type))

//! \brief	allocates \a count elements of the given \a type
#define ALLOCATE_ARRAY(type,count) (type*)malloc(sizeof(type)*(count))

//! \brief	free's \a obj if it's not NULL and sets it to NULL
#define SAFE_FREE(obj){if(obj){free(obj);(obj) = NULL;}}

//! \brief	zeros out the given \a obj of the given \a type
#define ZERO(obj,type) memset((obj),0,sizeof(type))

//! \brief	zeros the given \a obj array of \a count objects of the given \a type
#define ZERO_ARRAY(obj,type,count) memset((obj),0,sizeof(type)*(count))
