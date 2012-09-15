Toroid
======

Game Idea
---------
* Player plays in a tube
* Boss fires colored projectiles down the tube
* Player must collide with projectiles of his color to "attack" the boss and dodge the others
* Player's color changes with random color pickups
* Control initially with keyboard - integrate sphero in the future
* tube initially linear, expand to a spline
* add pertubations in tube
	* Bumps
	* Holes in the tube
	* Walls blocking off sections

Coding Style:
-------------
* Opening bracket '{' on a new line
* Indent using Tabs
* Doxygen-style comments on function implementations, only document header if element does not appear in source
* All implementations in source files, not headers
* Each translation unit should encapsulate one "class"
* functions should have the form ClassName_functionName
	* First letter of the class name is Capital, camel case afterwards
	* first leter of function name is lower case, camel case afterwards
* if non-static, the first parameter should be a "this" pointer
* a struct representing the class MUST be named similar to the source file
* user facing structs must be defined as:
	> typedef struct StructName_t {} StructName
* Special functions:
	* ClassName* ClassName_Init()
		* Initializer
	* void ClassNamer_Cleanup(ClassName* pClassName)
		* Destructor
	* Singleton classes follow the same pattern but with the ClassName* portions ignored
	* Singletons must have _Init and _Cleanup even if they do nothing
* Inter-object communication should go through the event system when at all possible
	* primary exception being platform dependent communication between subsystems (ex: glWindow raising a keyboard event directly)
* Enumerations should be of the form:  

>	typedef EnumName_t  
>	{  
>		EnumName_Invalid = -1, // optional  
>		EnumName_Elem0 = 0,  
>		EnumName_Elem1,  
>		EnumName_Elem2,  
>		// &c.  
>		
>		EnumName_Count  
>	} EnumName;

* naming:
	* '_' only used to indicate classes/modules (grouping) or enumeration membership
	* all names should use CamelCase
	* Pointers should start with 'p'
	* Arrays should start with 'a'
	* Non-public member variables in structs should start with 'm_'
	* Static variables should start with 's_'
	* Constant variables should start with 'k' (before the 'a' or 'p')
